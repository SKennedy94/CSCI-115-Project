#ifndef SCENE_H
#define SCENE_H

#include <GL/gl.h>

class Scene
{
    public:
        Scene();
        virtual ~Scene();

        void drawScene();
        void bindTexture(char* string);

    protected:

    private:

    GLuint texture;
};

#endif // SCENE_H
