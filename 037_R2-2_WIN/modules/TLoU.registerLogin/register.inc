// This function performs account register and character
function RegisterPlayerAccount(playerid, password[])
{
    new hashStr[65];
    SHA256_PassHash(password, "tlou2014", hashStr, sizeof(hashStr));
    DB_RegisterNewAccount(playerid, hashStr);
}

// Preparing backend data for user.
PrepareDataAfterRegister(playerid)
{
    LoadCache(playerid);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);

    // Give player empty ingredient values to DB.
    for(new i = 0; i != 5; i++)
    {
        DB_GivePlayerItem(i, 0, .ownerUid = eCharsInfo[playerid][e_iUid]);
    }

    Iter_Clear(iAudioHandle3D<playerid>);

    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetPlayerHealthEx(playerid, 100.0);

    SetSpawnInfoEx(playerid, 312, 2491.2930, -1672.1010, 13.3359, 66.4064);
    SpawnPlayer(playerid);

    for(new i = 0; i != MAX_WEAPON_SLOTS; i++)
    {
        ePlayerWeapon[playerid][e_iWeaponClipAmmo][i] = -1;

        if(i < 4) WeapMenu_Ls_WeaponId(playerid, i) = -1;

        if(i < 3) WeapMenu_Rs_WeaponId(playerid, i) = -1;
    }

    FadePlayerScreen(playerid, FADE_OUT);
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "Player %s [%d] has signed up for the first time!", ePlayerInfo[playerid][e_sName], playerid);
    return 0;
}