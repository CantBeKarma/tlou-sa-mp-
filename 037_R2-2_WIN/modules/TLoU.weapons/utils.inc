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
		return IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][2]) || IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
	}

	// Show player custom-crosshair for weapons.
	ShowPlayerCrosshair(playerid)
	{
		if(Bit_Get(ePlayerFlag[e_bAiming], playerid) && !IsCrosshairVisibleForPlayer(playerid))
		{
			if(CurrentPlayerWeaponId[playerid] == WEAPON_SHOTGUN || CurrentPlayerWeaponId[playerid] == WEAPON_SHORTY) 
				TdShowForPlayer(playerid, eTextDrawInfo[e_Aim][1]), TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][0]);
			else 
				TdShowForPlayer(playerid, eTextDrawInfo[e_Aim][0]), TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][1]);
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
		if(Bit_Get(ePlayerFlag[e_bAiming], playerid))
		{
			if(bodypart == BODY_PART_HEAD) 
				TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][2]), TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
			else 
				TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][3]), TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][2]);


			if(!IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][2]) || !IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][3]))
			{
				SetTimerEx("HidePlayerHitMarker", 300, false, "d", playerid);
			}
		}
	}

	// Hide crosshair damage symbol.
	function HidePlayerHitMarker(playerid)
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
	// GetCustomWeaponSlot(weaponId)
	// {
	// 	new slot = -1;
	// 	switch(weaponId)
	// 	{
	// 		case 
	// 			WEAPON_PIPE, 
	// 			WEAPON_SCISSOR_PIPE, 
	// 			WEAPON_HATCHET, 
	// 			WEAPON_MACHETE: 	slot = WEAPON_SLOT_MELEE;

	// 		case WEAPON_SHIV: 		slot = WEAPON_SLOT_SHIV;
	// 		case WEAPON_MEDKIT: 	slot = WEAPON_SLOT_MEDKIT;
	// 		case WEAPON_MOLOTOV: 	slot = WEAPON_SLOT_MOLOTOV;
	// 		case WEAPON_NAIL_BOMB: 	slot = WEAPON_SLOT_BOMB;
	// 		case WEAPON_SMOKE_BOMB: slot = WEAPON_SLOT_SMOKE_BOMB;

	// 		case WEAPON_9MM: 		slot = WEAPON_SLOT_9MM;
	// 		case WEAPON_REVOLVER: 	slot = WEAPON_SLOT_REVOLVER;
	// 		case WEAPON_SHORTY:		slot = WEAPON_SLOT_SHORTY;
			
	// 		case WEAPON_SHOTGUN:	slot = WEAPON_SLOT_SHOTGUN;
	// 		case WEAPON_RIFLE:		slot = WEAPON_SLOT_RIFLE;
	// 		case WEAPON_ASSAULT:	slot = WEAPON_SLOT_ASSAULT;
	// 		case WEAPON_BOW: 		slot = WEAPON_SLOT_BOW;
			
	// 		case WEAPON_BRICK, 
	// 			WEAPON_BOTTLE:		slot = WEAPON_SLOT_BRICK_BOTTLE;
	// 	}
	// 	return slot;
	// }

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

		HidePlayerWeapon(playerid);
		CurrentPlayerWeaponId[playerid] = weaponId;

		if(ammo <= 0)
		{
			if(weaponId == WEAPON_BOW) SPAO(playerid, ITEM_MODEL_BOW, true);
			else if(IsBulletWeapon(weaponId)) GivePlayerWeapon(playerid, weaponId, _:FLOAT_INFINITY);
			else return;
		}
		else if(ammo > 0)
		{
			CurrentPlayerWeaponAmmo[playerid] = ammo;
			PlayerWeaponBodyDamage[playerid] = eItemsData[weapon_itemId][e_iItem_BodyDmg];
			PlayerWeaponHeadDamage[playerid] = eItemsData[weapon_itemId][e_iItem_HeadDmg];

			if(IsMeleeWeapon(weaponId)) GivePlayerWeapon(playerid, weaponId, 1);
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

		// Detach weapon from player's body when it is in use.
		DetachWeaponFromPlayersBody(playerid, weaponId);

		// Update HUD weapon display.
		ShowHud(playerid);
		HudTd_UpdateWeapon(playerid, weaponId);
		HudTd_UpdateWeaponAmmo(playerid, weaponId);
		HudTd_DisplayMeleeWeaponBars(playerid, weaponId);
		HudTd_DisplayWeaponCurrentAmmo(playerid, weaponId);
		return;
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

		// Remove attached object if it was a projectile/medkit/throwable.
		RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
		ResetPlayerWeapons(playerid);

		// Don't do any attachments with my current weapon if it
		// was a projectile or a throwable weapon (not necessary to call).
		// Additionally: It may be re-attached in SetPlayerWeapon at the very end!
		if(!IsProjectileWeapon(currentWeapon) && !IsThrowableWeapon(currentWeapon))
		{
			SPAO(playerid, GetWeaponModelFromWeaponId(currentWeapon));
		}

		// Update HUD weapon display 
		HudTd_UpdateWeapon(playerid, WEAPON_UNARMED);
		
		HudTd_UpdateWeaponAmmo(playerid, WEAPON_UNARMED);
		HudTd_HideWeaponCurrentAmmo(playerid, currentWeapon);

		HudTd_HideMeleeWeaponBars(playerid, currentWeapon);
		HudTd_HideMeleeUpgradedBars(playerid, currentWeapon);
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
			return;

		if(CurrentPlayerWeaponId[playerid] == weaponId)
			HidePlayerWeapon(playerid);

		if(LastPlayerWeaponId[playerid] == weaponId)
			LastPlayerWeaponId[playerid] = GetFirstWeaponWithAmmo(playerid);

		// switch(weaponId)
		// {
		// 	case 
		// 		WEAPON_SHOTGUN, 
		// 		WEAPON_RIFLE, 
		// 		WEAPON_ASSAULT, 
		// 		WEAPON_BOW:
		// 		{
		// 			WeapMenu_Ls_SortWeapons(playerid);
		// 		}
		// }

		new weaponSlot = GetCustomWeaponSlot(weaponId);
		ePlayerWeapon[playerid][e_iWeaponsId][weaponSlot] = 0;
		ePlayerWeapon[playerid][e_iWeaponsAmmo][weaponSlot] = 0;

		if(IsMeleeWeapon(weaponId))
			HudTd_UpdateMelee(playerid);

		if(weaponId == WEAPON_SHIV)
			HudTd_UpdateShivs(playerid);
	}

	// Reloads player's weapon
	ReloadWeapon(playerid)
	{
		new currentWeapon = CurrentPlayerWeaponId[playerid];

		if(IsPlayerPlayingActions(playerid)
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
	SetPlayerLastWeapon(playerid)
	{
		// If player's current weapon isn't melee (hands or melee weapon).
		if(!IsMeleeWeapon(CurrentPlayerWeaponId[playerid])) 
			return;

		// If player uses backpack animations / or doing other actions.
		if(GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_2IDLE 
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_IN
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_LOOP
		|| IsPlayerPlayingActions(playerid))
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
			SCMF(playerid, -1, "Player is allowed to shiv. I won't equip a melee weapon.");
			HidePlayerWeapon(playerid);
			return 1;
		}

		new weaponId = 
			(
				  IsPlayerGotMeleeWeapon(playerid) ? GetPlayerMelee[playerid]
				: IsPlayerGotThrowableItem(playerid) ? GetPlayerThrowable[playerid]
				: (NO_MELEE)
			);

		SCMF(playerid, -1, "Melee weaponId: %d", weaponId);

		if(weaponId == NO_MELEE) HidePlayerWeapon(playerid);
		else
		{
			new durability = (weaponId == GetPlayerMelee[playerid] ? GetPlayerMeleeCnd[playerid] : 1);
			SetPlayerWeapon(playerid, weaponId, durability);
		}
		return 0;
	}
//#endregion