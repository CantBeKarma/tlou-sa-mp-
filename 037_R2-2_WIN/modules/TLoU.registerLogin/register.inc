// This function performs account register and character
function RegisterPlayerAccount(playerid, password[])
{
    new salt[11];
    for(new i = 0; i < 10; i++)
    {
        salt[i] = random(79) + 47;
    }
    salt[10] = 0;

    new hashStr[65];
    SHA256_PassHash(password, salt, hashStr, sizeof(hashStr));
    DB_RegisterNewAccount(playerid, salt, hashStr);
}

// Preparing backend data for user.
PrepareDataAfterRegister(playerid)
{
    LoadCache(playerid);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);

    ResetVariables(playerid);

    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 50);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetPlayerHealthEx(playerid, 100.0);

    CreatePlayerLabel(playerid);
    SetPlayerVirtualWorld(playerid, 0);
    SetSpawnInfoEx(playerid, 312, 2491.2930, -1672.1010, 13.3359, 66.4064);
    SpawnPlayer(playerid);

    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "Player %s [%d] has signed up for the first time!", ePlayerInfo[playerid][e_sName], playerid);
    return 0;
}