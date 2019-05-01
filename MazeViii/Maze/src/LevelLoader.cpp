#include "LevelLoader.h"
#include <fstream>

LevelLoader::LevelLoader()
{
    //ctor
}

LevelLoader::~LevelLoader()
{
    //dtor
}

void LevelLoader::load(string fileName){
    ifstream file;
    file.open(fileName);

    if (file.good()){
        string line;
        getline(file,line);
        int s = stoi(line);
        M->setGridSize(s);
        M->setUnitWidth((float)2/s);
        cout<<M->getGridSize()<<endl;
    }
    char index;
    file >> index;
    for (int i=0; i<M->getGridSize(); i++){
        for (int j=0; j<M->getGridSize(); j++){
            if(file.eof()){
                    cout<<"case eof"<<endl;
                break;
            }
            else if(index == '0'){
            }
            else if(index == '1'){
                matrix[i][j]=1;
                W[wallCount].wallInit(M->getGridSize(),"images/wall.png");
                W[wallCount].placeWall(i,j);
                W[wallCount].liveWall = true;
                wallCount++;
            }
            else if(index == '2'){
                matrix[i][j]=0;
                E[enemyCount].initEnm(M->getGridSize(),4,"images/e.png");
                E[enemyCount].placeEnemy(i,j);
                enemyCount++;
            }
            else if(index == '4'){
                matrix[i][j]=0;
                P->initPlayer(M->getGridSize(),6,"images/p.png");
                P->placePlayer(i,j);
            }
            cout<<index<<" ";
            file >> index;
        }
        cout<<endl;
    }
    cout<<"level Loaded"<<endl;
    file.close();
}

void LevelLoader::reset(){
    for (int i=0; i<M->getGridSize(); i++){
        for (int j=0; j<M->getGridSize(); j++){
            matrix[i][j] = 0;
        }
    }
    for(int i=0; i<wallCount;i++){
        W[i].liveWall = false;
    }
    for(int i=0; i<enemyCount;i++){
        E[i].live = false;
    }

    // reset variables
    wallCount = 0;
    enemyCount = 0;
}
