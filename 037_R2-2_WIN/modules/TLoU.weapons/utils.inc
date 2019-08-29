// Clearing weapons data for player.
ResetEnum_ePlayerWeapon(playerid)
{
	static sBlank[e_PLAYER_WEAPONS_DATA];
	ePlayerWeapon[playerid] = sBlank;

	print("ResetEnum_ePlayerWeapon");
}

//#region Crosshair

	// Does a check if player see's a crosshair currently.
	IsCrosshairVisibleForPlayer(playerid)
	{
		return IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][0]) || IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][1]);
	}

	// Show player custom-crosshair for weapons.
	ShowPlayerCrosshair(playerid, bool:forceUpdate = false)
	{
		if(!forceUpdate)
		{
			if(!IsPlayerAimingWeapon(playerid) || IsCrosshairVisibleForPlayer(playerid))
			{
				return;
			}
		}

		new nativeWeaponId = GetItemNativeWeaponId(GetPlayerEquippedItemId(playerid));
		if(nativeWeaponId == WEAPON_SHOTGUN || nativeWeaponId == WEAPON_SHORTY)
		{
			TdShowForPlayer(playerid, eTextDrawInfo[e_Aim][1]);
			TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][0]);
		}
		else
		{
			TdShowForPlayer(playerid, eTextDrawInfo[e_Aim][0]);
			TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][1]);
		}
	}

	// Hide player custom-crosshair.
	HidePlayerCrosshair(playerid)
	{
		TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][0]);
		TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][1]);
	}

	// Show damage crosshair symbol.
	ShowPlayerHitMarker(playerid, bodypart)
	{
		if(!IsPlayerAimingWeapon(playerid))
			return;

		if(bodypart == BODY_PART_HEAD)
		{
			TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
			TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
		}
		else 
		{
			TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
			TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
		}

		if(!IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][2]) || !IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][3]))
		{
			defer HidePlayerHitMarker(playerid);
		}
	}

	// Hide crosshair damage symbol.
	timer HidePlayerHitMarker[TICK_RATE_HIDE_CROSSHAIR_SYMBOL](playerid)
	{
		TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
		TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
	}

//#endregion

//#region Validations

	// Return for which weapon id the specified ammo is for.
	GetWeaponIdForAmmoType(ammoModelId)
	{
		switch(ammoModelId)
		{
			case ITEM_MODEL_ARROW:	return WEAPON_BOW;
		}
		return -1;
	}

//#endregion

// Reduces condition of player's melee weapon by one.
DegradePlayerMeleeWeapon(playerid)
{
	new playerMeleeItemId = GetPlayerMeleeItemId(playerid);

	// Durability logic (reduce upgraded durability or basic one?)
	new newMeleeDurability = (IsPlayerMeleeUpgraded(playerid)) ? (GetPlayerMeleeExtraCnd(playerid, playerMeleeItemId)-1) : (GetPlayerItemAmount(playerid, playerMeleeItemId)-1);
	HudTd_UpdateMeleeWeaponCND(playerid, playerMeleeItemId, newMeleeDurability);

	if(newMeleeDurability > 0)
	{
		return;
	}

	// Removes melee weapon if broke.
	if(!IsPlayerMeleeUpgraded(playerid))
	{
		RemovePlayerItem(playerid, playerMeleeItemId);
		return;
	}

	// If melee is upgraded - remove its upgraded feature and hide bars of it.
	HudTd_HideMeleeUpgradedBars(playerid, playerMeleeItemId);

	if(playerMeleeItemId == WEAPON_SCISSOR_PIPE)
	{
		new playerMeleeCnd = GetPlayerItemAmount(playerid, playerMeleeItemId);
		RemovePlayerItem(playerid, playerMeleeItemId);
		GivePlayerItem(playerid, _:ITEM_IDX_PIPE);
		SetPlayerItemAmount(playerid, _:ITEM_IDX_PIPE, playerMeleeCnd);
		EquipItem(playerid, GetPlayerMeleeItemId(playerid));
	}
}

// Reloads player's weapon
ReloadWeapon(playerid)
{
    new currentEquippedItemId = GetPlayerEquippedItemId(playerid);

    if(currentEquippedItemId == -1 || !IsItemBulletWeapon(currentEquippedItemId)
    || IsPlayerUnableToPerformAction(playerid) || IsPlayerUsingBackpack(playerid) || IsPlayerUsingWeaponMenu(playerid)
    || GetPlayerAnimationIndex(playerid) == RIFLE_RIFLE_CROUCHLOAD || GetPlayerAnimationIndex(playerid) == RIFLE_RIFLE_LOAD)
    {
        return false;
    }

    new 
        currentAmmo = GetPlayerItemAmount(playerid, currentEquippedItemId),
        currentClipSize = GetPlayerItemClipAmount(playerid, currentEquippedItemId),
        maxWeaponClipSize = GetPlayerItemMaxClipSize(playerid, currentEquippedItemId);

    if(currentAmmo <= currentClipSize || currentClipSize == maxWeaponClipSize)
    {
        return false;
    }

    ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;
    
    new newClipSize = ePlayerItemData[playerid][currentEquippedItemId][e_iItemClipAmount] = (currentAmmo > maxWeaponClipSize) ? (maxWeaponClipSize) : (currentAmmo);
    OnPlayerReloadWeapon(playerid, currentEquippedItemId, currentClipSize, newClipSize);
    return true;
}