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
        
        if(!Bit_Get(ePlayerFlag[e_bAiming], playerid) 
        && !IsPlayerUsingBackpack(playerid) 
        && !IsPlayerUsingWeaponMenu(playerid)
        && !IsPlayerPlayingActions(playerid))
        {
            // Healing
            if(currentWeaponId == WEAPON_MEDKIT && GetPlayerMedkits[playerid] > 0)
            {
                if(!Bit_Get(ePlayerFlag[e_bIsHealing], playerid) && GetPlayerHealthEx(playerid) < 100.0 && !IsPlayerBurning(playerid))
                {
                    Bit_Let(ePlayerFlag[e_bIsHealing], playerid);
                    OnPlayerStartedHealing(playerid);
                }
            }

            // Equipping melee weapon
            else if(IsBulletWeapon(currentWeaponId) 
            || IsProjectileWeapon(currentWeaponId) 
            || currentWeaponId == WEAPON_BOW 
            || currentWeaponId == WEAPON_UNARMED)
            {
                if(currentWeaponId != WEAPON_NAIL_BOMB)
                {
                    SetPlayerMeleeWeapon(playerid);
                }
            }
        }

        // If player uses bullet weapon and has no ammo in a weapon magazine...
        if(IsBulletWeapon(currentWeaponId) && CurrentPlayerWeaponClipSize[playerid] <= 0 && Bit_Get(ePlayerFlag[e_bAiming], playerid))
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
                            ShowPlayerCrosshair(playerid);
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
        if(IsPlayerUsingWeaponMenu(playerid) && !Bit_Get(ePlayerFlag[e_bIsSwitchingWeapons], playerid))
        {
            HideWeaponMenu(playerid);
        }

        // If player doesn't have any weapon equipped (or has melee)
        // and pressed KEY_AIM | instant equip last weapon used.
        SetPlayerLastWeapon(playerid);
    }
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
//------------------ OnPlayerShootDynamicObject -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShootDynamicObject(playerid, weaponid, objectid, Float:x, Float:y, Float:z)
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

    CallLocalFunction("OnPlayerWeaponShoot", "ddddddfff", playerid, currentWeaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, x, y, z);
    return 1;
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
    if(weaponId == WEAPON_RIFLE)
        ApplyAnimation(playerid, "RIFLE", (IsPlayerCrouched(playerid) ? "RIFLE_CROUCHLOAD" : "RIFLE_LOAD"), 4.1, 0, 1, 1, 0, 0);
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerReloadWeapon ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerReloadWeapon(playerid, weaponId, oldClipAmmo, newClipAmmo)
{
    PlayPlayerReloadSound(playerid, weaponId);
    PerformReloadAnimation(playerid, weaponId);
	HudTd_DisplayWeaponCurrentAmmo(playerid, weaponId);
	TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponChange --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerWeaponChange(playerid, oldWeaponId, newWeaponId)
{
    // Hide player weapon (that was used).
    HidePlayerWeapon(playerid, oldWeaponId);

    // Detach weapon from player's body when it is in use.
    DetachWeaponFromPlayersBody(playerid, newWeaponId);

    // Update HUD weapon display.
    ShowHud(playerid);
    HudTd_UpdateWeapon(playerid, newWeaponId);
    HudTd_UpdateWeaponAmmo(playerid, newWeaponId);
    HudTd_DisplayMeleeWeaponBars(playerid, newWeaponId);
    HudTd_DisplayWeaponCurrentAmmo(playerid, newWeaponId);
    return 0;
}