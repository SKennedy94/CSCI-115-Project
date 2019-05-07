/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */

#include <Enemies.h>
#include <iostream>
#include <time.h>
#include <stdlib.h>

using namespace std;



Timer *T1 = new Timer();                             // Set Timer for animation
Enemies::Enemies()
{
    //ctor
    enmLoc.x=0;
    enmLoc.y=0;

    xmax =1;
    ymax =0.25;
    xmin =0;
    ymin =0;
    up= down = left=right=false;
    live = true;

}

Enemies::~Enemies()
{
    //dtor
}

void Enemies::initEnm(int grid,int numFrames, char * FileName)
{
    srand(time(NULL));
    gridSize = grid;
    frames = numFrames;
    xmax =1/(float)frames;
    T1->Start();
    t = (float)(2.0/grid)/frames;
    unitWidth = (float)2.0/gridSize;
    enmTex = TextureLoader(FileName);
}


void Enemies::drawEnemy()
{
    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,enmTex);


    glPushMatrix();
    animate();
    glTranslatef(enmLoc.x,enmLoc.y,0.0);

    glScaled(1.0/(float)gridSize,1.0/(float)gridSize,1);

    glBegin(GL_POLYGON);
        glTexCoord2f(xmin,ymin);
        glVertex3f(1,1,0.0f);

        glTexCoord2f(xmax,ymin);
        glVertex3f(-1,1,0.0f);

        glTexCoord2f(xmax,ymax);
        glVertex3f(-1,-1,0.0f);

        glTexCoord2f(xmin,ymax);
        glVertex3f(1,-1,0.0f);
     glEnd();
     glPopMatrix();
}

void Enemies::placeEnemy(int x, int y)
{

    unitWidth = 2.0/gridSize;
    ePos.x = x;
    ePos.y = y;
    x+=1;
    y+=1;
    enmLoc.x =  -1-unitWidth/2+(unitWidth)*x;
    enmLoc.y =  -1-unitWidth/2+(unitWidth)*y;
}

void Enemies::moveEnemy(int matrix[25][25], bool isHidden)
{
    if(live){
    prevPos = ePos;
    if (isVisible(matrix) && !isHidden){eState = CHASE;}
   else {eState = WANDER;}
    if(eState == WANDER){
        Wander(matrix);
    }
    else {
        Chase(matrix);
    }
    // HANDLES MOVING in matrix
    matrix[ePos.x][ePos.y] = 0;
    if(dir == "right"){
        if(matrix[ePos.x+1][ePos.y] != 1 && matrix[ePos.x+1][ePos.y] != 2){        //Right
            matrix[ePos.x+1][ePos.y] = 2;
            ePos.x+=1;
        }
    }
    else if(dir == "left"){
        if(matrix[ePos.x-1][ePos.y] != 1 && matrix[ePos.x+1][ePos.y] != 2){        //Right
            matrix[ePos.x-1][ePos.y] = 2;
            ePos.x-=1;
        }
    }
    else if(dir == "up"){
        if(matrix[ePos.x][ePos.y+1] != 1 && matrix[ePos.x+1][ePos.y] != 2){        //Right
            matrix[ePos.x][ePos.y+1] = 2;
            ePos.y+=1;
        }
    }
    else if(dir == "down"){
        if(matrix[ePos.x][ePos.y-1] != 1 && matrix[ePos.x+1][ePos.y] != 2){        //Right
            matrix[ePos.x][ePos.y-1] = 2;
            ePos.y-=1;
        }
    }
    else {
        ePos = prevPos;
        dir = "";
        matrix[ePos.x][ePos.y] == 2;
    }
    //moves sprite
    if(moveDis<=0){
        if(dir=="up"){up=true; down=left=right=false;}
        else if(dir=="down"){down=true; up=left=right=false;}
        else if(dir=="left"){left=true; down=up=right=false;}
        else if(dir=="right"){right=true; down=left=up=false;}
        else{up=left=right=false;}
    }
    }
}

