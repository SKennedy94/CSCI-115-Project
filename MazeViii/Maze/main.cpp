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

/* GLUT callback Handlers */

using namespace std;

enum GameStates{LANDING, TITLE, GAME};

LevelLoader *L = new LevelLoader();
Scene *Title = new Scene();

GameStates gameState = TITLE;

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

    L->M->loadBackgroundImage("images/bak.jpg");           // Load maze background image
    L->load("levels/level1.txt");
    Title->bindTexture("images/Title.png");
}

void checkCollisions(){
    for (int i = 0; i < L->wallCount; i++){
        if(L->P->wPos.x == L->W[i].wallPos.x && L->P->wPos.y == L->W[i].wallPos.y){
            L->matrix[L->P->wPos.x][L->P->wPos.y] = 1;
            L->matrix[L->P->prevPos.x][L->P->prevPos.y] = 4;
            L->P->wPos = L->P->prevPos;
            L->P->placePlayer(L->P->wPos.x, L->P->wPos.y);
        }
    }
}

void display(void)
{
    checkCollisions();
    time_now = glutGet(GLUT_ELAPSED_TIME);
    if ((time_now - time_prev) > 1000/FPS){
  glClear (GL_COLOR_BUFFER_BIT);        // clear display screen

    if (gameState == TITLE){
        glPushMatrix();
            Title->drawScene();
        glPopMatrix();
    }
    else if (gameState = GAME){
        glPushMatrix();
            L->M->drawBackground();
        glPopMatrix();

        glPushMatrix();
            L->M->drawGrid();
        glPopMatrix();

        glPushMatrix();
            L->P->drawplayer();
        glPopMatrix();

        glPushMatrix();
            L->P->drawArrow();
        glPopMatrix();

         glPushMatrix();
           //L->M->drawChest();
        glPopMatrix();

        glPushMatrix();
           //L->M->drawArrows();
        glPopMatrix();


        for(int i=0; i<(sizeof(L->E)/sizeof(Enemies));i++)
        {
            glPushMatrix();
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

    glutSwapBuffers();
    time_prev = time_now;
    }
}




void key(unsigned char key, int x, int y)
{
    switch(key){
        case 'p':
            if(gameState == TITLE){
                gameState = GAME;
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
        case ' ':
            if(gameState = GAME){
                L->reset();
                L->load("levels/level2.txt");
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
    L->P->prevPos = L->P->wPos;
    // Your Code here
    switch(key)
    {
    if(gameState == GAME){
        case GLUT_KEY_UP:
            //Move Enemy
             L->matrix[L->P->wPos.x][L->P->wPos.y] = 0;
             L->P->movePlayer("up");
             L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
        break;

        case GLUT_KEY_DOWN:

             L->matrix[L->P->wPos.x][L->P->wPos.y] = 0;
             L->P->movePlayer("down");
             L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
        break;

        case GLUT_KEY_LEFT:

             L->matrix[L->P->wPos.x][L->P->wPos.y] = 0;
             L->P->movePlayer("left");
             L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;

        break;

        case GLUT_KEY_RIGHT:

             L->matrix[L->P->wPos.x][L->P->wPos.y] = 0;
             L->P->movePlayer("right");
             L->matrix[L->P->wPos.x][L->P->wPos.y] = 4;
        break;

       }
    }
  glutPostRedisplay();
}


/* Program entry point */

int main(int argc, char *argv[])
{
   glutInit(&argc, argv);

   glutInitDisplayMode(GLUT_RGBA | GLUT_DOUBLE | GLUT_DEPTH);
   glutInitWindowSize (800, 800);                //window screen
   glutInitWindowPosition (100, 100);            //window position
   glutCreateWindow ("Maze");                    //program title
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
