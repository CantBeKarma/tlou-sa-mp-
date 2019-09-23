//#region Items manipulation give/update/save/load

    // Gives player an ownership over an item.
    DB_GivePlayerItem(playerid, itemId)
    {
        if(GetItemType(itemId) == ITEM_TYPE_AMMO)
        {
            return false;
        }

        new query[90];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO items_data (id_item, uid_owner, is_created) VALUES (%d, %d, 0)", itemId, GetPlayerCharUid(playerid));
        mysql_tquery(eServerInfo[e_SqlHandler], query);
        return true;
    }

    // Updates player weapon ammo/amount | melee condition
    DB_UpdateCharItemAmount(playerid, itemId, amount, meleeExtCnd = 0)
    {
        new query[110];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET amount = %d, meleeExtCnd = %d WHERE id_item = %d AND uid_owner = %d", amount, meleeExtCnd, itemId, GetPlayerCharUid(playerid));
        mysql_tquery(eServerInfo[e_SqlHandler], query);

        if(IsItemType(itemId, ITEM_TYPE_MELEE) && IsPlayerMeleeUpgraded(playerid))
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Updated itemId: %d with CND/ExtCND: %d/%d for character: %s (UID: %d | GUID: %d)", itemId, amount, meleeExtCnd, PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
        }
        else
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Updated itemId: %d with amount: %d for character: %s (UID: %d | GUID: %d)", itemId, amount, PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
        }
    }

    // Remove player item (remove ownership) from DB and recreate in the world (optionally).
    DB_RemovePlayerItem(playerid, itemId, bool:recreateInWorld = false, amount = 0, meleeExtCnd = 0)
    {
        if(recreateInWorld)
        {
            inline Response()
            {
                new updateItemUid = -1;
                cache_get_value_name_int(0, "uid", updateItemUid);

                if(updateItemUid != -1)
                {
                    CreateOrUpdateItem(itemId, amount, meleeExtCnd, playerid, .updateItemUid = updateItemUid);
                }

                new query[90];
                mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET uid_owner = -1 WHERE id_item = %d AND uid_owner = %d", itemId, GetPlayerCharUid(playerid));
                mysql_tquery(eServerInfo[e_SqlHandler], query);
            }

            new query[90];
            mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT uid FROM `items_data` WHERE id_item = %d AND uid_owner = %d LIMIT 1", itemId, GetPlayerCharUid(playerid));
            mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline Response);
        }
        else
        {
            new query[90];
            mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE items_data SET uid_owner = -1 WHERE id_item = %d AND uid_owner = %d", itemId, GetPlayerCharUid(playerid));
            mysql_tquery(eServerInfo[e_SqlHandler], query);
        }
    }

//#endregion

//#region Collectibles manipulation

    // Gives player ownership of the collectible UID in DB.
    DB_GiveCharacterCollectible(playerid, itemUid)
    {
        new query[80];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE collectibles_data SET uid_owner = %d, is_created = 0 WHERE uid = %d", GetPlayerCharUid(playerid), itemUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Load single collectible data (title) and display it on the collectibles list.
    // Textdraw showing happens in OnPlayerCollectibleLoad()
    DB_LoadCollectibleData(playerid, selectedCategory, uid)
    {
        if(selectedCategory == COLL_ITEM_ARTIFACTS && !Iter_Contains(ArtifactsOwned<playerid>, uid)
        || selectedCategory == COLL_ITEM_FIREFLY_PEND && !Iter_Contains(PendantsOwned<playerid>, uid))
        {
            return false;
        }

        inline OnCollectibleDataReceived()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                return false;
            }

            new row_title[MAX_COLL_TIT_LEN];
            cache_get_value_name(0, "title", row_title, sizeof(row_title));

            OnPlayerCollectibleLoad(playerid, row_title);
        }

        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT title FROM collectibles_data WHERE uid = %d AND uid_owner = %d AND id_type = %d LIMIT 1", uid, GetPlayerCharUid(playerid), selectedCategory);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCollectibleDataReceived);
        return true;
    }

    // Read single player's artifact from DB with title and its message content.
    DB_ReadCollectibleData(playerid, uid)
    {
        inline OnCollectibleDataReceived()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                return false;
            }

            new
                row_title[MAX_COLL_TIT_LEN],
                row_message[MAX_COLL_MSG_LEN];

            cache_get_value_name(0, "title", row_title, sizeof(row_title));
            cache_get_value_name(0, "message", row_message, sizeof(row_message));

            OnPlayerReadCollectible(playerid, row_title, row_message);
        }

        new query[100];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT title, message FROM collectibles_data WHERE uid = %d AND uid_owner = %d LIMIT 1", uid, GetPlayerCharUid(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCollectibleDataReceived);
        return true;
    }

