# Generalities

This folder contains all the game entities classes. All of these inherit from AGameEntity and call it's protected constructor in their respective constructors.

The logic contained in these class (if any) is restricted in the sense that an entity isn't aware of anything else than herself. It can however register itself to events, like a player moving for instance.
Therefore the logic contained here consists mostly of the following:
    - For Bombs and Flames, setting it's state to State::DYING when the lifetime of the entity is over. (Bomb Manager then destroys the object)
    - For Flames, bonuses, enemies and WinSpot casting the proper event event (player death, bonus activate, game won ...) when a player moves to it's position

There isn't anything else to say here really. Game Entities are really mostly supposed to be data container that are then used in the managers.