Campaign map's editor
===================


It's JSON files with the following values:

----------


grid
-------------

> It's a **2D** array of int. Each int represent a **type of occupation**:

> - **0**: empty
> - **-1**: destructible block
> - **-2**: indestructible block
> - **1**: initial player position (one and only one per grid)
> - **2-9**: reserved for mobs types
>  - **2**: bomb-mob
>  - **3-9**: empty
> - **10-19**: reserved for bonus types
>  - **10**: power-bomb +1
>  - **11**: power-bomb -1
>  - **12**: nb-bomb +1
>  - **13**: nb-bomb -1
>  - **14**: speed +1
>  - **15**: speed -1
>  - **16-19**: empty

exemple:
```
"grid":	[[ 0, 0, 0, 0, 0],
         [ 0,-2,-1,-2, 0],
         [ 0,-1, 0,-1, 0],
         [ 0,-2, 0,-2, 0],
         [ 0, 0, 1, 0, 0]]
```


model
-------------

> It's the **design of blocks:**

> - **bricks**: destructible blocks design
> - **undestroyable**: indestructible blocks design

exemple:
```
"models":{
    "bricks": "assets/models/bricks.obj",
    "undestroyable": "assets/models/undestroyable.obj"
  }
```


sun
-------------

> It's **sun parameters:**

> - **pos**: position of the sun
> - **color**: the RGB color (0 to 1)

exemple:
```
"sun":{
    "pos":[20.0, 20.0, 20.0],
    "color":[0.682, 0.356, 0.803]
  }
```


stars
-------------

> It's an array who give **the maximum number** of bomb to use to win 3, 2 and 1 **stars**

exemple:
```
"stars":[1,2,3]
```
