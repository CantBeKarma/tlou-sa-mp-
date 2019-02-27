#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;
    
    if(PRESSED(KEY_ENTER))
    {
        ReloadWeapon(playerid);
    }

    if(PRESSED(KEY_FIRE))
    {
        new currentWeaponId = CurrentPlayerWeaponId[playerid];
        
        if(!IsPlayerAimingWeapon(playerid)
        && !IsPlayerUsingBackpack(playerid) 
        && !IsPlayerUsingWeaponMenu(playerid)
        && !IsPlayerUnableToPerformAction(playerid))
        {
            // Healing
            if(currentWeaponId == WEAPON_MEDKIT && GetPlayerMedkits[playerid] > 0)
            {
                if(!IsPlayerHealing(playerid) && GetPlayerHealthEx(playerid) < 100.0)
                {
                    Bit_Let(ePlayerFlag[e_bIsHealing], playerid);
                    OnPlayerStartedHealing(playerid);
                }
            }
            // Equipping melee weapon
            else if(currentWeaponId != WEAPON_NAIL_BOMB && currentWeaponId != WEAPON_UNARMED)
            {
                SetPlayerMeleeWeapon(playerid);
            }
        }

        // If player uses bullet weapon and has no ammo in a weapon magazine...
        if(IsBulletWeapon(currentWeaponId) && CurrentPlayerWeaponClipSize[playerid] <= 0 && IsPlayerAimingWeapon(playerid))
        {
            // perform a sound effect when trying to shoot (no ammo click noise) a few times...
            if(ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] < RandomEx(MIN_NOAMMO_CLICKS, MAX_NOAMMO_CLICKS))
            {   
                Audio_PlayEx(playerid, AUDIO_WEAP_NOAMMO);
                ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks]++;
            }
            else
            {
                // reload a weapon if still has some ammo left or seek for the new weapon in the inventory.
                if(GetPlayerWeaponAmmo(playerid, currentWeaponId) > 0) ReloadWeapon(playerid);
                else
                {
                    new newWeaponId = GetFirstWeaponWithAmmo(playerid);
                    if(newWeaponId == 0) Audio_PlayEx(playerid, AUDIO_WEAP_NOAMMO);
                    else
                    {
                        if(currentWeaponId != WEAPON_SHOTGUN && newWeaponId == WEAPON_SHOTGUN)
                        {
                            // This function is going to be called only if player keeps aiming
                            // It is needed here because of different crosshairs for shotgun and other weapons
                            // and when weapon changes automatically with "GetFirstWeaponWithAmmo"
                            ShowPlayerCrosshair(playerid, .forceUpdate = true);
                        }

                        ApplyAnimation(playerid, "TLoU", "WEAPON_SWITCH", 4.1, 0, 1, 1, 0, 0);
                        SetPlayerWeapon(playerid, newWeaponId, GetPlayerWeaponAmmo(playerid, newWeaponId));
                        TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
                    }
                }                  
            }
        }
    }

    if(PRESSED(KEY_AIM))
    {
        // If player tries to AIM while using weapon menu ..
        // Hide weapon menu and start aiming (doesn't apply when player is switching weapons).
        if(IsPlayerUsingWeaponMenu(playerid) && !IsPlayerSwitchingWeapons(playerid))
        {
            HideWeaponMenu(playerid);
        }

        // If player doesn't have any weapon equipped (or has melee)
        // and pressed KEY_AIM | instant equip last weapon used.
        SetPlayerLastWeapon(playerid);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartAimingBulletWeapon -----------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingBulletWeapon(playerid)
{
	if(CurrentPlayerWeaponClipSize[playerid] > 0)
	{
		TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------- OnPlayerStopAimingBulletWeapon -----------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopAimingBulletWeapon(playerid)
{
	TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerShootDynamicObject -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShootDynamicObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
{
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponShot ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    new currentWeaponId = CurrentPlayerWeaponId[playerid];
    if(currentWeaponId <= 0)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Player %s [%d] [%s] has shot a weapon (%d) which he was not given by the server!", PlayerName(playerid), playerid, GetPlayerIpAddress(playerid), weaponid);
        return 0;
    }

    new 
        oldAmmo = GetPlayerWeaponAmmo(playerid, currentWeaponId),
        oldClipAmmo = CurrentPlayerWeaponClipSize[playerid];

    SetPlayerWeaponAmmo(playerid, currentWeaponId, oldAmmo-1);
    CurrentPlayerWeaponClipSize[playerid] -= 1;

    new
        newAmmo = GetPlayerWeaponAmmo(playerid, currentWeaponId),
        newClipAmmo = CurrentPlayerWeaponClipSize[playerid];

    CallLocalFunction("OnPlayerWeaponShoot", "ddddddfff", playerid, currentWeaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, fX, fY, fZ);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerWeaponShoot ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerWeaponShoot(playerid, weaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ)
{
    // If no ammo in clip - disallow shooting.
    if(newClipAmmo <= 0)
    {
        TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
	}

    // Update bullet display weapon ammo (textdraws!)
    HudTd_ReduceWeaponCurrentAmmo(playerid, weaponId, newClipAmmo);

    // Every rifle shot perform "reload" animation
    // applies to rifle because of "SetWeaponNoReload" was causing rapid fire of the weapon.
    if(weaponId == WEAPON_RIFLE && newClipAmmo > 0)
    {
        ApplyAnimation(playerid, "RIFLE", (IsPlayerCrouched(playerid) ? "RIFLE_CROUCHLOAD" : "RIFLE_LOAD"), 4.1, 0, 1, 1, 0, 0);
    }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerReloadWeapon ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerReloadWeapon(playerid, weaponId, oldClipAmmo, newClipAmmo)
{
    PlayPlayer3D_ReloadSFX(playerid, weaponId);
    PerformReloadAnimation(playerid, weaponId);
	HudTd_DisplayWeaponCurrentAmmo(playerid, weaponId);
	TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponChange --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerWeaponChange(playerid, newWeaponId, oldWeaponId)
{
    // Don't do any attachments with my current weapon if it
    // was a projectile or a throwable weapon (not necessary to call).
    if(!IsProjectileWeapon(oldWeaponId) && !IsThrowableWeapon(oldWeaponId))
    {
        SPAO(playerid, GetWeaponModelFromWeaponId(oldWeaponId));
    }

    // Update HUD weapon display.
    HudTd_UpdateWeapon(playerid, newWeaponId);
    HudTd_UpdateWeaponAmmo(playerid, newWeaponId);

    if(newWeaponId == WEAPON_UNARMED)
    {
        // Remove attached object if it was a projectile/medkit/throwable.
		ResetPlayerWeapons(playerid);
        RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);

        HudTd_HideWeaponCurrentAmmo(playerid, oldWeaponId);
		HudTd_HideMeleeWeaponBars(playerid, oldWeaponId);
		HudTd_HideMeleeUpgradedBars(playerid, oldWeaponId);
    }
    else
    {
		ShowHud(playerid);
        
        HudTd_DisplayMeleeWeaponBars(playerid, newWeaponId);
        HudTd_DisplayWeaponCurrentAmmo(playerid, newWeaponId);

        // Detach weapon from player's body when it is in use.
        DetachWeaponFromPlayersBody(playerid, newWeaponId);
    }
    return 0;
}
