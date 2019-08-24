//#region Validation

    IsValidItemId(itemId)
    {
        return (itemId >= 0 && itemId < MAX_ITEMS_DATA);
    }

    GetItemType(itemId)
    {
        if(!IsValidItemId(itemId))
        {
            return -1;
        }

        return (eItemsData[itemId][e_iItem_Type]);
    }

    GetItemModel(itemId)
    {
        if(!IsValidItemId(itemId))
        {
            return -1;
        }

        return (eItemsData[itemId][e_iItem_ModelId]);
    }

    GetItemMaxUpgradeLvl(itemId, upgradeType)
    {
        if(!IsValidItemId(itemId))
        {
            return -1;
        }

        new maxUpgradeLevel = -1;
        for(new i = 0; i != MAX_UPGRADE_ITEMS; i++)
        {
            if(eItemsUpgrades[i][e_iItemId] != ITEM_IDX:itemId
            || eItemsUpgrades[i][e_iItem_UpgradeType] != upgradeType)
            {
                continue;
            }

            maxUpgradeLevel = eItemsUpgrades[i][e_iItem_UpgradeLvl];
        }
        return maxUpgradeLevel;
    }

    GetItemNativeWeaponId(itemId)
    {
        if(!IsValidItemId(itemId))
        {
            return -1;
        }

        return (eItemsData[itemId][e_iItem_SA_WeaponId]);
    }

    bool:IsItemUpgradeable(itemId, upgradeType = -1)
    {
        for(new i = 0; i != MAX_UPGRADE_ITEMS; i++)
        {
            if(upgradeType == -1)
            {
                if(ITEM_IDX:itemId == eItemsUpgrades[i][e_iItemId])
                {
                    return true;
                }
            }
            else
            {
                if(ITEM_IDX:itemId == eItemsUpgrades[i][e_iItemId]
                && upgradeType == eItemsUpgrades[i][e_iItem_UpgradeType])
                {
                    return true;
                }
            }
        }
        return false;
    }

    bool:IsPlayerFullOnItem(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        new bool:result = false;

        if(eItemsData[itemId][e_iItem_MaxAmount] == -1 && (IsItemUpgradeable(itemId) || GetItemType(itemId) == ITEM_TYPE_MELEE))
        {
            result = (GetPlayerItemAmount(playerid, itemId) >= GetPlayerItemMaxAmount(playerid, itemId));
        }
        else
        {
            result = (GetPlayerItemAmount(playerid, itemId) >= eItemsData[itemId][e_iItem_MaxAmount]);
        }
        return result;
    }

    IsItemBulletWeapon(itemId)
    {
        return (IsValidItemId(itemId) && GetItemType(itemId) == ITEM_TYPE_WEAPON && ITEM_IDX:itemId != ITEM_IDX:ITEM_IDX_BOW);
    }

    IsItemMeleeWeapon(itemId)
    {
        return (IsValidItemId(itemId) && GetItemType(itemId) == ITEM_TYPE_MELEE);
    }

//#endregion

