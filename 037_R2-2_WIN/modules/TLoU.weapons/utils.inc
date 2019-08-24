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