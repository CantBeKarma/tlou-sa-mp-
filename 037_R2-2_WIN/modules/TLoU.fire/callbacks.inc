#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerBurn ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerBurn(playerid)
{
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - FIRE_DAMAGE_AMOUNT);

    if(IsPlayerDead(playerid))
    {
        OnPlayerDeathEx(playerid, INVALID_PLAYER_ID, WEAPON_FIRE, BODY_PART_TORSO);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStartBurning -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartBurning(playerid)
{
    SPAO(playerid, OBJECT_MODEL_BURNING);
    Disable_OnPlayerLookAt(playerid);

    if(IsPlayerUsingWeaponMenu(playerid))
    {
        HideWeaponMenu(playerid);
    }

    HidePlayerWeapon(playerid);
    HideBackpack(playerid, .forceClose = true);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerStopBurning ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopBurning(playerid)
{
    Enable_OnPlayerLookAt(playerid);
    RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
    return 0;
}

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
        
    foreach(new i : ServerFires)
    {
        if(areaid == eFireData[i][e_iFireAreaId])
        {
            PutOutFireOnPlayer(playerid, FIRE_PUT_OUT_DELAY);
            break;
        }
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    Iter_Clear(ServerFires);
    return 1;
}