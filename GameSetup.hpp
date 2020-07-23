
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
#include "City.h"
#include "Map.h"
#include "Helpers.hpp"
using namespace std;



class Game
{
    public:
    
    Game();
    void loadMap(string file);
    void loadHeroes(string file);
    void loadWarriors(string file);
    void loadCities(string file);
    void loadFreeWarriorsLocation(int idx);
    
    void checkDragonglass(int idx);
    void travelMain();
    void restUser();
    void consultGodsUser();
    void moveHelp(int idx, bool hero);
    void moveNonMainHeroes();
    void moveFreeWarriors();

    void changeTileControl(int w, int l);
    Hero acquireWarriors(Hero w, Hero l);
    void retireOpponent(int w, int l);
    void encounterUser(Hero h);
    void encounterFreeWarrior();
    void encounterComputer(int a, int b);
    void runEncounterComputer();

    int closestHero();
    void newCity(int c);
    void afterEncounter();
    void endOfTurn();
    void turnTen();
    void whiteWalkerBattle();
    void updateWarriors();
    bool turnCheck();

    void mainGame();

    private:

    vector<Hero> heroes;
    vector<Warrior> warriors;
    vector<City> cities;
    Map westeros;
    Helpers help;

    string userName;
    int numHeroesGame;
    int choiceHero;
    bool win;
    bool lose;
    int numWarriorsGame;
    int turnNumber;

};
