//#region Item Create/Update/Destroy/Validate

    // Create an item.
    CreateOrUpdateItem(itemId, itemAmount, meleeExtCnd = 0, creatorId = INVALID_PLAYER_ID, \
                itemTitle[] = "", innerMsg[] = "", Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, virtualWorld = 0, interiorId = 0, bool:update = false, updateItemUid = -1)
    {
        // Shorty crashes when shot (unable to create item).
        if(itemId == 12)
        {
            return 1;
        }

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

    // Find item's index in the table by its weapon id.
    FindItemIdxByWeaponId(weaponid)
    {
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(weaponid == eItemsData[i][e_iItem_SA_WeaponId])
            {
                return i;
            }
        }
        return -1;
    }

    // Validates item's creation positions, amounts and conditions.
    ValidateItemCreation(itemId, &itemAmount, &meleeExtCnd, &Float:z, &Float:rX, &Float:rY, &Float:rZ)
    {
        switch(itemId)
        {
            // Ingredients.
            case 0..5:
            {
                // Validate if the specified value is only every 25.
                if((itemAmount % 25) != 0 || itemAmount <= 0 || itemAmount > 300)
                {
                    return 1;
                }
                rZ = float(random(360));
            }

            // Melee weapons.
            case 6..9:
            {
                new weaponId = eItemsData[itemId][e_iItem_SA_WeaponId];
                if(meleeExtCnd != 0)
                {
                    new maxExtCnd = GetMeleeWeapon_Durability(weaponId, true);

                    if(meleeExtCnd > maxExtCnd)
                        meleeExtCnd = maxExtCnd;
                }

                new meleeBasicCnd = GetMeleeWeapon_Durability(weaponId, false);

                if(itemAmount > meleeBasicCnd)
                    itemAmount = meleeBasicCnd;

                rX = 90.0;
                rY = float(random(360));
            }

            // Weapons.
            case 10..15:
            {
                rX = 90.0;
                rY = float(random(360));
            }

            // Bow, Shiv, Medkit.
            case 16, 17, 18:
            {
                z += 0.06;
                rZ = float(random(360));
            }

            // Molotov Cocktail, Nail bomb, Smoke bomb.
            case 19, 20, 21:
            {
                switch(random(2))
                {
                    case 0:rY = -15;
                    case 1:rX = 90, z += 0.02;
                }
                rZ = float(random(360));
            }

            // Bottle.
            case 23:
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

//#region Utils

    // Check if player is full on the specified ingredient.
    IsPlayerFullOnIngr(playerid, ingrId) return Craft_IngrAmount(playerid, ingrId) >= 300;

    // Check if player has a melee weapon (any or a specific one).
    bool:HasPlayerGotMeleeWeapon(playerid, meleeId = NO_MELEE)
    {
        new playerMelee = GetPlayerMelee[playerid];

        if(meleeId == NO_MELEE)
            return (playerMelee != NO_MELEE && GetPlayerMeleeCnd[playerid] > 0);
        else
            return (playerMelee == meleeId && GetPlayerMeleeCnd[playerid] > 0);
    }

    // Check if player has a throwable item (any or a specific one).
    bool:HasPlayerGotThrowableItem(playerid, throwableId = NO_THROWABLE)
    {
        new playerThrowable = GetPlayerThrowable[playerid];

        if(throwableId == NO_THROWABLE)
            return (playerThrowable != NO_THROWABLE && GetPlayerWeaponAmmo(playerid, playerThrowable) > 0);
        else
            return (playerThrowable == throwableId && GetPlayerWeaponAmmo(playerid, playerThrowable) > 0);
    }

    // Show player item label of the objectid.
    ShowPlayerItemLabel(playerid, item_objectId)
    {
        new itemUid, itemId, Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId, itemAmount, itemExtAmount, bool:itemCreated, itemModel, itemType, itemWeaponId;
        ReadItemRelations(item_objectId, itemUid, itemId, itemPosX, itemPosY, itemPosZ, itemVw, itemIntId, itemAmount, itemExtAmount, itemCreated, itemModel, itemType, itemWeaponId);

        if(!itemCreated)
        {
            return;
        }

        if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
        {
            DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
        }

        new labelStr[32];
        switch(itemType)
        {
            case ITEM_TYPE_HEAL:        format(labelStr, sizeof(labelStr), "%s%s", eItemsData[itemId][e_sItem_Name], (GetPlayerWeaponAmmo(playerid, itemWeaponId) >= MAX_AMOUNT_MEDKIT ? "\n{FF0000}(FULL)" : ""));
            case ITEM_TYPE_THROWABLE:   format(labelStr, sizeof(labelStr), "%s%s", eItemsData[itemId][e_sItem_Name], (HasPlayerGotThrowableItem(playerid, itemWeaponId) ? "\n{FF0000}(FULL)" : ""));
            case ITEM_TYPE_COLLECTIBLE: format(labelStr, sizeof(labelStr), "%s", eObjData[e_sObj_Title]);

            case ITEM_TYPE_MELEE:
            {
                new bool:isItemMeleeUpgraded = (itemExtAmount > 0);
                format(labelStr, sizeof(labelStr), "%s\n(CND: %d/%d)", eItemsData[itemId][e_sItem_Name], \
                        (isItemMeleeUpgraded) ? (itemAmount + itemExtAmount) : (itemAmount), \
                        (isItemMeleeUpgraded) ? GetMeleeWeapon_Durability(itemWeaponId, false) + GetMeleeWeapon_Durability(itemWeaponId, true) : GetMeleeWeapon_Durability(itemWeaponId, false));
            }

            case ITEM_TYPE_INGREDIENT:
            {
                // itemId = ingredientId (this is the ONLY case!)
                if(IsPlayerFullOnIngr(playerid, itemId))
                {
                    format(labelStr, sizeof(labelStr), "%s\n{FF0000}(FULL)", eItemsData[itemId][e_sItem_Name]);
                }
                else 
                {
                    format(labelStr, sizeof(labelStr), "%s\n(%0.2f)", eItemsData[itemId][e_sItem_Name], (float(itemAmount) / 100));
                }
            }

            case ITEM_TYPE_WEAPON:
            {
                if(IsPlayerWeaponFullOnAmmo(playerid, itemWeaponId))
                {
                    format(labelStr, sizeof(labelStr), "%s\n{FF0000}(FULL)", eItemsData[itemId][e_sItem_Name]);
                }
                else
                {
                    switch(itemModel)
                    {
                        case 
                            ITEM_MODEL_SHIV,
                            ITEM_MODEL_BOMB,
                            ITEM_MODEL_MOLOTOV,
                            ITEM_MODEL_SMKBMB,
                            ITEM_MODEL_ARROW:
                            {
                                format(labelStr, sizeof(labelStr), "%s", eItemsData[itemId][e_sItem_Name]);
                            }
                        
                        default:
                        {
                            format(labelStr, sizeof(labelStr), "%s\n(ammo: %d)", eItemsData[itemId][e_sItem_Name], floatround(itemAmount));
                        }
                    }
                }
            }
        }

        ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel(labelStr, COLOR_WHITE_TRANS, itemPosX, itemPosY, itemPosZ, ITEM_LABEL_DRAW_DIST, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, itemVw, itemIntId, playerid);
        Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
    }

//#endregion

// Set player ingredient amount to dest. value.
SetPlayerIngrAmount(playerid, ingrId, amount)
{
    // Validate if the specified value is only every 25.
    if((amount % 25) != 0)
        return 1;

    new calcAmount = ((amount > 300) ? (300) : (amount < 0) ? (0) : (amount));
    Craft_IngrAmount(playerid, ingrId) = calcAmount;
    
    // Update UI textdraws if player using them.
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
    {
        new selectedCraft = Craft_GetSelection(playerid);
        CraftTd_CheckWhatsToCraft(playerid);
        CraftTd_UpdateViewingItem(playerid, selectedCraft);
        CraftTd_HighlightSelection(playerid, selectedCraft);
        CraftTd_UpdateIngr(playerid, ingrId);
    }
    else if(IsPlayerUsingWeaponMenu(playerid))
    {
        CraftTd_CheckWhatsToCraft(playerid);
    }

    UpdateNavItemsColors(playerid);
    DB_UpdatePlayerItemAmount(ingrId, calcAmount, .ownerUid = eCharsInfo[playerid][e_iUid]);
    return 0;
}

// Set player's item amount.
SetPlayerItemAmount(playerid, itemId, amount, meleeUpgradedCnd = 0)
{
    // Ingredients.
    if(0 <= itemId <= 5)
    {
        SetPlayerIngrAmount(playerid, itemId, amount);
        return 0;
    }

    new 
        calcAmount = 0, 
        weaponId = eItemsData[itemId][e_iItem_SA_WeaponId],
        maxWeaponAmmo = GetPlayerWeaponLvl_MaxAmmo(playerid, weaponId),
        maxItemAmount = IsMeleeWeapon(weaponId) ? GetMeleeWeapon_Durability(weaponId, false) : maxWeaponAmmo;

    if(maxWeaponAmmo == 0)
        return 1;

    // Melee weapons.
    if(6 <= itemId <= 9)
    {
        if(meleeUpgradedCnd > 0)
        {
            IsPlayerMeleeUpgraded[playerid] = true;

            new maxMeleeExtCnd = GetMeleeWeapon_Durability(weaponId, true);
            GetPlayerMeleeExtCnd[playerid] = (meleeUpgradedCnd > maxMeleeExtCnd) ? (maxMeleeExtCnd) : (meleeUpgradedCnd);
        }

        // If trying to give melee weapon and player 
        // already had a melee weapon - remove ownership of it from DB.
        if(HasPlayerGotMeleeWeapon(playerid))
        {
            new playerMeleeItemIdx = FindItemIdxByWeaponId(GetPlayerMelee[playerid]);
            if(playerMeleeItemIdx != -1)
            {
                DB_RemovePlayerItem(playerid, playerMeleeItemIdx);
            }
        }
    }

    // Validate item amount for player to be given.
    calcAmount = (amount > maxItemAmount) ? (maxItemAmount) : (amount < 0) ? (0) : (amount);

    // Update database info & give player item.
    if(!HasPlayerWeapon(playerid, weaponId))
    {
        GivePlayerWeaponInSlot(playerid, weaponId);
        DB_GivePlayerItem(itemId, calcAmount, (IsMeleeWeapon(weaponId) ? GetPlayerMeleeExtCnd[playerid] : 0), eCharsInfo[playerid][e_iUid]);
    }

    // Give player specific amount for the item.
    SetPlayerWeaponAmmo(playerid, weaponId, calcAmount);
    DB_UpdatePlayerItemAmount(itemId, calcAmount, .ownerUid = eCharsInfo[playerid][e_iUid]);

    // Update player UI if using it.
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
    {
        if(IsMeleeWeapon(weaponId)) CraftTd_UpdateViewingItem(playerid, CRAFT_ITEM_MELEE);
        else if(!IsBulletWeapon(weaponId)) CraftTd_UpdateItemAmount(playerid, eItemsData[itemId][e_iItem_CraftItemId], calcAmount); // Need to add a validation for nailbomb (projetiles weapons???)
    }
    else
    {
        if(IsMeleeWeapon(weaponId) || weaponId == WEAPON_SHIV) ShowHud(playerid);
        else if(IsPlayerUsingWeaponMenu(playerid))
        {
            WeapMenuTd_DisplayWeapsAndAmmo(playerid);
            CraftTd_CheckWhatsToCraft(playerid);
        }
    }
    return 0;
}

// Set player's supplements amount.
SetPlayerSupplements(playerid, amount)
{
    eCharsInfo[playerid][e_iSkills_SuppsAmount] = ((amount < 0) ? (0) : (amount > MAX_PLAYER_SUPPS_AMOUNT) ? (MAX_PLAYER_SUPPS_AMOUNT) : (amount));

    // Update UI.
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        SkillsTd_CheckSuppsAmount(playerid);
        SkillsTd_CheckWhatsToUpgrade(playerid);
        SkillTd_UpdateViewingItem(playerid, Skill_GetSelection(playerid));
    }
    UpdateNavItemsColors(playerid);
}