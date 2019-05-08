/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <string.h>
#include <CommonThings.h>
#include <Maze.h>
#include <iostream>
#include <Timer.h>
#include <player.h>

#ifdef __APPLE__
#include <GLUT/glut.h>
#else
#include <GL/glut.h>
#endif

#include <stdlib.h>
#include <Enemies.h>

#include <wall.h>
#include <math.h>
#include <LevelLoader.h>
#include <Scene.h>
#include <Sounds.h>

/* GLUT callback Handlers */

using namespace std;

enum GameStates{LANDING, TITLE, GAME, VICTORY, DEFEAT};

LevelLoader *L = new LevelLoader();
Scene *Title = new Scene();
Scene *winScreen = new Scene();
Scene *loseScreen = new Scene();

Sounds *audio = new Sounds();

GameStates gameState = TITLE;
bool action = true;

Timer *T0 = new Timer();                        // animation timer

float wWidth, wHeight;                          // display window width and Height
float xPos,yPos;                                // Viewpoar mapping

float FPS = 60.0;
int time_now, time_prev;


void display(void);                             // Main Display : this runs in a loop

void resize(int width, int height)              // resizing case on the window
{
    wWidth = width;
    wHeight = height;

    if(width<=height)
        glViewport(0,(GLsizei) (height-width)/2,(GLsizei) width,(GLsizei) width);
    else
        glViewport((GLsizei) (width-height)/2 ,0 ,(GLsizei) height,(GLsizei) height);
}

void init()
{
    glEnable(GL_COLOR_MATERIAL);

    glHint(GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST);
    glShadeModel(GL_SMOOTH);
    glEnable(GL_LINE_SMOOTH);
    glEnable(GL_POLYGON_SMOOTH);
    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    glClearColor(0.0,0.0,0.0,0.0);
    gluOrtho2D(0, wWidth, 0, wHeight);

    T0->Start();                                        // set timer to 0

    glEnable(GL_BLEND);                                 //display images with transparent
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    audio->playMusic("sounds/intro.mp3");

    L->M->loadBackgroundImage("images/bak.png");           // Load maze background image
    L->load("levels/level1.txt");
    Title->bindTexture("images/Title.png");
    winScreen->bindTexture("images/victory.png");
    loseScreen->bindTexture("images/defeat.png");
}

// check Collisions can go into levelLoader Code
void checkCollisions(){
    L->P->isHidden = false;
    //check player wall collisions
    for (int i = 0; i < L->wallCount; i++){
        if(L->P->wPos.x == L->W[i].wallPos.x && L->P->wPos.y == L->W[i].wallPos.y){
            L->matrix[L->P->wPos.x][L->P->wPos.y] = 1;
            L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 4;
            L->P->wPos = L->P->prevPos;
            L->P->placePlayer(L->P->wPos.x, L->P->wPos.y);
        }
    }
    //check enemy wall collisions
    for (int e = 0; e < L->enemyCount; e++){
        for (int i = 0; i < L->wallCount; i++){
            if(L->E[e].ePos.x == L->W[i].wallPos.x && L->E[e].ePos.y == L->W[i].wallPos.y){
                L->matrix[L->E[e].ePos.x][L->E[e].ePos.y] = 1;
                L->matrix[L->E[e].prevPos.x][L->E[e].prevPos.y] = 2;
                L->E[e].ePos = L->E[e].prevPos;
                L->E->placeEnemy(L->E[e].ePos.x, L->E[e].ePos.y);
            }
        }

        for(int j = 1; j < L->enemyCount; j++){
            if(j != e) {
                if(L->E[e].live && L->E[j].live && L->E[e].ePos.x == L->E[j].ePos.x && L->E[e].ePos.y == L->E[j].ePos.y) {
                    L->E[e].ePos.x = L->E[e].prevPos.x;
                    L->E[e].ePos.y = L->E[e].prevPos.y;
                    L->E[e].placeEnemy(L->E[e].ePos.x,L->E[e].ePos.y);
                }
            }
        }
    }

    for (int b = 0; b < L->bushCount; b++){
        if(L->P->wPos.x == L->B[b].wallPos.x && L->P->wPos.y == L->B[b].wallPos.y){
            //L->P->isHidden = true;
            L->B[b].hasPlayer = true;
        }
        else{
            //L->P->isHidden = false;
            L->B[b].hasPlayer = false;

        }
        if (!L->B[b].hasPlayer && L->P->prevPos.x == L->B[b].wallPos.x && L->P->prevPos.y == L->B[b].wallPos.y) {
            L->matrix[L->B[b].wallPos.x][L->B[b].wallPos.y] = 5;
        }
        if(L->B[b].hasPlayer){
            L->P->isHidden = true;
        }
    }
    for (int e = 0; e < L->enemyCount; e++){
        if (L->P->wPos.x == L->E[e].ePos.x && L->P->wPos.y ==L->E[e].ePos.y){
            // LEVEL SELECTOR AND VARIABLE TO KEEP TRACK
            //audio->stopAllSounds();
            audio->playSound("sounds/death.mp3");
            L->P->livePlayer = false;
            gameState = DEFEAT;
            L->levelSelector = 1;
        }
        if (L->P->wPos.x == L->E[e].prevPos.x && L->P->wPos.y ==L->E[e].prevPos.y){
            // LEVEL SELECTOR AND VARIABLE TO KEEP TRACK
            //audio->stopAllSounds();
            audio->playSound("sounds/death.mp3");
            L->P->livePlayer = false;
            gameState = DEFEAT;
            L->levelSelector = 1;
        }
    }
    //check player chest collision
    if (L->P->wPos.x == L->M->GetChestLoc().x && L->P->wPos.y == L->M->GetChestLoc().y){
        // LEVEL SELECTOR AND VARIABLE TO KEEP TRACK
        if(L->P->livePlayer)
        audio->playSound("sounds/victory.mp3");
        gameState = VICTORY;
        L->levelSelector++;
    }

}