//#region Items | Get Values

    HasPlayerGotItem(playerid, itemId)      return (IsValidItemId(itemId) && ePlayerItemData[playerid][itemId][e_iItemId] != -1);
    GetPlayerEquippedItemId(playerid)       return (ePlayerInfo[playerid][e_iCurrentEquippedItemId]);
    GetPlayerLastEquippedItemId(playerid)   return (ePlayerInfo[playerid][e_iLastEquippedItemId]);

    // Gets player item's specific upgrade level.
    // Returns:
    // (-1) if player hasn't got an item or item doesn't appear to be upgradeable for specific upgradeType
    // (x) value of the upgrade level (integer)
    GetPlayerItemUpgradeLvl(playerid, itemId, upgradeType)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return -1;
        }

        switch(upgradeType)
        {
            case UPGRADE_TYPE_CLIPSIZE: return ePlayerItemData[playerid][itemId][e_iItemUpgradeLvl_ClipSize];
        }
        return -1;
    }

    // Returns maximal possible weapon clipsize that player can have for a specific weapon.
    // Returns:
    // (-1) if player hasn't got an item or didn't find specified item in the list of upgradeable items.
    // (x) value of max clip size for that item
    GetPlayerItemMaxClipSize(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId) || !IsItemBulletWeapon(itemId))
        {
            return -1;
        }

        if(eItemsData[itemId][e_iItem_MaxClipSize] != -1)
        {
            return eItemsData[itemId][e_iItem_MaxClipSize];
        }

        new playerItemClipSizeLvl = GetPlayerItemUpgradeLvl(playerid, itemId, UPGRADE_TYPE_CLIPSIZE);

        for(new i = 0; i != MAX_UPGRADE_ITEMS; i++)
        {
            if(eItemsUpgrades[i][e_iItemId] == ITEM_IDX:itemId
            && eItemsUpgrades[i][e_iItem_UpgradeLvl] == playerItemClipSizeLvl
            && eItemsUpgrades[i][e_iItem_UpgradeType] == UPGRADE_TYPE_CLIPSIZE)
            {
                return eItemsUpgrades[i][e_iItem_ClipSize];
            }
        }
        return -1;
    }

    // Gets player's current clip amount of the item (fire weapon) specified
    // Returns:
    // (-1) if player hasn't got an item OR item is not a weapon OR item is a bow
    // (x) current clip amount
    GetPlayerItemClipAmount(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return -1;
        }

        if(GetItemType(itemId) != ITEM_TYPE_WEAPON || ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_BOW)
        {
            return -1;
        }

        return ePlayerItemData[playerid][itemId][e_iItemClipAmount];
    }

    // Gets player item's amount of specified itemId.
    // Returns:
    // (-1) if player hasn't got an item or is invalid.
    // (x) value of the amount that player has for that item (integer)
    GetPlayerItemAmount(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return -1;
        }

        return ePlayerItemData[playerid][itemId][e_iItemAmount];
    }

    // Gets maximal amount of an item type that's possible to get for player.
    // Returns:
    // (-1) if player hasn't got an item or system tried to check for melee extra CND on item which is not a melee weapon
    // (x) maximal value of an item 
    // (results are possible to be returned from 3 tables - eItemsData | eItemsUpgrades | eItemsMeleeCND)
    GetPlayerItemMaxAmount(playerid, itemId, bool:checkMeleeExtraCND = false)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return -1;
        }

        new maxAmount = -1;
        if(checkMeleeExtraCND == true)
        {
            if(GetItemType(itemId) != ITEM_TYPE_MELEE)
            {
                return -1;
            }

            for(new i = 0; i != MAX_MELEE_ITEMS; i++)
            {
                if(eItemsMeleeCND[i][e_iItemId] == ITEM_IDX:itemId)
                {
                    maxAmount = eItemsMeleeCND[i][e_iItemExtraCND];
                    break;
                }
            }
        }
        else
        {
            maxAmount = eItemsData[itemId][e_iItem_MaxAmount];
            if(maxAmount == -1)
            {
                if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
                {
                    new playerItemClipSizeLvl = GetPlayerItemUpgradeLvl(playerid, itemId, UPGRADE_TYPE_CLIPSIZE);
                
                    for(new i = 0; i != MAX_UPGRADE_ITEMS; i++)
                    {
                        if(eItemsUpgrades[i][e_iItemId] == ITEM_IDX:itemId
                        && eItemsUpgrades[i][e_iItem_UpgradeLvl] == playerItemClipSizeLvl
                        && eItemsUpgrades[i][e_iItem_UpgradeType] == UPGRADE_TYPE_CLIPSIZE)
                        {
                            maxAmount = eItemsUpgrades[i][e_iItem_MaxAmount];
                            break;
                        }
                    }
                }
                else if(GetItemType(itemId) == ITEM_TYPE_MELEE)
                {
                    for(new i = 0; i != MAX_MELEE_ITEMS; i++)
                    {
                        if(eItemsMeleeCND[i][e_iItemId] == ITEM_IDX:itemId)
                        {
                            maxAmount = eItemsMeleeCND[i][e_iItemBasicCND];
                            break;
                        }
                    }
                }
            }
        }
        return maxAmount;
    }

    // Gets player's melee item id (MELEE or THROWABLE)
    // Returns:
    // (-1) if no melee weapon/throwable was found
    // (x) item id.
    GetPlayerMeleeItemId(playerid)
    {
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(GetItemType(i) != ITEM_TYPE_MELEE
            || GetItemType(i) != ITEM_TYPE_THROWABLE)
            {
                continue;
            }

            if(!HasPlayerGotItem(playerid, i))
            {
                continue;
            }

            if(GetPlayerItemAmount(playerid, i) > 0)
            {
                return i;
            }
        }
        return -1;
    }

    // Gets first bullet weapon that player's own and has some ammo for it.
    // Returns:
    // (-1) if no weapon found with ammo in it.
    // (x) item id.
	GetFirstWeaponWithAmmo(playerid)
	{
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(GetItemType(i) != ITEM_TYPE_WEAPON || !HasPlayerGotItem(playerid, i) || GetPlayerItemAmount(playerid, i) <= 0)
            {
                continue;
            }

            return i;
        }
        return -1;
    }

