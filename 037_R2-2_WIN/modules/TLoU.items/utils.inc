//#region Validation

    IsValidItemId(itemId)
    {
        return (itemId >= 0 && itemId < MAX_ITEMS_DATA);
    }

    ITEM_TYPE:GetItemType(itemId)
    {
        if(!IsValidItemId(itemId))
        {
            return ITEM_TYPE:-1;
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

        if(eItemsData[itemId][e_iItem_MaxAmount] == -1 && (IsItemUpgradeable(itemId) || IsItemType(itemId, ITEM_TYPE_MELEE)))
        {
            result = (GetPlayerItemAmount(playerid, itemId) >= GetPlayerItemMaxAmount(playerid, itemId));
        }
        else
        {
            result = (GetPlayerItemAmount(playerid, itemId) >= eItemsData[itemId][e_iItem_MaxAmount]);
        }
        return result;
    }

    IsItemType(itemId, ITEM_TYPE:typeId)
    {
        switch(typeId)
        {
            case ITEM_TYPE_BULLET_WEAPON: return (GetItemType(itemId) == ITEM_TYPE:ITEM_TYPE_WEAPON && ITEM_IDX:itemId != ITEM_IDX:ITEM_IDX_BOW);
            case ITEM_TYPE_MEDKIT:        return (GetItemType(itemId) == ITEM_TYPE:ITEM_TYPE_HEAL && ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_MEDKIT);
            case ITEM_TYPE_BOW:           return (GetItemType(itemId) == ITEM_TYPE:ITEM_TYPE_WEAPON && ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_BOW);
        }
        return (GetItemType(itemId) == typeId);
    }

    bool:HasPlayerGotThrowableItem(playerid)
    {
        return HasPlayerGotItem(playerid, _:ITEM_IDX_BRICK) || HasPlayerGotItem(playerid, _:ITEM_IDX_BOTTLE);
    }

    ValidateItemCreation(itemId, &itemAmount, &meleeExtCnd, &Float:z, &Float:rX, &Float:rY, &Float:rZ)
    {
        switch(GetItemType(itemId))
        {
            // Ingredients.
            case ITEM_TYPE_INGREDIENT:
            {
                // Validate if the specified value is only every 25.
                if((itemAmount % 25) != 0 || itemAmount <= 0 || itemAmount > 300)
                {
                    return 1;
                }
                rZ = float(random(360));
            }

            // Melee weapons.
            case ITEM_TYPE_MELEE:
            {
                if(meleeExtCnd > 0)
                {
                    meleeExtCnd = GetPlayerItemMaxAmount(INVALID_PLAYER_ID, itemId, true);
                }

                itemAmount = GetPlayerItemMaxAmount(INVALID_PLAYER_ID, itemId);

                rX = 90.0;
                rY = float(random(360));
            }

            // Weapons.
            case ITEM_TYPE_BULLET_WEAPON:
            {
                rX = 90.0;
                rY = float(random(360));
            }

            // Bow, Shiv, Medkit.
            case ITEM_TYPE_BOW, ITEM_TYPE_SHIV, ITEM_TYPE_MEDKIT:
            {
                z += 0.06;
                rZ = float(random(360));
            }

            // Molotov Cocktail, Nail bomb, Smoke bomb.
            case ITEM_TYPE_PROJECTILE:
            {
                switch(random(2))
                {
                    case 0:rY = -15;
                    case 1:rX = 90, z += 0.02;
                }
                rZ = float(random(360));
            }

            // Bottle.
            case 23://ITEM_TYPE_THROWABLE:
            {
                switch(random(2))
                {
                    case 0: z += 0.1783;
                    case 1: rX = 90.0, rY = float(random(360)), rZ = float(random(360)), z += 0.04;
                }
            }
        }
        return 0;
    }

//#endregion

//#region Item Create/Update/Destroy

    // Create an item.
    CreateOrUpdateItem(itemId, itemAmount, meleeExtCnd = 0, creatorId = INVALID_PLAYER_ID, \
                itemTitle[] = "", innerMsg[] = "", Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, virtualWorld = 0, interiorId = 0, bool:update = false, updateItemUid = -1)
    {
        // Unable to create item if item amount is invalid.
        if(itemAmount <= 0 || meleeExtCnd < 0)
        {
            return 1;
        }

        // Create item at position where the player is if creatorId is specified 
        // (and it isn't an update of the item).
        if(creatorId != INVALID_PLAYER_ID && update == false)
        {
            GetPlayerPos(creatorId, x, y, z);
            CA_FindZ_For2DCoord(x, y, z);

            virtualWorld = GetPlayerVirtualWorld(creatorId);
            interiorId = GetPlayerInterior(creatorId);
        }

        // Validation position & rotation of the item.
        new Float:rX, Float:rY, Float:rZ;
        if(ValidateItemCreation(itemId, itemAmount, meleeExtCnd, z, rX, rY, rZ) == 1)
        {
            return 1;
        }

        if(update)
        {
            DB_UpdateExistingItem(updateItemUid, itemId, itemAmount, meleeExtCnd, x, y, z, rX, rY, rZ, virtualWorld, interiorId, creatorId);
        }
        else
        {
            DB_CommitNewItem(itemId, x, y, z, rX, rY, rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle, innerMsg, creatorId);
        }
        return 0;
    }

    // Destroying item (object) and clear its data.
    DestroyItem(item_objectId, bool:update = false)
    {
        if(!IsValidDynamicObject(item_objectId))
        {
            return;
        }

        if(!update)
        {
            OnItemDestroy(eObjData[e_iObj_Uid]);
        }

        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Uid]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_ItemId]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_X]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_Y]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_Z]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Vw]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Int]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_ItemAmount]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_MeleeUpgradedCnd]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_sObj_Title]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_sObj_InnerMessage]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_bObj_Created]);

        DestroyDynamicObject(item_objectId);
    }

