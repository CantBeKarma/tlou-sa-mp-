// Clearing weapons data for player.
ResetEnum_ePlayerWeapon(playerid)
{
		static sBlank[e_PLAYER_WEAPONS_DATA];
		ePlayerWeapon[playerid] = sBlank;

		print("ResetEnum_ePlayerWeapon");
}

//#region Crosshair

	// Does a check if player sees a crosshair currently.
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
			{
				TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
				TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
			}
			else 
			{
				TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Aim][3]);
				TdHideForPlayer(playerid, eTextDrawInfo[e_Aim][2]);
			}

			if(!IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][2]) 
				|| !IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Aim][3]))
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
			if(weaponId != eItemsData[i][e_iItem_SA_WeaponId])
				continue;
				
			weapon_itemId = i;
			break;
		}
		return weapon_itemId;
	}

	// Return weapon's model based on ID.
	GetWeaponModelFromId(weaponId)
	{
		new weapon_itemId = IsValidWeapon(weaponId);
		if(weapon_itemId == -1)
		{
			return -1;
		}
		return eItemsData[weapon_itemId][e_iItem_ModelId];
	}

	// Return weapon's slot index in the player's inventory.
	GetWeaponSlotId(weaponId)
	{
		new weapon_itemId = IsValidWeapon(weaponId);
		if(weapon_itemId == -1)
		{
			return -1;
		}
		return eItemsData[weapon_itemId][e_iItem_WeaponCustomSlot];
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
	}

	// Check if a weapon is melee type weapon.
	IsMeleeWeapon(weaponId)
	{
		return weaponId == WEAPON_UNARMED 
			|| weaponId == WEAPON_PIPE 
			|| weaponId == WEAPON_SCISSOR_PIPE 
			|| weaponId == WEAPON_HATCHET 
			|| weaponId == WEAPON_MACHETE;
	}

	// Check if a weapon is projectile type of a weapon.
	IsProjectileWeapon(weaponId)
	{
		return weaponId == WEAPON_MOLOTOV 
			|| weaponId == WEAPON_NAIL_BOMB 
			|| weaponId == WEAPON_SMOKE_BOMB;
	}

	// Check if a weapon is throwable type of a weapon.
	IsThrowableWeapon(weaponId)
	{
		return weaponId == WEAPON_BRICK 
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
		new weapon_itemId = IsValidWeapon(weaponId);

		if(weapon_itemId == -1
		|| weaponId == WEAPON_UNARMED
		|| weaponId == CurrentPlayerWeaponId[playerid] && !IsMeleeWeapon(weaponId))
		{
			return;
		}

		new oldWeaponId = CurrentPlayerWeaponId[playerid];
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

				new weaponSlotId = GetWeaponSlotId(weaponId);
				if(weaponSlotId == -1)
				{
					return;
				}

				new clipAmmoSaved = ePlayerWeapon[playerid][e_iWeaponClipAmmo][weaponSlotId];
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

		OnPlayerWeaponChange(playerid, oldWeaponId, weaponId);
	}

	// Hide's player current weapon equipped.
	HidePlayerWeapon(playerid, weaponId)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return;
		}

		if(weaponId != WEAPON_UNARMED)
		{
			new 
				weaponAmmo = GetPlayerWeaponAmmo(playerid, weaponId),
				weaponClipAmmo = CurrentPlayerWeaponClipSize[playerid];

			// Clear current player's weapon data.
			CurrentPlayerWeaponId[playerid] = WEAPON_UNARMED;
			CurrentPlayerWeaponAmmo[playerid] = CurrentPlayerWeaponClipSize[playerid] = 0;
			PlayerWeaponBodyDamage[playerid] = PlayerWeaponHeadDamage[playerid] = 8.0;
			ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

			// Save player's weapon ammo and save it as a last used weaponId by player.
			LastPlayerWeaponId[playerid] = weaponId;
			ePlayerWeapon[playerid][e_iWeaponClipAmmo][weaponSlotId] = weaponClipAmmo;
			SetPlayerWeaponAmmo(playerid, weaponId, weaponAmmo);

			// Remove attached object if it was a projectile/medkit/throwable.
			RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
			ResetPlayerWeapons(playerid);

			// Don't do any any attachments with my current weapon if it
			// was a projectile or a throwable weapon (not necessary to call).
			// Additionally: It may be re-attached in SetPlayerWeapon at the very end!
			if(!IsProjectileWeapon(weaponId) && !IsThrowableWeapon(weaponId))
			{
				SPAO(playerid, GetWeaponModelFromId(weaponId));
			}

			// Update HUD weapon display 
			HudTd_UpdateWeapon(playerid, WEAPON_UNARMED);
			
			HudTd_UpdateWeaponAmmo(playerid, WEAPON_UNARMED);
			HudTd_HideWeaponCurrentAmmo(playerid, weaponId);

			HudTd_HideMeleeWeaponBars(playerid, weaponId);
			HudTd_HideMeleeUpgradedBars(playerid, weaponId);
		}
	}

	// Set player's weapon ammo (doesn't matter if weapon is equipped or not).
	SetPlayerWeaponAmmo(playerid, weaponId, ammo)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return;
		}

		if(CurrentPlayerWeaponId[playerid] != weaponId) PlayerWeaponSlotAmmo[playerid, weaponSlotId] = ammo;
		else
		{
			CurrentPlayerWeaponAmmo[playerid] = ammo;
			HudTd_UpdateWeaponAmmo(playerid, weaponId);
		}
	}

	// Get player's weapon ammo (doesn't matter if weapon is equipped or not).
	GetPlayerWeaponAmmo(playerid, weaponId)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return -1;
		}

		return CurrentPlayerWeaponId[playerid] == weaponId ? CurrentPlayerWeaponAmmo[playerid] : PlayerWeaponSlotAmmo[playerid, weaponSlotId];
	}

	// Check if player has certain weapon.
	HasPlayerWeapon(playerid, weaponId)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return -1;
		}

		new bool:result = false;
		if(weaponId <= 0) result = false;
		else if(CurrentPlayerWeaponId[playerid] == weaponId) result = true;
		else
		{
			result = (ePlayerWeapon[playerid][e_iWeaponsId][weaponSlotId] == weaponId) ? (true) : (false);
		}
		return result;
	}

	// Iter_HasPlayerWeapon(playerid, weaponId)
	// {
	// 	return Iter_Contains(PlayerWeapons[playerid]<weaponId>);
	// }

	// Iter_GivePlayerWeapon(playerid, weaponId)
	// {
	// 	if(Iter_HasPlayerWeapon(playerid, weaponId))
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

	// 	Iter_Add(PlayerWeapons<playerid>, weaponId);

	// 	if(IsPlayerUsingWeaponMenu(playerid))
	// 	{
	// 		WeapMenuTd_DisplayWeapsAndAmmo(playerid);
	// 	}
	// }

	// Give player weapon based on slot.
	GivePlayerWeaponInSlot(playerid, weaponId)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return;
		}

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

		ePlayerWeapon[playerid][e_iWeaponsId][weaponSlotId] = weaponId;

		if(IsPlayerUsingWeaponMenu(playerid))
		{
			WeapMenuTd_DisplayWeapsAndAmmo(playerid);
		}
	}

	// Remove player's weapon (doesn't matter if equipped or not).
	RemovePlayerWeapon(playerid, weaponId)
	{
		new weaponSlotId = GetWeaponSlotId(weaponId);
		if(weaponSlotId == -1)
		{
			return;
		}

		if(!HasPlayerWeapon(playerid, weaponId))
			return;

		if(CurrentPlayerWeaponId[playerid] == weaponId)
			HidePlayerWeapon(playerid, weaponId);

		if(LastPlayerWeaponId[playerid] == weaponId)
			LastPlayerWeaponId[playerid] = GetNextWeaponWithAmmo(playerid);

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

		ePlayerWeapon[playerid][e_iWeaponsId][weaponSlotId] = 0;
		ePlayerWeapon[playerid][e_iWeaponsAmmo][weaponSlotId] = 0;

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
			return 1;
		}

		new 
			currentAmmo = CurrentPlayerWeaponAmmo[playerid],
			currentClipSize = CurrentPlayerWeaponClipSize[playerid],
			maxWeaponClipSize = GetPlayerWeaponLvl_MaxClipSize(playerid, currentWeapon);

		if(currentAmmo <= currentClipSize || currentClipSize == maxWeaponClipSize)
		{
			return 1;
		}

		ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;
		
		new newClipSize = CurrentPlayerWeaponClipSize[playerid] = (currentAmmo > maxWeaponClipSize) ? (maxWeaponClipSize) : (currentAmmo);
		OnPlayerReloadWeapon(playerid, currentWeapon, currentClipSize, newClipSize);
		return 0;
	}

	// Gets first bullet weapon that player's own and has some ammo for it (used on the "Click noise")
	GetNextWeaponWithAmmo(playerid)
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
			return 1;

		// If player uses backpack animations / or doing other actions.
		if(GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_2IDLE 
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_IN
		|| GetPlayerAnimationIndex(playerid) == BOMBER_BOM_PLANT_LOOP
		|| IsPlayerPlayingActions(playerid))
		{
			return 1;
		}

		new lastWeaponId = LastPlayerWeaponId[playerid];
		if(IsBulletWeapon(lastWeaponId) || IsProjectileWeapon(lastWeaponId) || IsThrowableWeapon(lastWeaponId) || lastWeaponId == WEAPON_BOW)
		{
			new lastWeapon_Ammo = GetPlayerWeaponAmmo(playerid, lastWeaponId);
			SetPlayerWeapon(playerid, lastWeaponId, lastWeapon_Ammo);
		}
		return 0;
	}

	// Gives player his melee weapon.
	SetPlayerMeleeWeapon(playerid)
	{
		if(IsPlayerAllowedToShiv(playerid))
		{
			SCMF(playerid, -1, "Player is allowed to shiv. I won't equip a melee weapon.");
			HidePlayerWeapon(playerid, CurrentPlayerWeaponId[playerid]);
			return 1;
		}

		new weaponId = 
			(
				  IsPlayerGotMeleeWeapon(playerid) ? GetPlayerMelee[playerid]
				: IsPlayerGotThrowableItem(playerid) ? GetPlayerThrowable[playerid]
				: (NO_MELEE)
			);

		SCMF(playerid, -1, "Melee weaponId: %d", weaponId);

		if(weaponId == NO_MELEE) HidePlayerWeapon(playerid, CurrentPlayerWeaponId[playerid]);
		else
		{
			new durability = (weaponId == GetPlayerMelee[playerid] ? GetPlayerMeleeCnd[playerid] : 1);
			SetPlayerWeapon(playerid, weaponId, durability);
		}
		return 0;
	}
//#endregion