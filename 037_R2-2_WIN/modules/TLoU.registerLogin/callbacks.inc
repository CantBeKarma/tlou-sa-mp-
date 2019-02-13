#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerPrepareToSign ------------------ 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerPrepareToSign(playerid)
{
    FreezePlayer(playerid, false, -1);
    SetPlayerVirtualWorld(playerid, playerid);
    SetPlayerPos(playerid, -260.3906, 674.5479, 280.5438);

    ClearChat(playerid);
    CreatePlayerIntroTds(playerid);
    ShowPlayerBlackScreen(playerid);
    DisableDefaultAmbientNoise(playerid);

    SetPlayerCameraPos(playerid, -258.3556, 670.9673, 277.2066);
    SetPlayerCameraLookAt(playerid, -257.3723, 671.1423, 277.3569);

    GetPlayerName(playerid, ePlayerInfo[playerid][e_sName], MAX_PLAYER_NAME);
    ToggleHUDComponentForPlayer(playerid, HUD_COMPONENT_ALL, false);

    for(new i = 0; i != sizeof(RemoveObjectsData); i++)
    {
        RemoveBuildingForPlayer(playerid, RemoveObjectsData[i], 0.0, 0.0, 0.0, 6000.0);
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerKeyStateChange ------------------ 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(PRESSED(KEY_ENTER))
    {
        if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) && IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_MainMenu_Prompt]))
        {
            TextDrawHideForPlayer(playerid, eTextDrawInfo[e_MainMenu_Prompt]);

            new isPlayerRegistered = DB_CheckAccountExistence(playerid);
            ShowRegisterLoginDialog(playerid, isPlayerRegistered);
        }
    }
    return 1;
}