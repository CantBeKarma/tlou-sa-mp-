// Hashes player's input from dialog box during register.
HashPlayerPassword(playerid, password[])
{
    bcrypt_hash(password, BCRYPT_COST, "BCrypt_OnPasswordHashed", "d", playerid);
}

// Preparing player to play the game.
// Clearing all the player's data like iterators and variables.
// Additionally we're loading cache (preloading anims, per-player textdraws) and spawning player.
PrepareDataAfterRegister(playerid)
{
    LoadCache(playerid);
    ResetVariables(playerid);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetPlayerHealthEx(playerid, 100.0);
    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 50);

    CreatePlayerLabel(playerid);
    SetPlayerVirtualWorld(playerid, DEFAULT_VIRTUAL_WORLD);
    SetSpawnInfoEx(playerid, 312, 2491.2930, -1672.1010, 13.3359, 66.4064);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);
}