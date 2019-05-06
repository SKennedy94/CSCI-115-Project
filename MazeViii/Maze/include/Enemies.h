/*
 * MAZE Game Framework
 * Written by Dr. Dhanyu Amarasinghe Spring 2018
 */
#ifndef ENEMIES_H
#define ENEMIES_H

#include<CommonThings.h>
#include <Maze.h>
#include <time.h>
#include <thread>
#include <string>
#include <math.h>       /* ceil */

enum State {WANDER,CHASE};
enum Inter {UR,UL,DR,DL,URL,LUD,DRL,RUD,A,N};

class Enemies
{
    public:
        Enemies();                          // constructor
        virtual ~Enemies();                 // DeConstructor
        void initEnm(int, int, char *);     // Initialize Enemy

        void placeEnemy(int,int);           // place enemy
        void drawEnemy();                   // Draw Enemy with Animation
        void moveEnemy(int [25][25]);             // move Enemy left,right,up,down
        void animate();                     // Animate sprite
        GridLoc getEnemyLoc();              // Return Enemy current grid location
        int nextMove;                       // 0-3 {N,E,S,W} used to determine random movement
        State eState;                       // logic state enemy is in
        GridLoc ePos;                       // enemy position in array
        GridLoc prevPos;
        string dir = "right";


        int gridSize;                       // Grid size of the maze
        float unitWidth;                    // Unit width of the grid
        int stepsPerMove;                   // Step count for animation
        bool live;                          // Status of the Enemy
    protected:

    private:
         int frames;                        // number of frames generally
         float t;                           // steps for animation count
         GLuint enmTex;                     // Image Texture

         float xmax, xmin,ymax,ymin;        // Animated Texture mapping
         bool up,down,left,right;           // move direction
         float moveDis=0;                   // Moving distance for animation
         loc enmLoc;                        // location of the enemy
         void Wander(int [25][25]);
         Inter isIntersection(int [25][25]);
};

#endif // ENEMIES_H
