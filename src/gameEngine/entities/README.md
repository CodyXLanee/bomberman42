# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation through our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **entities folder**!

## Generalities

This folder contains all the game entities classes. All of these inherit from AGameEntity and call it's protected constructor in their respective constructors.

The logic contained in these class (if any) is restricted in the sense that an entity isn't aware of anything else than herself. It can however register itself to events, like a player moving for instance.
Therefore the logic contained here consists mostly of the following:
 - For Bombs and Flames, setting it's state to State::DYING when the lifetime of the entity is over. (Bomb Manager then destroys the object)
 - For Flames, bonuses, enemies and WinSpot casting the proper event event (player death, bonus activate, game won ...) when a player moves to it's position

There isn't anything else to say here really. Game Entities are really mostly supposed to be data container that are then used in the managers.