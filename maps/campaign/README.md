# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation through our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **campaign folder**! We would explain you how  to **add/edit maps in campaign mode**. Here we go!


## Campaign map's editor


It's JSON files. The **name of the file** determine the **level**. The JSON need following values:

### grid
It's a **2D** array of int. Each int represent a **type of occupation**:
> - **0**: empty
> - **-1**: destructible block
> - **-2**: indestructible block
> - **1**: initial player position (one and only one per grid)
> - **2-9**: reserved for mobs types
>  - **2**: bomb-mob
>  - **3-9**: empty
> - **10-19**: reserved for bonus types
>  - **10**: power-bomb +1
>  - **11**: power-bomb -1 (not implemented)
>  - **12**: nb-bomb +1
>  - **13**: nb-bomb -1 (not implemented)
>  - **14**: speed +1
>  - **15**: speed -1 (not implemented)
>  - **16-19**: empty

exemple:
```
"grid": [[ 0, 0, 0, 0, 0],
         [ 0,-2,-1,-2, 0],
         [ 0,-1, 0,-1, 0],
         [ 0,-2, 0,-2, 0],
         [ 0, 0, 1, 0, 0]]
```


### model
It's the **design of blocks:**
> - **bricks**: destructible blocks design
> - **undestroyable**: indestructible blocks design

exemple:
```
"models":{
    "bricks": "assets/models/bricks.obj",
    "undestroyable": "assets/models/undestroyable.obj"
  }
```
### sun
It's **sun parameters:**
> - **pos**: position of the sun
> - **color**: the RGB color (0 to 1)

exemple:
```
"sun":{
    "pos":[20.0, 20.0, 20.0],
    "color":[0.682, 0.356, 0.803]
  }
```


### stars
It's an array who give **the maximum number** of bomb to use to win 3, 2 and 1 **stars**

exemple:
```
"stars":[1,2,3]
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

## Add a level

 - create a file with the following name (ex: if the last file if **6.json**, call it **7.json**)
 - edit the json like above
 - add in the folder **assets/textures** the file **level7.png**
 - edit the line 51 of the file **src/includes/bomberman.hpp**
    ```
    namespace Level {
        enum Enum {ONE, TWO, THREE, FOUR, FIVE, SIX};
    }
    ``` 
    by
    ```
    namespace Level {
        enum Enum {ONE, TWO, THREE, FOUR, FIVE, SIX, SEVEN};
    }
    ```
- edit the line 565 of the file **src/renderEngine/NuklearGUI.cpp**
    ```
    if (level < Level::SIX)
    {
      ...  
    }
    ```
    by
    ```
    if (level < Level::SEVEN)
    {
      ...  
    }
    ```
- For each JSON in saves folder, add a **0** in the **"stars_campaign"**
- go to the root folder and **re-build**