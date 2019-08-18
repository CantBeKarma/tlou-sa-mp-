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

		if(CurrentPlayerWeaponId[playerid] == WEAPON_SHOTGUN || CurrentPlayerWeaponId[playerid] == WEAPON_SHORTY)
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
	IsValidWeapon(weaponId)
	{
		new weapon_itemId = -1;
		for(new i = 0; i != MAX_ITEMS_DATA; i++)
		{
			if(weaponId == eItemsData[i][e_iItem_SA_WeaponId])
			{
				weapon_itemId = i;
				break;
			}
		}
		return weapon_itemId;
	}

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
	// Set player weapon to dest.
	SetPlayerWeapon(playerid, weaponId, ammo = 1)
	{
		if(weaponId == WEAPON_UNARMED
			|| weaponId == CurrentPlayerWeaponId[playerid] && !IsMeleeWeapon(weaponId))
		{
			return;
		}

		new weapon_itemId = IsValidWeapon(weaponId);
		if(weapon_itemId == -1)
		{
			return;
		}

		new oldWeaponId = CurrentPlayerWeaponId[playerid];
		HidePlayerWeapon(playerid);

		if(ammo <= 0)
		{
			if(weaponId == WEAPON_BOW)
			{
				SPAO(playerid, ITEM_MODEL_BOW, true);
			}
			else if(IsBulletWeapon(weaponId))
			{
				GivePlayerWeapon(playerid, weaponId, _:FLOAT_INFINITY);
			}
			else
			{
				return;
			}
		}
		else if(ammo > 0)
		{
			CurrentPlayerWeaponAmmo[playerid] = ammo;
			PlayerWeaponBodyDamage[playerid] = eItemsData[weapon_itemId][e_iItem_BodyDmg];
			PlayerWeaponHeadDamage[playerid] = eItemsData[weapon_itemId][e_iItem_HeadDmg];

			if(IsMeleeWeapon(weaponId))
			{
				GivePlayerWeapon(playerid, weaponId, 1);
			}
			else if(IsBulletWeapon(weaponId))
			{
				GivePlayerWeapon(playerid, weaponId, _:FLOAT_INFINITY);
				SetPlayerNoReload(playerid, (weaponId == WEAPON_SHOTGUN) ? (false) : (true));

				new clipAmmoSaved = ePlayerWeapon[playerid][e_iWeaponClipAmmo][GetCustomWeaponSlot(weaponId)];
				if(clipAmmoSaved > -1) CurrentPlayerWeaponClipSize[playerid] = clipAmmoSaved;
				else
				{
					new maxWeaponClipSize = GetPlayerWeaponLvl_MaxClipSize(playerid, weaponId);
					CurrentPlayerWeaponClipSize[playerid] = (ammo < maxWeaponClipSize) ? (ammo) : (maxWeaponClipSize);
				}
			}
			else
			{
				SPAO(playerid, \
					(weaponId == WEAPON_MEDKIT ? ITEM_MODEL_BINDING : eItemsData[weapon_itemId][e_iItem_ModelId]), \
					(weaponId == WEAPON_BOW ? true : false));
			}
		}
		
		new newWeaponId = CurrentPlayerWeaponId[playerid] = weaponId;
		OnPlayerWeaponChange(playerid, newWeaponId, oldWeaponId);
	}

	// Hide's player current weapon equipped.
	HidePlayerWeapon(playerid)
	{
		new currentWeapon = CurrentPlayerWeaponId[playerid];

		if(currentWeapon == WEAPON_UNARMED)
			return;
	
		new 
			currentAmmo = GetPlayerWeaponAmmo(playerid, currentWeapon),
			currentClipAmmo = CurrentPlayerWeaponClipSize[playerid];

		// Clear current player's weapon data.
		CurrentPlayerWeaponId[playerid] = WEAPON_UNARMED;
		CurrentPlayerWeaponAmmo[playerid] = 0;
		CurrentPlayerWeaponClipSize[playerid] = 0;
		PlayerWeaponBodyDamage[playerid] = 8.0;
		PlayerWeaponHeadDamage[playerid] = 8.0;
		ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

		// Save player's weapon ammo and save it as a last used weaponId by player.
		LastPlayerWeaponId[playerid] = currentWeapon;
		ePlayerWeapon[playerid][e_iWeaponClipAmmo][GetCustomWeaponSlot(currentWeapon)] = currentClipAmmo;
		SetPlayerWeaponAmmo(playerid, currentWeapon, currentAmmo);

		OnPlayerWeaponChange(playerid, WEAPON_UNARMED, currentWeapon);
	}

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

	// Give player weapon based on slot.
	GivePlayerWeaponInSlot(playerid, weaponId)
	{
		if(HasPlayerWeapon(playerid, weaponId))
			return;

		switch(weaponId)
		{
			case WEAPON_SHOTGUN, WEAPON_RIFLE, WEAPON_BOW, WEAPON_ASSAULT:
			{
				new weapMenu_Ls_freeSlot = WeapMenuTd_GetFreeSlot_H_Ls(playerid);
				WeapMenu_Ls_WeaponId(playerid, weapMenu_Ls_freeSlot) = weaponId;
			}

			case WEAPON_9MM, WEAPON_REVOLVER, WEAPON_SHORTY:
			{
				new weapMenu_Rs_freeSlot = WeapMenuTd_GetFreeSlot_H_Rs(playerid);
				WeapMenu_Rs_WeaponId(playerid, weapMenu_Rs_freeSlot) = weaponId;
			}
		}

		ePlayerWeapon[playerid][e_iWeaponsId][GetCustomWeaponSlot(weaponId)] = weaponId;

		if(IsPlayerUsingWeaponMenu(playerid))
		{
			WeapMenuTd_DisplayWeapsAndAmmo(playerid);
		}
	}

	// Remove player's weapon (doesn't matter if equipped or not).
	RemovePlayerWeapon(playerid, weaponId)
	{
		if(!HasPlayerWeapon(playerid, weaponId))
		{
			return;
		}

		if(CurrentPlayerWeaponId[playerid] == weaponId)
		{
			HidePlayerWeapon(playerid);
		}

		if(LastPlayerWeaponId[playerid] == weaponId)
		{
			LastPlayerWeaponId[playerid] = GetFirstWeaponWithAmmo(playerid);
		}

		switch(weaponId)
		{
			case
				WEAPON_9MM, WEAPON_REVOLVER, WEAPON_SHORTY:
				{
					for(new i = 0; i < 3; i++)
					{
						if(weaponId == WeapMenu_Rs_WeaponId(playerid, i))
						{
							WeapMenu_Rs_WeaponId(playerid, i) = -1;
							break;
						}
					}
					Weapon_Rs_SortWeapons(playerid);
				}

			case 
				WEAPON_SHOTGUN, WEAPON_RIFLE, 
				WEAPON_ASSAULT, WEAPON_BOW:
				{
					for(new i = 0; i < 4; i++)
					{
						if(weaponId == WeapMenu_Ls_WeaponId(playerid, i))
						{
							WeapMenu_Ls_WeaponId(playerid, i) = -1;
							break;
						}
					}
					WeapMenu_Ls_SortWeapons(playerid);
				}
		}

		new weaponSlot = GetCustomWeaponSlot(weaponId);
		ePlayerWeapon[playerid][e_iWeaponsId][weaponSlot] = 0;
		ePlayerWeapon[playerid][e_iWeaponsAmmo][weaponSlot] = 0;

		if(IsMeleeWeapon(weaponId))
		{
			HudTd_UpdateMelee(playerid);
		}

		if(weaponId == WEAPON_SHIV)
		{
			HudTd_UpdateShivs(playerid);
		}

		DetachWeaponFromPlayersBody(playerid, weaponId);
	}

	// Reloads player's weapon
	ReloadWeapon(playerid)
	{
		new currentWeapon = CurrentPlayerWeaponId[playerid];

		if(IsPlayerUnableToPerformAction(playerid)
		|| GetPlayerAnimationIndex(playerid) == RIFLE_RIFLE_CROUCHLOAD || GetPlayerAnimationIndex(playerid) == RIFLE_RIFLE_LOAD
		|| IsPlayerUsingBackpack(playerid) || IsPlayerUsingWeaponMenu(playerid) || !IsBulletWeapon(currentWeapon))
		{
			return;
		}

		new 
			currentAmmo = CurrentPlayerWeaponAmmo[playerid],
			currentClipSize = CurrentPlayerWeaponClipSize[playerid],
			maxWeaponClipSize = GetPlayerWeaponLvl_MaxClipSize(playerid, currentWeapon);

		if(currentAmmo <= currentClipSize || currentClipSize == maxWeaponClipSize)
		{
			return;
		}

		ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;
		
		new newClipSize = CurrentPlayerWeaponClipSize[playerid] = (currentAmmo > maxWeaponClipSize) ? (maxWeaponClipSize) : (currentAmmo);
		OnPlayerReloadWeapon(playerid, currentWeapon, currentClipSize, newClipSize);
	}

	// Gets first bullet weapon that player's own and has some ammo for it (used on the "Click noise")
	GetFirstWeaponWithAmmo(playerid)
	{
		new
			returnWeaponId = 0,
			playerWeaponId = 0;

		// Index = 5 because that's where the bullet weapons indexes start in the weapon's slots.
		// 0..4 = Melee/Projectiles/Shiv
		for(new i = 5; i != MAX_WEAPON_SLOTS; i++)
		{
			playerWeaponId = ePlayerWeapon[playerid][e_iWeaponsId][i];
			
			if(!IsBulletWeapon(playerWeaponId) || CurrentPlayerWeaponId[playerid] == playerWeaponId) 
				continue;

			if(GetPlayerWeaponAmmo(playerid, playerWeaponId) > 0)
			{
				returnWeaponId = playerWeaponId;
				break;
			}
		}
		return returnWeaponId;
	}

	// Gives player his last used weapon (used for instant equips).
	function SetPlayerLastWeapon(playerid)
	{
		// If player's current weapon isn't melee (hands or melee weapon).
		if(!IsMeleeWeapon(CurrentPlayerWeaponId[playerid]))
		{
			return;
		}

		// If player uses backpack animations / or doing other actions.
		if(GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_2IDLE 
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_IN
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_LOOP
		|| IsPlayerUnableToPerformAction(playerid))
		{
			return;
		}

		new lastWeaponId = LastPlayerWeaponId[playerid];
		if(IsBulletWeapon(lastWeaponId) || IsProjectileWeapon(lastWeaponId) || IsThrowableWeapon(lastWeaponId) || lastWeaponId == WEAPON_BOW)
		{
			new lastWeapon_Ammo = GetPlayerWeaponAmmo(playerid, lastWeaponId);
			SetPlayerWeapon(playerid, lastWeaponId, lastWeapon_Ammo);
		}
	}

	// Gives player his melee weapon.
	SetPlayerMeleeWeapon(playerid)
	{
		if(IsPlayerAllowedToShiv(playerid))
		{
			HidePlayerWeapon(playerid);
			return 1;
		}

		new weaponId = 
			(
				  HasPlayerGotMeleeWeapon(playerid) ? GetPlayerMelee[playerid]
				: HasPlayerGotThrowableItem(playerid) ? GetPlayerThrowable[playerid]
				: (WEAPON_UNARMED)
			);

		if(weaponId == WEAPON_UNARMED) HidePlayerWeapon(playerid);
		else
		{
			new durability = (weaponId == GetPlayerMelee[playerid] ? GetPlayerMeleeCnd[playerid] : 1);
			SetPlayerWeapon(playerid, weaponId, durability);
		}
		return 0;
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
		if(!IsPlayerMeleeUpgraded[playerid])
		{
			RemovePlayerWeapon(playerid, weaponid);
			return;
		}

		// If melee is upgraded - remove its upgraded feature and hide bars of it.
		IsPlayerMeleeUpgraded[playerid] = false;
		HudTd_HideMeleeUpgradedBars(playerid, weaponid);

		if(weaponid == WEAPON_SCISSOR_PIPE)
		{
			GetPlayerMelee[playerid] = WEAPON_PIPE;
			SetPlayerWeapon(playerid, GetPlayerMelee[playerid], GetPlayerMeleeCnd[playerid]);
		}
	}
//#endregion