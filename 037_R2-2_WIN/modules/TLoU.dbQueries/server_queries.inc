// Create new item in database.
DB_CommitNewItem(itemId, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId)
{
    new query[512];
    mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), 
        "INSERT INTO `items_data` (`id_item`, `x`, `y`, `z`, `rX`, `rY`, `rZ`, `virtualWorld`, `interiorId`, `amount`, `meleeExtCnd`, `collectibleTitle`, `collectibleInnerMsg` ) \ 
        VALUES \
        (%d, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d, '%s', '%s')", \
        itemId, \
        x, y, z, \
        rX, rY, rZ, \
        virtualWorld, interiorId, \
        itemAmount, meleeExtCnd, itemTitle, innerMsg);

    mysql_tquery(eServerInfo[e_SqlHandler], query, "OnItemCreate", "dffffffddddssd", itemId, x, y, z, rX, rY, rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle, innerMsg, creatorId);
}

// Update an existing item in database.
DB_UpdateExistingItem(updateItemUid, itemId, itemAmount, meleeExtCnd, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, creatorId)
{
    new query[128];
    mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE `items_data` SET `id_item` = %d, `amount` = %d, `meleeExtCnd` = %d WHERE `uid` = %d", itemId, itemAmount, meleeExtCnd, updateItemUid);
    mysql_tquery(eServerInfo[e_SqlHandler], query, "OnItemUpdate", "ddddffffffddd", updateItemUid, itemId, itemAmount, meleeExtCnd, x, y, z, rX, rY, rZ, virtualWorld, interiorId, creatorId);
}

// Change the flag 'is_created' (destroy) item.
DB_DestroyItem(uid_item)
{
    new query[64];
    mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE `items_data` SET `is_created` = 0 WHERE `uid` = %d", uid_item);
    mysql_tquery(eServerInfo[e_SqlHandler], query);
}

// Load items back to the world from the database.
DB_LoadItems()
{
    new rows = 0;
    inline OnItemsLoad()
    {
        cache_get_row_count(rows);

        if(!rows)
            return rows;

        new 
            row_itemUid,
            row_idItem,
            Float:row_itemX, Float:row_itemY, Float:row_itemZ,
            row_virtualWorld, row_interiorId,
            row_itemAmount,
            row_meleeExtCnd,
            row_itemTitle[32],
            row_itemInnerMsg[128];

        for(new i = 0; i != rows; i++)
        {
            cache_get_value_name_int(i, "uid", row_itemUid);
            cache_get_value_name_int(i, "id_item", row_idItem);
            cache_get_value_float(i, "x", row_itemX);
            cache_get_value_float(i, "y", row_itemY);
            cache_get_value_float(i, "z", row_itemZ);
            cache_get_value_name_int(i, "virtualWorld", row_virtualWorld);
            cache_get_value_name_int(i, "interiorId", row_interiorId);
            cache_get_value_name_int(i, "amount", row_itemAmount);
            cache_get_value_name_int(i, "meleeExtCnd", row_meleeExtCnd);
            cache_get_value_name(i, "collectibleTitle", row_itemTitle, sizeof(row_itemTitle));
            cache_get_value_name(i, "collectibleInnerMsg", row_itemInnerMsg, sizeof(row_itemInnerMsg));
            
            CreateOrUpdateItem(row_idItem, row_itemAmount, row_meleeExtCnd, INVALID_PLAYER_ID, \
                row_itemTitle, row_itemInnerMsg, row_itemX, row_itemY, row_itemZ, row_virtualWorld, row_interiorId, true, row_itemUid);
        }
    }
    mysql_tquery_inline(eServerInfo[e_SqlHandler], "SELECT * FROM `items_data` WHERE `uid_owner` = -1", using inline OnItemsLoad);
    
    return rows;
}