// Preparing player to play the game.
// Clearing all the player's data like iterators and variables.
// Additionally we're loading cache (preloading anims, per-player textdraws) and spawning player.
PrepareDataAfterLogin(playerid)
{
    LoadCache(playerid);
    ResetVariables(playerid);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 50);

    CreatePlayerLabel(playerid);
    SetPlayerVirtualWorld(playerid, eCharsInfo[playerid][e_iPosVw]);
    SetSpawnInfoEx(playerid, GetPlayerSkinEx(playerid), eCharsInfo[playerid][e_fPosX], eCharsInfo[playerid][e_fPosY], eCharsInfo[playerid][e_fPosZ], eCharsInfo[playerid][e_fPosAng]);

    SkillsTd_FillPlayerSkillBars(playerid);
    DB_UpdatePlayerLastLoginDate(playerid);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);
}