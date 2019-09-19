// Try signing in to the player's account with password specified.
TryLoginPlayerToAccount(playerid, password[])
{
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s is trying to sign in to account of Guid: %d", PlayerName(playerid), GetPlayerGuid(playerid));

    inline OnPlayerPasswordReceived()
    {
        new rowsCount;
        cache_get_row_count(rowsCount);

        if(!rowsCount)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "There's missing password for player's account: %s (GUID: %d)", PlayerName(playerid), GetPlayerGuid(playerid));
            return false;
        }

        new row_password[BCRYPT_HASH_LENGTH];
        cache_get_value_name(0, "password", row_password);

        bcrypt_check(password, row_password, "BCrypt_OnLoginPasswordChecked", "d", playerid);
    }

    new query[63];
    mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `password` FROM `player_data` WHERE `guid` = %d LIMIT 1", GetPlayerGuid(playerid));
    mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnPlayerPasswordReceived);
    return true;
}

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

    SkillTd_FillPlayerSkillBars(playerid);
    DB_UpdatePlayerLastLoginDate(playerid);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);
    
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (UID: %d | GUID: %d) has logged in successfully!", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
}

ShowPlayerLoginDialog(playerid)
{
    static tit_signIn[8] = "Sign in";
    static msg_signIn[144] = "Welcome back to the \"The Last of Us (SA-MP) Project\"\nWe have found you in our database. Please enter your account password below to sign in:";

    inline Response(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, listitem, inputtext
        if(!response)
        {
            Kick(playerid);
            return false;
        }

        if(isnull(inputtext))
        {
            Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signIn, msg_signIn, "Sign in", "Quit");
            return false;
        }
        TryLoginPlayerToAccount(playerid, inputtext);
    }
    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signIn, msg_signIn, "Sign in", "Quit");
    return true;
}