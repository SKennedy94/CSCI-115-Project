#include "Scene.h"
#include "CommonThings.h"

using namespace std;

Scene::Scene()
{
    //ctor
}

Scene::~Scene()
{
    //dtor
}

void Scene::bindTexture(char* fileName){
    texture = TextureLoader(fileName);
}

void Scene::drawScene(){

    glColor3f(1.0,1.0,1.0);
    glBindTexture(GL_TEXTURE_2D,texture);

    glBegin(GL_QUADS);

        glTexCoord2f(0.0f, 1.0f);
        glVertex3f(-1.0f,-1.0f, 1.0f);

        glTexCoord2f(1.0f, 1.0f);
        glVertex3f( 1.0f,-1.0f, 1.0f);

        glTexCoord2f(1.0f, 0.0f);
        glVertex3f( 1.0f, 1.0f, 1.0f);

        glTexCoord2f(0.0f, 0.0f);
        glVertex3f(-1.0f, 1.0f, 1.0f);

     glEnd();
}