bool Enemies::isVisible(int matrix[25][25]){

        for(int i = 0; i < sightDistance; i++){
            //look up
            if(matrix[ePos.x][ePos.y+i] == 4 && !WU){
                return true;
            }
            else if(matrix[ePos.x][ePos.y+i] == 1 || matrix[ePos.x][ePos.y+i] == 5){
                    WU=true;
            }
        }
        for(int i = 0; i < sightDistance; i++){
            //look down
            if(matrix[ePos.x][ePos.y-i] == 4 && !WD){
                return true;
            }
            else if(matrix[ePos.x][ePos.y-i] == 1 || matrix[ePos.x][ePos.y-i] == 5){
                    WD = true;
            }
        }

        for(int i = 0; i < sightDistance; i++){
            //look right
            if(matrix[ePos.x+i][ePos.y] == 4 && !WR){
                return true;
            }
            else if(matrix[ePos.x+i][ePos.y] == 1 || matrix[ePos.x+i][ePos.y] == 5){
                    WR=true;
            }
        }
        //look left
        for(int i = 0; i < sightDistance; i++){
            if(matrix[ePos.x-i][ePos.y] == 4 && !WL){
                return true;
            }
            else if(matrix[ePos.x-i][ePos.y] == 1 || matrix[ePos.x-i][ePos.y] == 5){
                    WL=true;
            }
        }
    WR =false;
    WL=false;
    WD=false;
    WU=false;
    return false;   //NOT VISIBLE
}

int Enemies::findPlayer(int matrix[25][25]){
    if(isVisible(matrix)){
        for(int i = 1; i < sightDistance; i++){
            //look up
            if(matrix[ePos.x][ePos.y+i] == 4){
                return 0;
            }
            //look down
            else if(matrix[ePos.x][ePos.y-i] == 4){
                return 1;
            }
            //look right
            else if(matrix[ePos.x+i][ePos.y] == 4){
                return 2;
            }
            //look left
            else if(matrix[ePos.x-i][ePos.y] == 4){
                return 3;
            }
        }
    }
}

void Enemies::Chase(int matrix[25][25]){
    switch(findPlayer(matrix)){
    case 0:// Chase Up
        dir="up";
    break;
    case 1:// Chase Down
        dir="down";
    break;
    case 2:// Chase Right
        dir="right";
    break;
    case 3:// Chase Left
        dir="left";
    break;

    }
}

Inter Enemies::isIntersection(int matrix[25][25]){
    if(matrix[ePos.x+1][ePos.y] == 0 &&
       matrix[ePos.x-1][ePos.y] == 0 &&
       matrix[ePos.x][ePos.y-1] == 0 &&
       matrix[ePos.x][ePos.y+1] == 0){cout<<"A intersection"<<endl; return A;}
    else if(matrix[ePos.x+1][ePos.y] == 0 &&
            matrix[ePos.x-1][ePos.y] != 0 &&
            matrix[ePos.x][ePos.y-1] != 0 &&
            matrix[ePos.x][ePos.y+1] != 0){cout<<"ER intersection"<<endl; return ER;}
    else if(matrix[ePos.x+1][ePos.y] != 0 &&
            matrix[ePos.x-1][ePos.y] == 0 &&
            matrix[ePos.x][ePos.y-1] != 0 &&
            matrix[ePos.x][ePos.y+1] != 0){cout<<"EL intersection"<<endl; return EL;}
    else if(matrix[ePos.x+1][ePos.y] != 0 &&
            matrix[ePos.x-1][ePos.y] != 0 &&
            matrix[ePos.x][ePos.y-1] != 0 &&
            matrix[ePos.x][ePos.y+1] == 0){cout<<"EU intersection"<<endl; return EU;}
    else if(matrix[ePos.x+1][ePos.y] != 0 &&
            matrix[ePos.x-1][ePos.y] != 0 &&
            matrix[ePos.x][ePos.y-1] == 0 &&
            matrix[ePos.x][ePos.y+1] != 0){cout<<"ED intersection"<<endl; return ED;}
    else if(matrix[ePos.x][ePos.y+1]== 0 &&
            matrix[ePos.x+1][ePos.y]== 0 &&
            matrix[ePos.x-1][ePos.y]== 0 ){cout<<"URL intersection"<<endl; return URL;}        //URL
    else if(matrix[ePos.x][ePos.y-1]== 0 &&
            matrix[ePos.x+1][ePos.y]== 0 &&
            matrix[ePos.x-1][ePos.y]== 0 ){cout<<"DRL intersection"<<endl; return DRL;}        //DRL
    else if(matrix[ePos.x][ePos.y+1]== 0 &&
            matrix[ePos.x][ePos.y-1]== 0 &&
            matrix[ePos.x-1][ePos.y]== 0 ){cout<<"LUD intersection"<<endl; return LUD;}        //LUD
    else if(matrix[ePos.x][ePos.y+1]== 0 &&
            matrix[ePos.x][ePos.y-1]== 0 &&
            matrix[ePos.x+1][ePos.y]== 0 ){cout<<"RUD intersection"<<endl; return RUD;}        //RUD
    else if(matrix[ePos.x+1][ePos.y] == 0 &&
            matrix[ePos.x][ePos.y+1]== 0 ){cout<<"UR intersection"<<endl; return UR;}         //up-right
    else if(matrix[ePos.x-1][ePos.y]== 0 &&
            matrix[ePos.x][ePos.y+1]== 0 ){cout<<"UL intersection"<<endl; return UL;}         //up-left
    else if(matrix[ePos.x-1][ePos.y]== 0 &&
            matrix[ePos.x][ePos.y-1]== 0 ){cout<<"DL intersection"<<endl; return DL;}         //down-left
    else if(matrix[ePos.x+1][ePos.y]== 0 &&
            matrix[ePos.x][ePos.y-1]== 0 ){cout<<"DR intersection"<<endl; return DR;}         //down-right
    else{cout<<"no intersection"<<endl;return N;}
}

