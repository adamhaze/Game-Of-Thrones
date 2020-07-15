# Game Of Thrones

## Game Setup

### Heroes
The user can choose to play The Game of Thrones as one of 6 provided heros, or they can create their own hero. The file **text_files/heroesGOT.txt** has information about each of the 6 heroes, in the following format:

`<Name>,<$>,<influence>,<army_size>,<w1>,<w2>,<w3>,<w4>,<r>,<c>,<ship>`
  
Cersei Lannister,8000,300,300,Jaime Lannister,The Mountain,NULL,NULL,16,7,no

Stannis Baratheon,1000,100,200,Melisandre,Davos Seaworth,NULL,NULL,14,9,no

Jon Snow,100,50,20,Jon Snow,Samwell Tarly,NULL,NULL,4,5,no

Sansa Stark,1000,100,200,Brienne of Tarth,NULL,NULL,NULL,7,5,no

Euron Greyjoy,1000,50,100,NULL,NULL,NULL,NULL,13,2,yes

Daenerys Targaryen,100,1000,50,NULL,NULL,NULL,NULL,18,16,no

* At the beginning of the game, each hero has at their disposal a starting sum of money and an army of knights, which they will use to find and defeat the other heroes.
* Each hero also has avalue for `influence`.This metric speaks about each hero’s ability to build support for their cause. Having a high influence helps a hero strengthen their faction by winning new warriors on their side.
* Each hero has a starting location on the map, represented by an integer row-column pair:`<r>,<c>`.These are referenced relative to the upper-left corner. For example, **Jon Snow** starts at row 4, column 5, which corresponds to the tile marked `A` on the map which, according to the file **text_files/citiesGOT.txt**, is the city of Castle Black.
* Heroes can have between as few as 0 and as many as 4 warriors. Some heroes start with warriors, while others do not. The warriors who start with a hero are represented by names in the <w1> and <w2> spots. As the game progresses, heroes may acquire other warriors, filling up slots <w1> through <w4> sequentially. A value of *NULL* means that warrior slot for that hero is empty. *Jon Snow* is both a hero and a warrior.
* The ship characteristic of each hero refers to their ability to move to *water* locations on the map. Only the hero *Euron Greyjoy* starts with a ship. No other heroes will have the ability to move onto *water* locations unless they acquire a warrior with a ship. Hero *Stannis Baratheon* starts with *Davos Seaworth* as one of his warriors. Since **Davos Seaworth** has a ship, **Stannis Baratheon** also has the ability to move to *water* locations.
* The first five heroes start on the same continent, while the last hero **Daenerys Targaryen** starts alone on another continent.

### Warriors

There are 14 Warriors in the game, each possessing several characteristics. The values for each warrior are defined in **text_files/warriorsGOT.txt** in the following format:

`<Name>,<strength>,<loyalty>,<morale>,<free>,<ship>,<has_dragonglass>`

* <strength>, <loyalty>, and <morale> are *integer* valued characteristics, each provide different advantages to whichever hero has a given warrior in their army
* <free>, <ship>, and <has_dragonglass> are *bool* valued characteristics which define whether a given warrior is free, has ability to travel on water, or posses dragonglass, respectively.
* Having a warrior who possesses *dragonglass* will greatly increase a hero's chance of defeating the White Walkers in the final battle of the game. As each hero (and their warriors) travel the map, they can discover *dragonglass* and equip one of their warriors with it. The only warrior that starts the game with *dragonglass* is **Samwell Tarly**
  
### Map
The map of **Westeros** is defined in **text_files/mapGOT.txt** and is interpreted as a 2D array with 35 rows and 26 columns. There are a total of 910 tiles on the map and each tile represented by letter where a `p` indicates a land tile, a `w` indicates a water tile, and a capital letter indicates a city. There are `400` land tiles, `484` water tiles, and `26` cities. Each city provides unique benefits to whichever hero visits and controls it. The cities of **Westeros** are defined in **text_files/citiesGOT.txt** in the following format:

`<Letter>,<Name>,<$>,<army_size>,<points>`

As the heroes travel to different locations on the map, they acquire more money and influence, as well as increase the size of their armies. The heroes take control of every place they visit, which brings them points (as well as stat bonuses) in the game. Controlling a `water` location is worth **1 point** and garners a hero **20** units of *money* and **10** units of *influence*. A `land` location is worth **2 points** and garners a hero **20** units of *money* and **10** units of both *army* and *influence*. Controlling a city is worth between **5** and **35** points (along with defined stat bonuses). If a hero controlled the entire map, the total number of points would be **1600 points**:
  
  * 800 points for controlling all `land` tiles
  * 484 points for controlling all `water` tiles
  * 316 points for controlling all cities

