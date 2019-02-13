#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// --------------------- OnPlayerUpdate --------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
	if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) return 0;

	// Aiming.
	if(IsPlayerHoldingKey(playerid, KEY_AIM))
	{
		new currentWeaponId = CurrentPlayerWeaponId[playerid];

		if(!Bit_Get(ePlayerFlag[e_bAiming], playerid))
		{
			// Bullet weapons.
			if(IsBulletWeapon(currentWeaponId) && IsPlayerUsingAnim(playerid, ANIM_COLLECTION_AIM))
			{
				Bit_Let(ePlayerFlag[e_bAiming], playerid);
				OnPlayerStartAimingBulletWeapon(playerid);
			}

			// Projectiles / Bow.
			if(GetPlayerWeaponAmmo(playerid, currentWeaponId) > 0 && !IsPlayerPlayingActions(playerid))
			{
				if(currentWeaponId == WEAPON_BOW)
				{
					Bit_Let(ePlayerFlag[e_bAiming], playerid);
					OnPlayerStartAimingBow(playerid);
				}
				else if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
				{
					Bit_Let(ePlayerFlag[e_bAiming], playerid);
					OnPlayerStartAimingProjectile(playerid, currentWeaponId);
				}
			}
		}
		else
		{
			if(IsBulletWeapon(currentWeaponId) || currentWeaponId == WEAPON_BOW)
			{
				// Weapon Sway.
				new drunkLevel = 
					  (Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY) == 0) ? 5000
					: (Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY) == 1) ? 2500
					: 0;

				if(drunkLevel > 0)
				{
					if(IsPlayerCrouched(playerid))
					{
						drunkLevel = floatround(drunkLevel / 2);
					}
					SetPlayerDrunkLevel(playerid, drunkLevel);
				}

				// Player targets another player.
				new playerTarget = GetPlayerTargetPlayer(playerid);
				if(playerTarget != ePlayerInfo[playerid][e_iTargetedPlayer])
				{
					OnPlayerTargetPlayer(playerid, playerTarget, ePlayerInfo[playerid][e_iTargetedPlayer]);
					ePlayerInfo[playerid][e_iTargetedPlayer] = playerTarget;
				}

				// If using bullet weapon but not using AIM animation (hiding crosshair).
				// if(!IsPlayerUsingAnim(playerid, ANIM_COLLECTION_AIM))
				// {
				// 	Bit_Vet(ePlayerFlag[e_bAiming], playerid);
				// 	OnPlayerStopAimingWeapon(playerid);
				// }
			}
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
//------------- OnPlayerStartAimingBulletWeapon -----------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingBulletWeapon(playerid)
{
	KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
	
	ShowPlayerCrosshair(playerid);
	Disable_OnPlayerLookAt(playerid);

	if(CurrentPlayerWeaponClipSize[playerid] > 0)
	{
		TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStopAimingWeapon --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopAimingWeapon(playerid)
{
	KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
	ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);
	
	HidePlayerCrosshair(playerid);
	HidePlayerHitMarker(playerid);
	Enable_OnPlayerLookAt(playerid);

	new currentWeaponId = CurrentPlayerWeaponId[playerid];
	if(currentWeaponId == WEAPON_BOW) OnPlayerStopAimingBow(playerid);
	else if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId)) OnPlayerStopAimingProjectile(playerid);
	else if(IsBulletWeapon(currentWeaponId))
	{
		SetPlayerDrunkLevel(playerid, 0);
		TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
	}
	return 1;
}