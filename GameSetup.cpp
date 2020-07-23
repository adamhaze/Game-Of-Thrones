
#include <iostream>
#include <fstream>
#include <string>
#include <locale>
#include <algorithm>
#include "City.h"
#include "Map.h"
#include "GameSetup.hpp"
using namespace std;

int split(string words, char delim, string array[]){
    if (words.length() == 0) { 
        return 0; 
    }
    string word = ""; 
    int j = 0; //initialize counter for num of words
    //array[]; 
    words = words + delim; //states there is a delimiter at end of sentence
    for (int i = 0; i < words.length(); i++){ 
        if (words[i] == delim) { //checks if current item is delimiter
        	if (word.length() == 0) continue; //moves on if so
            array[j] = word; //fills current spot in array with full word
            
            j++; //adds on to word counter, keeps track of current spot in array
            word = ""; //resets word so you can accumulate the next word without the previous
        } 
        else { //if not delimiter
            word = word + words[i]; //accumulates each character until you hit delimiter
        }
        
    }
    return j ; //returns num of words
}
int randomRow(){
    return rand() % 25;
}
int randomCol()
{
    return rand() % 16;
}
int randomMovement()
{
    return rand() % 4;
}
int probability(){
    return rand() % 100;
}

//default constructor for Game class
//initializes all private variables
Game::Game(){

    choiceHero = -1;
    numHeroesGame = 0;
    win = false;
    lose = false;
    numWarriorsGame = 14;
    turnNumber = 0;
    
}
void Game::loadMap(string file){
    ifstream myFile; 
    myFile.open(file); //opens the inputted file
    string line = "";
    string split_map[mapcols];
    int i = 0;
    if(myFile.is_open()){
        while(getline(myFile,line)){
            split(line, ',', split_map);
            for(int j = 0; j < mapcols; j++){
                westeros.setMapArray(i, j, split_map[j]);
            }
            i++;
        }
    }
    // set dragonglass in 5 random locations that are unoccupied
    int d = 0;
    while(d<5){
        int row = randomRow();
        int col = randomCol();
        if(westeros.getMapLocation(row,col) != "w" && westeros.isCaptured(row,col) == -1){ 
            westeros.setDragonGlass(row,col);
            d++;
        }
    }
}

/*
1. opens and reads through file containing all the heroes
2. fills a vector of Hero objects with the information on the line that the Hero is located
*/
void Game::loadHeroes(string file){
    ifstream myFile;
    myFile.open(file); //opens the inputted file
    string line = "";
    if (myFile.is_open())
    {
        while (getline(myFile, line))
        {
            Hero killer;
            string split_hero[11];
            split(line, ',', split_hero); // populates array with each individual quality of hero on current line
            killer.name = split_hero[0];
            for (int i = 1; i < 4; i++)
            {
                killer.ratings[i - 1] = stoi(split_hero[i]);
            }
            int numW = 0; // counter for number of warriors a hero starts with
            for (int i = 4; i < 8; i++)
            { // assign each hero there initial warriors
                for (int w = 0; w < 14; w++)
                {
                    if (split_hero[i] == warriors[w].name)
                    {
                        killer.heroWarriors[i-4] = warriors[w];
                        numW++;
                    }
                }
            }
            killer.numHeroWarriors = numW;
            killer.location[0] = stoi(split_hero[8]) - 1;
            killer.location[1] = stoi(split_hero[9]) - 1;
            killer.traveled[stoi(split_hero[8]) - 1][stoi(split_hero[9]) - 1] = true;
            char loc = westeros.getMapLocation(killer.location[0], killer.location[1])[0];

            if (int(loc) >= 65 && int(loc) <= 90)
            { // if hero starts on a city, give that hero proper ratings increase and points
                for (int i = 0; i < 26; i++)
                {
                    if (loc == cities[i].getLetter())
                    {
                        killer.ratings[0] += cities[i].getCityRating(0);
                        killer.ratings[2] += cities[i].getCityRating(1);
                        killer.heroPoints = cities[i].getCityRating(2);
                        break;
                    }
                }
            } else if (loc == 'p'){ // land tile
                killer.heroPoints = 2;
                killer.ratings[0] += 20;
                killer.ratings[1] += 10;
                killer.ratings[2] += 10;
            } else{ // water tile
                killer.heroPoints = 1;
                killer.ratings[0] += 20;
                killer.ratings[1] += 10;
            }

            // if hero starts with ships or not
            if (split_hero[10] == "yes"){
                killer.ship = true;
            } else{
                killer.ship = false;
            }
            // mark current location as traveled
            heroes.push_back(killer);
        } 
    }
}

