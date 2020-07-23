
#include <iostream>
#include <fstream>
#include "Helpers.hpp"
using namespace std;

Helpers::Helpers(){
    userName = "";
    heroidx = -1;
}
string Helpers::welcome()
{
    string user;
    cout << endl;
    cout << endl;
    cout << "Welcome to the G A M E of T H R O N E S!" << endl;
    cout << "" << endl;
    cout << "State your name, Your Grace: " << endl;
    cin >> user;
    userName = user; // set callable username for helper functions
    return user;
}

string Helpers::askUser()
{
    string userChoice;
    cout << endl;
    cout << "Would you like to choose a hero " << userName << ", Your Grace?(Y/N): " << endl;
    cin >> userChoice;
    return userChoice;
}

int Helpers::heroOptionsMenu()
{
    string choice;
    cout << endl;
    cout << "Please choose from the following heroes:" << endl;
    cout << "" << endl;
    cout << "1. Cersei Lannister" << endl;
    cout << "2. Stannis Baratheon" << endl;
    cout << "3. Jon Snow" << endl;
    cout << "4. Sansa Stark" << endl;
    cout << "5. Euron Greyjoy" << endl;
    cout << "6. Daenerys Targaryen" << endl;
    cin >> choice;
    heroidx = stoi(choice) - 1;
    return heroidx;
}


int Helpers::displayTurnChoices()
{
    string turnChoice;
    cout << userName << ", you have 3 different choices for this turn: " << endl;
    cout << "" << endl;
    cout << "1. Travel" << endl;
    cout << "2. Rest" << endl;
    cout << "3. Consult with the gods" << endl;
    cin >> turnChoice;
    return stoi(turnChoice);
}

void Helpers::printHeroRatings(Hero h){
    string ship;
    if(h.ship == true){
        ship = "yes";
    }
    else{
        ship = "no";
    }
    cout << "Name: " << h.name << endl;
    cout << "Money: " << h.ratings[0] << endl;
    cout << "Influence: " << h.ratings[1] << endl;
    cout << "Army Size: " << h.ratings[2] << endl;
    cout << "Location: row = " << h.location[0] << " column = " << h.location[1] << endl;
    cout << "Ship? " << ship << endl;
    cout << "Total Points: " << h.heroPoints << endl;
}

void Helpers::printWarriorRatings(Warrior w){
    string free = "no";
    string ship = "no";
    string dragonglass = "no";
    if(w.boolWarrior[0] == true){
        free = "yes";
    }
    if(w.boolWarrior[1] == true){
        ship = "yes";
    }
    if(w.boolWarrior[2] == true){
        dragonglass = "yes";
    }
    cout << "Name: " << w.name << endl;
    cout << "Strength: " << w.ratingsWarrior[0];
    cout << " Loyalty: " << w.ratingsWarrior[1];
    cout << " Morale: " << w.ratingsWarrior[2] << endl;
    cout << "Free? " << free << " Ship? " << ship << " Dragonglass? " << dragonglass << endl;
    if(free == "yes"){
        cout << "Location: (" << w.freeWarriorLocation[0] <<","<<w.freeWarriorLocation[1]<<")"<<endl;
    }
    cout << endl;
}

void Helpers::createUserHero(Map westeros){
    string money,influence,army,row,col,ship;
    bool ship_bool = false;
    int total = 1500;
    cout << endl;
    cout << "Your Grace " << userName << ", you have chosen to go on this journey alone." << endl;
    cout << endl;
    cout << "You are alotted a total of 1500 skillpoints to distribute amongst your hero's attributes. ";
    cout << "You may also choose to begin the game with possession of a ship which grants you the ability to travel on water. ";
    cout << "However, this will cost you 300 skillpoints. Please choose wisely." << endl;
    cout << endl;
    bool valid = false;
    while(valid == false){
        cout << "Alright your Grace, would you like to begin the game with a ship?" << endl;
        cin >> ship;
        if(ship == "Y" || ship == "y" || ship == "yes" || ship == "Yes"){
            total = total - 300;
            ship_bool = true;
            ship = "yes";
        } else{
            ship = "no";
        }
        cout << endl;
        cout << "The 3 attributes you will now assign are Money, Influence, and Army Size." << endl;
        cout << endl;
        cout << "Money: " << endl;
        cin >> money;
        total -= stoi(money);

        cout << "Influence: " << endl;
        cin >> influence;
        total -= stoi(influence);

        cout << "Army Size: " << endl;
        cin >> army;
        total -= stoi(army);
        if(total < 0){
            cout << "I'm sorry, " << userName << " but you have exceeded your alotted skillpoints. Please try again." << endl;
            cout << endl;
        }
        else{
            valid = true;
        }
    }
    cout<<endl;
    cout << "You must now choose a starting location on the map, however you can not start at the same location as any other hero." << endl;
    cout << "You may NOT choose any of the following locations: (23,9) OR (19,11) OR (3,4) OR (9,4) OR (19,1) OR (25,22)" << endl;
    cout << endl;
    bool located = false;
    while(located == false){
        cout << "Row: " << endl;
        cin >> row;
        cout << "Column: " << endl;
        cin >> col;
        if(ship_bool == true){
            located = true;
        }
        else{
            string loc = westeros.getMapLocation(stoi(row)-1,stoi(col)-1);
            if(loc == "w") {
                cout << "This is a water tile, and you do not possess a ship. Try again." << endl;
                cout<<endl;
                continue;
            }
            else{located = true;}
        }
    }
    ofstream heroFile("text_files/heroesGOT.txt",fstream::app);
    if(heroFile.is_open()){
        heroFile << userName << "," << money << "," << influence << "," << army << ",NULL,NULL,NULL,NULL," << row << "," << col << "," << ship << endl;
    }
    heroFile.close();
}

