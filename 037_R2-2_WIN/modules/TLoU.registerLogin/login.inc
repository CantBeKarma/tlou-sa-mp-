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
}

ShowPlayerLoginDialog(playerid, bool:wrongPassword = false)
{
    static tit_signIn[8] = "Sign in";
    static msg_signIn[144] = "Welcome back to the \"The Last of Us (SA-MP) Project\"\nWe have found you in our database. Please enter your account password below to sign in:";
    new msg_signInFailed[144 + 41];
    
    if(wrongPassword)
    {
        SCMF(playerid, -1, "Wrong pass!");
        strcat(msg_signInFailed, msg_signIn, sizeof(msg_signInFailed));
        strcat(msg_signInFailed, "\n\n\t\t\t\t       {FF0000}Wrong password!", sizeof(msg_signInFailed));
    }

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

        DB_TryLoginPlayerToAccount(playerid, inputtext);
    }
    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signIn, (wrongPassword) ? (msg_signInFailed) : (msg_signIn), "Sign in", "Quit");
    wrongPassword = false;
    return true;
}