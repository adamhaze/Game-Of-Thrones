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

* Atthebeginningofthegame,eachherohasattheirdisposalastartingsumofmoneyand an army of knights, which they will use to find and defeat the other heroes.
* Eachheroalsohasavaluefor​influence​.Thismetricspeaksabouteachhero’sabilitytobuildsupportfortheircause.Havingahigh​influencehelpsaherostrengthentheirfaction by winning new warriors on their side.
* Eachherohasastartinglocationonthemap,representedbyanintegerrow-columnpair:<r>​,​<c>​.Thesearereferencedrelativetotheupper-leftcorner.Forexample,​JonSnow​startsatrow4,column5,whichcorrespondstothetilemarked‘A’onthemapwhich, according to the file ​citiesGOT.txt,​ is the city of Castle Black.
* Heroes canhavebetweenasfewas0andasmanyas4warriors.Someheroesstartwithwarriors,whileothersdonot.Thewarriorswhostartwithaheroarerepresentedbynamesinthe​w1and​w2spots.Asthegameprogresses,heroesmayacquireotherwarriors,fillingupslots​w1through​w4sequentially.Avalueof​NULLmeansthatwarriorslot for that hero is empty. Jon Snow is both a hero and a warrior.
* The shipcharacteristicofeachheroreferstotheirabilitytomoveto‘water’locationsonthemap.Onlythehero​EuronGreyjoy​startswithaship.Nootherheroeswillhavetheabilitytomoveonto‘water’locationsunlesstheyacquireawarriorwithaship.HeroStannisBaratheonstartswith​DavosSeaworthasoneofhiswarriors.SinceDavosSeaworthhasaship,​StannisBaratheonalsohastheabilitytomoveto‘water’ locations.
* Thefirstfiveheroesstartonthesamecontinent,whilethelasthero​DaenerysTargaryen​ starts alone on another continent.