//#endregion

//#region Player's account data manipulation

    // Updates player's last login date with current date and time.
    DB_UpdatePlayerLastLoginDate(playerid)
    {
        new query[90];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE `player_data` SET `last_login_date` = NOW() WHERE `guid` = %d", GetPlayerGuid(playerid));
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Check if player's account exist.
    DB_DoesPlayerAccountExist(playerid)
    {
        new 
            Cache:result,
            query[61 + MAX_PLAYER_NAME];

        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `guid` FROM `player_data` WHERE `name` = '%s' LIMIT 1", PlayerName(playerid));
        result = mysql_query(eServerInfo[e_SqlHandler], query);

        new rowsCount;
        cache_get_row_count(rowsCount);

        if(rowsCount > 0) 
        {
            cache_get_value_name_int(0, "guid", ePlayerInfo[playerid][e_iGuid]);
            cache_delete(result);
            return true;
        }
        return false;
    }

    // Registers new account and sets player his character Guid.
    DB_RegisterNewAccount(playerid, passwordHash[])
    {
        new query[300];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO `player_data` (`name`, `ip_address`, `register_date`, `last_login_date`, `password`) VALUES ('%e', '%e', NOW(), NOW(), '%e')", PlayerName(playerid), GetPlayerIpAddress(playerid), passwordHash);
        mysql_tquery(eServerInfo[e_SqlHandler], query, "OnPlayerRegister", "d", playerid);
    }

    // Try signing in to the player's account with password specified.
    DB_TryLoginPlayerToAccount(playerid, const password[])
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s is trying to sign in to account of GUID: %d", PlayerName(playerid), GetPlayerGuid(playerid));

        new query[63];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `password` FROM `player_data` WHERE `guid` = %d LIMIT 1", GetPlayerGuid(playerid));
        mysql_tquery(eServerInfo[e_SqlHandler], query, "OnPlayerPasswordReceived", "ds", playerid, password);
    }

//#endregion

