

#include <Bush.h>
#include <iostream>

using namespace std;


Bush::Bush()
{
    //ctor
    liveWall = true;
}

Bush::~Bush()
{
    //dtor
}

void Bush::wallInit(int grid,char* FileName)
{
    gridSize = grid;
    unitWidth = (float)2/grid;
    wallTex = TextureLoader(FileName);
}

void Bush::drawWall()
{
    if(liveWall)
    {

    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,wallTex);

    glPushMatrix();
    glTranslatef(wallBrk.x,wallBrk.y,0.0);

    glScalef(1.0/(float)gridSize,1.0/(float)gridSize,1);


     glBegin(GL_POLYGON);
            glTexCoord2f(0,0);
            glVertex3f(1,1,0.0f);

            glTexCoord2f(1,0);
            glVertex3f(-1,1,0.0f);

            glTexCoord2f(1,1);
            glVertex3f(-1,-1,0.0f);

            glTexCoord2f(0,1);
            glVertex3f(1,-1,0.0f);
    glEnd();
    glPopMatrix();
    }
}

void Bush::placeWall(int x, int y)
{
    GetWallLoc.x= x;
    GetWallLoc.y= y;

    wallPos.x = x;
    wallPos.y = y;

    x+=1;
    y+=1;

    wallBrk.x =  (unitWidth)*x-1-unitWidth/2;
    wallBrk.y =  (unitWidth)*y-1-unitWidth/2;
}




