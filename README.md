# Raider of the Lost World
--
Raider of the Lost World is a 2-D platformer developed by Adrián Font and Dídac Romero 
(Links to both Githubs below!) for the Development subject at UPC's University degree of 
Videogame Design and Development. 
We mainly used C++, xml and Tiled to create the game.
We also have some Profiling working with Brofiler, use it if you want to measure time on different methods and actionsfrom our game!

Thank you for checking our game!

### Gameplay

<iframe width="560" height="315" src="https://www.youtube.com/embed/BoTzxbeSZW8" frameborder="0" allow="accelerometer; autoplay; encrypted-media; gyroscope; picture-in-picture" allowfullscreen></iframe>

---
### CONTROLS:


(keyboard) Playing Normally
  MOVE RIGHT: D
  
  MOVE LEFT: A
  
  JUMP: W
  
  CROUCH: S
  
  ATTACK: G       

  PAUSE GAME: ESC

You can perform different attacks depending on if you're not moving, crouching, whilst going up after jumping 
and if you reach max horizontal speed you can perform a sliding low-attack!

You can perform a double jump by pressing W whilst in the air.

(keyboard) Playing with GodMode Activated

MOVE RIGHT: D

MOVE LEFT: A

MOVE UP: W

MOVE DOWN: S

---
 ### DEBUG FUNCTIONALITIES

* KEY F1: Start from the very first level
*  "  F2: Start from the beginning of the current level
*  "  F4: Switch maps
*  "  F5: Save the current state 
*  "  F6: Load the previous state (even across levels!)
*  "  F9: To view colliders/Logic/Enemy Pathfinding
*  "  F8: UI Debug
*  "  F10: God Mode (you can fly and nothing will kill you)
*  "  F11: Enable/Disable FPS Cap to 30 FPS

---
  ### POTENTIAL INNOVATIONS

-The enemy Bat emmits its sound with Panning and distance attenuation!
	If you want to play around with how it sounds without running around the bat, 
	you can right click with the mouse, and a sound will be emmited from
	that position in respect to the player. (*Use headphones to experience this innovation*,
	as if you're using speakers you will have to turn  the volume too much or separate them a good distance.
	If you are using a mono speaker you will not experience the effect under any circumstance.)
	
-The player has 3 types of attacks. One attack whilst stopped, one when going at max horizontal speed
which will perform a low-slide attack, and another one while going upwards.

-Only collisions on the screen are checked, the rest of colliders outsid ethe camera won't perform any operation.

-Only elements inside the camera are printed (except for the background parallax).

-Enemies have a Range of perception.

-When the player picks up more than three lifes,this forth is applied as a shield which will absorve damage of enviromental traps
and enemies.

---
 ### Github links

Adrián Font Romero: [AdrianFR99](https://github.com/AdrianFR99)

Dídac Romero Camó: [didacromero](https://github.com/DidacRomero)

Repository of the game: [Raider of the Lost world](https://github.com/AdrianFR99/Raider-of-the-Lost-World/)
---

### TEAM
 
 
 <p align="center">
  <img src="https://raw.githubusercontent.com/AdrianFR99/Raider-of-the-Lost-World/gh-pages/team/Team_Didac%26Adrian.jpg"/>
</p>

			(At the left Dídac Romero Camó and at the right Adrián Font Romero)

### CODE SUBSYSTEMS

-MAP
   The subsystem maps have a secuence of funtions that allows to load xml file, this loads the data based in the different layers
   that Tiled a 2D map creator program has, this ones are, tile layers, image layers and game objects.Also the map module draws and 
   creates the colliders of the map. 
-ENTITIES
    This module is the one who manages the differents entities of the game, such as enemies,and interactable items(coins and lifes).
-PATHFINDING
    This subsystem  contains the A star alghorithm which allows entities to move around the map from point to point,includes a debug
    functionalities which shows the path taken by the entitie which is using the alghorithm.
-GUI
    The subsystem is organized as a tree which includes the code for the the functionalities for various element , such as  
    buttons, sliders, texts displays and other elements for the game UI. 
			

 ### WHO DID WHAT? (TASKS)

Assignment 2:
* Enemies: DidacRomero & AdrianFR99
* Entities: DidacRomero & AdrianFR99
* UML: AdrianFR99
* Player Attacks: AdrianFR99
* Deltatime implementation and normalization: AdrianFR99
* Player Rework: AdrianFR99
* Textures adaptations and implemntations: AdrianFR99


* Pathfinding: DidacRomero
* Player Collisions Rework: DidacRomero
* Brofiler Integration: DidacRomero
* Basic sound effects: (attacks not included) DidacRomero (using bfxr.net)
* Music for both levels: DidacRomero
* Environmental sounds (Panning and distance attenuation): DidacRomero

Assignment 3:
	-UI: DidacRomero & AdrianFR99
* coins and lifes entities and the behaviour of them: AdrianFR99
* logo of the game: AdrianFR99
* Music and sound effects of the UI: DidacRomero

We developed the UI in this repository and then we used the code in the project: https://github.com/DidacRomero/Development-Homework
All commits from 27th of november to 16th december are related to UI.

---
 ### Credits

DISCLAIMER: WE DO NOT OWN ANY OF THE ART HEREBY CREDITED, IT ONLY BELONGS TO THE ARTISTS THEMSELVES

Player Sprites made by rvros: https://rvros.itch.io/animated-pixel-hero

Ground Undead enemy sprites by: AceWay and Darkeyed link to thehir itch.io page https://finalgatestudios.itch.io/undead-sprite-pack

Attack sounds by: Mike Koenig under an Attribution 3.0 License link: http://soundbible.com/1008-Decapitation.html
Map Sprites:
Artwork and flying enemy created by Luis Zuno (@ansimuz) link to flying enemy: https://ansimuz.itch.io/grotto-escape-chibi-monsters

License for Everyone. This piece is under a CC-BY-3.0 License http://creativecommons.org/licenses/by/3.0/

You can copy, modify, distribute and perform the work, even for commercial purposes, as long as you give appropiate credit. 

You are not require to credit this work if you are or had been a Patreon Supporter at https://www.patreon.com/ansimuz

Thanks get more like these at: https://www.patreon.com/ansimuz

-------------------------------------------------------------
### License & Copyright

Copyright (c) 2018 Adrián Font Romero & Dídac Romero Camó

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