//#region Character database data manipulation

    // Creates new character in database for specified player GUID.
    DB_CreateNewCharacter(playerid)
    {
        inline OnNewCharsDataReceived()
        {
            eCharsInfo[playerid][e_iUid] = cache_insert_id();
            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Created new character. Name: %s (UID: %d | GUID: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
        }

        new query[62 + MAX_PLAYER_NAME];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO `chars_data` (`guid`, `name`) VALUES ('%d', '%e')", GetPlayerGuid(playerid), PlayerName(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnNewCharsDataReceived);
    }
    
    // Loads character data from database.
    DB_LoadPlayerCharacter(playerid)
    {
        inline OnPlayerCharDataReceived()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Character: %s (for GUID: %d) wasn't found.", PlayerName(playerid), GetPlayerGuid(playerid));
                return false;
            }

            cache_get_value_name_int(0, "uid",              eCharsInfo[playerid][e_iUid]);
            cache_get_value_float   (0, "health",           eCharsInfo[playerid][e_fHealth]);
            cache_get_value_name_int(0, "skinid",           eCharsInfo[playerid][e_iSkinId]);
            cache_get_value_float   (0, "last_x",           eCharsInfo[playerid][e_fPosX]); 
            cache_get_value_float   (0, "last_y",           eCharsInfo[playerid][e_fPosY]); 
            cache_get_value_float   (0, "last_z",           eCharsInfo[playerid][e_fPosZ]);
            cache_get_value_float   (0, "last_ang",         eCharsInfo[playerid][e_fPosAng]);
            cache_get_value_name_int(0, "last_int",         eCharsInfo[playerid][e_iPosInt]);
            cache_get_value_name_int(0, "last_vw",          eCharsInfo[playerid][e_iPosVw]);
            cache_get_value_name_int(0, "supplements",      eCharsInfo[playerid][e_iSkills_SuppsAmount]);
            cache_get_value_name_int(0, "skill_maxhealth",  eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_MAXHP]);
            cache_get_value_name_int(0, "skill_listenmode", eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_LISTEN_DIST]);
            cache_get_value_name_int(0, "skill_crafting",   eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_CRAFT_SPEED]);
            cache_get_value_name_int(0, "skill_healing",    eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_HEAL_SPEED]);
            cache_get_value_name_int(0, "skill_weaponsway", eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_WEAP_SWAY]);
            cache_get_value_name_int(0, "skill_shivmaster", eCharsInfo[playerid][e_iSkillLevel][SKILL_ITEM_SHIV_MASTER]);

            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Loaded data for character: %s (UID: %d | GUID: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));

            DB_LoadCharItems(playerid);
            DB_LoadCharCollectibles(playerid, COLL_ITEM_ARTIFACTS);
            DB_LoadCharCollectibles(playerid, COLL_ITEM_FIREFLY_PEND);

            PrepareDataAfterLogin(playerid);
        }

        new query[73 + MAX_PLAYER_NAME];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT * FROM `chars_data` WHERE `guid` = %d AND `name` = '%s' LIMIT 1", GetPlayerGuid(playerid), PlayerName(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnPlayerCharDataReceived);
        return true;
    }

//#endregion

//#region Character loading data (items & collectibles)

    // Loads character's items from database.
    DB_LoadCharItems(playerid)
    {
        inline OnCharItemsDataReceived()
        {
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(!rowsCount)
            {
                Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Loaded: 0 items for character: %s (UID: %d | GUID: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
                return true;
            }

            new 
                row_idItem,
                row_amount,
                row_meleeExtCnd;

            for(new i = 0; i != rowsCount; i++)
            {
                cache_get_value_name_int(i, "id_item", row_idItem);
                cache_get_value_name_int(i, "amount", row_amount);

                GivePlayerItem(playerid, row_idItem, .logOrLoadFromDB = true);

                if(IsItemType(row_idItem, ITEM_TYPE_MELEE))
                {
                    cache_get_value_name_int(i, "meleeExtCnd", row_meleeExtCnd);
                    SetPlayerItemAmount(playerid, row_idItem, row_amount, row_meleeExtCnd, .logAndNotLoadingFromDB = false);
                }
                else
                {
                    SetPlayerItemAmount(playerid, row_idItem, row_amount, .logAndNotLoadingFromDB = false);
                }
            }

            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Loaded: %d items for character: %s (UID: %d | GUID: %d)", rowsCount, PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
        }

        new query[110];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `id_item`, `amount`, `meleeExtCnd` FROM `items_data` WHERE `uid_owner` = %d ORDER BY `uid_owner`", GetPlayerCharUid(playerid));
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCharItemsDataReceived);
        return true;
    }

    // Load player's collectibles from DB and assign their UIDs to player's iterator.
    DB_LoadCharCollectibles(playerid, selectedCategory)
    {
        if(selectedCategory != COLL_ITEM_ARTIFACTS
        && selectedCategory != COLL_ITEM_FIREFLY_PEND)
        {
            return false;
        }

        inline OnCollectiblesDataReceived()
        {
            static collectibleName[24];
            new rowsCount;
            cache_get_row_count(rowsCount);

            if(selectedCategory == COLL_ITEM_ARTIFACTS)
            {
                Iter_Clear(ArtifactsOwned<playerid>);
                collectibleName = "Artifacts";
            }
            else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
            {
                Iter_Clear(PendantsOwned<playerid>);
                collectibleName = "Pendants";
            }

            if(!rowsCount)
            {
                Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Loaded 0 %s for character: %s (UID: %d | GUID: %d)", collectibleName, PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
                return false;
            }
            
            new row_uid;

            for(new i = 0; i != rowsCount; i++)
            {
                cache_get_value_name_int(i, "uid", row_uid);

                if(selectedCategory == COLL_ITEM_ARTIFACTS)
                {
                    Iter_Add(ArtifactsOwned<playerid>, row_uid);
                }
                else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
                {
                    Iter_Add(PendantsOwned<playerid>, row_uid);
                }
            }

            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Loaded %d %s for character: %s (UID: %d | GUID: %d)", rowsCount, collectibleName, PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
        }

        new query[100];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT uid FROM collectibles_data WHERE uid_owner = %d AND id_type = %d", GetPlayerCharUid(playerid), selectedCategory);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCollectiblesDataReceived);
        return true;
    }

