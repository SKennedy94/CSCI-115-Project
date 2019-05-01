#ifndef LEVELLOADER_H
#define LEVELLOADER_H

#include <string>
#include <Maze.h>
#include <wall.h>
#include <Enemies.h>
#include <player.h>

class LevelLoader
{
    public:
        LevelLoader();
        virtual ~LevelLoader();

        Maze *M = new Maze(20);
        Enemies E[20];
        wall W[400];                         // Set Maze grid size
        Player *P = new Player();
        int matrix[20][20];
        void load(string);
        void reset();

        int wallCount=0, enemyCount=0;
    protected:

    private:
        int _currentLevel = 0;
};

#endif // LEVELLOADER_H
