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

    GetItemNativeWeaponId(itemId)
    {
        if(!IsValidItemId(itemId))
        {
            return -1;
        }

        return (eItemsData[itemId][e_iItem_SA_WeaponId]);
    }

    IsItemUpgradeable(itemId, upgradeType = -1)
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

    IsPlayerFullOnItem(playerid, itemId)
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

//#endregion

//#region Items | Get Values

    HasPlayerGotItem(playerid, itemId)      return (IsValidItemId(itemId) && ePlayerItemData[playerid][itemId][e_iItemId] != -1);
    GetPlayerEquippedItemId(playerid)       return (ePlayerInfo[playerid][e_iCurrentEquippedItemId]);
    GetPlayerLastEquippedItemId(playerid)   return (ePlayerInfo[playerid][e_iLastEquippedItemId]);

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
        return 0;
    }

    GetPlayerItemAmount(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return -1;
        }

        return ePlayerItemData[playerid][itemId][e_iItemAmount];
    }

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

//#endregion

//#region Items | Set Values

    bool:SetPlayerItemUpgradeLvl(playerid, itemId, level, upgradeType)
    {
        if(!IsItemUpgradeable(itemId, upgradeType) || !HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        // GetItemMaxUpgradeLvl(itemId, upgradeType)

        switch(upgradeType)
        {
            case UPGRADE_TYPE_CLIPSIZE: ePlayerItemData[playerid][itemId][e_iItemUpgradeLvl_ClipSize] = level;
        }
        return true;
    }

    bool:SetPlayerMeleeWeaponCND(playerid, itemId, basicCND, extraCND = 0)
    {
        if(!HasPlayerGotItem(playerid, itemId) || eItemsData[itemId][e_iItem_Type] != ITEM_TYPE_MELEE)
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

    SetPlayerItemAmountEx(playerid, itemId, amount, meleeExtraCND = 0)
    {
        if(!HasPlayerGotItem(playerid, itemId) || IsPlayerFullOnItem(playerid, itemId))
        {
            return false;
        }

        if(GetItemType(itemId) == ITEM_TYPE_INGREDIENT)
        {
            if((amount % 25) != 0)
            {
                return false;
            }
        }

        if(GetItemType(itemId) == ITEM_TYPE_MELEE)
        {
            return SetPlayerMeleeWeaponCND(playerid, itemId, amount, meleeExtraCND);
        }

        new maxItemAmount = GetPlayerItemMaxAmount(playerid, itemId);
        if(maxItemAmount == -1)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SetPlayerItemAmount: received invalid max item amount (-1). ItemId: %d", itemId);
            return false;
        }

        ePlayerItemData[playerid][itemId][e_iItemAmount] = (amount > maxItemAmount) ? (maxItemAmount) : (amount);

        if(GetPlayerEquippedItemId(playerid) == itemId)
        {
            //HudTd_UpdateWeaponAmmo(playerid, weaponId);
        }
        return true;
    }

//#endregion

//#region Items | Give/Remove

    GivePlayerItem(playerid, itemId)
    {
        if(HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

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
            ePlayerInfo[playerid][e_iLastEquippedItemId] = -1;
            // GetNextWeaponWithAmmo
        }

        // If it was an item (weapon) from horizontal weapon menu bar
        // below function is going to sort its order if necessary
        WeapMenu_RemoveAndSortItems(playerid, itemId);

        ePlayerItemData[playerid][itemId][e_iItemId] = -1;
        ePlayerItemData[playerid][itemId][e_iItemAmount] = -1;

        if(GetItemType(itemId) == ITEM_TYPE_MELEE)
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

    EquipItem(playerid, itemId)
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            return false;
        }

        if(GetPlayerEquippedItemId(playerid) != -1)
        {
            HideItem(playerid);
        }

        new itemAmount = GetPlayerItemAmount(playerid, itemId);
        new nativeWeaponId = GetItemNativeWeaponId(itemId);

        if(itemAmount <= 0)
        {
            if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
            {
                if(nativeWeaponId == WEAPON_BOW)
                {
                    SPAO(playerid, ITEM_MODEL_BOW, true);
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
                    GivePlayerWeapon(playerid, nativeWeaponId, 1);

                case ITEM_TYPE_WEAPON:
                {
                    GivePlayerWeapon(playerid, nativeWeaponId, _:FLOAT_INFINITY);
                    SetPlayerNoReload(playerid, (nativeWeaponId != WEAPON_SHOTGUN));

                    // We need some extra data here like clip ammo
                }
            
                case ITEM_TYPE_PROJECTILE, ITEM_TYPE_THROWABLE:
                {
                    SPAO(playerid, \
                        (nativeWeaponId == WEAPON_MEDKIT ? ITEM_MODEL_BINDING : GetItemModel(itemId)), \
                        (nativeWeaponId == WEAPON_BOW ? true : false));
                }
            }
        }

        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = itemId;
        return true;
    }

    HideItem(playerid)
    {
        if(GetPlayerEquippedItemId(playerid) == -1)
        {
            return false;
        }

        ePlayerInfo[playerid][e_iLastEquippedItemId] = GetPlayerEquippedItemId(playerid);
        ePlayerInfo[playerid][e_iCurrentEquippedItemId] = -1;

        PlayerWeaponBodyDamage[playerid] = DAMAGE_VALUE_FISTS;
		PlayerWeaponHeadDamage[playerid] = DAMAGE_VALUE_FISTS;

        ePlayerWeapon[playerid][e_iTickCounter_NoAmmoClicks] = 0;

        // OnPlayerWeaponChange(playerid, WEAPON_UNARMED, currentWeapon);

        return true;
    }

//#endregion

CMD:giveitem(playerid, params[])
{
    new itemId, amount, meleeExtraCND = 0;
    if(sscanf(params, "dd", itemId, amount))return SCM(playerid, -1, "/giveitem [itemId] [amount]");
    {
        GivePlayerItem(playerid, itemId);

        if(!SetPlayerItemAmountEx(playerid, itemId, amount, meleeExtraCND))
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