//#endregion

//#region Items | Give/Remove

    // Gives player a specified item.
    // Returns:
    // false - if player has specified item already.
    // true - if item has been given correctly
    bool:GivePlayerItem(playerid, itemId)
    {
        if(HasPlayerGotItem(playerid, itemId) || IsItemType(itemId, ITEM_TYPE_MELEE) && GetPlayerMeleeItemId(playerid) != -1)
        {
            return false;
        }

        // Update database info & give player item.
        // DB_GivePlayerItem(itemId, calcAmount, (IsMeleeWeapon(weaponId) ? GetPlayerMeleeExtCnd[playerid] : 0), eCharsInfo[playerid][e_iUid]);

        if(IsItemType(itemId, ITEM_TYPE_WEAPON))
        {
            new freeSlotId = WeapMenu_AssignItemToFreeSlot(playerid, itemId);
            if(freeSlotId == -1)
            {
                Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "GivePlayerItem: charUid: %d couldn't find a free slot for the weapon in the weapon menu.", GetPlayerCharUid(playerid), itemId);
                return false;
            }
        }

        ePlayerItemData[playerid][itemId][e_iItemId] = itemId;
        UpdateTextDrawsIfNecessary(playerid, itemId);

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "GivePlayerItem: charUid: %d was given itemId: %d", GetPlayerCharUid(playerid), itemId);
        return true;
    }

    // Removes item from player (doesn't matter if equipped or not).
    // Returns:
    // false - if player didn't have that item.
    // true - if item has been removed correctly.
    bool:RemovePlayerItem(playerid, itemId)
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
        ePlayerItemData[playerid][itemId][e_iItemAmount] = 0;

        if(IsItemType(itemId, ITEM_TYPE_MELEE))
		{
            ePlayerItemData[playerid][itemId][e_iItemMeleeExtraCND] = 0;
			HudTd_UpdateMelee(playerid);
		}

		if(ITEM_IDX:itemId == ITEM_IDX_SHIV)
		{
			HudTd_UpdateShivs(playerid);
		}

		DetachWeaponFromPlayersBody(playerid, itemId);
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "RemovePlayerItem: charUid: %d got his itemId: %d removed", GetPlayerCharUid(playerid), itemId);
        return true;
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
    // (results are possible to be returned from 2 tables - eItemsData | eItemsUpgrades)
    GetPlayerItemMaxClipSize(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId) || !IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON))
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

    // Gets player's current clip amount of the item specified
    // Returns:
    // (-1) if player hasn't got an item OR item is not a bullet type weapon
    // (x) current clip amount
    GetPlayerItemClipAmount(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId) || !IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON))
        {
            return -1;
        }

        return ePlayerItemData[playerid][itemId][e_iItemClipAmount];
    }

    // Gets player item's amount of specified itemId.
    // Returns:
    // (0) if player hasn't got an item or is invalid.
    // (x) value of the amount that player has for that item (integer)
    GetPlayerItemAmount(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return 0;
        }

        return ePlayerItemData[playerid][itemId][e_iItemAmount];
    }

    // Checks if player's melee weapon is upgraded.
    // Returns:
    // false - if player hasn't got a melee item
    // true if player has a melee and its extra CND is higher that 0
    bool:IsPlayerMeleeUpgraded(playerid)
    {
        new playerMeleeItemId = GetPlayerMeleeItemId(playerid);
        if(playerMeleeItemId == -1)
        {
            return false;
        }

        return (GetPlayerMeleeExtraCnd(playerid, playerMeleeItemId) > 0);
    }

    // Gets player's melee weapon extra CND (upgraded)
    // Returns:
    // (-1) if player hasn't got an item or specified itemId isn't a melee weapon.
    // (x) number (int) of melee's upgraded condition
    GetPlayerMeleeExtraCnd(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId) && !IsPlayerMeleeUpgraded(playerid) && !IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            return -1;
        }

        return ePlayerItemData[playerid][itemId][e_iItemMeleeExtraCND];
    }

    // Gets maximal amount of an item type that's possible to get for player.
    // Returns:
    // (-1) if system didn't find max item values in eItemsUpgrade or eItemsMeleeCND
    // (0) if system tried to check for melee extra CND on item which is not a melee weapon
    // (x) maximal value of an item 
    // (results are possible to be returned from 3 tables - eItemsData | eItemsUpgrades | eItemsMeleeCND)
    GetPlayerItemMaxAmount(playerid, itemId, bool:checkMeleeExtraCND = false)
    {
        // if(!HasPlayerGotItem(playerid, itemId))
        // {
        //     return 0;
        // }

        new maxAmount = 0;
        if(checkMeleeExtraCND == true)
        {
            if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MELEE))
            {
                return 0;
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
                if(IsItemType(itemId, ITEM_TYPE_WEAPON))
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
                else if(IsItemType(itemId, ITEM_TYPE_MELEE))
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

    // Gets player's melee item id (MELEE)
    // Returns:
    // (-1) if no melee weapon was found
    // (x) item id.
    GetPlayerMeleeItemId(playerid)
    {
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(!IsItemType(i, ITEM_TYPE_MELEE))
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

    // Gets player throwable item id
    // Returns:
    // (-1) if no throwable item found
    // (x) item id
    GetPlayerThrowableItemId(playerid)
    {
        if(HasPlayerGotItem(playerid, _:ITEM_IDX_BRICK))
        {
            return _:ITEM_IDX_BRICK;
        }
        else if(HasPlayerGotItem(playerid, _:ITEM_IDX_BOTTLE))
        {
            return _:ITEM_IDX_BOTTLE;
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
            if(!IsItemType(i, ITEM_TYPE_WEAPON) || !HasPlayerGotItem(playerid, i) || GetPlayerItemAmount(playerid, i) <= 0)
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

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SetPlayerItemUpgradeLvl: set upgrade level to: %d of upgrade type: %d for item id: %d (charUid: %d)", level, upgradeType, itemId, GetPlayerCharUid(playerid));
        return true;
    }

    // ! DO NOT USE MANUALLY ANYWHERE ! this function is used internally in SetPlayerItemAmount !
    // ( IF CND set to 0 an item is going to be removed from player!)
    // Returns:
    // false - if player hasn't got an item OR item isn't a melee weapon
    // true - if melee's weapon CND set correctly
    bool:SetPlayerMeleeWeaponCND(playerid, itemId, basicCnd, extraCnd = 0)
    {
        if(!HasPlayerGotItem(playerid, itemId) || !IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            return false;
        }

        if(basicCnd > 0)
        {
            new maxBasicCnd = GetPlayerItemMaxAmount(playerid, itemId);
            ePlayerItemData[playerid][itemId][e_iItemAmount] = (basicCnd > maxBasicCnd) ? (maxBasicCnd) : (basicCnd);
        }
        else
        {
            ePlayerItemData[playerid][itemId][e_iItemAmount] = 0;
        }

        if(extraCnd > 0)
        {
            new maxExtraCnd = GetPlayerItemMaxAmount(playerid, itemId, true);
            ePlayerItemData[playerid][itemId][e_iItemMeleeExtraCND] = (extraCnd > maxExtraCnd) ? (maxExtraCnd) : (extraCnd);
        }
        else
        {
            ePlayerItemData[playerid][itemId][e_iItemMeleeExtraCND] = 0;
        }
        return true;
    }

    // Set player item's amount
    // Returns:
    // false - if player hasn't got an item OR player is full on that item already OR amount for ingredient is not specified correctly (every 25) OR can't find MaxAmount for the item!
    // true - if player item's amount has been set correctly
    SetPlayerItemAmount(playerid, itemId, amount, meleeExtraCnd = 0)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        new maxItemAmount = GetPlayerItemMaxAmount(playerid, itemId);
        if(maxItemAmount == -1)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], ERROR, "SetPlayerItemAmount: received invalid max item amount (-1). ItemId: %d", itemId);
            return false;
        }

        if(!IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            if(IsPlayerFullOnItem(playerid, itemId) && amount >= maxItemAmount)
            {
                return false;
            }
        }

        new bool:success = true;
        if(IsItemType(itemId, ITEM_TYPE_INGREDIENT))
        {
            success = ((amount % 25) == 0);
        }

        if(IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            success = SetPlayerMeleeWeaponCND(playerid, itemId, amount, meleeExtraCnd);
        }

        if(!success)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], ERROR, "SetPlayerItemAmount: Something went wrong. ItemId: %d | amount: %d | maxItemAmount: %d", itemId, amount, maxItemAmount);
            return false;
        }

        if(!IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            ePlayerItemData[playerid][itemId][e_iItemAmount] = (amount > maxItemAmount) ? (maxItemAmount) : (amount);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SetPlayerItemAmount: set item amount to: %d (extraCnd = %d) of item id: %d for charUid: %d", ePlayerItemData[playerid][itemId][e_iItemAmount], meleeExtraCnd, itemId, GetPlayerCharUid(playerid));
        // DB_UpdatePlayerItemAmount(itemId, calcAmount, .ownerUid = eCharsInfo[playerid][e_iUid]);

        UpdateTextDrawsIfNecessary(playerid, itemId);
        return true;
    }

    UpdateTextDrawsIfNecessary(playerid, itemId)
    {
        if(IsPlayerUsingWeaponMenu(playerid)
        && (IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_WEAPON) || IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_PROJECTILE)
        ||  IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_THROWABLE) || IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MEDKIT) || IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_INGREDIENT)))
        {
            WeapMenuTd_DisplayWeapsAndAmmo(playerid, itemId);
            CraftTd_CheckWhatsToCraft(playerid);
        }

        if(GetPlayerEquippedItemId(playerid) == itemId)
        {
            HudTd_UpdateWeaponAmmo(playerid, itemId);
            HudTd_DisplayMeleeWeaponBars(playerid, itemId);
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
                    CraftTd_DisplayIngredients(playerid, itemId);
                }
                
                UpdateNavItemsColors(playerid);
            }

            case ITEM_TYPE_MELEE, ITEM_TYPE_SHIV:
            {
                if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
                {
                    if(ITEM_IDX:itemId == ITEM_IDX_SHIV)
                    {
                        CraftTd_UpdateItemAmount(playerid, itemId);
                    }
                    else
                    {
                        CraftTd_UpdateViewingItem(playerid, CRAFT_ITEM_MELEE);
                    }
                }
                else
                {
                    ShowHud(playerid);
                }
            }

            default:
            {
                if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
                {
                    CraftTd_UpdateItemAmount(playerid, itemId);
                }
            }
        }
    }