/*
1. opens and reads through file containing all the warriors
2. fills a vector of Warrior objects with the information on the line that the Warrior is located
*/
void Game::loadWarriors(string file){
    ifstream myFile; 
    myFile.open(file); //opens the inputted file
    string line = "";
    if(myFile.is_open()){
        while(getline(myFile,line)){ //loops through each line of warriors file(i.e. all 14 warriors)
            Warrior fighter; //initialize warrior object
            string split_warrior[7];
            split(line, ',', split_warrior); //populates array with each individual quality of warrior on current line
            fighter.name = split_warrior[0];
            fighter.ratingsWarrior[0] = stoi(split_warrior[1]);
            fighter.ratingsWarrior[1] = stoi(split_warrior[2]);
            fighter.ratingsWarrior[2] = stoi(split_warrior[3]);

            // freedom, ship, dragonglass
            for (int j = 4; j < 7; j++)
            { 
                if(split_warrior[j] == "yes"){
                    fighter.boolWarrior[j-4] = true;
                }
                else{
                    fighter.boolWarrior[j-4] = false;
                }
            }
            warriors.push_back(fighter);
        }
    }
}

// takes index of warrior in warriors vector
// checks if they have a ship
// if so, assigns them to random water location on the map that is not currently occupied by another hero/warrior
// if no ship, assigns them to random land location on the map that is not currently occupied by another hero/warrior
// assigns value to capture array indicating their is a warrior at this location
// checks if this location has dragonglass, assigns the warrior dragonglass if so and removes dragonglass from that location
void Game::loadFreeWarriorsLocation(int idx){

    bool water = warriors[idx].boolWarrior[1]; // can warrior travel on water or not
    bool located = false;
    while(located == false){
        bool occupied = false;
        int row = randomRow();
        int col = randomCol();
        for(int i=0; i < numHeroesGame; i++){
            if(row == heroes[i].location[0] && col == heroes[i].location[1]){
                occupied = true;
                break;
            }
        }
        if(occupied == true) continue;
        if(westeros.getMapLocation(row,col) == "w" && water == false) continue;
        warriors[idx].freeWarriorLocation[0] = row;
        warriors[idx].freeWarriorLocation[1] = col;
        // westeros.captureIt(row,col,100);
        located = true;
    }
}

/*
1. iterate through file containing information about each city
2. each line of the file becomes a city object, populate City vector
*/
void Game::loadCities(string file){
    ifstream myFile; 
    myFile.open(file); //opens the inputted file
    string line = "";
    
    if(myFile.is_open()){
        while(getline(myFile,line)){
            City A;
            string split_city[5];
            split(line, ',', split_city); //populates array with each individual quality of city on current line
            string letter = split_city[0];
            A.setLetter(letter[0]);
            A.setName(split_city[1]);
            A.setCityRating(stoi(split_city[2]),stoi(split_city[3]),stoi(split_city[4]));
            cities.push_back(A);
        }
    }
}


/*
1. If the user chooses to travel, prompts the user to choose which direction
2. checks if the user is able to travel in that direction
3. if they can't, tell them to pick a new direction
4. update the location of the main hero
*/

void Game::checkDragonglass(int idx){
    int row = heroes[idx].location[0];
    int col = heroes[idx].location[1];
    if(westeros.getDragonGlass(row,col) == true){
        int num = 0;
        cout << "Great news! You have stumbled upon Dragonglass. ";
        if (heroes[idx].heroWarriors[0].name == ""){
            cout << endl;
            cout << "Bad new is... you do not have any warriors to hold your Dragonglass. Return when you have aquired a warrior in your army." <<endl;
            return;
        }
        cout << "Choose which one of your warriors would you like to equip with Dragonglass (enter the NUMBER): " << endl;
        cout << endl;
        for(int i=0; i<heroes[idx].numHeroWarriors; i++){
            cout << i << ": " << heroes[idx].heroWarriors[i].name << endl; // print each hero warrior name with number, if no warriors -> no dragonglass
        }
        cin >> num;

        heroes[idx].heroWarriors[num].boolWarrior[2] = true;
        westeros.setDragonGlass(row,col);
    }
}

void Game::travelMain(){
    char direction;
    bool located = false;
    cout<<""<<endl;
    cout<<"Please choose the direction in which to travel (N,S,E,or W): "<<endl; //travel option for user
    cin>>direction; //saves chosen direction
    cout<<""<<endl;
    direction = tolower(direction);
    int row = heroes[choiceHero].location[0];
    int col = heroes[choiceHero].location[1];

    while(located == false){
        string destination;
        switch(direction){
            case 'n':
                row-=1;
                break;
            case 's':
                row+=1;
                break;
            case 'e':
                col+=1;
                break;
            case 'w':
                col-=1;
                break;
        }
        destination = westeros.getMapLocation(row, col);
        if (help.checkValid(heroes[choiceHero],destination) == false){
            cout << "You cannot travel here because you do not have ships, please choose a new direction: " << endl;
            cin >> direction; //new direction
            continue;
        }
        heroes[choiceHero].location[0] = row;
        heroes[choiceHero].location[1] = col;
        checkDragonglass(choiceHero);
        located = true;
    }
}

