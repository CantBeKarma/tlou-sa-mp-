#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//#region Fire areas callbacks

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerEnterDynamicArea ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    hook OnPlayerEnterDynArea(playerid, areaid)
    {
        if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid) || IsPlayerBurning(playerid))
        {
            return 1;
        }

        if(IsPlayerInFireArea(playerid, areaid))
        {
            SCMF(playerid, -1, "I'm in the fire area!");

            SetPlayerOnFire(playerid, eFireData[areaid][e_iFireCreatorId]);
        }
        return 1;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerLeaveDynamicArea ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    hook OnPlayerLeaveDynArea(playerid, areaid)
    {
        if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid) || !IsPlayerBurning(playerid))
        {
            return 1;
        }

        if(IsPlayerInFireArea(playerid, areaid))
        {
            SCMF(playerid, -1, "I left fire area!");
            defer PutOutFireOnPlayer(playerid);
        }
        return 1;
    }

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStartBurning -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartBurning(playerid, fireCreatorId)
{
    SCMF(playerid, -1, "I began burning...");

    Disable_OnPlayerLookAt(playerid);

    HideWeaponMenu(playerid);
    HidePlayerWeapon(playerid);
    HideBackpack(playerid, .forceClose = true);

    ePlayerInfo[playerid][e_iTimer_Burning] = repeat OnPlayerBurn(playerid, fireCreatorId);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerBurn ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer OnPlayerBurn[FIRE_DAMAGE_RATE](playerid, fireCreatorId)
{
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - FIRE_DAMAGE_AMOUNT, fireCreatorId, WEAPON_MOLOTOV);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStopBurning ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopBurning(playerid)
{
    stop ePlayerInfo[playerid][e_iTimer_Burning];
    SCMF(playerid, -1, "I have finished burning.");
    
    if(IsPlayerDead(playerid))
    {
        return 1;
    }
    
    Enable_OnPlayerLookAt(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    Iter_Clear(ServerFires);
    return 1;
}