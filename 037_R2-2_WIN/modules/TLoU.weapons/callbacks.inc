#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }
    
    if(PRESSED(KEY_ENTER))
    {
        ReloadWeapon(playerid);
    }

    if(PRESSED(KEY_FIRE))
    {
        new currentItemId = GetPlayerEquippedItemId(playerid);
        
        if(IsPlayerIdle(playerid))
        {
            // Healing
            if(GetItemType(currentItemId) == ITEM_TYPE_HEAL)
            {
                if(!IsPlayerHealing(playerid) && GetPlayerHealthEx(playerid) < 100.0)
                {
                    SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_HEALING);
                    OnPlayerStartedHealing(playerid);
                }
            }
            // Equipping melee weapon
            else if(ITEM_IDX:currentItemId != ITEM_IDX:ITEM_IDX_NAIL_BOMB)
            {
                new meleeItemId = GetPlayerMeleeItemId(playerid);
                if(meleeItemId == -1)
                {
                    meleeItemId = GetPlayerThrowableItemId(playerid);
                }

                if(meleeItemId != -1)
                {
                    EquipItem(playerid, meleeItemId);
                }
                else
                {
                    HideItem(playerid);
                }
            }
        }

        // If player uses bullet weapon and has no ammo in a weapon magazine...
        if(IsPlayerAimingWeapon(playerid) && IsItemBulletWeapon(currentItemId) && GetPlayerItemClipAmount(playerid, currentItemId) <= 0)
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
                if(GetPlayerItemAmount(playerid, currentItemId) > 0) ReloadWeapon(playerid);
                else
                {
                    new itemId = GetFirstWeaponWithAmmo(playerid);
                    if(itemId == -1) Audio_PlayEx(playerid, AUDIO_WEAP_NOAMMO);
                    else
                    {
                        ApplyAnimation(playerid, "TLoU", "WEAPON_SWITCH", 4.1, 0, 1, 1, 0, 0);
                        EquipItem(playerid, itemId);
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
            SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
        }

        // If player doesn't have any weapon equipped (or has melee)
        // and pressed KEY_AIM | instant equip last weapon used.
        EquipItem(playerid, GetPlayerLastEquippedItemId(playerid));
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartAimingBulletWeapon -----------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingBulletWeapon(playerid)
{
    if(GetPlayerItemClipAmount(playerid, GetPlayerEquippedItemId(playerid)) > 0)
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
    if(GetObjectModel(objectid) == -1)
    {
        new nailBmbIdx = FindNailbombIdx(objectid);
        if(nailBmbIdx != INVALID_ITERATOR_SLOT)
        {
            new Float:objX, Float:objY, Float:objZ;
            GetDynamicObjectPos(objectid, objX, objY, objZ);

            OnNailbombExplode(playerid, eProjectileData[nailBmbIdx][e_iProjectile_ThrowerId], nailBmbIdx, objX, objY, objZ);
        }
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponShot ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerWeaponShot(playerid, weaponid, hittype, hitid, Float:fX, Float:fY, Float:fZ)
{
    new currentItemId = GetPlayerEquippedItemId(playerid);
    if(currentItemId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Player %s [charUid: %d] has shot a weapon (%d) which he was not given by the server!", PlayerName(playerid), GetPlayerCharUid(playerid), weaponid);
        return 0;
    }

    new
        oldAmmo = GetPlayerItemAmount(playerid, currentItemId),
        oldClipAmmo = GetPlayerItemClipAmount(playerid, currentItemId);

    ePlayerItemData[playerid][currentItemId][e_iItemAmount]--;
    ePlayerItemData[playerid][currentItemId][e_iItemClipAmount]--;

    new
        newAmmo = GetPlayerItemAmount(playerid, currentItemId),
        newClipAmmo = GetPlayerItemClipAmount(playerid, currentItemId);

    CLF("OnPlayerWeaponShoot", "ddddddfff", playerid, currentItemId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, fX, fY, fZ);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerWeaponShoot ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerWeaponShoot(playerid, itemId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ)
{
    // If no ammo in clip - disallow shooting.
    if(newClipAmmo <= 0)
    {
        TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
	}

    // Update bullet display weapon ammo (textdraws!)
    HudTd_UpdateWeaponAmmo(playerid, itemId);
    HudTd_ReduceWeaponCurrentAmmo(playerid, itemId, newClipAmmo);

    // Every rifle shot perform "reload" animation
    // applies to rifle because "SetWeaponNoReload" caused a rapid fire.
    if(ITEM_IDX:itemId == ITEM_IDX_RIFLE && newClipAmmo > 0)
    {
        ApplyAnimation(playerid, "RIFLE", (IsPlayerCrouched(playerid) ? "RIFLE_CROUCHLOAD" : "RIFLE_LOAD"), 4.1, 0, 1, 1, 0, 0);
    }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerReloadWeapon ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerReloadWeapon(playerid, itemId, oldClipAmmo, newClipAmmo)
{
    PlayPlayerReloadAnim(playerid, itemId);
    PlayPlayer3D_ReloadSFX(playerid, GetItemNativeWeaponId(itemId));
    HudTd_DisplayWeaponCurrentAmmo(playerid, itemId);
    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerWeaponChange --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerWeaponChange(playerid, newItemId, oldItemId)
{
    if(oldItemId != -1)
    {
        // Attach old item to player it if was only a WEAPON type.
        if(IsItemWeapon(oldItemId))
        {
            SPAO(playerid, GetItemModel(oldItemId));
            HudTd_HideWeaponCurrentAmmo(playerid, oldItemId);
        }

        if(IsItemWeapon(oldItemId) 
        && (IsItemBow(newItemId) || IsItemMedkit(newItemId)
        ||  IsItemProjectile(newItemId) || IsItemThrowable(newItemId)))
        {
            ResetPlayerWeapons(playerid);
        }

        if(IsItemWeapon(newItemId)
        && (IsItemBow(oldItemId) || IsItemMedkit(oldItemId)
        ||  IsItemProjectile(oldItemId) || IsItemThrowable(oldItemId)))
        {
            RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
        }

		// HudTd_HideMeleeWeaponBars(playerid, oldItemId);
		// HudTd_HideMeleeUpgradedBars(playerid, oldItemId);
    }

    HudTd_UpdateWeapon(playerid, newItemId);
    HudTd_UpdateWeaponAmmo(playerid, newItemId);

    if(newItemId == -1)
    {
        ResetPlayerWeapons(playerid);

        // Remove attached object if it was a projectile/throwable/medkit.
        RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
    }
    else
    {
        if(IsPlayerAimingWeapon(playerid) 
        && ((oldItemId != _:ITEM_IDX_SHOTGUN && (newItemId == _:ITEM_IDX_SHOTGUN || newItemId == _:ITEM_IDX_SHORTY) 
        || (oldItemId == _:ITEM_IDX_SHOTGUN || oldItemId == _:ITEM_IDX_SHORTY))))
        {
            // This function is going to be called only if player keeps aiming
            // It is needed here because of different crosshairs for shotgun and other weapons
            // and when weapon changes automatically with "GetFirstWeaponWithAmmo"
            ShowPlayerCrosshair(playerid, .forceUpdate = true);
            TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, true);
        }

		ShowHud(playerid);
        // HudTd_DisplayMeleeWeaponBars(playerid, newItemId);
        HudTd_DisplayWeaponCurrentAmmo(playerid, newItemId);

        // Detach weapon from player's body when it is in use 
        // (don't detach BOW or projectile/throwable/medkit since it's attached to player's hand).
        if(!IsItemBow(newItemId) && !IsItemMedkit(newItemId)
        && !IsItemProjectile(newItemId) && !IsItemThrowable(newItemId))
        {
            DetachWeaponFromPlayersBody(playerid, newItemId);
        }
    }

    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerWeaponChange: Changed from itemId: %d to itemId %d (charUid: %d)", oldItemId, newItemId, GetPlayerCharUid(playerid));
    return 0;
}