//#endregion

//#region Items | Set Values

    // Set player item's upgrade level.
    // Returns:
    // false - player hasn't got an item OR if item is not upgradeable OR system is trying to set level over the maximal possible
    // true - if item's level set correctly
    bool:SetPlayerItemUpgradeLvl(playerid, itemId, level, upgradeType)
    {
        if(!HasPlayerGotItem(playerid, itemId) || !IsItemUpgradeable(itemId, upgradeType)
        || level > GetItemMaxUpgradeLvl(itemId, upgradeType))
        {
            return false;
        }

        switch(upgradeType)
        {
            case UPGRADE_TYPE_CLIPSIZE: ePlayerItemData[playerid][itemId][e_iItemUpgradeLvl_ClipSize] = level;
        }
        return true;
    }

    // ! DO NOT USE MANUALLY ANYWHERE ! this function is used internally in SetPlayerItemAmount !
    // ( IF CND set to 0 an item is going to be removed from player!)
    // Returns:
    // false - if player hasn't got an item OR item isn't a melee weapon
    // true - if melee's weapon CND set correctly
    bool:SetPlayerMeleeWeaponCND(playerid, itemId, basicCND, extraCND = 0)
    {
        //     // Melee weapons.
        //     if(meleeUpgradedCnd > 0)
        //     {
        //         IsPlayerMeleeUpgraded[playerid] = true;

        //         new maxMeleeExtCnd = GetMeleeWeapon_Durability(weaponId, true);
        //         GetPlayerMeleeExtCnd[playerid] = (meleeUpgradedCnd > maxMeleeExtCnd) ? (maxMeleeExtCnd) : (meleeUpgradedCnd);
        //     }

        //     // If trying to give melee weapon and player 
        //     // already had a melee weapon - remove ownership of it from DB.
        //     if(HasPlayerGotMeleeWeapon(playerid))
        //     {
        //         new playerMeleeItemIdx = FindItemIdxByWeaponId(GetPlayerMelee[playerid]);
        //         if(playerMeleeItemIdx != -1)
        //         {
        //             DB_RemovePlayerItem(playerid, playerMeleeItemIdx);
        //         }
        //     }

        if(!HasPlayerGotItem(playerid, itemId) || !IsItemMeleeWeapon(itemId))
        {
            return false;
        }

        if(basicCND <= 0)
        {
            RemovePlayerItem(playerid, itemId);
        }
        else
        {
            new maxBasicCND = GetPlayerItemMaxAmount(playerid, itemId);
            ePlayerItemData[playerid][itemId][e_iItemAmount] = (basicCND > maxBasicCND) ? (maxBasicCND) : (basicCND);
        }

        if(extraCND > 0)
        {
            new maxExtraCND = GetPlayerItemMaxAmount(playerid, itemId, true);
            ePlayerItemData[playerid][itemId][e_iItemMeleeExtraCND] = (extraCND > maxExtraCND) ? (maxExtraCND) : (extraCND);
        }
        return true;
    }

    // Set player item's amount
    // Returns:
    // false - if player hasn't got an item OR player is full on that item already OR amount for ingredient is not specified correctly (every 25) OR can't find MaxAmount for the item!
    // true - if player item's amount has been set correctly
    SetPlayerItemAmount(playerid, itemId, amount, meleeExtraCND = 0)
    {
        if(!HasPlayerGotItem(playerid, itemId) || IsPlayerFullOnItem(playerid, itemId))
        {
            return false;
        }

        new maxItemAmount = GetPlayerItemMaxAmount(playerid, itemId);
        if(maxItemAmount == -1)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SetPlayerItemAmount: received invalid max item amount (-1). ItemId: %d", itemId);
            return false;
        }

        new bool:success = false;
        if(GetItemType(itemId) == ITEM_TYPE_INGREDIENT)
        {
            success = ((amount % 25) == 0);
        }

        if(GetItemType(itemId) == ITEM_TYPE_MELEE)
        {
            success = SetPlayerMeleeWeaponCND(playerid, itemId, amount, meleeExtraCND);
        }

        if(!success)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SetPlayerItemAmount: Something went wrong. ItemId: %d | amount: %d | maxItemAmount: %d", itemId, amount, maxItemAmount);
            return false;
        }

        ePlayerItemData[playerid][itemId][e_iItemAmount] = (amount > maxItemAmount) ? (maxItemAmount) : (amount);

        // DB_UpdatePlayerItemAmount(itemId, calcAmount, .ownerUid = eCharsInfo[playerid][e_iUid]);

        UpdateTextDrawsIfNecessary(playerid, itemId);
        return true;
    }


    UpdateTextDrawsIfNecessary(playerid, itemId)
    {
        if(IsPlayerUsingWeaponMenu(playerid))
        {
            WeapMenuTd_DisplayWeapsAndAmmo(playerid);
            CraftTd_CheckWhatsToCraft(playerid);
        }

        switch(GetItemType(itemId))
        {
            case ITEM_TYPE_INGREDIENT:
            {
                if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
                {
                    new selectedCraft = Craft_GetSelection(playerid);
                    CraftTd_CheckWhatsToCraft(playerid);
                    CraftTd_UpdateViewingItem(playerid, selectedCraft);
                    CraftTd_HighlightSelection(playerid, selectedCraft);
                    CraftTd_UpdateIngr(playerid, itemId);
                }
                
                UpdateNavItemsColors(playerid);
            }

            case ITEM_TYPE_MELEE, ITEM_TYPE_MISC:
            {
                if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING && ITEM_IDX:itemId != ITEM_IDX_SHIV)
                {
                    CraftTd_UpdateViewingItem(playerid, CRAFT_ITEM_MELEE);
                }
                else
                {
                    ShowHud(playerid);
                }
            }

            case ITEM_TYPE_WEAPON:
            {
                if(GetPlayerEquippedItemId(playerid) == itemId)
                {
                    HudTd_UpdateWeaponAmmo(playerid, itemId);
                }
            }

            default:
            {
                if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
                {
                    // Update backpack items amounts. Foreach?
                    //CraftTd_UpdateItemAmount(playerid, eItemsData[itemId][e_iItem_CraftItemId], calcAmount);
                }
            }
        }
    }

