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

void Enemies::moveEnemy(int matrix[25][25])
{
   for (int i=0; i<10; i++){
        for (int j=0; j<10; j++){
                cout<<matrix[i][j]<<" ";
        }
        cout<<endl;
   }

    prevPos = ePos;
    //if(eState == WANDER){
    Wander(matrix);
    //}
  if(moveDis<=0){
   if(dir=="up"){up=true; down=left=right=false;}
   else if(dir=="down"){down=true; up=left=right=false;}
   else if(dir=="left"){left=true; down=up=right=false;}
   else if(dir=="right"){right=true; down=left=up=false;}
   else{up=left=right=false;}
    }
    cout<<ePos.x<<" , "<<ePos.y<<endl;
}

Inter Enemies::isIntersection(int matrix[25][25]){
    if(matrix[ePos.x+1][ePos.y] != 1 && matrix[ePos.x-1][ePos.y] != 1 && matrix[ePos.x][ePos.y-1] != 1 && matrix[ePos.x][ePos.y+1] != 1){cout<<"A intersection"<<endl; return A;}
    else if(matrix[ePos.x][ePos.y+1]!= 1 && matrix[ePos.x+1][ePos.y]!= 1&& matrix[ePos.x-1][ePos.y]!= 1){cout<<"URL intersection"<<endl; return URL;}        //URL
    else if(matrix[ePos.x][ePos.y-1]!= 1 && matrix[ePos.x+1][ePos.y]!= 1&& matrix[ePos.x-1][ePos.y]!= 1){cout<<"DRL intersection"<<endl; return DRL;}        //DRL
    else if(matrix[ePos.x][ePos.y+1]!= 1 && matrix[ePos.x][ePos.y-1]!= 1&& matrix[ePos.x-1][ePos.y]!= 1){cout<<"LUD intersection"<<endl; return LUD;}        //LUD
    else if(matrix[ePos.x][ePos.y+1]!= 1 && matrix[ePos.x][ePos.y-1]!= 1 && matrix[ePos.x+1][ePos.y]!= 1){cout<<"RUD intersection"<<endl; return RUD;}        //RUD
    else if(matrix[ePos.x+1][ePos.y] != 1 && matrix[ePos.x][ePos.y+1]!= 1){cout<<"UR intersection"<<endl; return UR;}         //up-right
    else if(matrix[ePos.x-1][ePos.y]!= 1 && matrix[ePos.x][ePos.y+1]!= 1){cout<<"UL intersection"<<endl; return UL;}         //up-left
    else if(matrix[ePos.x-1][ePos.y]!= 1 && matrix[ePos.x][ePos.y-1]!= 1){cout<<"DL intersection"<<endl; return DL;}         //down-left
    else if(matrix[ePos.x+1][ePos.y]!= 1 && matrix[ePos.x][ePos.y-1]!= 1){cout<<"DR intersection"<<endl; return DR;}         //down-right
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
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "up";}
                else {dir = "right";}
            break;

            case UL:
                nextMove = rand()%2;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "up";}
                else {dir = "left";}
            break;

            case DL:
                nextMove = rand()%2;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "left";}
                else {dir = "down";}
            break;
            case DR:
                nextMove = rand()%2;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "right";}
                else {dir = "down";}
            break;

            case URL:
                nextMove = rand()%3;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "up";}
            break;
            case DRL:
                nextMove = rand()%3;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "down";}
            break;
            case LUD:
                nextMove = rand()%3;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "down";}
                else if(nextMove == 1) {dir = "left";}
                else {dir = "up";}
            break;
            case RUD:
                nextMove = rand()%3;
                cout<<nextMove<<endl;
                if (nextMove == 0) {dir = "right";}
                else if(nextMove == 1) {dir = "down";}
                else {dir = "up";}
            break;
            case N:
            break;
        }
// HANDLE DEAD ENDS
if(inter == N){
    if(dir=="right" && matrix[ePos.x+1][ePos.y]!= 0){
        dir = "left";
    }
    else if(dir=="left" && matrix[ePos.x-1][ePos.y]!= 0){
        dir = "right";
    }
    else if(dir=="up" && matrix[ePos.x][ePos.y+1]!= 0){
        dir = "down";
    }
    else if(dir=="down" && matrix[ePos.x][ePos.y-1]!= 0){
        dir = "up";
    }
}
// HANDLES MOVING
    matrix[ePos.x][ePos.y] = 0;
    if(dir == "right"){
        if(matrix[ePos.x+1][ePos.y] == 0){        //Right
            matrix[ePos.x+1][ePos.y] = 2;
            ePos.x+=1;
        }
    }
    else if(dir == "left"){
        if(matrix[ePos.x-1][ePos.y] == 0){        //Right
            matrix[ePos.x-1][ePos.y] = 2;
            ePos.x-=1;
        }
    }
    else if(dir == "up"){
        if(matrix[ePos.x][ePos.y+1] == 0){        //Right
            matrix[ePos.x][ePos.y+1] = 2;
            ePos.y+=1;
        }
    }
    else if(dir == "down"){
        if(matrix[ePos.x][ePos.y-1] == 0){        //Right
            matrix[ePos.x][ePos.y-1] = 2;
            ePos.y-=1;
        }
    }
    else {
        ePos = prevPos;
        dir = "";
    }

}

void Enemies::animate()
{

if(moveDis < unitWidth)
{

if(T1->GetTicks()<1000)
   {

if(up)
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

  else if(down)
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
   else if(left)
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

   else if(right)
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

