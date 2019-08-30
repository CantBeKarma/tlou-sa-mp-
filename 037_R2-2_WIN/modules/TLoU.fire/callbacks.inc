#include	<YSI\y_hooks>

//#region Fire areas callbacks

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerEnterDynamicArea ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    hook OnPlayerEnterDynArea(playerid, areaid)
    {
        if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
        {
            return 1;
        }

        if(IsPlayerInFireArea(areaid))
        {
            PutPlayerOnFire(playerid, areaid);
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

        if(IsPlayerInFireArea(areaid))
        {
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
    HideItem(playerid);
    SPAO(playerid, OBJECT_MODEL_BURNING);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerBurn ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer OnPlayerBurn[FIRE_DAMAGE_RATE](playerid, fireCreatorId)
{
    if(!IsPlayerBurning(playerid))
    {
        stop ePlayerInfo[playerid][e_iTimer_Burning];
        return 1;
    }

    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - FIRE_DAMAGE_AMOUNT, fireCreatorId, WEAPON_MOLOTOV);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStopBurning ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopBurning(playerid)
{
    RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    foreach(new fireId : ServerFires)
    {
        DestroyFire(fireId);
    }
    Iter_Clear(ServerFires);
    return 1;
}