//#endregion

//#region Items | Give/Remove

    GivePlayerItem(playerid, itemId)
    {
        if(HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        // // Update database info & give player item.
        // if(!HasPlayerGotItem(playerid, weaponId))
        // {
        //     //GivePlayerWeaponInSlot(playerid, weaponId);
        //     DB_GivePlayerItem(itemId, calcAmount, (IsMeleeWeapon(weaponId) ? GetPlayerMeleeExtCnd[playerid] : 0), eCharsInfo[playerid][e_iUid]);
        // }

        WeapMenu_AssignItemToFreeSlot(playerid, itemId);

        ePlayerItemData[playerid][itemId][e_iItemId] = itemId;

        if(IsPlayerUsingWeaponMenu(playerid))
        {
            WeapMenuTd_DisplayWeapsAndAmmo(playerid);
        }
        return true;
    }

    RemovePlayerItem(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        if(GetPlayerEquippedItemId(playerid) == itemId)
        {
            HideItem(playerid);
        }
        else if(GetPlayerLastEquippedItemId(playerid) == itemId)
        {
            ePlayerInfo[playerid][e_iLastEquippedItemId] = GetFirstWeaponWithAmmo(playerid);
        }

        // If it was an item (weapon) from horizontal weapon menu bar
        // below function is going to sort its order if necessary
        WeapMenu_RemoveAndSortItems(playerid, itemId);

        ePlayerItemData[playerid][itemId][e_iItemId] = -1;
        ePlayerItemData[playerid][itemId][e_iItemAmount] = -1;

        if(IsItemMeleeWeapon(itemId))
		{
			HudTd_UpdateMelee(playerid);
		}

		if(ITEM_IDX:itemId == ITEM_IDX_SHIV)
		{
			HudTd_UpdateShivs(playerid);
		}

		DetachWeaponFromPlayersBody(playerid, GetItemNativeWeaponId(itemId));
        return true;
    }

//#endregion

//#region Items | Equipping/Hiding

    function EquipItem(playerid, itemId)
    {
        SCMF(playerid, -1, "EquipItem: %d", itemId);

        if(!HasPlayerGotItem(playerid, itemId) 
        || IsPlayerUnableToPerformAction(playerid)
        || IsPlayerUsingBackpackAnims(playerid))
        {
            SCMF(playerid, -1, "Stopped at 1st validation");
            return false;
        }

        // If player is allowed to shiv, do not equip melee weapon
        // and hide player weapon.
        if(IsPlayerAllowedToShiv(playerid) && GetItemType(itemId) == ITEM_TYPE_MELEE)
		{
            SCMF(playerid, -1, "Stopped at 2nd validation");
            HideItem(playerid);
            return false;
        }

        // Tries to equip last weapon but has some weapon equipped already (and not using weapon menu).
        if(GetPlayerEquippedItemId(playerid) != -1 
        && GetPlayerLastEquippedItemId(playerid) == itemId
        && !IsPlayerUsingWeaponMenu(playerid))
        {
            SCMF(playerid, -1, "Stopped at 3rd validation");
            return false;
        }

        new itemAmount = GetPlayerItemAmount(playerid, itemId);
        
        // Equipping weapons with no ammo/amount is allowed 
        // only for BOW and items of type WEAPON.
        if(itemAmount <= 0 
        && (GetItemType(itemId) == ITEM_TYPE_PROJECTILE 
        ||  GetItemType(itemId) == ITEM_TYPE_THROWABLE)
        ||  GetItemType(itemId) == ITEM_TYPE_HEAL
        ||  GetItemType(itemId) == ITEM_TYPE_MELEE)
        {
            SCMF(playerid, -1, "Stopped at 4th validation");
            return false;
        }

        // Hides player's currently equipped itemId if there are any.
        HideItem(playerid);

        SCMF(playerid, -1, "Passed all validation.");

        new nativeWeaponId = GetItemNativeWeaponId(itemId);
        if(itemAmount <= 0)
        {
            if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
            {
                if(ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_BOW)
                {
                    SPAO(playerid, GetItemModel(itemId), true);
                }
                else
                {
                    GivePlayerWeapon(playerid, nativeWeaponId, _:FLOAT_INFINITY);
                }
            }
        }
        else
        {
            SCMF(playerid, -1, "I had some ammo for that item.");
            PlayerWeaponBodyDamage[playerid] = eItemsData[itemId][e_iItem_BodyDmg];
			PlayerWeaponHeadDamage[playerid] = eItemsData[itemId][e_iItem_HeadDmg];

            switch(GetItemType(itemId))
            {
                case ITEM_TYPE_MELEE:
                {
                    SCMF(playerid, -1, "It's a MELEE item.");
                    GivePlayerWeapon(playerid, nativeWeaponId, 1);
                }

                case ITEM_TYPE_WEAPON:
                {
                    SCMF(playerid, -1, "It's a WEAPON item.");
                    if(ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_BOW)
                    {
                        SPAO(playerid, GetItemModel(itemId), true);
                    }
                    else
                    {
                        SCMF(playerid, -1, "Equipping bullet weapon");
                        GivePlayerWeapon(playerid, nativeWeaponId, _:FLOAT_INFINITY);
                        SetPlayerNoReload(playerid, (nativeWeaponId != WEAPON_SHOTGUN));

                        if(GetPlayerItemClipAmount(playerid, itemId) == -1)
                        {
                            new maxItemClipSizeAmount = GetPlayerItemMaxClipSize(playerid, itemId);
                            ePlayerItemData[playerid][itemId][e_iItemClipAmount] = (itemAmount < maxItemClipSizeAmount) ? (itemAmount) : (maxItemClipSizeAmount);

                            SCMF(playerid, -1, "GetPlayerItemClipAmount == -1. maxItemClipSizeAmount = %d. Result: %d", maxItemClipSizeAmount, ePlayerItemData[playerid][itemId][e_iItemClipAmount]);
                        }
                    }
                }
            
                case ITEM_TYPE_PROJECTILE, ITEM_TYPE_THROWABLE:
                {
                    SCMF(playerid, -1, "It's either PROJECTILE or THROWABLE");
                    SPAO(playerid, (nativeWeaponId == WEAPON_MEDKIT) ? (ITEM_MODEL_BINDING) : GetItemModel(itemId));
                }
            }
        }

        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = itemId;

        OnPlayerWeaponChange(playerid, itemId, ePlayerInfo[playerid][e_iLastEquippedItemId]);
        return true;
    }

    HideItem(playerid)
    {
        SCMF(playerid, -1, "HideItem");
        if(GetPlayerEquippedItemId(playerid) == -1)
        {
            SCMF(playerid, -1, "No weapon to hide.");
            return false;
        }

        ePlayerInfo[playerid][e_iLastEquippedItemId] = GetPlayerEquippedItemId(playerid);
        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = -1;

        SCMF(playerid, -1, "Saved as lastweap: %d.", ePlayerInfo[playerid][e_iLastEquippedItemId]);
        
        PlayerWeaponBodyDamage[playerid] = DAMAGE_VALUE_FISTS;
		PlayerWeaponHeadDamage[playerid] = DAMAGE_VALUE_FISTS;

        ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

        OnPlayerWeaponChange(playerid, -1, ePlayerInfo[playerid][e_iLastEquippedItemId]);
        return true;
    }

//#endregion

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

CMD:giveitem(playerid, params[])
{
    new itemId, amount, meleeExtraCND = 0;
    if(sscanf(params, "dd", itemId, amount))return SCM(playerid, -1, "/giveitem [itemId] [amount]");
    {
        GivePlayerItem(playerid, itemId);

        if(!SetPlayerItemAmount(playerid, itemId, amount, meleeExtraCND))
        {
            SCMF(playerid, -1, "Amount failure!");
        }
    }
    return 1;
}

CMD:myitems(playerid, params[])
{
    SCM(playerid, -1, "My items:");

    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        if(!HasPlayerGotItem(playerid, i))
        {
            continue;
        }

        SCMF(playerid, -1, "Item Name: %s | Item Idx: %d | Amount: %d | Max amount: %d | (Melee) MaxExtraCND: %d | Upgrade (clipsize) level: %d", eItemsData[i][e_sItem_Name], i, GetPlayerItemAmount(playerid, i), GetPlayerItemMaxAmount(playerid, i), GetPlayerItemMaxAmount(playerid, i, true), GetPlayerItemUpgradeLvl(playerid, i, UPGRADE_TYPE_CLIPSIZE));
    }
    return 1;
}

CMD:setlvl(playerid, params[])
{
    new itemId, level;
    if(sscanf(params, "dd", itemId, level))return SCM(playerid, -1, "/setlvl [itemId] [level]");
    {
        if(!SetPlayerItemUpgradeLvl(playerid, itemId, level, UPGRADE_TYPE_CLIPSIZE))
        {
            SCMF(playerid, -1, "Something went wrong!");
        }
    }
    return 1;
}

CMD:equipitem(playerid, params[])
{
    new itemId;
    if(sscanf(params, "d", itemId))return SCM(playerid, -1, "/equipitem [item_index] (/myitems)");
    {
        EquipItem(playerid, itemId);
    }
    return 1;
}

CMD:equipped(playerid, params[])
{
    SCMF(playerid, -1, "Currently equipped: %d", GetPlayerEquippedItemId(playerid));
    return 1;
}

CMD:removeitem(playerid, params[])
{
    new itemId;
    if(sscanf(params, "d", itemId))return SCM(playerid, -1, "/removeitem [item_index] (/myitems)");
    {
        RemovePlayerItem(playerid, itemId);
    }
    return 1;
}