void moveEnemies(){
    if (gameState == GAME){
        for(int i = 0; i < L->enemyCount; i++){
            if(L->E[i].live)
            L->E[i].moveEnemy(L->matrix, L->P->isHidden);
        }
    }
}

void display(void)
{

    time_now = glutGet(GLUT_ELAPSED_TIME);
    if ((time_now - time_prev) > 1000/FPS){
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    if (gameState == TITLE){
        glPushMatrix();
            Title->drawScene();
        glPopMatrix();
    }
    else if (gameState == GAME){
        glPushMatrix();
            L->M->drawBackground();
        glPopMatrix();

        glPushMatrix();
            L->M->drawGrid();
        glPopMatrix();
        for(int i=0; i<L->bushCount;i++){
           glPushMatrix();
           L->B[i].drawWall();
           glPopMatrix();
        }
        glPushMatrix();
            L->P->drawplayer();
        glPopMatrix();

        glPushMatrix();
            L->P->drawArrow();
        glPopMatrix();

         glPushMatrix();
           L->M->drawChest();
        glPopMatrix();

        glPushMatrix();
           //L->M->drawArrows();
        glPopMatrix();


        for(int i=0; i<(sizeof(L->E)/sizeof(Enemies));i++)
        {
            glPushMatrix();
            L->E[i].placeEnemy(L->E[i].ePos.x, L->E[i].ePos.y);
            L->E[i].drawEnemy();
            glPopMatrix();
        }

        for(int i=0; i<L->wallCount;i++)
        {
           glPushMatrix();
           L->W[i].drawWall();
           glPopMatrix();
        }

    }
    else if(gameState == VICTORY){
        glPushMatrix();
            winScreen->drawScene();
        glPopMatrix();
    }
    else if (gameState == DEFEAT){
        glPushMatrix();
            loseScreen->drawScene();
        glPopMatrix();
    }

    glutSwapBuffers();
    time_prev = time_now;
    }
}

void key(unsigned char key, int x, int y)
{
    switch(key){
        case 'p':
            if(gameState == TITLE || gameState == DEFEAT || gameState == VICTORY ){
                //make a current level variable
                if(L->levelSelector == 1){
                    L->reset();
                    L->load("levels/level1.txt");
                }
                if(L->levelSelector == 2){
                    L->reset();
                    L->load("levels/level2.txt");
                }
                if(L->levelSelector == 3){
                    L->reset();
                    L->load("levels/level3.txt");
                }
                if(L->levelSelector > 2){
                    gameState = TITLE;
                    L->levelSelector = 0;
                }
                gameState = GAME;
            }

        break;
        case 'm':
            if (gameState == VICTORY || gameState == DEFEAT){
                gameState = TITLE;
            }
            break;
        case 27:
            if(gameState == TITLE){
            exit(0);
            }
            else {
                gameState = TITLE;
            }
        break;

    }
    glutPostRedisplay();
}


 void GetOGLPos(int x, int y)
{
    GLint viewport[4];
    GLdouble modelview[16];
    GLdouble projection[16];
    GLfloat winX, winY, winZ;
    GLdouble posX, posY, posZ;

    glGetDoublev( GL_MODELVIEW_MATRIX, modelview );
    glGetDoublev( GL_PROJECTION_MATRIX, projection );
    glGetIntegerv( GL_VIEWPORT, viewport );

    winX = (float)x;
    winY = (float)viewport[3] - (float)y;
    glReadPixels( x, int(winY), 1, 1, GL_DEPTH_COMPONENT, GL_FLOAT, &winZ );

    gluUnProject( winX, winY, winZ, modelview, projection, viewport, &posX, &posY, &posZ);

    xPos =posX ;
    yPos =posY ;
}

 void idle(void)
{

    //Your Code here

    glutPostRedisplay();
}

void Specialkeys(int key, int x, int y)
{
    checkCollisions();
    L->P->prevPos = L->P->wPos;
    // Your Code here
    if(gameState == GAME){
        switch(key)
        {

            case GLUT_KEY_UP:
                 moveEnemies();
                 if (L->matrix[L->P->wPos.x][L->P->wPos.y+1] != 1){
                    L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 0;
                     L->P->movePlayer("up");
                     L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
                 }
            break;

            case GLUT_KEY_DOWN:
                 moveEnemies();
                 if (L->matrix[L->P->wPos.x][L->P->wPos.y-1] != 1){
                    L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 0;
                     L->P->movePlayer("down");
                     L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
                 }
            break;

            case GLUT_KEY_LEFT:
                 moveEnemies();
                 if (L->matrix[L->P->wPos.x-1][L->P->wPos.y] != 1){
                    L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 0;
                     L->P->movePlayer("left");
                     L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
                 }
            break;

            case GLUT_KEY_RIGHT:
                 moveEnemies();
                 if (L->matrix[L->P->wPos.x+1][L->P->wPos.y] != 1){
                    L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 0;
                     L->P->movePlayer("right");
                     L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
                 }
            break;

    }
  glutPostRedisplay();

}
        checkCollisions();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("CSCI 115 Project");                    //program title
   init();

   glutDisplayFunc(display);                     //callback function for display
   glutReshapeFunc(resize);                      //callback for reshape
   glutKeyboardFunc(key);                        //callback function for keyboard
   glutSpecialFunc(Specialkeys);
   //glutMouseFunc(mouse);
   glutIdleFunc(idle);
   glutMainLoop();

   return EXIT_SUCCESS;
}
