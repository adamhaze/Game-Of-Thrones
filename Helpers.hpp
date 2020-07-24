#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "City.h"
#include "Map.h"
using namespace std;

struct Warrior
{
    string name = "";
    int ratingsWarrior[3]{}; //strength,loyalty,morale
    bool boolWarrior[3]{};   //free,ship,dragonglass
    int freeWarriorLocation[2]{}; 
};

struct Hero
{
    string name = "";
    int location[2]{}; //row and column values
    int ratings[3]{};  //money,influence,army size
    Warrior heroWarriors[4]{};
    bool ship;
    int heroPoints;
    int numHeroWarriors = 0;
    bool traveled[maprows][mapcols];
};

class Helpers
{
    public:

    Helpers();
    string welcome();
    string askUser();
    int heroOptionsMenu(); // sets user's hero index for hero vector
    int displayTurnChoices();
    void printHeroRatings(Hero h);
    void printWarriorRatings(Warrior w);
    void createUserHero(Map westeros);
    void startTurn(Hero h);
    bool proximityCheck(int row1, int col1, int row2, int col2);
    bool checkValid(Hero h, string loc);
    int encounterChoices(Hero h);
    void turn10prints();
    void p1();
    void p2();
    void nextTurn();

    private:
    string userName;
    int heroidx;
};
