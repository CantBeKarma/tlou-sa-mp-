#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerEnterDynArea -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerEnterDynArea(playerid, areaid)
{
    if(!IsPlayerLogged(playerid) 
    || !IsPlayerControllable(playerid))
    {
        return 1;
    }

    if(IsPlayerInFireArea(areaid))
    {
        PutPlayerOnFire(playerid, eFireData[e_iFireCreatorId]);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerLeaveDynArea -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerLeaveDynArea(playerid, areaid)
{
    if(!IsPlayerLogged(playerid) 
    || !IsPlayerControllable(playerid))
    {
        return 1;
    }

    if(IsPlayerInFireArea(areaid))
    {
        defer PutOutFireOnPlayer(playerid);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStartBurning -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartBurning(playerid)
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

    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - FIRE_DAMAGE_AMOUNT, fireCreatorId, _:ITEM_IDX_MOLOTOV);
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