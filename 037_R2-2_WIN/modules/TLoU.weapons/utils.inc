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

	// Check if weapon exists in the items table and return its ITEM_ID.
	// IsValidWeapon(weaponId)
	// {
	// 	new weapon_itemId = -1;
	// 	for(new i = 0; i != MAX_ITEMS_DATA; i++)
	// 	{
	// 		if(weaponId == eItemsData[i][e_iItem_SA_WeaponId])
	// 		{
	// 			weapon_itemId = i;
	// 			break;
	// 		}
	// 	}
	// 	return weapon_itemId;
	// }

	// Check if a weapon is bullet type weapon.
	IsBulletWeapon(weaponId)
	{
		return weaponId == WEAPON_9MM
			|| weaponId == WEAPON_REVOLVER
			|| weaponId == WEAPON_SHORTY
			|| weaponId == WEAPON_SHOTGUN
			|| weaponId == WEAPON_RIFLE
			|| weaponId == WEAPON_ASSAULT;
		//return (WEAPON_COLT45 <= weaponid <= WEAPON_SNIPER) || weaponid == WEAPON_MINIGUN;
	}

	// Check if a weapon is melee type weapon.
	IsMeleeWeapon(weaponId)
	{
		return weaponId == WEAPON_UNARMED 
			|| weaponId == WEAPON_PIPE 
			|| weaponId == WEAPON_SCISSOR_PIPE 
			|| weaponId == WEAPON_HATCHET 
			|| weaponId == WEAPON_MACHETE;
		//return (WEAPON_UNARMED <= weaponid <= WEAPON_KATANA) || (WEAPON_DILDO <= weaponid <= WEAPON_CANE) || weaponid == WEAPON_PISTOLWHIP;
	}

	// Check if a weapon is projectile type of a weapon.
	IsProjectileWeapon(weaponId)
	{
		return 
			weaponId == WEAPON_MOLOTOV 
			|| weaponId == WEAPON_NAIL_BOMB 
			|| weaponId == WEAPON_SMOKE_BOMB;
	}

	// Check if a weapon is throwable type of a weapon.
	IsThrowableWeapon(weaponId)
	{
		return 
			weaponId == WEAPON_BRICK 
			|| weaponId == WEAPON_BOTTLE;
	}

	// Return for which weapon id the specified ammo is for.
	GetWeaponIdForAmmoType(ammoModelId)
	{
		switch(ammoModelId)
		{
			case ITEM_MODEL_ARROW:	return WEAPON_BOW;
		}
		return -1;
	}

	// Return weapon's model based on ID.
	GetWeaponModelFromWeaponId(weaponId)
	{
		new item_modelId = -1;
		for(new i = 0; i != MAX_ITEMS_DATA; i++)
		{
			if(weaponId == eItemsData[i][e_iItem_SA_WeaponId])
			{
				item_modelId = eItemsData[i][e_iItem_ModelId];
				break;
			}
		}
		return item_modelId;
	}

	// Return slot of a weapon (server-sided weapons).
	GetCustomWeaponSlot(weaponId)
	{
		new item_slotId = -1;
		for(new i = 0; i != MAX_ITEMS_DATA; i++)
		{
			if(weaponId == eItemsData[i][e_iItem_SA_WeaponId])
			{
				item_slotId = eItemsData[i][e_iItem_SlotId];
				break;
			}
		}
		return item_slotId;
	}

	// Check if player is full on ammo for certain weapon.
	IsPlayerWeaponFullOnAmmo(playerid, weaponId)
	{
		return GetPlayerWeaponAmmo(playerid, weaponId) >= GetPlayerWeaponLvl_MaxAmmo(playerid, weaponId);
	}
//#endregion