void Helpers::startTurn(Hero h){
    cout << "" << endl;
    cout << "Here are your current ratings, your grace:" << endl;
    cout << "" << endl;
    cout << "Money: " << h.ratings[0] << endl;
    cout << "Army Size: " << h.ratings[2] << endl;
    cout << "Influence: " << h.ratings[1] << endl;
    cout << "Total Points: " << h.heroPoints << endl;
    cout << "" << endl;
    cout << "" << endl;
    cout << "Here are the current ratings of the warriors in your party: " << endl;
    for(int i=0; i<h.numHeroWarriors; i++){
        printWarriorRatings(h.heroWarriors[i]);
    }
    cout << endl;
    cout << endl;
    cout << "Here is where you are on the map:" << endl;
    cout << endl;
}

bool Helpers::proximityCheck(int row1, int col1, int row2, int col2)
{
    int rowdif = abs(row1 - row2);
    int coldif = abs(col1 - col2);
    if (rowdif <= 2 && coldif <= 2){
        return true;
    } else{
        return false;
    }
}

bool Helpers::checkValid(Hero h, string loc)
{
    if (loc == "w" && h.ship == false)
    {
        return false;
    }
    else
    {
        return true;
    }
}

int Helpers::encounterChoices(Hero h){
    string choice;
    cout << endl;
    cout << endl;
    cout << "Your grace, you have encountered " << h.name;
    cout << ", an opposing hero. What would you like to do?" << endl;
    cout << endl;
    cout << "1. Give a speech" << endl;
    cout << "2. Buy them outright" << endl;
    cout << "3. Fight to the death" << endl;
    cin >> choice;
    return stoi(choice);

}

void Helpers::turn10prints(){
    string respects;
    bool paid = false;
    cout << endl;
    cout << "You have now made it through 10 days in Westeros, and certain events are taking place." << endl;
    cout << endl;
    cout << "1. There is confirmation that Daenerys Targaryen now has Dragons, and she has acquired a massive army." << endl;
    cout << "2. Stannis Baratheon has been consumed by his demons, and has retired from the game." << endl;
    while(paid == false){
        cout << "Press F to pay respects." << endl;
        cin >> respects;
        if(respects == "f" || respects == "F"){
            cout << "Thank you, your Grace." << endl;
            paid = true;
        } else{
            cout << "Have you no respect?! ";
        }
    }
    cout << "3. Arya Stark has found her sister Sansa, and has joined her cause." << endl;
    cout << "4. Jaime Lannister has abandoned his dear sister Cersei." << endl;
    cout << "5. The bastard in the North, Jon Snow, is gaining influence in Westeros." << endl;
    cout << endl;
}   

void Helpers::p1(){
    cout << endl;
    cout << "Hold on while the other heroes and warriors travel..." << endl;
    cout << endl;
}
void Helpers::p2(){
    cout << endl;
    cout << "Resolving all possible encounters..." << endl;
    cout << endl;
}
void Helpers::nextTurn(){
    for(;;){
        string val;
        cout << endl;
        cout << "Please press 'N' to proceed to your next turn..." << endl;
        cin >> val;
        if(val == "N" || val == "n"){
            for(int i=0; i < 5; i++){
                cout << endl;
            }
            return;
        }
    }

}
