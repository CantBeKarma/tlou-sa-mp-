<p align="center">
  <img src="tlou-logos/tlou-logo-medium.png">
</p>

# The Last of Us (SA-MP) Project
So here it is, since the beginning of 2014 december, after many failed attempts and re-writing entire project from scratch twice (this is the third one) here's another failed attempt of mine of launching this gamemode.

As much as I'm in love with the original The Last of Us game and I was addicted with San Andreas Multi Player (SA-MP) since it's very early 0.1b version launch back in 2005 I cannot work on that project anymore and I guess it's my time to depart from the community.

# Third attempt 
This project's (3rd attempt) rewriting process has started at 2019-01-10, **I didn't use any SVN back then yet** so that may be why some of the commits have started somewhere mid 2019 and not from the beginning.

The reason why another attempt has occured is because I couldn't read my own code after coming back to it, so I've decided to switch onto modular programming style and now not to say - it is definetely way easier to understand the logic of such a complex gamemode and debug instead of one huge *.pwn file.

# Future
Even though I'm trying not to be very connected with the code I have written for this project - *I can't* - simply because if I wouldn't then I would have no-one to thank for my current Software Developer ASP.NET Job. Crazy how simple procedural PAWN language can lead you to higher standards.

I am not interested in participating of the development of that gamemode anymore. If you message me I might answer some questions but other than that I wouldn't want to write any more code for it.

# Prerequisites
In order to continue the development of the project it is necessary to know:

| Prerequisite                                          	| Description                                                                                                       	|
|-------------------------------------------------------	|-------------------------------------------------------------------------------------------------------------------	|
| Pawn Language                                         	| I would recommend some regular level of knowledge, not basic definetely                                           	|
| C/C++ Language                                        	| useful for plugin development and fixes                                                                           	|
| Database queries knowledge                            	| phpMyAdmin, SQL                                                                                                   	|
| 3Ds Max                                               	| to create/edit/rig 3D models for the game & custom animations crafting & managment                                	|
| Install Creator Pro                                   	| to create an installer for the files (which are going to be installed on the client machine)                      	|
| Sony Vegas                                            	| or any other audio-editing tool with which you can create SFX                                                     	|
| GIT                                                   	| to carry on with Subversion Control                                                                               	|
| Alci's IMG Editor, Alci's SAAT, CollEditor2, MagicTXD 	| a basic set of tools to manage GTA: SA files such as libraries, collision, texture files and models               	|
| The Last of Us original game                          	| that's helpful if you played it - so you'll know what features to implement apart from the ones I did             	|
| SA-MP                                                 	| understand SA-MP's sync. logic, features and other things that are required to create a multi-player gamemode 	|

# Branches

This repository is split into 2 branches which either of them contain different stuff:

- master
  
| Title     | Link                                                               | Description                                                                                                                                 |
|-----------|--------------------------------------------------------------------|---------------------------------------------------------------------------------------------------------------------------------------------|
| Docs      | https://github.com/Jedrzej94/tlou-sa-mp-/tree/master/documentation | Some documentation parts from 2014-2016 & 2019                                                                                              |
| Assests   | https://github.com/Jedrzej94/tlou-sa-mp-/tree/master/assets        | Game assets such as in-game models, textures, sprites, animations etc.                                                                      |
| Installer | https://github.com/Jedrzej94/tlou-sa-mp-/tree/master/installer     | An installation package ready to compile for Install Creator Pro software (creates an *.exe file for the client machine to be installed on) |


- 0_3_7_R2-2_sampctl

The heart of the gamemode - code itself.

# Forks & Pull requests

Feel free to fork this repository. If you're willing to have anything implemented into this repository as well, you may create a pull request and I will have a look into it.

Remember to keep that original author somewhere.

# Dependencies

This project require some dependencies in order to run correctly. All of them are compiled-in already but here's the list of them anyway:

| Name        | Language | Type    |                                            |
|-------------|----------|---------|--------------------------------------------|
| FCNPC       | C/C++    | Plugin  |                                            |
| PawnPlus    | C/C++    | Plugin  |                                            |
| YSF         | C/C++    | Plugin  |                                            |
| Audio       | C/C++    | Plugin  |                                            |
| Crashdetect | C/C++    | Plugin  |                                            |
| log-plugin  | C/C++    | Plugin  |                                            |
| MySQL       | C/C++    | Plugin  |                                            |
| Sscanf2     | C/C++    | Plugin  |                                            |
| Streamer    | C/C++    | Plugin  |                                            |
| tlou_server | C/C++    | Plugin  |                                            |
| PathFinder  | C/C++    | Plugin  | Implemented internally in FCNPC            |
| projectiles | PAWN     | Include |                                            |
| tlou        | PAWN     | Include | Related to tlou_server plugin (ex. SA-MP+) |