//#region Weapons functions

	// Hide's player current weapon equipped.
	// HidePlayerWeapon(playerid)
	// {
	// 	new currentWeapon = CurrentPlayerWeaponId[playerid];

	// 	if(currentWeapon == WEAPON_UNARMED)
	// 		return;
	
	// 	new 
	// 		currentAmmo = GetPlayerWeaponAmmo(playerid, currentWeapon);

	// 	// Clear current player's weapon data.
	// 	CurrentPlayerWeaponId[playerid] = WEAPON_UNARMED;
	// 	CurrentPlayerWeaponAmmo[playerid] = 0;
	// 	PlayerWeaponBodyDamage[playerid] = 8.0;
	// 	PlayerWeaponHeadDamage[playerid] = 8.0;
	// 	ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

	// 	// Save player's weapon ammo and save it as a last used weaponId by player.
	// 	LastPlayerWeaponId[playerid] = currentWeapon;
	// 	//ePlayerWeapon[playerid][e_iWeaponClipAmmo][GetCustomWeaponSlot(currentWeapon)] = currentClipAmmo;
	// 	SetPlayerWeaponAmmo(playerid, currentWeapon, currentAmmo);

	// 	OnPlayerWeaponChange(playerid, WEAPON_UNARMED, currentWeapon);
	// }

	// Set player's weapon ammo (doesn't matter if weapon is equipped or not).
	SetPlayerWeaponAmmo(playerid, weaponId, ammo)
	{
		if(CurrentPlayerWeaponId[playerid] != weaponId) PlayerWeaponSlotAmmo[playerid, GetCustomWeaponSlot(weaponId)] = ammo;
		else
		{
			CurrentPlayerWeaponAmmo[playerid] = ammo;
			HudTd_UpdateWeaponAmmo(playerid, weaponId);
		}
	}

	// Get player's weapon ammo (doesn't matter if weapon is equipped or not).
	GetPlayerWeaponAmmo(playerid, weaponId)
	{
		return 
			CurrentPlayerWeaponId[playerid] == weaponId 
			? CurrentPlayerWeaponAmmo[playerid] 
			: PlayerWeaponSlotAmmo[playerid, GetCustomWeaponSlot(weaponId)];
	}

	// Check if player has certain weapon.
	HasPlayerWeapon(playerid, weaponId)
	{
		new bool:result = false;
		if(weaponId <= 0) result = false;
		else if(CurrentPlayerWeaponId[playerid] == weaponId) result = true;
		else
		{
			result = (ePlayerWeapon[playerid][e_iWeaponsId][GetCustomWeaponSlot(weaponId)] == weaponId) ? (true) : (false);
		}
		return result;
	}