/*
1. if the user chooses to rest, do not change location of the hero
2. increase strength of each warrior in main hero's party by 1
*/

void Game::restUser(){
    bool rested=false;
    int cnt=0;
    while(rested == false){
        if(heroes[choiceHero].heroWarriors[cnt].name == ""){
            rested = true;
        } else{
            heroes[choiceHero].heroWarriors[cnt].ratingsWarrior[0] += 5;
            cnt += 1;
        }
    }
}

/*
1. if user chooses to consult, do not change location of main hero
2. increase morale of each warrior in main hero's party by 1
*/

void Game::consultGodsUser(){
    bool consult=false;
    int cnt=0;
    while(consult == false){
        if (heroes[choiceHero].heroWarriors[cnt].name == ""){
            consult = true;
        } else{
            heroes[choiceHero].heroWarriors[cnt].ratingsWarrior[2] += 5;
            cnt += 1;
        }
    }
}

void Game::moveHelp(int idx, bool hero){
    int row;
    int col;
    bool located = false;
    while(located == false){
        if(hero == true){
            row = heroes[idx].location[0];
            col = heroes[idx].location[1];
        } else {
            row = warriors[idx].freeWarriorLocation[0];
            col = warriors[idx].freeWarriorLocation[1];
        }
        int move = randomMovement();
        string dest;
        switch (move)
        {
        case 0:
            row -= 1; // North
            break;
        case 1:
            row += 1; // South
            break;
        case 2:
            col += 1; // East
            break;
        case 3:
            col -= 1; // West
            break;
        }
        if(row >= maprows || col >= mapcols) continue;

        dest = westeros.getMapLocation(row, col);
        if(hero == true && help.checkValid(heroes[idx], dest) == false) continue;
        else if(hero == false && dest == "w" && warriors[idx].boolWarrior[1] == false) continue;
        else{
            if(hero == true){
                heroes[idx].location[0] = row;
                heroes[idx].location[1] = col;
                if (westeros.getDragonGlass(row, col) && heroes[idx].heroWarriors[0].name != "")
                {
                    heroes[idx].heroWarriors[0].boolWarrior[2] = true;
                }
            } else{
                warriors[idx].freeWarriorLocation[0] = row;
                warriors[idx].freeWarriorLocation[1] = col;
                if (westeros.getDragonGlass(row, col))
                {
                    warriors[idx].boolWarrior[2] = true;
                }
            }
        }
        located = true;
    }
}

/*
1. iterate through each non-main hero and moves them one tile in a legal direction
2. update location of each non-main hero
*/
void Game::moveNonMainHeroes(){
    for(int i=0; i < numHeroesGame; i++){
        if(i == choiceHero) continue;
        moveHelp(i,true);
    }
}

/*
1 - iterate through each warrior checking if they are free
2 - if they are, assign them a random direction to travel on tile, check if that tile is legal
3 - once legal tile found, update that warriors location
*/

void Game::moveFreeWarriors(){
    for(int i=0; i<numWarriorsGame; i++){
        if(warriors[i].boolWarrior[0] == false) continue;
        moveHelp(i,false);
    }
}



/*
1. winner assigned control of each map tile currently controlled by loser
2. winner granted total points from all new tiles they gained control of
*/
void Game::changeTileControl(int win, int lose){
    int tot = 0;
    for(int i=0; i < maprows; i++){
        for(int j=0; j < mapcols; j++){
            if(westeros.isCaptured(i,j) == lose){
                westeros.captureIt(i,j,win);
                if(westeros.getMapLocation(i,j) == "p"){
                    tot += 2;
                } else if(westeros.getMapLocation(i,j) == "w"){
                    tot += 1;
                } else{
                    for(int c=0; c < 26; c++){
                        if(cities[c].getLetter() == westeros.getMapLocation(i,j)[0]){
                            tot += cities[c].getCityRating(2);
                            break;
                        }
                    }
                }
            }
        }
    }
    heroes[win].heroPoints += tot;
}

