# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation throw our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **maps folder**! We would explain you how to **edit the brawl mode map**. If you want to add maps in campaign mode, just go in the campaign folder

## Brawl map editor

The effectif JSON file is **brawl_0.json**

### grid
The grid element is a 2D integers array. Each integers make reference of a type of occupation:

> - **0**: Is randomly empty (4/10) or fill with a destructible bloc (6/10)
> - **-1**: Is forced to be an empty case
> - **-2**: indestructible bloc

Keep in mind some elements:
 - the array size make the map size.
 - each destructible bloc have 60% chance to make appeared a bonus.
 - each enemy appeared in a corner of the map, so keep this blocs to -1.

exemple:
```
"grid":[[-1,-1, 0, 0, 0, 0, 0, 0, 0,-1,-1],
        [-1,-2, 0,-2, 0,-2, 0,-2, 0,-2,-1],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [ 0,-2, 0,-2, 0,-2, 0,-2, 0,-2, 0],
        [ 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0],
        [-1,-2, 0,-2, 0,-2, 0,-2, 0,-2,-1],
        [-1,-1, 0, 0, 0, 0, 0, 0, 0,-1,-1]]
```
### models
It's textures references to make a different ambiance. Exemple:
```
"models":{
    "bricks": "assets/models/bricks.obj",
    "undestroyable": "assets/models/undestroyable.obj"
}
```

### sun
Give the position and the color of the main light. Give the possibility to be the morning/night. Exemple:
```
"sun":{
    "pos":[20.0, 20.0, 20.0],
    "color":[0.682, 0.356, 0.803]
}
```

### win
This give the possibility to change the win conditions and the end spot. Not used for the brawl mode. So... don't touch it ;)
```
"win":{
    "condition": 0,
    "spot": [5.0, 0.0]
}
```

### enemies
This give the possibility to add enemies on the map. Not used for the brawl mode. So... don't touch it, again ;) ;)
```
"enemies": []
```