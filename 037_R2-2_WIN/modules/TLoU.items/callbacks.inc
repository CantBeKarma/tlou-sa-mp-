#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Loading items from database. Please wait...");
    
	new itemsLoadedCount = DB_LoadItems();

    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Loaded %d items from database!", itemsLoadedCount);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerLookAtItem -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerLookAtItem(playerid, item_objectId)
{
    if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
        DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);

    Streamer_GetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

    new 
        itemId = eObjData[e_iObj_ItemId],
        Float:x = eObjData[e_fObj_X],
        Float:y = eObjData[e_fObj_Y],
        Float:z = eObjData[e_fObj_Z],
        virtualWorld = eObjData[e_iObj_Vw],
        interiorId = eObjData[e_iObj_Int],
        Float:itemAmount = eObjData[e_iObj_ItemAmount],
        itemMeleeUpgradedCnd = eObjData[e_iObj_MeleeUpgradedCnd],
        bool:meleeUpgraded = ((itemMeleeUpgradedCnd > 0) ? (true) : (false)),
        itemType = eItemsData[itemId][e_iItem_Type],
        itemModel = eItemsData[itemId][e_iItem_ModelId],
        itemWeaponId = eItemsData[itemId][e_iItem_SA_WeaponId];

    new labelStr[32];
    switch(itemType)
    {
        case ITEM_TYPE_HEAL:        format(labelStr, sizeof(labelStr), "%s%s", eItemsData[itemId][e_sItem_Name], (GetPlayerWeaponAmmo(playerid, itemWeaponId) >= MAX_AMOUNT_MEDKIT ? "\n{FF0000}(FULL)" : ""));
        case ITEM_TYPE_THROWABLE:   format(labelStr, sizeof(labelStr), "%s%s", eItemsData[itemId][e_sItem_Name], (HasPlayerGotThrowableItem(playerid, itemWeaponId) ? "\n{FF0000}(FULL)" : ""));
        case ITEM_TYPE_COLLECTIBLE: format(labelStr, sizeof(labelStr), "%s", eObjData[e_sObj_Title]);

        case ITEM_TYPE_MELEE:
        {
            format(labelStr, sizeof(labelStr), "%s\n(CND: %d/%d)", eItemsData[itemId][e_sItem_Name], \
                    (meleeUpgraded) ? (floatround(itemAmount) + itemMeleeUpgradedCnd) : (floatround(itemAmount)), \
                    (meleeUpgraded) ? GetMeleeWeapon_Durability(itemWeaponId, false) + GetMeleeWeapon_Durability(itemWeaponId, true) 
                    : GetMeleeWeapon_Durability(itemWeaponId, false));
        }

        case ITEM_TYPE_INGREDIENT:
        {
            // itemId = ingredientId (this is the ONLY case!)
            if(IsPlayerFullOnIngr(playerid, itemId)) format(labelStr, sizeof(labelStr), "%s\n{FF0000}(FULL)", eItemsData[itemId][e_sItem_Name]);
            else format(labelStr, sizeof(labelStr), "%s\n(%0.2f)", eItemsData[itemId][e_sItem_Name], (itemAmount / 100));
        }

        case ITEM_TYPE_WEAPON:
        {
            if(IsPlayerWeaponFullOnAmmo(playerid, itemWeaponId)) format(labelStr, sizeof(labelStr), "%s\n{FF0000}(FULL)", eItemsData[itemId][e_sItem_Name]);
            else if(itemModel == ITEM_MODEL_SHIV || itemModel == ITEM_MODEL_BOMB 
            || itemModel == ITEM_MODEL_MOLOTOV || itemModel == ITEM_MODEL_SMKBMB || itemModel == ITEM_MODEL_ARROW) 
            {
                format(labelStr, sizeof(labelStr), "%s", eItemsData[itemId][e_sItem_Name]);
            }
            else format(labelStr, sizeof(labelStr), "%s\n(ammo: %d)", eItemsData[itemId][e_sItem_Name], floatround(itemAmount));
        }
    }

	ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel(labelStr, COLOR_WHITE_TRANS, x, y, z, ITEM_LABEL_DRAW_DIST, INVALID_PLAYER_ID, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);
    Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
	return 0;
}


