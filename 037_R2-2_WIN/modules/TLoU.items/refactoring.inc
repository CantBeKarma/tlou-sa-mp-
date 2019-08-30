CMD:giveitem(playerid, params[])
{
    new itemId, amount;
    if(sscanf(params, "dd", itemId, amount))return SCM(playerid, -1, "/giveitem [itemId] [amount]");
    {
        if(IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MELEE))
        {
            SCMF(playerid, -1, "To give melee use /givemelee cmd instead");
            return 0;
        }

        GivePlayerItem(playerid, itemId);

        if(!SetPlayerItemAmount(playerid, itemId, amount))
        {
            SCMF(playerid, -1, "Amount failure!");
        }
    }
    return 1;
}

CMD:givemelee(playerid, params[])
{
    new itemId, amount, meleeExtraCnd;
    if(sscanf(params, "ddd", itemId, amount, meleeExtraCnd))return SCM(playerid, -1, "/givemelee [itemId] [cnd] [extraCnd]");
    {
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MELEE))
        {
            SCMF(playerid, -1, "It's not a melee!");
            return 0;
        }
        
        GivePlayerItem(playerid, itemId);

        if(!SetPlayerItemAmount(playerid, itemId, amount, meleeExtraCnd))
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