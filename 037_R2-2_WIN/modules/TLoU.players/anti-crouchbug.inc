#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerCrouchBug ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCrouchBug(playerid)
{
	ApplyAnimation(playerid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0);
	defer DelayedTimer_AnimGetUp(playerid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)	
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;

	if(PRESSED(KEY_CROUCH))
	{
		if(Bit_Get(ePlayerFlag[e_bDoingCrouchBug], playerid) 
		&& (GetConnectedTime(playerid) - ePlayerInfo[playerid][e_iTickCounter_crouchBug]) < TICK_RATE_CBUG)
		{
            OnPlayerCrouchBug(playerid);
		}
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponShoot ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerWeaponShoot(playerid, weaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ)
{
	if(weaponId == WEAPON_REVOLVER || weaponId == WEAPON_SHOTGUN)
	{	
		if((GetConnectedTime(playerid) - ePlayerInfo[playerid][e_iTickCounter_crouchBug]) > TICK_RATE_CBUG)
		{
			Bit_Let(ePlayerFlag[e_bDoingCrouchBug], playerid);
			ePlayerInfo[playerid][e_iTickCounter_crouchBug] = GetConnectedTime(playerid);
		}
	}
	return 0;
}