//#region converted
	// Give player weapon based on slot.
	// GivePlayerWeaponInSlot(playerid, weaponId)
	// {
	// 	if(HasPlayerWeapon(playerid, weaponId))
	// 		return;

	// 	switch(weaponId)
	// 	{
	// 		case WEAPON_SHOTGUN, WEAPON_RIFLE, WEAPON_BOW, WEAPON_ASSAULT:
	// 		{
	// 			new weapMenu_Ls_freeSlot = WeapMenuTd_GetFreeSlot_H_Ls(playerid);
	// 			WeapMenu_Ls_WeaponId(playerid, weapMenu_Ls_freeSlot) = weaponId;
	// 		}

	// 		case WEAPON_9MM, WEAPON_REVOLVER, WEAPON_SHORTY:
	// 		{
	// 			new weapMenu_Rs_freeSlot = WeapMenuTd_GetFreeSlot_H_Rs(playerid);
	// 			WeapMenu_Rs_WeaponId(playerid, weapMenu_Rs_freeSlot) = weaponId;
	// 		}
	// 	}

	// 	ePlayerWeapon[playerid][e_iWeaponsId][GetCustomWeaponSlot(weaponId)] = weaponId;

	// 	if(IsPlayerUsingWeaponMenu(playerid))
	// 	{
	// 		WeapMenuTd_DisplayWeapsAndAmmo(playerid);
	// 	}
	// }

	// Remove player's weapon (doesn't matter if equipped or not).
	// RemovePlayerWeapon(playerid, weaponId)
	// {
	// 	if(!HasPlayerWeapon(playerid, weaponId))
	// 	{
	// 		return;
	// 	}

	// 	if(CurrentPlayerWeaponId[playerid] == weaponId)
	// 	{
	// 		HidePlayerWeapon(playerid);
	// 	}

	// 	if(LastPlayerWeaponId[playerid] == weaponId)
	// 	{
	// 		LastPlayerWeaponId[playerid] = GetFirstWeaponWithAmmo(playerid);
	// 	}

	// 	switch(weaponId)
	// 	{
	// 		case
	// 			WEAPON_9MM, WEAPON_REVOLVER, WEAPON_SHORTY:
	// 			{
	// 				for(new i = 0; i < 3; i++)
	// 				{
	// 					if(weaponId == WeapMenu_Rs_WeaponId(playerid, i))
	// 					{
	// 						WeapMenu_Rs_WeaponId(playerid, i) = -1;
	// 						break;
	// 					}
	// 				}
	// 				//WeapMenu_Rs_SortWeapons(playerid);
	// 			}

	// 		case 
	// 			WEAPON_SHOTGUN, WEAPON_RIFLE, 
	// 			WEAPON_ASSAULT, WEAPON_BOW:
	// 			{
	// 				for(new i = 0; i < 4; i++)
	// 				{
	// 					if(weaponId == WeapMenu_Ls_WeaponId(playerid, i))
	// 					{
	// 						WeapMenu_Ls_WeaponId(playerid, i) = -1;
	// 						break;
	// 					}
	// 				}
	// 				//WeapMenu_Ls_SortWeapons(playerid);
	// 			}
	// 	}

	// 	new weaponSlot = GetCustomWeaponSlot(weaponId);
	// 	ePlayerWeapon[playerid][e_iWeaponsId][weaponSlot] = 0;
	// 	ePlayerWeapon[playerid][e_iWeaponsAmmo][weaponSlot] = 0;

	// 	if(IsMeleeWeapon(weaponId))
	// 	{
	// 		HudTd_UpdateMelee(playerid);
	// 	}

	// 	if(weaponId == WEAPON_SHIV)
	// 	{
	// 		HudTd_UpdateShivs(playerid);
	// 	}

	// 	DetachWeaponFromPlayersBody(playerid, weaponId);
	// }

//#endregion

	// Reloads player's weapon
	ReloadWeapon(playerid)
	{
		new currentEquippedItemId = GetPlayerEquippedItemId(playerid);

		if(currentEquippedItemId == -1 || GetItemType(currentEquippedItemId) != ITEM_TYPE_WEAPON 
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

	// Reduces condition of player's melee weapon by one.
	DegradePlayerMeleeWeapon(playerid)
	{
		new weaponid = GetPlayerMelee[playerid];

		// Durability logic (reduce upgraded durability or basic one?)
		new newMeleeDurability = (IsPlayerMeleeUpgraded[playerid]) ? (GetPlayerMeleeExtCnd[playerid]-1) : (GetPlayerMeleeCnd[playerid]-1);
		HudTd_UpdateMeleeWeaponCND(playerid, weaponid, newMeleeDurability);

		if(newMeleeDurability > 0)
		{
			return;
		}

		// Removes melee weapon if broke.
		// if(!IsPlayerMeleeUpgraded[playerid])
		// {
		// 	RemovePlayerWeapon(playerid, weaponid);
		// 	return;
		// }

		// If melee is upgraded - remove its upgraded feature and hide bars of it.
		IsPlayerMeleeUpgraded[playerid] = false;
		HudTd_HideMeleeUpgradedBars(playerid, weaponid);

		if(weaponid == WEAPON_SCISSOR_PIPE)
		{
			GetPlayerMelee[playerid] = WEAPON_PIPE;
			//SetPlayerWeapon(playerid, GetPlayerMelee[playerid], GetPlayerMeleeCnd[playerid]);
		}
	}
//#endregion