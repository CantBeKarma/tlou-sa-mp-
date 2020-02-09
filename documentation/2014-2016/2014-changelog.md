

# Change log of the 1st attempt on the project
---
---
---

# Fixes
*09/05/2014*
* Fixed a problem, when player died, his current weapon ammo did not saved properly. So after spawn he had old ammo amount (before he shot).
* Problem with custom weapon damages should be fixed now.

*10/05/2014*
* Fixed problem with streamer, where "*** Streamer_AppendArrayData" warning appeared in a console.
* Fixed item object destroying, when player is picking it up.

*12/05/2014*
* Fixed problem, when player had equipped the same type of weapon that he tried to pick up. The problem was: ammo did not add.
* Fixed problem, if player had equipped weapon and he left the game, ammo didn't update in database. That has been fixed.
* When player picked up a weapon that he did NOT have, it will be attached to his body automatically.

*16/05/2014*
* Added more security questions, and more fixes has been provided.

*18/05/2014*
* Fixed problem with picking up the items - and adding ammuntion to the weapon if we have one equipped.
  
*19/05/2014*
* After week of fight with weapons, they're finally fixed. I had a problem with removing the weapon from player's hand when they switch to the other one with weapon menu. Player is allowed to equip only one weapon at the time (just like original game). The problem was, that weapon stayed in player's (GTA SA) inventory, and he/she was able to scroll the weapons.

Problem location:
Removing weapon with SetPlayerAmmo function.

Time: 23:14
* Since the gamemode is really complex, many fixes has been done again, that I have found (regarding picking up items, equpping them etc.)

*20/05/2014*
* Sometimes sound of moving around the backpack wasn't played. That has been fixed.

---
**MORE FIXES WHICH WEREN'T DOCUMENTED**
---

---
---
---

# Updates & new features


*08/05/2014*
* Updated MySQL version from R5 to R38.

*12/05/2014*
* Edited database fields and rows completely. What means, that whole gamemode had to have re-written every queries and tested everything again. The purpose of that was, to remove registration system from game, and player will have to create account in forum and manage his characters in forum as well to play on the server.
* Removed custom class selection from the gamemode (reason above).
* Added saving player's data when gamemode shuts down for some reason (attacks?)
* Pipe can be upgraded now via backpack crafting menu, to add some scissors to her.
* When player is not holding any weapon, and if he/she has pipe/scissor pipe in inventory, weapon will appear in player's hands automatically when they're trying to hit something/someone. If player doesn't use it, it will go back to his back.

*13/05/2014*
* Updated version of YSF. Very useful for backpack settings (less lag and less code to call).

*16/05/2014*

Time: 01:23.
* Debugged whole script, to remove unnecessary code lines and save some arrays and other data manipulation stuff.
* Moving around the backpack is not making lag anymore as it used to do before.
* New crosshair | new symbol when you shot player in a body | new symbol when you did headshot to the other player.
* Whenever you open your backpack now, it will have selected your latest item you had selected before you closed it.
* New lamp posts and new traffic lights models (+ fixed collisions).

Time: 13:32.
* Removed HEX counting arrays, to make fade screen and blood on screen (not needed anymore).

Time: 17:50.
* Edited weapon damages system, to use less resources and less amount of lines. Also, damages depends on the distance between the players'. If distance is bigger than 20 meters, damage will be reduced by 40%.

*19/05/2014*
* Game objects are now being loaded from the file(s).

Time: 23:24.
* While user is crafting (using backpack menu), the speed between moving the objects (items) has been significantly RAISED! FPS level stays exactly the same, not as before!

FPS level before update: ~40.
FPS level after update:   ~58 - 60.

Please, consider my laptop into this.

Time: 00:35.
* Added player Frames - Per - Second (FPS) check in player's stats (/stats).

*20/05/2014*
* Saved ~500 lines of code, responsible for weapon switch menu and backpack menu's. Also, improved speed again.

*21/05/2014*
* Added animations to the game and their list (CMD: /anim).
* Added callback OnPlayerAnimationApply(playerid, type).
* Added callback OnPlayerAnimationFinish(playerid, animlib[], animname[]).

*22/05/2014*
* Created new system for dialogs, to avoid any bugs, problems and crashes.

*28/05/2014*
* Since now, whenever player hide his fire weapon, and he had some ammo in it, he can equip the weapon straight away by pressing RIGHT MOUSE BUTTON.

*29/05/2014*
* Revolver has 6 bullets ammo capacity now. Whenever player shoot the weapon, or change the weapon, ammo capacity for revolver (deagle) will be 6 always.
* Weapon(s) icons is/are in a grey color now, if we don't have any ammo to it. Otherwise, it will be white (original game concept/feature).

*** COLLEGE BREAK + REAL JOB ***

*19/10/2014*
* Items in weapon switch menu don't appear until player finds the item. Once it's found, it will appear in the weapon menu (http://bankfotek.pl/image/1803461)

*24/10/2014*
* When you shoot player/NPC in a head, different symbol will be shown around the crosshair.

* NPCs are having different values of health now (may be different before we open the server):
- Runner has 120 health points.
- Stalker has 150 health points.
- Clicker has 200 health points.
- Bloater has 250 health points.

* When you shoot an NPC in a stomach (torso) from shotgun weapon, and if it was either: runner, stalker, clicker there's a CHANCE (not every time) that NPC will fall on back on the ground.

* If you shoot runner in a head, runner will be killed instantly.

*01/11/2014*
* Added OnPlayerPauseStateChange callback.
* When player enters pause menu, text above the head will appear "Away From Keyboard".
* Added a special function to get size of all the objects added by mappers.

* Draw distance and stream distance of the objects is specified now by object size. Bigger object - bigger draw distance, smaller object - smaller draw distance. Purpose of that is to increase FPS amount and to avoid loading unnecessary objects, which player can't see from that distance anyway.

* New vehicle models (http://i.imgur.com/uDN87nr.png)

*04/11/2014*
* All hud components were hidden. New heads-up display will be created soon.
* New weapons have been added: Shorty, Hatchet, Machete.

*05/11/2014*
* Added server logo, when connected to the game
* Added object loading from text files (useful for my side, for debugging script and checking progress) (http://i.imgur.com/l37aFo7.png)

*16/11/2014*
* First category of the backpack - final version has been done (http://imgur.com/JaZ6xte,WDICPV4,V75sBGA,7v4AP3f, http://i.imgur.com/dwOB7x0.jpg)
* Frames Per Second (FPS) while using backpack for me is: 60 - 70. Also, connecting to the server (speed) has been increased.
* When someone has shot his weapon, switched it to another gun and then switched back, ammo capacity stays the same. Normally in GTA, when you switch the weapon it automatically reloads (avoid fast-reloaders).

*17/11/2014*
* Added waves, when storm weather is on
