//#region Items manipulation give/update/save/load

    // Gives player an ownership over a weapon.
    DB_GivePlayerItem(itemId, amount, meleeExtCnd = 0, ownerUid)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "INSERT INTO `items_data` (`id_item`, `amount`, `meleeExtCnd`, `uid_owner`, `is_created`) VALUES (%d, %d, %d, %d, 0)", itemId, amount, meleeExtCnd, ownerUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Updates player weapon ammo/amount | melee condition
    DB_UpdatePlayerItemAmount(itemId, amount, meleeExtCnd = 0, ownerUid)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE `items_data` SET `amount` = %d, `meleeExtCnd` = %d WHERE `id_item` = %d AND `uid_owner` = %d", amount, meleeExtCnd, itemId, ownerUid);
        mysql_tquery(eServerInfo[e_SqlHandler], query);
    }

    // Remove player item (remove ownership) from DB and recreate in the world (optionally).
    DB_RemovePlayerItem(playerid, itemId, bool:recreateInWorld = false, amount = 0, meleeExtCnd = 0)
    {
        new query[128];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "UPDATE `items_data` SET `uid_owner` = -1 WHERE `id_item` = %d AND `uid_owner` = %d", itemId, eCharsInfo[playerid][e_iUid]);
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
                return 1;
            }

            new 
                row_idItem,
                row_amount,
                row_meleeExtCnd;

            new weaponId;

            for(new i = 0; i != rows; i++)
            {
                cache_get_value_name_int(i, "id_item", row_idItem);
                cache_get_value_name_int(i, "amount", row_amount);

                // Ingredients.
                if(0 <= row_idItem <= 5)
                {
                    //Craft_IngrAmount(playerid, row_idItem) = row_amount;
                    continue;
                }

                weaponId = eItemsData[row_idItem][e_iItem_SA_WeaponId];

                // Melee weapon durability.
                if(IsItemMeleeWeapon(weaponId))
                {
                    cache_get_value_name_int(i, "meleeExtCnd", row_meleeExtCnd);

                    if(row_meleeExtCnd > 0)
                    {
                        ePlayerItemData[playerid][i][e_iItemMeleeExtraCND] = row_meleeExtCnd;
                    }
                }

                // Other weapons.
                //GivePlayerWeaponInSlot(playerid, weaponId);
                //SetPlayerWeaponAmmo(playerid, weaponId, row_amount);
            }
        }

        new query[110];
        mysql_format(eServerInfo[e_SqlHandler], query, sizeof(query), "SELECT `id_item`, `amount`, `meleeExtCnd` FROM `items_data` WHERE `uid_owner` = %d ORDER BY `uid_owner`", charUid);
        mysql_tquery_inline(eServerInfo[e_SqlHandler], query, using inline OnCharsItemsDataReceived);
        return 0;
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
            switch(i)
            {
                // Ingredients.
                // case 0..5:
                //     itemAmount = Craft_IngrAmount(playerid, i);
                
                // Melee weapon CND.
                case 6..9:
                {
                    if(HasPlayerGotItem(playerid, i))
                    {
                        itemAmount = GetPlayerItemAmount(playerid, i);

                        if(IsPlayerMeleeUpgraded(playerid))
                        {
                            meleeExtCnd = ePlayerItemData[playerid][i][e_iItemMeleeExtraCND];
                        }
                    }
                }

                // Other weapons.
                default:
                {
                    if(eItemsData[i][e_iItem_SA_WeaponId] != -1)
                    {
                        itemAmount = GetPlayerItemAmount(playerid, i);
                    }
                }
            }

            //DB_UpdatePlayerItemAmount(i, itemAmount, meleeExtCnd, eCharsInfo[playerid][e_iUid]);
            itemAmount = meleeExtCnd = 0;
        }
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