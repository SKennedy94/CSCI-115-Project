#include "Sounds.h"
#include <iostream>

ISoundEngine* engine = createIrrKlangDevice();

Sounds::Sounds()
{
    engine->setSoundVolume(.5);
    //ctor
}

Sounds::~Sounds()
{
    //dtor
    engine->drop();
}

void Sounds::playMusic(char* fileName) // play music
{
    engine->play2D(fileName, true);
}

void Sounds::playSound(char* fileName) // play sound
{
    engine->play2D(fileName);
}

void Sounds::stopAllSounds() // stop sound
{
    engine->stopAllSounds();
}