void Enemies::Wander(int matrix[25][25]){
Inter inter = isIntersection(matrix);
    //HANDLES INTERSECTIONS
        switch(inter){
            case A:
                nextMove = rand()%4;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 0) {dir = "left";}
                else if (nextMove == 0) {dir = "down";}
                else {dir = "up";}
            break;

            case UR:
                nextMove = rand()%2;
                if (nextMove == 0) {dir = "up";}
                else {dir = "right";}
            break;

            case UL:
                nextMove = rand()%2;
                if (nextMove == 0) {dir = "up";}
                else {dir = "left";}
            break;

            case DL:
                nextMove = rand()%2;
                if (nextMove == 0) {dir = "left";}
                else {dir = "down";}
            break;
            case DR:
                nextMove = rand()%2;
                if (nextMove == 0) {dir = "right";}
                else {dir = "down";}
            break;

            case URL:
                nextMove = rand()%3;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "up";}
            break;
            case DRL:
                nextMove = rand()%3;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "down";}
            break;
            case LUD:
                nextMove = rand()%3;
                if (nextMove == 0) {dir = "down";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "up";}
            break;
            case RUD:
                nextMove = rand()%3;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "down";}
                else {dir = "up";}
            break;
            case ER:
                dir = "right";
            break;
            case EL:
                dir = "left";
            break;
            case EU:
                dir = "up";
            break;
            case ED:
                dir = "down";
            break;
            case N:
            break;
        }
}

void Enemies::animate()
{

if(moveDis < unitWidth)
{

if(T1->GetTicks()<1000)
   {

if(down)
   {
       if(enmLoc.y< 1-unitWidth/2)
        {
            enmLoc.y += t;
         }   moveDis +=t;

        xmin +=1/(float)frames;
        xmax +=1/(float)frames;
        ymin =0.0;
        ymax =0.25;
        if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }
    }

  else if(left)
   {
         if(enmLoc.y > -1+unitWidth/2)
           {
              enmLoc.y -= t;
           }  moveDis +=t;

            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.25;
            ymax =0.5;

            if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }

   }
   else if(right)
   {
       if(enmLoc.x>-1+unitWidth/2)
        {
            enmLoc.x -= t;
         }   moveDis +=t;

        xmin +=1/(float)frames;
        xmax +=1/(float)frames;
        ymin =0.75;
        ymax =1.0;

        if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
            }
   }

   else if(up)
   {
       if(enmLoc.x<1-unitWidth/2)
          {
              enmLoc.x += t;
           }   moveDis +=t;

            xmin +=1/(float)frames;
            xmax +=1/(float)frames;
            ymin =0.5;
            ymax =0.75;

            if(xmax>1){
                xmax =1/(float)frames;
                xmin =0;
              }
     }

  }else T1->Reset();
}
 else {moveDis =0;down=up=left=right=false; }
}


GridLoc Enemies::getEnemyLoc()
{
   GridLoc val;
   val.x = (int)(ceil((enmLoc.x +(1-unitWidth))/unitWidth));
   val.y = (int)(ceil((enmLoc.y +(1-unitWidth))/unitWidth));

    return val;
}

