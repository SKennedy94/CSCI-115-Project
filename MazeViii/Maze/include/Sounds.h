#ifndef SOUNDS_H
#define SOUNDS_H

#include <SNDS/irrKlang.h>

#pragma comment (lib,"irrKlang.lib") // link your dll to the program

using namespace irrklang;

class Sounds
{
    public:
        Sounds();
        virtual ~Sounds();

        void playMusic(char*);
        void playSound(char*);
        void stopAllSounds();

    protected:

    private:
};

#endif // SOUNDS_H
