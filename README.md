# Raider-of-the-Lost-World
Raider of the Lost World is a 2-D platformer developed by Adrián Font and Dídac Romero 
(Links to both Githubs below!) for the Development subject at UPC's University degree of 
Videogame Design and Development. 
We mainly used C++, xml and Tiled to create the game.
We also have some Profiling working with Brofiler, use it if you want to measure time on different methods and actionsfrom our game!

For the 2nd Assignment, there are enemies that can follow the player by using an A* pathfinding algorithm.
There are 2 enemies implemented in the game a flying Bat, and a ground Undead.
Undeads will only follow you if you are close enough to their vertical position and youre under their position or at the same 
height as them. 
Flying Bats will fly around chasing you when you enter their perception zone.
 
# ---------------------------CONTROLS:--------------------------------
(keyboard) Playing Normally
MOVE RIGHT: D
MOVE LEFT: A
JUMP: W
CROUCH: S
ATTACK: G            

You can perform different attacks depending on if you're not moving, crouching, whilst going up after jumping 
and if you reach max horizontal speed you can perform a sliding low-attack!

You can perform a double jump by pressing W whilst in the air.

(keyboard) Playing with GodMode Activated
MOVE RIGHT: D
MOVE LEFT: A
MOVE UP: W
MOVE DOWN: S

# ----------------------DEBUG FUNCTIONALITIES-------------------------

KEY F1: Start from the very first level
 " F2: Start from the beginning of the current level
 " F5: Save the current state 
 " F6: Load the previous state (even across levels!)
 " F9: To view colliders/Logic/Enemy Pathfinding
 " F8: Switch Maps
 " F10: God Mode (you can fly and nothing will kill you)
 " F11: Enable/Disable FPS Cap to 30 FPS

# --------------------POTENTIAL INNOVATIONS---------------------------
-The enemy Bat emmits its sound with Panning and distance attenuation!
	If you want to play around with how it sounds without running around the bat, 
	you can right click with the mouse, and a sound will be emmited from
	that position in respect to the player.
-The enemy has 3 types of attacks. One attack whilst stopped, one when going at max horizontal speed
which will perform a low-slide attack, and another one while going upwards.

-Only collisions on the screen are checked, the rest of colliders outsid ethe camera won't perform any operation.
-Only elements inside the camera are printed (except for the background parallax).
-Enemies have a Range of perception.

# ----------------------Github links-----------------------------------
Adrián Font Romero: https://github.com/AdrianFR99
Dídac Romero Camó: https://github.com/DidacRomero

Repository of the game: https://github.com/AdrianFR99/Raider-of-the-Lost-World/


# -----------------------WHO DID WHAT?---------------------------------

Assignment 2:

	-Enemies: DidacRomero & AdrianFR99
	-Entities: DidacRomero & AdrianFR99
-UML: AdrianFR99
-Player Attacks: AdrianFR99
-Deltatime implementation and normalization: AdrianFR99
-Player Rework: AdrianFR99
-Textures adaptations and implemntations: AdrianFR99

-Pathfinding: DidacRomero
-Player Collisions Rework: DidacRomero
-Brofiler Integration: DidacRomero
-Basic sound effects: (attacks not included) DidacRomero (using bfxr.net)
-Music for both levels: DidacRomero
-Environmental sounds (Panning and distance attenuation): DidacRomero

# ----------------------Credits----------------------------------------
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
------------------------------------------------------------------------

Thank you for checking our game!

# Copyright (c) 2018 Adrián Font Romero & Dídac Romero Camó

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
