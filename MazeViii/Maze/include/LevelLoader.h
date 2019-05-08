#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <Maze.h>
#include <wall.h>
#include <Enemies.h>
#include <player.h>
#include <Bush.h>

class LevelLoader
{
    public:
        LevelLoader();
        virtual ~LevelLoader();

        Maze *M = new Maze(25);
        Enemies E[20];
        wall W[400];                         // Set Maze grid size
        Player *P = new Player();
        Bush B[100];

        int matrix[25][25];
        void load(string);
        void reset();

        int levelSelector = 1;

        int wallCount=0, enemyCount=0; int bushCount=0;
    protected:

    private:
        int _currentLevel = 0;
};

#endif // LEVELLOADER_H