// NEED TO FREE WARRIORS THAT ARE NOT CHOSEN
Hero Game::acquireWarriors(Hero winner, Hero loser){
    bool usr = false;
    int loc = 0;
    int tot = winner.numHeroWarriors + loser.numHeroWarriors;
    if(winner.name == heroes[choiceHero].name){
        usr = true;
    }
    if(tot <= 4){ // winning hero can acquire all losing hero's warriors, without exceeding max of 4 warriors per hero
        cout << winner.name << " will now acquire all of " << loser.name <<"'s warriors." << endl;
        int cnt=winner.numHeroWarriors;
        for (int i = 0; i < loser.numHeroWarriors; i++){
            winner.heroWarriors[cnt] = loser.heroWarriors[i];
            winner.numHeroWarriors++;
            cnt++;
            if(cnt==4) break;
        }
    }
    else{
        if(usr == true){
            cout << "You must now choose which 4 warriors to keep: " << endl;
            for(int i=0; i < 8; i++){
                if(i < 4){
                    cout << i+1 << ": " << winner.heroWarriors[i].name << endl;
                } else{
                    cout << i+1 << ": " << loser.heroWarriors[i-4].name << endl;
                }
            }
            string choice;
            string num;
            cout << endl;
            for(int j=0; j<4; j++){
                if(j==0){
                    num = "first";
                } else if(j==1){
                    num = "second";
                } else if(j==2){
                    num = "third";
                } else{
                    num = "fourth";
                }
                cout << "Which warrior will be your " << num << " ?" << endl;
                cin >> choice;
                if(stoi(choice) <= 4){
                    winner.heroWarriors[j] = winner.heroWarriors[stoi(choice) - 1];
                }else{
                    winner.heroWarriors[j] = loser.heroWarriors[stoi(choice)-5];
                }
            }
        } else{
            int cnt = 0;
            for (int i = winner.numHeroWarriors; i < 4; i++){
                winner.heroWarriors[i] = loser.heroWarriors[cnt];
                cnt++;
            }
            
        }
        winner.numHeroWarriors = 4;
        
    }
    return winner;

}

/*
1 - takes free warrior in proximity of current hero, finds them in warrior array
2 - updates loyalty, morale, and freedom ratings for warrior
3 - adds that warrior to the current heroes army
*/

void Game::encounterFreeWarrior(){
    for(int idx=0; idx < numHeroesGame; idx++){
        if(heroes[idx].name == "") continue;
        if(heroes[idx].numHeroWarriors < 4){
            for(int i=0; i < numWarriorsGame; i++){
                if(warriors[i].boolWarrior[0] == false) continue;
                bool attainable = false;
                int row = warriors[i].freeWarriorLocation[0];
                int col = warriors[i].freeWarriorLocation[1];
                
                if(help.proximityCheck(heroes[idx].location[0], heroes[idx].location[1],row,col)){
                    attainable = true;
                    for(int j=0; j < numHeroesGame; j++){
                        if(j == idx) continue;
                        if(help.proximityCheck(heroes[j].location[0],heroes[j].location[1],row,col)){
                            attainable = false;
                            break;
                        }
                    }
                }
                if(attainable == true){   
                    warriors[i].boolWarrior[0] = false;
                    warriors[i].ratingsWarrior[1] += 10;
                    warriors[i].ratingsWarrior[2] += 10;
                    heroes[idx].heroWarriors[heroes[idx].numHeroWarriors] = warriors[i];
                    cout << heroes[idx].name << " has acquired roaming free warrior: " << warriors[i].name << endl;
                    heroes[idx].numHeroWarriors++;
                }
                
            }
        }
    }
}

// helper function to retire losing hero, and assign winning hero the losing hero's assets
void Game::retireOpponent(int w, int l){
    cout << heroes[l].name << " has retired from the game." << endl;
    cout << endl;
    heroes[w] = acquireWarriors(heroes[w], heroes[l]);
    changeTileControl(w, l);
    heroes[w].ratings[0] += heroes[l].ratings[0];
    heroes[w].ratings[1] += heroes[l].ratings[1];
    heroes[w].ratings[2] += heroes[l].ratings[2];
    Hero loser;
    heroes[l] = loser;
}