//#endregion

//#region Items | Equipping/Hiding

    // Equips player with an item.
    // Returns:
    // false - if player hasn't got an item OR is unable to equip at the time OR is using backpack anims OR is allowed to shiv OR itemAmount is 0 and it's not a WEAPON
    // true - if item equipped correctly
    function bool:EquipItem(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId)
        || GetPlayerEquippedItemId(playerid) == itemId
        || IsPlayerUnableToPerformAction(playerid)
        || IsPlayerUsingBackpackAnims(playerid))
        {
            SCMF(playerid, -1, "Stopped at 1st validation");
            return false;
        }

        // If player is allowed to shiv, do not equip melee weapon
        // and hide player weapon.
        if(IsPlayerAllowedToShiv(playerid) && IsItemType(itemId, ITEM_TYPE_MELEE))
		{
            SCMF(playerid, -1, "Stopped at 2nd validation");
            HideItem(playerid);
            return false;
        }

        // Tries to equip last weapon but has some weapon equipped already (and not using weapon menu).
        if(GetPlayerEquippedItemId(playerid) != -1 
        && GetPlayerLastEquippedItemId(playerid) == itemId
        && !IsPlayerUsingWeaponMenu(playerid)
        && ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] == 0)
        {
            SCMF(playerid, -1, "Stopped at 3rd validation");
            return false;
        }

        new itemAmount = GetPlayerItemAmount(playerid, itemId);
        
        // Equipping weapons with no ammo/amount is allowed 
        // only for BOW and items of type WEAPON.
        if(itemAmount <= 0 && !IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_WEAPON))
        {
            SCMF(playerid, -1, "Stopped at 4th validation");
            return false;
        }

        // Hides player's currently equipped itemId if there are any.
        HideItem(playerid, true);

        new nativeWeaponId = GetItemNativeWeaponId(itemId);
        if(itemAmount <= 0)
        {
            if(IsItemType(itemId, ITEM_TYPE_WEAPON))
            {
                if(IsItemType(itemId, ITEM_TYPE_BOW))
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
            PlayerWeaponBodyDamage[playerid] = eItemsData[itemId][e_iItem_BodyDmg];
			PlayerWeaponHeadDamage[playerid] = eItemsData[itemId][e_iItem_HeadDmg];

            switch(GetItemType(itemId))
            {
                case ITEM_TYPE_MELEE:
                {
                    GivePlayerWeapon(playerid, nativeWeaponId, 1);
                }

                case ITEM_TYPE_WEAPON:
                {
                    if(ITEM_IDX:itemId == ITEM_IDX:ITEM_IDX_BOW)
                    {
                        SPAO(playerid, GetItemModel(itemId), true);
                    }
                    else
                    {
                        GivePlayerWeapon(playerid, nativeWeaponId, _:FLOAT_INFINITY);
                        SetPlayerNoReload(playerid, (nativeWeaponId != WEAPON_SHOTGUN));

                        if(GetPlayerItemClipAmount(playerid, itemId) == -1)
                        {
                            new maxItemClipSizeAmount = GetPlayerItemMaxClipSize(playerid, itemId);
                            ePlayerItemData[playerid][itemId][e_iItemClipAmount] = (itemAmount < maxItemClipSizeAmount) ? (itemAmount) : (maxItemClipSizeAmount);
                        }
                    }
                }
            
                case ITEM_TYPE_PROJECTILE, ITEM_TYPE_THROWABLE, ITEM_TYPE_HEAL:
                {
                    SPAO(playerid, (IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MEDKIT)) ? (ITEM_MODEL_BINDING) : GetItemModel(itemId));
                }
            }
        }

        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = itemId;

        OnPlayerWeaponChange(playerid, ePlayerInfo[playerid][e_iCurrentEquippedItemId], ePlayerInfo[playerid][e_iLastEquippedItemId]);
        return true;
    }

    // Hides player item.
    // Returns:
    // false - if player didn't have any item equipped.
    // true - if item hid correctly.
    HideItem(playerid, bool:equippingNewItem = false)
    {
        new playerEquippedItemId = GetPlayerEquippedItemId(playerid);
        if(playerEquippedItemId == -1)
        {
            return false;
        }

        if(IsItemType(playerEquippedItemId, ITEM_TYPE:ITEM_TYPE_WEAPON) || IsItemType(playerEquippedItemId, ITEM_TYPE:ITEM_TYPE_PROJECTILE))
        {
            ePlayerInfo[playerid][e_iLastEquippedItemId] = GetPlayerEquippedItemId(playerid);
        }

        if(equippingNewItem == true)
        {
            return true;
        }

        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = -1;
        
        PlayerWeaponBodyDamage[playerid] = DAMAGE_VALUE_FISTS;
		PlayerWeaponHeadDamage[playerid] = DAMAGE_VALUE_FISTS;

        ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

        OnPlayerWeaponChange(playerid, -1, playerEquippedItemId);
        return true;
    }

