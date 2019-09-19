#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerTryToPlay -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTryToPlay(playerid)
{
    if(!DB_DoesPlayerAccountExist(playerid))
    {
        ShowPlayerRegisterDialog(playerid);
    }
    else
    {
        ShowPlayerLoginDialog(playerid);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerRegister --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerRegister(playerid)
{
    ePlayerInfo[playerid][e_iGuid] = cache_insert_id();
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "Registered new account for player: %s (GUID: %d)", PlayerName(playerid), GetPlayerGuid(playerid));

    DB_CreateNewCharacter(playerid);
    PrepareDataAfterRegister(playerid);

    wait_ms(1000);
    SpawnPlayer(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerLogin ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerLogin(playerid)
{
    LoadPlayerCharacterData(playerid);
    PrepareDataAfterLogin(playerid);
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (UID: %d | GUID: %d) has logged in successfully!", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));

    wait_ms(1000);
    SpawnPlayer(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------- OnPlayerPasswordReceived -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerPasswordReceived(playerid, password[])
{
    new rowsCount;
    cache_get_row_count(rowsCount);

    if(!rowsCount)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "There's missing password for player's account: %s (GUID: %d)", PlayerName(playerid), GetPlayerGuid(playerid));
        return false;
    }

    new row_password[BCRYPT_HASH_LENGTH];
    cache_get_value_name(0, "password", row_password);

    bcrypt_check(password, row_password, "BCrypt_OnLoginPasswordChecked", "d", playerid);
    return true;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- BCrypt_OnPasswordHashed -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public BCrypt_OnPasswordHashed(playerid)
{
    new passwordHash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(passwordHash);

    DB_RegisterNewAccount(playerid, passwordHash);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- BCrypt_OnLoginPasswordChecked -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public BCrypt_OnLoginPasswordChecked(playerid)
{
    if(!bcrypt_is_equal())
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Player: %s (GUID: %d) entered wrong password", PlayerName(playerid), GetPlayerGuid(playerid));
        return ShowPlayerLoginDialog(playerid);
    }

    OnPlayerLogin(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerKeyStateChange ------------------ 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_ENTER))
    {
        if(!IsPlayerLogged(playerid) && IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_MainMenu_Prompt]))
        {
            TextDrawHideForPlayer(playerid, eTextDrawInfo[e_MainMenu_Prompt]);
            OnPlayerTryToPlay(playerid);
        }
    }
    return 1;
}