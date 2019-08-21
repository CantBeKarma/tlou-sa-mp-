#include	<YSI\y_hooks>

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// --------------------- OnPlayerUpdate --------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 0;
    }
	
	// Aiming.
	if(IsPlayerHoldingKey(playerid, KEY_AIM))
	{
		if(!IsPlayerAimingWeapon(playerid))
		{
			OnPlayerTryAimingWeapon(playerid, GetPlayerEquippedItemId(playerid));
		}
		else
		{
			OnPlayerAimsWeapon(playerid, GetPlayerEquippedItemId(playerid));
		}
	}
	else
	{
		if(IsPlayerAimingWeapon(playerid) || ePlayerWeapon[playerid][e_bShootingCustomWeapon])
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
		}
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerTryAimingWeapon ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTryAimingWeapon(playerid, itemId)
{
	if(IsPlayerUnableToPerformAction(playerid) || ePlayerWeapon[playerid][e_bShootingCustomWeapon])
	{
		return 1;
	}

	// Bullet weapons.
	if(GetItemType(itemId) == ITEM_TYPE_WEAPON && ITEM_IDX:itemId != ITEM_IDX_BOW && IsPlayerUsingAnim(playerid, ANIM_COLLECTION_AIM))
	{
		SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_AIMING);
		OnPlayerStartAimingBulletWeapon(playerid);
	}

	// Projectiles / Bow.
	if(GetPlayerItemAmount(playerid, itemId) > 0)
	{
		if(ITEM_IDX:itemId == ITEM_IDX_BOW)
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_AIMING);
			//OnPlayerStartAimingBow(playerid);
		}
		else if(GetItemType(itemId) == ITEM_TYPE_PROJECTILE || GetItemType(itemId) == ITEM_TYPE_THROWABLE)
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_AIMING);
			//OnPlayerStartAimingProjectile(playerid, weaponId);
		}
	}

	if(IsPlayerAimingWeapon(playerid))
	{
		ShowPlayerCrosshair(playerid);
		stop ePlayerInfo[playerid][e_iTimer_HideHud];
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerAimsWeapon ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerAimsWeapon(playerid, itemId)
{
	// Weapon Sway.
	//if(IsBulletWeapon(weaponId) || weaponId != WEAPON_BOW)
	if(GetItemType(itemId) == ITEM_TYPE_WEAPON && ITEM_IDX:itemId != ITEM_IDX_BOW)
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
	ePlayerInfo[playerid][e_iTimer_HideHud] = defer HideHud(playerid);
	
	HidePlayerCrosshair(playerid);
	HidePlayerHitMarker(playerid);
	SetPlayerDrunkLevel(playerid, 0);

	// new currentWeaponId = CurrentPlayerWeaponId[playerid];
	// if(IsBulletWeapon(currentWeaponId))
	// {
	// 	OnPlayerStopAimingBulletWeapon(playerid);
	// }
	// else if(currentWeaponId == WEAPON_BOW)
	// {
	// 	OnPlayerStopAimingBow(playerid);
	// }
	// else if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
	// {
	// 	OnPlayerStopAimingProjectile(playerid);
	// }

	ePlayerWeapon[playerid][e_bShootingCustomWeapon] = false;
	return 0;
}