//#endregion

// Show player item label of the objectid.
ShowPlayerItemLabel(playerid, item_objectId)
{
    new itemUid, itemId, Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId, itemAmount, itemExtAmount, bool:itemCreated;
    ReadItemRelations(item_objectId, itemUid, itemId, itemPosX, itemPosY, itemPosZ, itemVw, itemIntId, itemAmount, itemExtAmount, itemCreated);

    if(!itemCreated)
    {
        return;
    }

    if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
    {
        DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
    }

    new labelStr[32];
    if(IsPlayerFullOnItem(playerid, itemId))
    {
        format(labelStr, sizeof(labelStr), "%s\n{FF0000}(FULL)", eItemsData[itemId][e_sItem_Name]);

        ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel(labelStr, COLOR_WHITE_TRANS, itemPosX, itemPosY, itemPosZ, ITEM_LABEL_DRAW_DIST, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, itemVw, itemIntId, playerid);
        Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
        return true;
    }

    new labelStr[32];
    switch(GetItemType(itemId))
    {
        case ITEM_TYPE_MELEE:
        {
            new bool:isItemMeleeUpgraded = (itemExtAmount > 0);

            format(labelStr, sizeof(labelStr), "%s\n(CND: %d/%d)", eItemsData[itemId][e_sItem_Name], \
                    (isItemMeleeUpgraded) ? (itemAmount + itemExtAmount) : (itemAmount), \
                    (isItemMeleeUpgraded) ? GetPlayerItemMaxAmount(INVALID_PLAYER_ID, itemId) + GetPlayerItemMaxAmount(INVALID_PLAYER_ID, itemId, true) : GetPlayerItemMaxAmount(INVALID_PLAYER_ID, itemId));
        }

        case ITEM_TYPE_INGREDIENT:  format(labelStr, sizeof(labelStr), "%s\n(%0.2f)", eItemsData[itemId][e_sItem_Name], (float(itemAmount) / 100));
        case ITEM_TYPE_WEAPON:      format(labelStr, sizeof(labelStr), "%s\n(ammo: %d)", eItemsData[itemId][e_sItem_Name], floatround(itemAmount));
        default:                    format(labelStr, sizeof(labelStr), "%s", eItemsData[itemId][e_sItem_Name]);
    }

    ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel(labelStr, COLOR_WHITE_TRANS, itemPosX, itemPosY, itemPosZ, ITEM_LABEL_DRAW_DIST, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, itemVw, itemIntId, playerid);
    Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
}