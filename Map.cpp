
#include <iostream>
#include "City.h"
#include "Map.h"
using namespace std;

Map::Map(){
    for(int i = 0; i < maprows; i++){
        for(int j = 0; j < mapcols; j++){
            capture[i][j] = -1;
        }
    }
    for(int i = 0; i < maprows; i++){
        for(int j = 0; j < mapcols; j++){
            dragonglass[i][j] = false;
        }
    }
}

void Map::setMapArray(int row, int col, string value){
    map[row][col] = value;
}

string Map::getMapLocation(int row, int col){
    return map[row][col];
}

void Map::captureIt(int row, int col, int hero){
    capture[row][col] = hero;
}
    
int Map::isCaptured(int row, int col){
    return capture[row][col];
}
void Map::setDragonGlass(int row, int col){
    if(capture[row][col] == -1 && dragonglass[row][col] == false){
        dragonglass[row][col] = true;
    }
    else{
        dragonglass[row][col] = false;
    }
    
}
bool Map::getDragonGlass(int row, int col){
    return dragonglass[row][col];
}

void Map::displayMap(int row, int col){
    for(int i = row-5; i <= row+5; i++){
        for(int j = col-5; j <= col+5; j++){
            if(i < 0 || i >= maprows || j < 0 || j >= mapcols){
                cout<<"x";
            }
            else if(i == row && j == col){
                cout <<"@";
            }
            else if(map[i][j] == "w"){
                cout<<'~';
            }
            else if(map[i][j] == "p"){
                cout<<'*';
            }
            else{
                cout<<map[i][j];
            }
        }
        cout<<endl;
    }
    cout<<endl;
}

void Map::displayMapEntire()
{
    for (int i = 0; i < maprows; i++)
    {
        for (int j = 0; j < mapcols; j++)
        {
            if (map[i][j] == "w"){
                cout << '~';
            } else if (map[i][j] == "p"){
                cout << '*';
            } else{
                cout << map[i][j];
            }
        }
        cout << endl;
    }
}

int Map::countTiles(int idx){
    int cnt=0;
    for(int i = 0; i < maprows; i++){
        for(int j = 0; j < mapcols; j++){
            if(capture[i][j] == idx){
                cnt++;
            }
        }
    }
    return cnt;
}