/*
1. Check if user is in proximity to any other hero
2. if so, announce which hero the user is encountering, and present user with 3 options for their encounter
3. This function includes functionality for every choice the user could make
4. Call the function which retires losing hero if the user wins
5. if main hero loses a battle the game ends, if they lose speech/buy then they only lose some stats
*/
void Game::encounterUser(Hero h){
    int opp = -1;
    for (int i = 0; i < numHeroesGame; i++){
        if(i == choiceHero) continue;
        if(help.proximityCheck(h.location[0],h.location[1],heroes[i].location[0],heroes[i].location[1])){
            opp = i;
            break;
        }
    }
    if(opp >= 0){
        int choice = help.encounterChoices(heroes[opp]);
        double winProb;
        int mor,inf,loy,money,army,strength,usr,cpu,cpuwar;
        bool battle = false;
        string war;
        switch(choice){
            case 1: // speech
                mor = heroes[opp].heroWarriors[0].ratingsWarrior[2];
                inf = h.ratings[1];
                winProb = (1-((double)mor/100))*((double)inf/1000);
                winProb *= 100;
                cout << "Win Prob: " << winProb << endl; // TODO
                if(probability() <= ceil(winProb)){
                    cout << endl;
                    cout << "Congratulations, you have effectively influenced your opponent." << endl;
                    retireOpponent(choiceHero,opp);
                    
                }
                else{
                    cout << "You have FAILED to influence " << heroes[opp].name << endl;
                    cout << "You have lost much of the influence you once held." << endl;
                    heroes[choiceHero].ratings[1] /= 2;
                }
                break;
            case 2: // buy outright
                loy = heroes[opp].heroWarriors[0].ratingsWarrior[1];
                money = h.ratings[0];
                winProb = (1-((double)loy/100))*((double)money/17800);
                winProb*=100;
                cout << "Win Prob: " << winProb << endl; // TODO
                if(probability() <= ceil(winProb)){
                    cout << endl;
                    cout << "Congratulations, you have succeeded in buying your opponent." << endl;
                    retireOpponent(choiceHero, opp);
                }
                else{
                    cout << "You have FAILED to purchase " << heroes[opp].name << endl;
                    cout << "You have lost much of your money." << endl;
                    heroes[choiceHero].ratings[0] /= 2;
                }
                break;
            case 3: // battle
                if(heroes[choiceHero].numHeroWarriors == 0){
                    lose = true;
                    return;
                } else if(heroes[opp].numHeroWarriors == 0){
                    battle = true;
                } else if(heroes[choiceHero].numHeroWarriors == 1){
                    strength = heroes[choiceHero].heroWarriors[0].ratingsWarrior[0];
                } else{
                    cout << endl;
                    cout << "Your grace, please choose which warrior you would like to represent you in battle:" << endl;
                    for(int i=0; i<heroes[choiceHero].numHeroWarriors; i++){
                        cout << i+1 << ": " << heroes[choiceHero].heroWarriors[i].name << endl;
                    }
                    cin >> war;
                    strength = heroes[choiceHero].heroWarriors[stoi(war)-1].ratingsWarrior[0];
                }
                if(battle == false){
                    army = heroes[choiceHero].ratings[2];
                    usr = army*strength;
                    cpuwar = rand() % heroes[opp].numHeroWarriors;
                    cpu = heroes[opp].ratings[2] * heroes[opp].heroWarriors[cpuwar].ratingsWarrior[0];

                    if(usr >= cpu){
                        battle = true;
                    } else{
                        lose = true;
                        return;
                    }
                }
                if(battle == true){ // user won battle
                    cout << endl;
                    cout << "Congratulations, you have defeated " << heroes[opp].name << " in battle!" << endl;
                    retireOpponent(choiceHero,opp);
                }
                break;
            
        }   
    } 
}

/*
1. takes two non-main hero's that are in proximity
2. initializes battle, compares army size and warrior strength between 2
3. losing hero is removed from heroes vector
4. winning hero takes losing hero's map ownership, army, money, influence
5. if a tie happens, both hero's lose 20 in army size
*/
void Game::encounterComputer(int a, int b){
    bool awin, bwin, tie = false;
    int awar,bwar,ascore,bscore;
    if(heroes[a].numHeroWarriors == 0 && heroes[b].numHeroWarriors == 0){ // neither hero has any warriors
        if(heroes[a].ratings[2] < heroes[b].ratings[2]){
            bwin = true;
        } else if(heroes[a].ratings[2] > heroes[b].ratings[2]){
            awin = true;
        } else{
            tie = true;
        }
    } else if(heroes[a].numHeroWarriors == 0){ // hero a has no warriors, but b does - b wins
        bwin = true;
    } else if(heroes[b].numHeroWarriors == 0){ // hero b has no warriors, but a does - a wins
        awin = false;
    } else{ // both have warriors
        awar = rand() % heroes[a].numHeroWarriors;
        bwar = rand() % heroes[b].numHeroWarriors;
        ascore = heroes[a].heroWarriors[awar].ratingsWarrior[0] * heroes[a].ratings[2];
        bscore = heroes[b].heroWarriors[bwar].ratingsWarrior[0] * heroes[b].ratings[2];
        if(ascore < bscore){
            bwin = true;
        } else if(bscore < ascore){
            awin = true;
        } else{
            tie = true;
        }
        cout << "score for: " <<heroes[a].name <<" = "<< ascore <<endl; // TODO
        cout << "score for: " <<heroes[b].name <<" = "<< bscore <<endl; 
    }
    if(awin == true){
        cout << heroes[a].name << " has defeated " << heroes[b].name << " in battle." << endl;
        retireOpponent(a,b);
    } else if(bwin == true){
        cout << heroes[b].name << " has defeated " << heroes[a].name << " in battle." << endl;
        retireOpponent(b,a);
    } else{ // tie
        cout << heroes[a].name << " has encountered " << heroes[b].name << " in the battlefield.";
        cout << " However, their battle has resulted in a TIE." << endl;
        if(heroes[a].ratings[2] > 0){
            heroes[a].ratings[2] /= 2;
        } 
        if(heroes[b].ratings[2] > 0){
            heroes[b].ratings[2] /= 2;
        }
    }
}

