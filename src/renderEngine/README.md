# Bomberman42

This is a 42 school project. The goal was to recreate the original game **bomberman** in 3D. The programming language needed to be **C++** with the **OpenGL** library for the graphical part. The detail in the game was an important part of the final mark.

## For developers

If you read that, that's mean you are interested in the project conception. To have an easier navigation through our folders, we have make a readme in each one to explain one point of interest in correspondence with this folder.

> Here is the **gameEngine folder**! We would explain you **how the game logic works**. All the code in this folder is all mostly organized around the class "GameEngine" wich is supposed to be the central element that contains all of the other, more specialized, "managers" that only take care of one certain aspect of the general logic.


## RenderEngine

This is a classical forward rendering engine. It uses 3 passes for render. The first render the shadows in a frameBuffer wich is use in the second normal pass. A third pass render blended model such as flames and particles.
Each kind of entity call is own render function. Every models use instantiation, so the draw method of models expects an array of transform matrix.
The most shaders variables are automaticaly set in the model engine in function of textures.
The engine implements normal mapping, specular mapping, directional lighting, omnidirectional lightning (finally unused);
