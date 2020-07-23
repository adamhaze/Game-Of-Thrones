
#include <iostream>
#include "City.h"
using namespace std;

City::City(){
    letter = ' ';
    name = "";
}

void City::setLetter(char let){
    letter = let;
}

char City::getLetter(){
    return letter;
}
void City::setName(string city){
    name = city;
}
string City::getName(){
    return name;
}
void City::setCityRating(int money, int army, int points){
    cityRatings[0] = money;
    cityRatings[1] = army;
    cityRatings[2] = points;
}

int City::getCityRating(int loc){
    return cityRatings[loc];
}