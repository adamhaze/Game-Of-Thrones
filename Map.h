
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "City.h"
using namespace std;

#ifndef MAP_H
#define MAP_H

const static int maprows = 35;
const static int mapcols = 26;

class Map
{
    public:
    Map();
    void setMapArray(int row, int col, string value);
    string getMapLocation(int row, int col);
    void captureIt(int row, int col, int hero);
    int isCaptured(int row, int col);   
    void setDragonGlass(int row, int col);
    bool getDragonGlass(int row, int col);
    void displayMap(int row, int col);
    void displayMapEntire();
    int countTiles(int idx);

    private:
    string map[maprows][mapcols];
    int capture[maprows][mapcols];
    bool dragonglass[maprows][mapcols]; 
    
};

#endif