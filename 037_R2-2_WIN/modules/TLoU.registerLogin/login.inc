// Load player and character data
function LoginPlayerToAccount(playerid, password[])
{
    new query[70];
    mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `salt` FROM `player_data` WHERE `guid` = %d LIMIT 1", GetPlayerGuid(playerid));
    mysql_tquery(eServerInfo[e_SqlHandler], query, "OnPlayerSaltReceived", "ds", playerid, password);

    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s is trying to sign in to account of Guid: %d", PlayerName(playerid), GetPlayerCharUid(playerid));

    return true;
}

// Player's SALT from database received. Signing in going on from here..
function OnPlayerSaltReceived(playerid, password[])
{
    new rowsCount;
    cache_get_row_count(rowsCount);

    if(!rowsCount)
    {
        return false;
    }

    new row_salt[11];
    cache_get_value_name(0, "salt", row_salt);
    
    new hashStr[65];
    SHA256_PassHash(password, row_salt, hashStr, sizeof(hashStr));
    
    DB_RequestPlayerData(playerid, hashStr);

    return true;
}

// Prepare player's data during sign in.
function PrepareDataAfterLogin(playerid)
{
    LoadCache(playerid);

    new Float:charHealth;

    cache_get_value_name_int(0, "uid", eCharsInfo[playerid][e_iUid]);
    cache_get_value_float(0, "health", charHealth);
    cache_get_value_name_int(0, "skinid", eCharsInfo[playerid][e_iSkinId]);
    cache_get_value_float(0, "last_x", eCharsInfo[playerid][e_fPosX]); 
    cache_get_value_float(0, "last_y", eCharsInfo[playerid][e_fPosY]); 
    cache_get_value_float(0, "last_z", eCharsInfo[playerid][e_fPosZ]);
    cache_get_value_float(0, "last_ang", eCharsInfo[playerid][e_fPosAng]);
    cache_get_value_name_int(0, "last_int", eCharsInfo[playerid][e_iPosInt]);
    cache_get_value_name_int(0, "last_vw", eCharsInfo[playerid][e_iPosVw]);
    cache_get_value_name_int(0, "supplements", eCharsInfo[playerid][e_iSkills_SuppsAmount]);
    cache_get_value_name_int(0, "skill_maxhealth", Skill_GetLevel(playerid, SKILL_ITEM_MAXHP));
    cache_get_value_name_int(0, "skill_listenmode", Skill_GetLevel(playerid, SKILL_ITEM_LISTEN_DIST));
    cache_get_value_name_int(0, "skill_crafting", Skill_GetLevel(playerid, SKILL_ITEM_CRAFT_SPEED));
    cache_get_value_name_int(0, "skill_healing", Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED));
    cache_get_value_name_int(0, "skill_weaponsway", Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY));
    cache_get_value_name_int(0, "skill_shivmaster", Skill_GetLevel(playerid, SKILL_ITEM_SHIV_MASTER));

    ResetVariables(playerid);
    DB_LoadPlayerItems(playerid);
    DB_LoadPlayerCollectibles(playerid, COLL_ITEM_ARTIFACTS);
    DB_LoadPlayerCollectibles(playerid, COLL_ITEM_FIREFLY_PEND);

    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);
    SetPlayerSkillLevel(playerid, WEAPONSKILL_SAWNOFF_SHOTGUN, 50);

    SkillTd_FillPlayerSkillBars(playerid);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetPlayerHealthEx(playerid, (charHealth <= 0.0) ? (25.0) : (charHealth));

    CreatePlayerLabel(playerid);
    SetPlayerVirtualWorld(playerid, eCharsInfo[playerid][e_iPosVw]);
    SetSpawnInfoEx(playerid, GetPlayerSkinEx(playerid), eCharsInfo[playerid][e_fPosX], eCharsInfo[playerid][e_fPosY], eCharsInfo[playerid][e_fPosZ], eCharsInfo[playerid][e_fPosAng]);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (charUid: %d) has logged in successfully!", PlayerName(playerid), GetPlayerCharUid(playerid));

    SpawnPlayer(playerid);
    return 0;
}