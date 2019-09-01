//#region Items manipulation give/update/save/load

    // Gives player an ownership over a weapon.
    DB_GivePlayerItem(itemId, amount, meleeExtCnd = 0, ownerUid)
    {
        new query[128];
        switch(GetItemType(itemId))
        {
            case ITEM_TYPE_WEAPON:
            {
                mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO items_data (id_item, amount, uid_owner, is_created) VALUES (%d, %d, %d, 0)", itemId, amount, ownerUid);
            }

            case ITEM_TYPE_MELEE:
            {
                mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO items_data (id_item, amount, meleeExtCnd, uid_owner, is_created) VALUES (%d, %d, %d, %d, 0)", itemId, amount, meleeExtCnd, ownerUid);
            }
            default:
            {
                return false;
            }
        }
        
        mysql_tquery(eServerInfo[e_SqlHandler], query);
        return true;
    }

    // Updates player weapon ammo/amount | melee condition
    DB_UpdatePlayerItemAmount(itemId, amount, meleeExtCnd = 0, ownerUid)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET amount = %d, meleeExtCnd = %d WHERE id_item = %d AND uid_owner = %d", amount, meleeExtCnd, itemId, ownerUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Remove player item (remove ownership) from DB and recreate in the world (optionally).
    DB_RemovePlayerItem(playerid, itemId, bool:recreateInWorld = false, amount = 0, meleeExtCnd = 0)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET uid_owner = -1 WHERE id_item = %d AND uid_owner = %d", itemId, GetPlayerCharUid(playerid)));
        mysql_tquery(eServerInfo[e_SqlHandler], query);

        if(recreateInWorld)
        {
            CreateOrUpdateItem(itemId, amount, meleeExtCnd, playerid);
        }
    }

    // Loads weapons from database for specified character (happens during sign-in).
    DB_LoadPlayerItems(playerid, charUid)
    {
        inline OnCharsItemsDataReceived()
        {
            new rows;
            cache_get_row_count(rows);

            if(!rows)
            {
                return true;
            }

            new 
                row_idItem,
                row_amount,
                row_meleeExtCnd;

            for(new i = 0; i != rows; i++)
            {
                cache_get_value_name_int(i, "id_item", row_idItem);
                cache_get_value_name_int(i, "amount", row_amount);

                GivePlayerItem(playerid, row_idItem);

                if(IsItemType(row_idItem, ITEM_TYPE_MELEE))
                {
                    cache_get_value_name_int(i, "meleeExtCnd", row_meleeExtCnd);
                    SetPlayerItemAmount(playerid, row_idItem, row_amount, row_meleeExtCnd);
                }
                else
                {
                    SetPlayerItemAmount(playerid, row_idItem, row_amount);
                }
            }
        }

        new query[110];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `id_item`, `amount`, `meleeExtCnd` FROM `items_data` WHERE `uid_owner` = %d ORDER BY `uid_owner`", charUid);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCharsItemsDataReceived);
        return true;
    }

    // Save all player items to the DB.
    DB_SavePlayerItems(playerid)
    {
        printf("DB_SavePlayerItems");
        new 
            itemAmount = 0,
            meleeExtCnd = 0;

        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(!HasPlayerGotItem(playerid, i))
            {
                continue;
            }

            itemAmount = GetPlayerItemAmount(playerid, i);

            if(IsItemType(i, ITEM_TYPE_MELEE) && IsPlayerMeleeUpgraded(playerid))
            {
                meleeExtCnd = GetPlayerMeleeExtraCnd(playerid, i);
            }

            //DB_UpdatePlayerItemAmount(i, itemAmount, meleeExtCnd, eCharsInfo[playerid][e_iUid]);
            itemAmount = meleeExtCnd = 0;
        }
    }

//#endregion

//#region Collectibles manipulation

    // Gives player ownership of the collectible UID in DB.
    DB_GiveCollectibleOwnership(playerid, itemUid)
    {
        new query[85];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE collectibles_data SET uid_owner = %d, is_created = 0 WHERE uid = %d", GetPlayerCharUid(playerid), itemUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Read top 10 collectibles player has in DB and display its title.
    DB_LoadPlayerCollectibles(playerid)
    {
        inline OnCollectiblesDataReceived()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                return true;
            }

            new row_title[32];
            for(new i = 0; i != rowsCount; i++)
            {
                cache_get_value_name(i, "title", row_title, sizeof(row_title));
            }
        }

        new query[70];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT title FROM collectibles_data WHERE uid_owner = %d LIMIT 9", GetPlayerCharUid(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCollectiblesDataReceived);
        return true;
    }

    // Read single player's artifact from DB with title and its message content.
    DB_ReadArtifact(playerid, uid)
    {
        inline ReadArtifactsData()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                return false;
            }

            new
                row_title[32],
                row_message[128];

            SCMF(playerid, -1, "Your artifact data:");

            cache_get_value_name(0, "title", row_title, sizeof(row_title));
            cache_get_value_name(0, "message", row_message, sizeof(row_message));

            SCMF(playerid, -1, "Uid: %d | Title: %s | Message: %s", row_title, row_message);
        }

        new query[100];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT title, message FROM collectibles_data WHERE uid = %d AND uid_owner = %d LIMIT 1", uid, GetPlayerCharUid(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline ReadArtifactsData);
        return true;
    }

