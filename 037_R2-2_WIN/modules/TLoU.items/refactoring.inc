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