// process to call encounterComputer() function
void Game::runEncounterComputer(){
    for(int i=0; i<numHeroesGame-1; i++){
        for(int j=i+1; j<numHeroesGame; j++){
            if(i == choiceHero || j == choiceHero) continue;
            if(help.proximityCheck(heroes[i].location[0],heroes[i].location[1],heroes[j].location[0],heroes[j].location[1])){
                encounterComputer(i,j);
            }
        }
    }
}

// finds out which hero is closest to the user
int Game::closestHero(){
    int row,col,dist,loc;
    int min = 1000;
    for(int i=0; i < numHeroesGame; i++){
        if(i == choiceHero){
            dist = 1000;
        } else{
            row = heroes[choiceHero].location[0] - heroes[i].location[0];
            col = heroes[choiceHero].location[1] - heroes[i].location[1];
            dist = abs(row) + abs(col);
        }
        if(dist < min){
            min = dist;
            loc = i;
        }
    }
    return loc;
}

// called if user enters new city for first time
// asks if they want to hear rumors or buy soldiers
void Game::newCity(int c){
    string yesno,knights;
    int close,dist;
    cout << endl;
    cout << "You have arrived in " << cities[c].getName() << ". Do you want to listen to the rumors? Y/N" << endl;
    cin >> yesno;
    transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
    if(yesno == "y" || yesno == "yes"){
        close = closestHero();
        dist = abs(heroes[choiceHero].location[0] - heroes[close].location[0]) + abs(heroes[choiceHero].location[1] - heroes[close].location[1]);
        cout << endl;
        cout << "Your grace, " << heroes[close].name << " is only " << dist << " days (turns) from your location." << endl;
    }
    cout << endl;
    cout << cities[c].getName() << " is excited about your arrival, and is providing you the opportunity to purchase soldiers." << endl;
    cout << "The price per knight is 5 units of money. Would you like to purchase any? "<< endl;
    cin >> yesno;
    transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
    if(yesno == "y" || yesno == "yes"){
        cout << endl;
        cout << "How many would you like to purchase? " << endl;
        cin >> knights;
        heroes[choiceHero].ratings[0] -= (5*stoi(knights));
        heroes[choiceHero].ratings[2] += stoi(knights);
    }
}

/*
1 - check if hero already controls current tile after turn
2 - update hero's ratings accordingly
3 - if tile controlled by OTHER hero, that hero loses the points for the tile
4 - if USER, check if first time visiting a city - newCity() function
5 - mark tile as traveled and controlled by hero
*/
void Game::afterEncounter(){
    for(int h=0; h < numHeroesGame; h++){
        int row = heroes[h].location[0];
        int col = heroes[h].location[1];
        int current=-1;
        char loc = westeros.getMapLocation(row,col)[0];
        if(westeros.isCaptured(row,col) != h){ // hero does not already control this tile
            if(westeros.isCaptured(row,col) >= 0 && westeros.isCaptured(row,col) <= 6){
                current = westeros.isCaptured(row,col); // index of hero currently in control of tile
            }
            if(loc == 'p'){
                if(heroes[h].traveled[row][col] == false){
                    heroes[h].ratings[0] += 20;
                    heroes[h].ratings[1] += 10;
                    heroes[h].ratings[2] += 10;
                }
                heroes[h].heroPoints += 2;
                if(current > -1){
                    heroes[current].heroPoints -= 2;
                }
            } else if(loc == 'w'){
                if(heroes[h].traveled[row][col] == false){
                    heroes[h].ratings[0] += 20;
                    heroes[h].ratings[1] += 10;
                }
                heroes[h].heroPoints += 1;
                if(current > -1){
                    heroes[current].heroPoints -= 1;
                }
            } else{
                for(int c=0; c < 26; c++){
                    if(cities[c].getLetter() == loc){
                        if(heroes[h].traveled[row][col] == false){
                            heroes[h].ratings[0] += cities[c].getCityRating(0);
                            heroes[h].ratings[2] += cities[c].getCityRating(1);
                            if(h == choiceHero){
                                newCity(c);
                            }
                        }
                        heroes[h].heroPoints += cities[c].getCityRating(2);
                        if(current > -1){
                            heroes[current].heroPoints -= cities[c].getCityRating(2);
                        }
                        break;
                    }
                }
            }
            westeros.captureIt(row,col,h);
        }
        heroes[h].traveled[row][col] = true;
    }
}

