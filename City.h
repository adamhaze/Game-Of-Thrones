
#include <iostream>
#include <string>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <math.h>
#include <vector>
using namespace std;

#ifndef CITY_H
#define CITY_H

class City
{
    public:
    City();
    void setLetter(char letter);
    char getLetter();
    void setName(string city);
    string getName();
    void setCityRating(int money, int army, int points);
    int getCityRating(int loc);

    private:
    char letter;
    string name;
    int cityRatings[3]; //money, army size, points
};

#endif 