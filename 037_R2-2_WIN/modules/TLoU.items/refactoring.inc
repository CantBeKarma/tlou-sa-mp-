IsValidItemId(itemId)               return (itemId >= 0 && itemId < MAX_ITEMS_DATA);
HasPlayerGotItem(playerid, itemId)  return (IsValidItemId(itemId) && ePlayerItemData[playerid][itemId][e_iItemId] != -1);
GetItemType(itemId)                 return (IsValidItemId(itemId) && eItemsData[itemId][e_iItem_Type]);

GetPlayerItemAmount(playerid, itemId)
{
    if(!HasPlayerGotItem(playerid, itemId))
    {
        return -1;
    }

    return ePlayerItemData[playerid][itemId][e_iItemAmount];
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

bool:SetPlayerItemUpgradeLvl(playerid, itemId, level, upgradeType)
{
    if(!IsItemUpgradeable(itemId, upgradeType) || !HasPlayerGotItem(playerid, itemId))
    {
        return false;
    }

    switch(upgradeType)
    {
        case UPGRADE_TYPE_CLIPSIZE: ePlayerItemData[playerid][itemId][e_iItemUpgradeLvl_ClipSize] = level;
    }
    return true;
}

GetPlayerItemMaxAmount(playerid, itemId)
{
    if(!HasPlayerGotItem(playerid, itemId))
    {
        return -1;
    }

    new maxAmount = eItemsData[itemId][e_iItem_MaxAmount];
    if(maxAmount == -1 && GetItemType(itemId) == ITEM_TYPE_WEAPON)
    {
        new playerItemClipSizeLvl = GetPlayerItemUpgradeLvl(playerid, itemId, UPGRADE_TYPE_CLIPSIZE);
        
        for(new i = 0; i != sizeof(eItemsUpgrades); i++)
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
    return maxAmount;
}

IsPlayerFullOnItem(playerid, itemId)
{
    if(!HasPlayerGotItem(playerid, itemId))
    {
        return false;
    }

    new bool:result = false;

    if(eItemsData[itemId][e_iItem_MaxAmount] == -1 && IsItemUpgradeable(itemId))
    {
        result = (GetPlayerItemAmount(playerid, itemId) >= GetPlayerItemMaxAmount(playerid, itemId));
    }
    else
    {
        result = (GetPlayerItemAmount(playerid, itemId) >= eItemsData[itemId][e_iItem_MaxAmount]);
    }
    return result;
}

GivePlayerItem(playerid, itemId)
{
    if(HasPlayerGotItem(playerid, itemId))
    {
        return false;
    }

    switch(ITEM_IDX:itemId)
    {
        case ITEM_IDX_SHOTGUN, ITEM_IDX_RIFLE, ITEM_IDX_BOW, ITEM_IDX_ASSAULT_RIFLE:
        {
            new freeSlot = WeapMenuTd_GetFreeSlot_H_Ls(playerid);
            WeapMenu_Ls_WeaponId(playerid, freeSlot) = itemId;
        }

        case ITEM_IDX_9MM, ITEM_IDX_REVOLVER, ITEM_IDX_SHORTY:
        {
            new freeSlot = WeapMenuTd_GetFreeSlot_H_Rs(playerid);
            WeapMenu_Rs_WeaponId(playerid, freeSlot) = itemId;
        }
    }

    ePlayerItemData[playerid][itemId][e_iItemId] = itemId;
    return true;
}

SetPlayerItemAmountEx(playerid, itemId, amount)
{
    if(!HasPlayerGotItem(playerid, itemId) || IsPlayerFullOnItem(playerid, itemId))
    {
        return false;
    }

    if(_:ITEM_IDX_BLADE <= itemId <= _:ITEM_IDX_SUGAR)
    {
        if((amount % 25) != 0)
        {
            return false;
        }
    }

    new maxItemAmount = GetPlayerItemMaxAmount(playerid, itemId);
    if(amount > maxItemAmount)
    {
        amount = maxItemAmount;
    }

    ePlayerItemData[playerid][itemId][e_iItemAmount] = amount;
    return true;
}

// SetPlayerMeleeWeaponCND(playerid, itemId, cnd, upgradedCnd = 0)
// {
//     if(!HasPlayerGotItem(playerid, itemId) || eItemsData[itemId][e_iItem_Type] != ITEM_TYPE_MELEE)
//     {
//         return false;
//     }

//     if(cnd <= 0)
//     {
//         //RemoveWeapon
//     }
//     else
//     {
//         ePlayerItemData[playerid][itemId][e_iItemAmount] = amount;
//     }
    

//     if(upgradedCnd > 0)
//     {
//         ePlayerItemData[playerid][itemId][e_iItemUpgradedCND] = upgradedCnd;
//     }
//     return true;
// }

CMD:giveitem(playerid, params[])
{
    new itemId, amount;
    if(sscanf(params, "dd", itemId))return SCM(playerid, -1, "/giveitem [itemId] [amount]");
    {
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmountEx(playerid, itemId, amount);
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

        SCMF(playerid, -1, "Item Name: %s | Item Idx: %d | Amount: %d | Upgrade (clipsize) level: %d", eItemsData[i][e_sItem_Name], i, GetPlayerItemAmount(playerid, i), GetPlayerItemUpgradeLvl(playerid, i, UPGRADE_TYPE_CLIPSIZE));
    }
    return 1;
}

CMD:setlvl(playerid, params[])
{
    new itemId, level;
    if(sscanf(params, "dd", itemId, level))return SCM(playerid, -1, "/setlvl [itemId] [level]");
    {
        new bool:success = SetPlayerItemUpgradeLvl(playerid, itemId, level, UPGRADE_TYPE_CLIPSIZE);

        if(!success)
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
        if(!HasPlayerGotItem(playerid, itemId))
        {
            SCM(playerid, -1, "You've tried to equip item that you either don't have or is invalid!");
            return 1;
        }

        if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
        {
            GivePlayerWeapon(playerid, eItemsData[itemId][e_iItem_SA_WeaponId], GetPlayerItemAmount(playerid, itemId));
        }
    }
    return 1;
}