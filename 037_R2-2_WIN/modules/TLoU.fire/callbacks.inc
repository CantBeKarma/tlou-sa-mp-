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
            SetPlayerOnFire(playerid);
            OnPlayerStartBurning(playerid, eFireData[areaid][e_iFireCreatorId]);
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
            PutOutFireOnPlayer(playerid, FIRE_PUT_OUT_DELAY);
        }
        return 1;
    }

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStartBurning -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartBurning(playerid, fireCreatorId)
{
    Disable_OnPlayerLookAt(playerid);

    HideWeaponMenu(playerid);
    HidePlayerWeapon(playerid);
    HideBackpack(playerid, .forceClose = true);

    KillTimer(ePlayerInfo[playerid][e_iTimer_Burning]);
    ePlayerInfo[playerid][e_iTimer_Burning] = SetTimerEx("OnPlayerBurn", FIRE_DAMAGE_RATE, true, "dd", playerid, fireCreatorId);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerBurn ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerBurn(playerid, fireCreatorId)
{
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - FIRE_DAMAGE_AMOUNT, fireCreatorId, WEAPON_MOLOTOV);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStopBurning ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopBurning(playerid)
{
    KillTimer(ePlayerInfo[playerid][e_iTimer_Burning]);

    if(IsPlayerDead(playerid))
    {
        return 1;
    }
    
    Enable_OnPlayerLookAt(playerid);
    RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
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