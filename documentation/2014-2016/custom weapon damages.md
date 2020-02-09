

# Logic of how weapon damages are being calculated in-game

First of all, we have to block default weapon damages, from affecting player's health. To achieve that, I am setting all the player's in team id: '0', because if all the players are in the same team, they are not allowed to damage each other. Whenever player spawns, his team is set to '0'.

Next step is OnPlayerGiveDamage call back, where the main code is being executed, to set player's health minus damage given by weapon. What happens, player's health will be reduced by custom weapon damage i.e.: player's health is 30.0 minus (damage of pistol) 15.0 then the result will be 15.0.
If distance between players is bigger than 20 meters, reduce amount of damage by 40%

|    Weapon name        |    Weapon custom damage    |
|-----------------------|----------------------------|
|    9mm pistol         |    15.0                    |
|    Revolver           |    25.0                    |
|    Shorty             |    35.0                    |
|    Shotgun            |    40.0                    |
|    Hunting rifle      |    50.0                    |
|    Assault rifle      |    10.0                    |
|    Military Sniper    |    80.0                    |
|    Pipe               |    10.0                    |
|    Scissor pipe       |    20.0                    |
|    Fists              |    8.0                     |


# Side note as of 2020-02-08
This logic is probably out-of-date. Something similar might have been implemented since 2015