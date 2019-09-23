// Checks if specified item is upgradeable
// Returns:
// true if item is, false otherwise.
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

// Gets item's maximal upgrade level.
// Returns:
// (-1) if item is invalid or item is not upgradeable.
// (x) value of maximal level.
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
    if(!HasPlayerGotItem(playerid, itemId) || !IsItemType(itemId, ITEM_TYPE_BULLET_WEAPON))
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