//#endregion

//#region Account register/login/save

    // Return either user has an account or not.
    DB_CheckAccountExistence(playerid)
    {
        new 
            query[60 + MAX_PLAYER_NAME],
            Cache:result;

        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `guid` FROM `chars_data` WHERE `name` = '%s' LIMIT 1", ePlayerInfo[playerid][e_sName]);
        result = mysql_query(eServerInfo[e_SqlHandler], query);

        new rows;
        cache_get_row_count(rows);

        if(rows > 0) 
        {
            cache_get_value_name_int(0, "guid", ePlayerInfo[playerid][e_iGuid]);
            cache_delete(result);
            return true;
        }
        else
        {
            return false;
        }
    }

    // Requests all player's data (during sign-in)
    DB_RequestPlayerData(playerid, passwordHash[])
    {
        new query[134 + MAX_PLAYER_NAME];
        inline OnPlayerDataReceived()
        {
            new rows;
            cache_get_row_count(rows);

            if(!rows)
            {
                return ShowRegisterLoginDialog(playerid, true);
            }

            mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT * FROM `chars_data` WHERE `guid` = %d AND `name` = '%s' LIMIT 1", ePlayerInfo[playerid][e_iGuid], ePlayerInfo[playerid][e_sName]);
            mysql_tquery(eServerInfo[e_SqlHandler], query, "PrepareDataAfterLogin", "d", playerid);

            mysql_tquery(eServerInfo[e_SqlHandler], "UPDATE `player_data` SET `last_login_date` = NOW()");
        }
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT * FROM `player_data` WHERE `guid` = %d AND `password` = '%s' LIMIT 1", ePlayerInfo[playerid][e_iGuid], passwordHash);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnPlayerDataReceived);
        return 0;
    }

    // Registers new account and sets player his character Guid.
    DB_RegisterNewAccount(playerid, passwordHash[])
    {
        new query[126 + MAX_PLAYER_NAME];
        inline OnPlayerDataReceived()
        {
            ePlayerInfo[playerid][e_iGuid] = cache_insert_id();
            
            inline OnCharsDataReceived()
            {
                eCharsInfo[playerid][e_iUid] = cache_insert_id();
                PrepareDataAfterRegister(playerid);
            }
            mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO `chars_data` (`guid`, `name`) VALUES ('%d', '%s')", ePlayerInfo[playerid][e_iGuid], ePlayerInfo[playerid][e_sName]);
            mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCharsDataReceived);
        }
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO `player_data` (`ip_address`, `register_date`, `last_login_date`, `password`) VALUES ('%s', NOW(), NOW(), '%s')", GetPlayerIpAddress(playerid), passwordHash);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnPlayerDataReceived);
    }

    // Save character's data
    DB_SaveCharsData(playerid, Float:playerHealth, Float:x, Float:y, Float:z, Float:ang, playerInterior, playerVirtualWorld, \
					playerSupps, playerSkill_MaxHp, playerSkill_MaxListenDist, playerSkill_CraftSpeed, playerSkill_HealSpeed, playerSkill_WeapSway, playerSkill_ShivMaster)
    {
        new query[350];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query),\
            "UPDATE `chars_data` SET \
            `health` = %f, `last_x` = %f, `last_y` = %f, `last_z` = %f, `last_ang` = %f, `last_int` = %d, `last_vw` = %d, \
            `supplements` = %d, `skill_maxhealth` = %d, `skill_listenmode` = %d, `skill_crafting` = %d, `skill_healing` = %d, `skill_weaponsway` = %d, `skill_shivmaster` = %d \
            WHERE `uid` = %d",\
            playerHealth, x, y, z, ang, playerInterior, playerVirtualWorld, \
            playerSupps, playerSkill_MaxHp, playerSkill_MaxListenDist, playerSkill_CraftSpeed, playerSkill_HealSpeed, playerSkill_WeapSway, playerSkill_ShivMaster, \
            eCharsInfo[playerid][e_iUid]);

        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

//#endregion