// 4 possible events each with probability 1/4 if function is called
void Game::endOfTurn(){
    int event = rand() % 4;
    switch(event){
        case 0: // drought
            cout << endl;
            cout << "Your grace, unfortunately you and your army are experiencing a DROUGHT." << endl;
            for(int i=0; i < heroes[choiceHero].numHeroWarriors; i++){
                heroes[choiceHero].heroWarriors[i].ratingsWarrior[0] -= 10;
                heroes[choiceHero].heroWarriors[i].ratingsWarrior[2] -= 10;
            }
            break;
        case 1: // deserters
            cout << endl;
            cout << "Your grace, unfortunately some of your soldiers have lost faith in your cause and are DESERTING." << endl;
            heroes[choiceHero].ratings[2] -= 10;
            for(int i=0; i < heroes[choiceHero].numHeroWarriors; i++){
                heroes[choiceHero].heroWarriors[i].ratingsWarrior[1] -= 10;
            }
            break;
        case 2: // jackpot
            cout << endl;
            cout << "Your grace, wealthy benefactors have taken notice of your cause. You have hit the JACKPOT." << endl;
            heroes[choiceHero].ratings[0] *= 2;
            break;
        case 3: // kindness
            cout << endl;
            cout << "Your grace, you have taken in some refugees and will be rewarded for your KINDNESS." << endl;
            heroes[choiceHero].ratings[2] += 10;
            for(int i=0; i < heroes[choiceHero].numHeroWarriors; i++){
                heroes[choiceHero].heroWarriors[i].ratingsWarrior[1] += 10;
            }
            break;
    }
}


// applies all things that happen on 10th turn of the game
void Game::turnTen(){
    help.turn10prints();
    // Daenerys 
    heroes[5].ratings[2] += 1000;
    Warrior dragons;
    dragons.ratingsWarrior[0] = 180;
    dragons.ratingsWarrior[1] = 100;
    dragons.ratingsWarrior[2] = 80;
    warriors.push_back(dragons);
    numWarriorsGame++;
    if(heroes[5].numHeroWarriors < 4){
        heroes[5].heroWarriors[heroes[5].numHeroWarriors-1] = dragons;
        heroes[5].numHeroWarriors++;
    } else{ // Daenerys already has 4 warriors, Dragons eat the last warrior
        Warrior placeHolder;
        placeHolder.boolWarrior[0] = false;
        for(int i=0; i < numWarriorsGame-1; i++){
            if(warriors[i].name == heroes[5].heroWarriors[3].name){
                dragons.ratingsWarrior[0] += 18;
                dragons.ratingsWarrior[1] += 10;
                dragons.ratingsWarrior[2] += 8;     
                heroes[5].heroWarriors[3] = dragons;
                cout << warriors[i].name << " has been eaten by Dragons." << endl;
                warriors[i] = placeHolder;
            }
        }
    }
    // Stannis
    if(heroes[1].name != ""){
        for(int i=0; i < heroes[1].numHeroWarriors; i++){
            for(int j=0; j < numWarriorsGame; j++){
                if(heroes[1].heroWarriors[i].name == warriors[j].name){
                    warriors[j].boolWarrior[0] = true; // free all Stannis' warriors
                    loadFreeWarriorsLocation(j);
                    break;
                }
            }
        }
        Hero deadHero;
        heroes[1] = deadHero; // remove Stannis from heroes vector
    }

    // Sansa
    if(warriors[9].boolWarrior[0] == false){ // Arya not free
        for(int i=0; i < numHeroesGame; i++){
            for(int j=0; j < heroes[i].numHeroWarriors; j++){
                if(heroes[i].heroWarriors[j].name == warriors[9].name){
                    for(int k=j; k < heroes[i].numHeroWarriors-1; k++){
                        heroes[i].heroWarriors[k] = heroes[i].heroWarriors[k+1];
                    }
                    cout << heroes[i].name << " has lost Arya Stark as one of their warriors" << endl;
                    i=numHeroesGame;
                    break;
                }
            }
        }
    }else{
        warriors[9].boolWarrior[0] = false;
    }
    heroes[3].heroWarriors[heroes[3].numHeroWarriors-1] = warriors[9];

    // Cersei
    for(int i=0; i < heroes[0].numHeroWarriors-1; i++){
        heroes[0].heroWarriors[i] = heroes[0].heroWarriors[i+1];
    }
    warriors[7].boolWarrior[0] = true; // Jaime is free
    loadFreeWarriorsLocation(7);

    // Jon Snow
    heroes[2].ratings[2] += 500;
    heroes[2].ratings[1] += 100;
}


