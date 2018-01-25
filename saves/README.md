# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation throw our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **saves folder**! You can found here all **saves slots** in JSON mode. If you want to bullshit all stars progression, you are at the good place ;)


## Saves slots
The name of the file give the slot number
### stars_campaign
It's an integer array. The position is the level in campaign mode and the value is the stars number. For exemple:
```
"stars_campaign":[0,2,1,0,3,0]
```
means that you have:
 - 0 star for levels 1, 2 and 6
 - 1 star for level 3
 - 2 stars for level 2
 - 3 stars for level 5
### campaign_max_level
Unused value actually.
### master_volume
It's a float between 0 and 1. Give... master volume!
### music_volume
It's a float between 0 and 1. Give... music volume!
### effects_volume
It's a float between 0 and 1. Give... effects volume!
### screen_format
It's used to give the resolution pref. For exemple:
```
"screen_format":{
	"resolution_width":1280,
	"resolution_height":720,
	"mode":0
}
```
> - mode 0: **windowed**
> - mode 1: **fullscreen**
### keys
It's the keyBinding for each possible movement.

Exemple:
```
"keys":{
	"left":97,    //'a'
	"right":100,  //'d'
	"up":119,     //'w'
	"down":115,   //'s'
	"bomb":32	  //space
}
```
### last_save
Date of the last slot save in standard format.

Exemple:
```
"last_save":"22-01-2018 09:30:50"
```