//#endregion

//#region Character data saving (items, skills, position, health etc.)

    // Save character's overall data
    DB_SavePlayerCharacter(playerid)
    {
        new Float:x, Float:y, Float:z, Float:ang;
        GetPlayerPos(playerid, x, y, z);
        GetPlayerFacingAngle(playerid, ang);

        new 
            Float:health = GetPlayerHealthEx(playerid),
            skinId = GetPlayerSkinEx(playerid),
            interiorId = GetPlayerInterior(playerid), 
            virtualWorld = GetPlayerVirtualWorld(playerid),
            supplementsAmount = GetPlayerSupplementsAmount(playerid),
            skill_maxHp = Skill_GetPlayerLevel(playerid, SKILL_ITEM_MAXHP),
            skill_maxListenDist = Skill_GetPlayerLevel(playerid, SKILL_ITEM_LISTEN_DIST),
            skill_craftSpeed = Skill_GetPlayerLevel(playerid, SKILL_ITEM_CRAFT_SPEED),
            skill_healSpeed = Skill_GetPlayerLevel(playerid, SKILL_ITEM_HEAL_SPEED),
            skill_weaponSway = Skill_GetPlayerLevel(playerid, SKILL_ITEM_WEAP_SWAY),
            skill_shivMaster = Skill_GetPlayerLevel(playerid, SKILL_ITEM_SHIV_MASTER);

        new query[450];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query),\
            "UPDATE `chars_data` SET \
            `health` = %f, skinid = %d, `last_x` = %f, `last_y` = %f, `last_z` = %f, `last_ang` = %f, `last_int` = %d, `last_vw` = %d, \
            `supplements` = %d, `skill_maxhealth` = %d, `skill_listenmode` = %d, `skill_crafting` = %d, `skill_healing` = %d, `skill_weaponsway` = %d, `skill_shivmaster` = %d \
            WHERE `uid` = %d",\
            health, skinId, x, y, z, ang, interiorId, virtualWorld, \
            supplementsAmount, skill_maxHp, skill_maxListenDist, skill_craftSpeed, skill_healSpeed, skill_weaponSway, skill_shivMaster, \
            GetPlayerCharUid(playerid));

        mysql_tquery(eServerInfo[e_SqlHandler], query);

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "[DB] Saved data for character %s (UID: %d | GUID: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));

        DB_SaveCharItems(playerid);
    }

    // Save character's items to database.
    DB_SaveCharItems(playerid)
    {
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

            DB_UpdateCharItemAmount(playerid, i, itemAmount, meleeExtCnd);
            itemAmount = meleeExtCnd = 0;
        }
    }

//#endregion