//calculates power score
//determines if hero wins based on probability
//only runs 3 times, before hero loses

void Game::whiteWalkerBattle(){
    int pow,probwin;
    int powerscore = 0;
    int numCities = 0;
    for(int i=0; i < heroes[choiceHero].numHeroWarriors; i++){
        powerscore += (heroes[choiceHero].heroWarriors[i].ratingsWarrior[0] * (heroes[choiceHero].heroWarriors[i].ratingsWarrior[2]/50));
        if (heroes[choiceHero].heroWarriors[i].boolWarrior[2]){
            powerscore+=10000;
        }
    }
    for(int i=0; i < maprows; i++){
        for(int j=0; j < mapcols; j++){
            char loc = westeros.getMapLocation(i,j)[0];
            if (int(loc) >= 65 && int(loc) <= 90 && westeros.isCaptured(i,j) == choiceHero){
                numCities++;
            }
        }
    }
    powerscore += (numCities+1)*(heroes[choiceHero].ratings[1]/4);
    pow = -(powerscore/30000);
    probwin = 1 - exp(pow);
    cout << "probability to win is: " << probwin << endl; 
}


// update warriors vector to current stats
void Game::updateWarriors(){
    for(int i=0; i < numWarriorsGame; i++){
        if(warriors[i].boolWarrior[0]) continue;
        for(int j=0; j < numHeroesGame; j++){
            // cout << heroes[j].name << endl;
            for(int k=0; k < heroes[j].numHeroWarriors; k++){
                if(heroes[j].heroWarriors[k].name == warriors[i].name){
                    warriors[i] = heroes[j].heroWarriors[k];
                }
            }
        }
    }
}

bool Game::turnCheck(){
    if(turnNumber == 50){
        return true;
    } else if(heroes[choiceHero].ratings[2] > 2000){
        return true;
    } else if(heroes[choiceHero].heroPoints >= 400){
        return true;
    } else if(westeros.countTiles(choiceHero) >= 200){
        return true;
    } else{
        return false;
    }
}


//This function loads all aspects of the games, and runs all necessary functions for playing the game
void Game::mainGame(){

    // load in map
    loadCities("text_files/citiesGOT.txt");
    loadMap("text_files/mapGOT.txt");

    // introduction of game
    userName = help.welcome();
    string yesno = help.askUser();
    transform(yesno.begin(), yesno.end(), yesno.begin(), ::tolower);
    if (yesno == "y" || yesno == "yes"){
        numHeroesGame = 6;
        choiceHero = help.heroOptionsMenu();
    } else{
        numHeroesGame = 7;
        help.createUserHero(westeros);
        choiceHero = 6; // location in heros array of created hero
    }
    // initialize all characters
    loadWarriors("text_files/warriorsGOT.txt");
    loadHeroes("text_files/heroesGOT.txt");

    for(int i=0; i < numWarriorsGame; i++){
        if(warriors[i].boolWarrior[0] == true){
            loadFreeWarriorsLocation(i);
        }
    }
    
    // assign each hero control over starting tile
    for(int j=0; j < numHeroesGame; j++){
        westeros.captureIt(heroes[j].location[0],heroes[j].location[1],j);
    }

    // westeros.displayMapEntire();

    while(lose == false)
    {
        string nextTurn;
        cout << "Turn Number: " << turnNumber << endl; // TODO
        // start turn and display map for user
        help.startTurn(heroes[choiceHero]);
        westeros.displayMap(heroes[choiceHero].location[0], heroes[choiceHero].location[1]);

        // User turn
        int turnChoice = help.displayTurnChoices();
        switch (turnChoice){
            case 1:
                travelMain();
                break;
            case 2:
                restUser();
                break;
            case 3:
                consultGodsUser();
                break;
            default:
                cout << "invalid choice" << endl;
        }

        // Non-user character movements
        help.p1();
        moveNonMainHeroes();
        cout << "heroes moved" << endl; // TODO
        moveFreeWarriors();
        cout << "warriors moved" << endl; // TODO

        // After movement encounters
        help.p2();
        encounterFreeWarrior();
        encounterUser(heroes[choiceHero]);
        if (lose == true) break; // check win/loss after user encounter
        runEncounterComputer();

        // post-encounter Hero updates
        afterEncounter();
        
        // End of turn events for User
        if(rand()%10 <= 2){
            endOfTurn();
        }

        // update warriors vector
        updateWarriors();
        cout << "warriors updated" << endl;

        // end of turn checks
        turnNumber++;
        if(turnNumber == 10){
            turnTen();
        }
        if(turnCheck()){
            whiteWalkerBattle();
        }

        // proceed to next turn
        help.nextTurn();

    }
}
