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
//--------------- BCrypt_OnPasswordHashed -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public BCrypt_OnPasswordHashed(playerid)
{
    new passwordHash[BCRYPT_HASH_LENGTH];
    bcrypt_get_hash(passwordHash);

    RegisterPlayerNewAccount(playerid, passwordHash);
    PrepareDataAfterRegister(playerid);

    wait_ms(1000);
    SpawnPlayer(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------ BCrypt_OnLoginPasswordChecked --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public BCrypt_OnLoginPasswordChecked(playerid)
{
    if(!bcrypt_is_equal()) // incorrect password.
    {
        return ShowPlayerLoginDialog(playerid);
    }

    LoadPlayerCharacterData(playerid);
    PrepareDataAfterLogin(playerid);

    wait_ms(1000);
    SpawnPlayer(playerid);
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