## Playing the Game of Thrones
At the beginning of the game the user can choose to play as one of the six pre-loaded heroes, or the can choose to create their own hero in which the game prompts the user to assign statistics and locations of their created hero (a user created hero starts with no warriors).

Before each turn, the user is presented with a breakdown of their current ratings as well as the ratings of each of their warriors. The user is also presented with an 11 by 11 area of the map centered on the main hero's location, denoted with `@`. Land tiles are displayed with the symbol `*` and water tiles with the symbol `~` while cities are denoted by a capital letter.

Each turn the user has 3 choices of what to do:
* **Travel** 
  * User is asked in which direction they wish to move 1 tile: North, South, East or West
  * If user does not have a ship, they cannot travel to a water tile
  * If user chooses to travel, they cannot stay in the same place
* **Rest**
  * User's hero remains in same location
  * Each warrior in the hero's party increases their strength by 5
* **Consult the Gods**
  * User's hero remains in same location
  * Each warrior in the hero's party increases their morale by 5
  

After the user's hero has completed their turn, all the non-user hero's and free warriors will randomly move 1 tile (Note: non-user characters will move 1 tile each turn).

### Encounters
After each character's locations have been updated, it is time to resolve all encounters. An encounter happens when a hero is within 2 tiles, in any direction, of another hero. If the user's hero encounters another hero they have 3 options:
* **Give a Speech**: the user's hero relies on their *influence* to win over the other hero's army. The probability of the user's hero winning the encounter is higher/lower depending on the *morale* of the first warrior in the opposing hero's party.
  * if the user's hero **wins**:
    * the losing hero retires from the game
    * the user's hero acquires all of the losing hero's:
      * warriors (max of 4) - if the total number of warriors would exceed 4 then the user chooses which 4 warriors to keep.
      * money
      * influence
      * army size
    * the user's hero takes control of all locations currently controlled by the losing hero
  * if the user's hero **loses**:
    * the encounter ends
    * the user's hero escapes
    * the user's hero loses half their *influence* value
* **Buy them outright**: the user's hero relies on their *money* to buy out the other hero's army. If the *loyalty* of the first warrior in the opposing hero's party is low, the probability of the user's hero winning the encounter is much higher.
  * if the user's hero **wins**:
    * the losing hero retires from the game
    * the user's hero acquires all of the losing hero's:
      * warriors (max of 4) - if the total number of warriors would exceed 4 then the user chooses which 4 warriors to keep.
      * money
      * influence
      * army size
    * the user's hero takes control of all locations currently controlled by the losing hero
  * if the user's hero **loses**:
    * the encounter ends
    * the user's hero escapes
    * the user's hero loses half their *money* value
* **Battle** both heroes fight to the death and rely on the *strength* of their warriors and the size of their *army* to win the encounter.
  * If either hero has no warriors, but the other hero does, then the hero with no warrior automatically loses
  * If the user's hero has more than 1 warrior, then the user will choose which warrior will represent their hero in battle
  * If the opposing hero has more than 1 warrior, the game randomly selects which warrior will represent them
  * The hero with the highest value for (*army size*)x(*strength of chosen warrior*) wins the encounter
  * if the user's hero **wins**:
    * the losing hero retires from the game
    * the user's hero acquires all of the losing hero's:
      * warriors (max of 4) - if the total number of warriors would exceed 4 then the user chooses which 4 warriors to keep.
      * money
      * influence
      * army size
    * the user's hero takes control of all locations currently controlled by the losing hero
  * if the user's hero **loses**:
    * **the game ends!**
    
    
After the user resolves all of their encounters, the non-user heroes resolve their encounters (based on same proximity rules) by **Battle** with the same guidelines as described above in which the losing hero retires from the game.

**Note**: If any hero is in proximity to any *free* roaming warriors, that hero will automatically acquire that warrior, so long as they do not already have 4 warriors in their party.
  * If the hero already has 4 warriors in their party, the warrior continues to roam free
  * If 2 or more heroes are in proximity with the warrior, no one will acquire the warrior that turn
  * A warrior that gets acquired by a hero increases their *loyalty* and *morale* by 10 points each
  
### End of Turn
At the end of each turn their is a 30% probability that one of the following random events will occur (each event with equal likelihood):
* **Drought**: there has been no rain for days, and because of this each warrior in your party has lost 10 points in both *morale* and *strength*

* **Deserters**: this campaign is wearing on your army and some soldiers have grown restless. As a result, your hero's *army size* has decreased by 10 points, and the *loyalty* for each of the warriors in your hero's party has decreased by 10 points

* **Jackpot**: your heroic cause has won the hearts of many noble and wealthy benefactors. As a result, your hero's *money* has **doubled**

* **Kindness**: you have decided to shelter some refugees. As a result, your hero's *army size* has increased by 10 points as well as the *loyalty* for each one of your hero's warriors
