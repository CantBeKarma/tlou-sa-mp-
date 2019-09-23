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

// Reduces condition of player's melee weapon by one.
DegradePlayerMeleeWeapon(playerid)
{
	new 
		playerMeleeItemId = GetPlayerMeleeItemId(playerid),
		playerMeleeBasicCnd = GetPlayerItemAmount(playerid, playerMeleeItemId),
		playerMeleeExtCnd = GetPlayerMeleeExtraCnd(playerid, playerMeleeItemId);

	// Durability logic (reduce upgraded durability or basic one?)
	new newMeleeDurability = (IsPlayerMeleeUpgraded(playerid)) ? (playerMeleeExtCnd-1) : (playerMeleeBasicCnd-1);

	HudTd_UpdateMeleeWeaponCND(playerid, playerMeleeItemId, newMeleeDurability);

	if(!IsPlayerMeleeUpgraded(playerid))
	{
		SetPlayerItemAmount(playerid, playerMeleeItemId, newMeleeDurability);
	}
	else
	{
		SetPlayerItemAmount(playerid, playerMeleeItemId, playerMeleeBasicCnd, newMeleeDurability);
	}

	if(newMeleeDurability <= 0)
	{
		if(!IsPlayerMeleeUpgraded(playerid) && GetPlayerItemAmount(playerid, playerMeleeItemId) <= 0)
		{
			RemovePlayerItem(playerid, playerMeleeItemId);
		}
		else
		{
			HudTd_HideMeleeUpgradedBars(playerid, playerMeleeItemId);

			if(playerMeleeItemId == _:ITEM_IDX_SCSR_PIPE)
			{
				RemovePlayerItem(playerid, playerMeleeItemId);

				playerMeleeItemId = _:ITEM_IDX_PIPE;
				GivePlayerItem(playerid, playerMeleeItemId);
				SetPlayerItemAmount(playerid, playerMeleeItemId, playerMeleeBasicCnd);
				EquipItem(playerid, playerMeleeItemId);
			}
		}
	}
}

// Reloads player's weapon
ReloadWeapon(playerid)
{
    new currentEquippedItemId = GetPlayerEquippedItemId(playerid);

    if(currentEquippedItemId == -1 || !IsItemType(currentEquippedItemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON)
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