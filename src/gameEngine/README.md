# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation throw our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **gameEngine folder**! We would explain you **how the game logic works**. All the code in this folder is all mostly organized around the class "GameEngine" wich is supposed to be the central element that contains all of the other, more specialized, "managers" that only take care of one certain aspect of the general logic.


## GameEngine

This class is instanciated once per game. It is given on instantiation an instance of GameParams as only parameter.

On initialization, the GameEngine handles the instantiation of all of it's managers and a Map. It then executes the logic that loads the map from the proper file, and also instantiates an entityList that contains all the game entities and is used in almost every manager.

During the game, GameEngine's only task situated in it's "compute" function that calls every manager's "update" function.

At the end of the game (whether the player lost or won), GameEngine communicates the proper events to NuklearGUI and SGameManager.

## GameParams

This class isn't meant to handle any logic. it only serves as a container for the datas GameEngine needs to start a game such as the map_path or the game_mode ...

## Loader

This class reads and uses the rapidJson api to parse a json file. It is used to load maps and slots.

## Slot

This class represents a save slot. It contains the load and save logic as well as the pertinent datas.

## AGameEntity 

This is the class from wich every game entity (see the entity folder) class inherits.

## SoundManager

Pretty self explanatory, it handles all the sounds. Note that this is the only manager that isn't contained as such in the GameEngine.

## AI

Contains the logic relative to the AI of a Brawl enemy. Basically the algorithm it follows looks something like this :
`if I can place a bomb
    Place bomb
else
    run to a safe place`

## PlayerManager

This is the interface beetween the Players (human or AI) and the actual Players game entities. What it mostly does is just propagates the proper events relative to player action like moving or spawning a bomb.

## CollisionManager

Handles the movement of players and enemies according to the datas set by the other managers, while (obviously) taking collisions into account.

## EnemyManager

Handles the movement logic of enemeies as well as what happens when an enemy dies.

## BombManager

Handles the logic relative to Bomb explosions.
Bombs set themselves to a certain state when they should explodes, the bomb manager then spawns flames accordingly and destroys the bomb.
It also deletes flames when they should disapear (similarly to the bombs, it's the Flames that set themselves to a certain state when they should disapear).

## BonusManager

Spawns Bonus when a brick breaks and applies it's effect on the player when it is activated. It's here that the probability for each bonus to drop is defined.

## WinManager

Checks if the game is won. If the game mode is Campaign, it spawns a WinSpot rather than finishing instantly the game once the win ncondition is met.