//#region Items manipulation (create/update/destroy/load items from DB)

    // Create new item in database.
    DB_CommitNewItem(itemId, itemType, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, creatorId)
    {
        new query[512];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), 
            "INSERT INTO items_data (id_item, x, y, z, rX, rY, rZ, virtualWorld, interiorId, amount, meleeExtCnd) \ 
            VALUES \
            (%d, %f, %f, %f, %f, %f, %f, %d, %d, %d, %d)", \
            itemId, \
            x, y, z, \
            rX, rY, rZ, \
            virtualWorld, interiorId, \
            itemAmount, meleeExtCnd);

        mysql_tquery(eServerInfo[e_SqlHandler], query, "OnItemCreate", "ddffffffddddssd", itemId, itemType, x, y, z, rX, rY, rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, "", "", creatorId);
    }

    // Update an existing item in database.
    DB_UpdateExistingItem(updateItemUid, itemId, itemAmount, meleeExtCnd, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, creatorId)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET id_item = %d, amount = %d, meleeExtCnd = %d WHERE uid = %d", itemId, itemAmount, meleeExtCnd, updateItemUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query, "OnItemUpdate", "ddddffffffddd", updateItemUid, itemId, itemAmount, meleeExtCnd, x, y, z, rX, rY, rZ, virtualWorld, interiorId, creatorId);
    }

    // Change the flag 'is_created' (destroy) item.
    DB_DestroyItem(uid_item)
    {
        new query[64];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET is_created = 0 WHERE uid = %d", uid_item);
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
            {
                return rows;
            }

            new 
                row_itemUid,
                row_idItem,
                Float:row_itemX, Float:row_itemY, Float:row_itemZ,
                row_virtualWorld, row_interiorId,
                row_itemAmount,
                row_meleeExtCnd;

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
                
                CreateOrUpdateItem(row_idItem, row_itemAmount, row_meleeExtCnd, INVALID_PLAYER_ID, row_itemX, row_itemY, row_itemZ, row_virtualWorld, row_interiorId, true, row_itemUid);
            }
        }
        mysql_tquery_inline(eServerInfo[e_SqlHandler], "SELECT * FROM items_data WHERE uid_owner = -1 AND is_created = 1", using inline OnItemsLoad);
        
        return rows;
    }

//#endregion

//#region Collectibles manipulation (create)

    // Create new collectible in database.
    DB_CommitNewCollectible(itemId, itemType, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, title[], message[], creatorId)
    {
        new query[512];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), 
            "INSERT INTO collectibles_data (id_item, id_type, x, y, z, rX, rY, rZ, virtualWorld, interiorId, title, message) \ 
            VALUES \
            (%d, %d, %f, %f, %f, %f, %f, %f, %d, %d, '%s', '%s')", \
            itemId, itemType, \
            x, y, z, \
            rX, rY, rZ, \
            virtualWorld, interiorId, \
            title, message);

        mysql_tquery(eServerInfo[e_SqlHandler], query, "OnItemCreate", "ddffffffddddssd", itemId, itemType, x, y, z, rX, rY, rZ, virtualWorld, interiorId, 0, 0, title, message, creatorId);
    }

//#endregion