//#region PickUp management callbacks (specific items type and main PickUp Item callback)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerPickUpItem -------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpItem(playerid, item_objectId)
    {
        Streamer_GetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

        new 
            itemUid = eObjData[e_iObj_Uid],
            itemId = eObjData[e_iObj_ItemId],
            itemAmount = eObjData[e_iObj_ItemAmount],
            itemMeleeUpgradedCnd = eObjData[e_iObj_MeleeUpgradedCnd],
            itemType = eItemsData[itemId][e_iItem_Type],
            itemModel = eItemsData[itemId][e_iItem_ModelId],
            itemWeaponId = (itemType == ITEM_TYPE_AMMO) ? GetWeaponIdForAmmoType(itemModel) : eItemsData[itemId][e_iItem_SA_WeaponId],
            Float:x = eObjData[e_fObj_X], 
            Float:y = eObjData[e_fObj_Y], 
            Float:z = eObjData[e_fObj_Z],
            virtualWorld = eObjData[e_iObj_Vw],
            interiorId = eObjData[e_iObj_Int];

        new 
            calcAmount,
            calcMeleeExtCnd,
            calcItemId_idxValue,
            bool:shouldUpdateObject;

        switch(itemType)
        {
            case ITEM_TYPE_INGREDIENT:
            {
                OnPlayerPickUpIngredient(playerid, itemUid, itemId, itemAmount, x, y, z, virtualWorld, interiorId);
            }

            case ITEM_TYPE_HEAL:
            {
                OnPlayerPickUpHealItem(playerid, itemId, itemAmount);
            }
        }



            // case ITEM_TYPE_MELEE:
            // {
            //     new playerMeleeWeapon = GetPlayerMelee[playerid];

            //     // If player has some melee weapon already - drop it on the ground.
            //     if(playerMeleeWeapon != NO_MELEE)
            //     {
            //         for(new i = 0; i != MAX_ITEMS_DATA; i++)
            //         {
            //             if(playerMeleeWeapon != eItemsData[i][e_iItem_SA_WeaponId])
            //                 continue;

            //             calcItemId_idxValue = i;
            //             shouldUpdateObject = true;
            //             calcAmount = GetPlayerMeleeCnd[playerid];

            //             if(IsPlayerMeleeUpgraded[playerid]) 
            //                 calcMeleeExtCnd = GetPlayerMeleeExtCnd[playerid];
                            
            //             break;
            //         }
            //     }

            //     SetPlayerItemAmount(playerid, itemId, itemAmount, itemMeleeUpgradedCnd);

            //     if(CurrentPlayerWeaponId[playerid] == playerMeleeWeapon && playerMeleeWeapon != NO_MELEE)
            //         SetPlayerMeleeWeapon(playerid);
            // }

            // case ITEM_TYPE_THROWABLE:
            // {
            //     new playerThrowable = GetPlayerThrowable[playerid];
            //     if(HasPlayerGotThrowableItem(playerid, itemWeaponId)) 
            //         return 1;

            //     // If player has some throwable already - drop it on the ground.
            //     if(playerThrowable != NO_THROWABLE && GetPlayerWeaponAmmo(playerid, playerThrowable) > 0)
            //     {
            //         for(new i = 0; i != MAX_ITEMS_DATA; i++)
            //         {
            //             if(playerThrowable != eItemsData[i][e_iItem_SA_WeaponId])
            //                 continue;

            //             calcItemId_idxValue = i;
            //             shouldUpdateObject = true;
            //             calcAmount = 1;
            //             break;
            //         }
            //     }

            //     // If player currently uses throwable item.
            //     if(IsThrowableWeapon(CurrentPlayerWeaponId[playerid])) SetPlayerWeapon(playerid, itemWeaponId);
            //     else
            //     {
            //         if(LastPlayerWeaponId[playerid] == playerThrowable)
            //             LastPlayerWeaponId[playerid] = itemWeaponId;
            //     }
                
            //     SetPlayerItemAmount(playerid, itemId, 1);
            // }

            // case ITEM_TYPE_WEAPON, ITEM_TYPE_AMMO:
            // {
                // if(IsPlayerWeaponFullOnAmmo(playerid, itemWeaponId)) 
                //     return 1;

                // calcAmount = GetPlayerWeaponAmmo(playerid, itemWeaponId) + itemAmount;
                // SetPlayerItemAmount(playerid, itemId, calcAmount);

                // // If player extended ammo for weapon - recreate same item with leftover ammo.
                // if(IsBulletWeapon(itemWeaponId) || itemWeaponId == WEAPON_BOW)
                // {
                //     new playerWeapon_MaxAmmo = GetPlayerWeaponLvl_MaxAmmo(playerid, itemWeaponId);
                //     if(playerWeapon_MaxAmmo == 0)
                //         return 1;
                        
                //     if(calcAmount > playerWeapon_MaxAmmo)
                //     {
                //         shouldUpdateObject = true;
                //         calcItemId_idxValue = itemId;
                //         calcAmount = calcAmount - playerWeapon_MaxAmmo;
                //     }
                // }
            //}

            //#region collectibles
                // case ITEM_TYPE_COLLECTIBLE:
                // {
                //     SCMF(playerid, -1, "My iter count: %d", Iter_Count(ArtifactsOwned<playerid>));
                //     SCMF(playerid, -1, "Trying to pick up UID: %d", itemUid);
                //     Iter_Add(ArtifactsOwned<playerid>, itemUid);
                //     SCMF(playerid, -1, "My iter count: %d", Iter_Count(ArtifactsOwned<playerid>));
                    
                //     strcpy(ePlayerBpInfo[playerid][e_iColl_ArtifTitle], eObjData[e_sObj_Title]);
                //     strcpy(ePlayerBpInfo[playerid][e_iColl_ArtifInnerMsg], eObjData[e_sObj_InnerMessage]);
                // }
            //#endregion
        //}

        if(eItemsData[itemId][e_iItem_AudioPickupId] != -1)
            Audio_PlayEx(playerid, eItemsData[itemId][e_iItem_AudioPickupId]);

        if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
        {
            GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
            DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
        }

        DestroyItem(item_objectId, shouldUpdateObject);

        // if(shouldUpdateObject)
        // {
        //     CreateOrUpdateItem(calcItemId_idxValue, calcAmount, calcMeleeExtCnd, playerid, \
        //                         .x = x, .y = y, .z = z, .virtualWorld = virtualWorld, .interiorId = interiorId, \
        //                             .update = true, .updateItemUid = itemUid);
        // }

        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpIngredient ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpIngredient(playerid, itemUid, itemId, itemAmount, Float:x, Float:y, Float:z, virtualWorld, interiorId)
    {
        // itemId = ingredientId (this is the ONLY case!)
        if(IsPlayerFullOnIngr(playerid, itemId))
        {
            return 1;
        }
        
        new calcAmount = Craft_IngrAmount(playerid, itemId) + itemAmount;
        SetPlayerItemAmount(playerid, itemId, calcAmount);

        if(calcAmount > 300)
        {
            CreateOrUpdateItem(itemId, .itemAmount = (calcAmount - 300), .creatorId = playerid, \
                    .x = x, .y = y, .z = z, .virtualWorld = virtualWorld, .interiorId = interiorId, \
                        .update = true, .updateItemUid = itemUid);
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //----------------- OnPlayerPickUpHealItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpHealItem(playerid, itemId, itemAmount)
    {
        new itemType = eItemsData[itemId][e_iItem_Type];
        new itemModel = itemModel = eItemsData[itemId][e_iItem_ModelId];
        new itemWeaponId = (itemType == ITEM_TYPE_AMMO) ? GetWeaponIdForAmmoType(itemModel) : eItemsData[itemId][e_iItem_SA_WeaponId];

        if(itemWeaponId == WEAPON_MEDKIT)
        {
            if(IsPlayerWeaponFullOnAmmo(playerid, WEAPON_MEDKIT))
            {
                return 1;
            }

            new calcAmount = GetPlayerWeaponAmmo(playerid, WEAPON_MEDKIT) + itemAmount;
            SetPlayerItemAmount(playerid, itemId, calcAmount);
        }
        else
        {
            SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) + HEAL_ITEM_RECOVER_HP);
        }
    }

    // CreateOrUpdateItem(itemId, itemAmount, meleeExtCnd = 0, creatorId = INVALID_PLAYER_ID, \
    //         itemTitle[] = "", innerMsg[] = "", Float:x = 0.0, Float:y = 0.0, Float:z = 0.0, virtualWorld = 0, interiorId = 0, bool:update = false, updateItemUid = -1)

//#endregion

//#region Items management callbacks (Create/Destroy/Update)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemCreate ----------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemCreate(itemId, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId)
    {
        new objectId = CreateDynamicObject(eItemsData[itemId][e_iItem_ModelId], x, y, z, rX, rY, rZ, virtualWorld, interiorId);

        if(creatorId != INVALID_PLAYER_ID)
        {
            Streamer_Update(creatorId, STREAMER_TYPE_OBJECT);
        }

        eObjData[e_iObj_Uid] = cache_insert_id();
        eObjData[e_iObj_ItemId] = itemId;
        eObjData[e_fObj_X] = x;
        eObjData[e_fObj_Y] = y;
        eObjData[e_fObj_Z] = z;
        eObjData[e_iObj_Vw] = virtualWorld;
        eObjData[e_iObj_Int] = interiorId;
        eObjData[e_iObj_ItemAmount] = itemAmount;
        eObjData[e_iObj_MeleeUpgradedCnd] = meleeExtCnd;
        strcpy(eObjData[e_sObj_Title], itemTitle, sizeof(eObjData[e_sObj_Title]));
        strcpy(eObjData[e_sObj_InnerMessage], innerMsg, sizeof(eObjData[e_sObj_InnerMessage]));
        eObjData[e_bObj_Created] = true;

        Streamer_SetArrayData(STREAMER_TYPE_OBJECT, objectId, E_STREAMER_EXTRA_ID, eObjData);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemDestroy ---------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemDestroy(uid_item)
    {
        DB_DestroyItem(uid_item);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemUpdate ----------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemUpdate(updateItemUid, newItemId, newItemAmount, newMeleeExtCnd, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, updaterId)
    {
        new objectId = CreateDynamicObject(eItemsData[newItemId][e_iItem_ModelId], x, y, z, rX, rY, rZ, virtualWorld, interiorId);

        if(updaterId != INVALID_PLAYER_ID)
        {
            Streamer_Update(updaterId, STREAMER_TYPE_OBJECT);
            Streamer_Update(updaterId, STREAMER_TYPE_3D_TEXT_LABEL);
        }

        eObjData[e_iObj_Uid] = updateItemUid;
        eObjData[e_iObj_ItemId] = newItemId;
        eObjData[e_fObj_X] = x;
        eObjData[e_fObj_Y] = y;
        eObjData[e_fObj_Z] = z;
        eObjData[e_iObj_Vw] = virtualWorld;
        eObjData[e_iObj_Int] = interiorId;
        eObjData[e_iObj_ItemAmount] = newItemAmount;
        eObjData[e_iObj_MeleeUpgradedCnd] = newMeleeExtCnd;
        strcpy(eObjData[e_sObj_Title], "", sizeof(eObjData[e_sObj_Title]));
        strcpy(eObjData[e_sObj_InnerMessage], "", sizeof(eObjData[e_sObj_InnerMessage]));
        eObjData[e_bObj_Created] = true;

        Streamer_SetArrayData(STREAMER_TYPE_OBJECT, objectId, E_STREAMER_EXTRA_ID, eObjData);
        return 0;
    }

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerKeyStateChange ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }
    
    if(PRESSED(KEY_NO))
    {
        if(IsPlayerLookingAtItem(playerid))
            OnPlayerPickUpItem(playerid, GetPlayerFocusedObjectId(playerid));
    }
	return 1;
}