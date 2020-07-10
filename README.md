# Game Of Thrones

## Game Setup
The user can choose to play The Game of Thrones as one of 6 provided heros, or they can create their own hero. The file **heroesGOT.txt** has information about each of the 6 heroes, in the following format:

`<Name>,<$>,<influence>,<army_size>,<w1>,<w2>,<w3>,<w4>,<r>,<c>,<ship>`
  
Cersei Lannister,8000,300,300,Jaime Lannister,The Mountain,NULL,NULL,16,7,no

Stannis Baratheon,1000,100,200,Melisandre,Davos Seaworth,NULL,NULL,14,9,no

Jon Snow,100,50,20,Jon Snow,Samwell Tarly,NULL,NULL,4,5,no

Sansa Stark,1000,100,200,Brienne of Tarth,NULL,NULL,NULL,7,5,no

Euron Greyjoy,1000,50,100,NULL,NULL,NULL,NULL,13,2,yes

Daenerys Targaryen,100,1000,50,NULL,NULL,NULL,NULL,18,16,no

* At the beginning of the game, each hero has at their disposal a starting sum of money and an army of knights, which they will use to find and defeat the other heroes.
* Each hero also has avalue for `influence`.This metric speaks about each hero’s ability to build support for their cause. Having a high influence helps a hero strengthen their faction by winning new warriors on their side.
* Each hero has a starting location on the map, represented by an integer row-column pair:`<r>,<c>`.These are referenced relative to the upper-left corner. For example, *JonSnow* starts at row 4, column 5, which corresponds to the tile marked `A` on the map which, according to the file **citiesGOT.txt**, is the city of Castle Black.
* Heroes can have between as few as 0 and as many as 4 warriors. Some heroes start with warriors, while others do not. The warriors who start with a hero are represented by names in the <w1> and <w2> spots. As the game progresses, heroes may acquire other warriors, filling up slots <w1> through <w4> sequentially. A value of *NULL* means that warrior slot for that hero is empty. *Jon Snow* is both a hero and a warrior.
* The ship characteristic of each hero refers to their ability to move to *water* locations on the map. Only the hero *Euron Greyjoy* starts with a ship. No other heroes will have the ability to move onto *water* locations unless they acquire a warrior with a ship. Hero *Stannis Baratheon* starts with *Davos Seaworth* as one of his warriors. Since *Davos Seaworth* has a ship, *Stannis Baratheon* also has the ability to move to *water* locations.
* The first five heroes start on the same continent, while the last hero *DaenerysTargaryen* starts alone on another continent.
