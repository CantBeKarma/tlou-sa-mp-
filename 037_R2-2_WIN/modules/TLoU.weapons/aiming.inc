#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// --------------------- OnPlayerUpdate --------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
	if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid))
		return 0;

	// Aiming.
	if(IsPlayerHoldingKey(playerid, KEY_AIM))
	{
		new currentWeaponId = CurrentPlayerWeaponId[playerid];

		if(!Bit_Get(ePlayerFlag[e_bAiming], playerid))
		{
			OnPlayerTryAimingWeapon(playerid, currentWeaponId);
		}
		else
		{
			OnPlayerAimsWeapon(playerid, currentWeaponId);
		}
	}
	else
	{
		if(Bit_Get(ePlayerFlag[e_bAiming], playerid))
		{
			Bit_Vet(ePlayerFlag[e_bAiming], playerid);
			OnPlayerStopAimingWeapon(playerid);
		}
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerTryAimingWeapon ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTryAimingWeapon(playerid, weaponId)
{
	// Bullet weapons.
	if(IsBulletWeapon(weaponId) && IsPlayerUsingAnim(playerid, ANIM_COLLECTION_AIM))
	{
		Bit_Let(ePlayerFlag[e_bAiming], playerid);
		OnPlayerStartAimingBulletWeapon(playerid);
	}

	// Projectiles / Bow.
	if(GetPlayerWeaponAmmo(playerid, weaponId) > 0 && !IsPlayerPlayingActions(playerid))
	{
		if(weaponId == WEAPON_BOW)
		{
			Bit_Let(ePlayerFlag[e_bAiming], playerid);
			OnPlayerStartAimingBow(playerid);
		}
		else if(IsProjectileWeapon(weaponId) || IsThrowableWeapon(weaponId))
		{
			Bit_Let(ePlayerFlag[e_bAiming], playerid);
			OnPlayerStartAimingProjectile(playerid, weaponId);
		}
	}

	if(Bit_Get(ePlayerFlag[e_bAiming], playerid))
	{
		ShowPlayerCrosshair(playerid);
		Disable_OnPlayerLookAt(playerid);
		KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerAimsWeapon ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerAimsWeapon(playerid, weaponId)
{
	// Weapon Sway.
	if(IsBulletWeapon(weaponId) || weaponId != WEAPON_BOW)
	{
		new drunkLevel = 
			  (Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY) == 0) ? 5000
			: (Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY) == 1) ? 2500
			: (0);

		if(drunkLevel > 0)
		{
			if(IsPlayerCrouched(playerid))
			{
				drunkLevel = floatround(drunkLevel / 2);
			}
			SetPlayerDrunkLevel(playerid, drunkLevel);
		}
	}

	// Player targets another player.
	new playerTarget = GetPlayerTargetPlayer(playerid);
	if(playerTarget != ePlayerInfo[playerid][e_iTargetedPlayer])
	{
		OnPlayerTargetPlayer(playerid, playerTarget, ePlayerInfo[playerid][e_iTargetedPlayer]);
		ePlayerInfo[playerid][e_iTargetedPlayer] = playerTarget;
	}

	// If using bullet weapon but not using AIM animation (hiding crosshair).
	// (hides crosshair when rolling for example).
	// if(!IsPlayerUsingAnim(playerid, ANIM_COLLECTION_AIM))
	// {
	// 	HidePlayerCrosshair(playerid);
	// }
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStopAimingWeapon --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopAimingWeapon(playerid)
{
	ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);
	
	HidePlayerCrosshair(playerid);
	HidePlayerHitMarker(playerid);
	Enable_OnPlayerLookAt(playerid);
	SetPlayerDrunkLevel(playerid, 0);

	new currentWeaponId = CurrentPlayerWeaponId[playerid];
	if(IsBulletWeapon(currentWeaponId))
	{
		OnPlayerStopAimingBulletWeapon(playerid);
	}
	else if(currentWeaponId == WEAPON_BOW)
	{
		OnPlayerStopAimingBow(playerid);
	}
	else if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
	{
		OnPlayerStopAimingProjectile(playerid);
	}
	return 0;
}