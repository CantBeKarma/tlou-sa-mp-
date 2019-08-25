#include	<YSI\y_hooks>

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
    ShowPlayerItemLabel(playerid, item_objectId);
	return 0;
}

//#region PickUp management callbacks (specific items type and main PickUp Item callback)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerPickUpItem -------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpItem(playerid, item_objectId)
    {
        #define     NEXT_ACTION_NONE        (0)
        #define     NEXT_ACTION_UPDATE      (1)
        #define     NEXT_ACTION_DESTROY     (2)

        new nextAction = NEXT_ACTION_NONE;

        new itemId, itemType;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemType = itemType);

        new
            newItemIdx,
            newItemAmount,
            newItemExtAmount;

        switch(itemType)
        {
            case ITEM_TYPE_INGREDIENT:  nextAction = OnPlayerPickUpIngredient(playerid, item_objectId, newItemAmount);
            case ITEM_TYPE_HEAL:        nextAction = OnPlayerPickUpHealItem(playerid, item_objectId);
            case ITEM_TYPE_MELEE:       nextAction = OnPlayerPickUpMeleeItem(playerid, item_objectId, newItemIdx, newItemAmount, newItemExtAmount);
            case ITEM_TYPE_THROWABLE:   nextAction = OnPlayerPickUpThrowableItem(playerid, item_objectId, newItemIdx);
            case 
                ITEM_TYPE_WEAPON, 
                ITEM_TYPE_AMMO:
                {
                    nextAction = OnPlayerPickUpWeaponOrAmmo(playerid, item_objectId, newItemAmount);
                }
        }

        if(nextAction == NEXT_ACTION_NONE)
        {
            return 1;
        }

        new bool:updateItem = (nextAction == NEXT_ACTION_UPDATE);

        new itemUid, Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
        if(updateItem)
        {
            ReadItemRelation(item_objectId, .itemUid = itemUid, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);
        }

        DestroyItem(item_objectId, updateItem);

        if(updateItem)
        {
            if(itemType == ITEM_TYPE_MELEE
            || itemType == ITEM_TYPE_THROWABLE)
            {
                itemId = newItemIdx;
            }
            else
            {
                newItemExtAmount = 0;
            }

            CreateOrUpdateItem(itemId, newItemAmount, newItemExtAmount, playerid, \
                    "", "", itemPosX, itemPosY, itemPosZ, itemVw, itemIntId, true, itemUid);
        }

        if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
        {
            GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
            DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
        }

        if(eItemsData[itemId][e_iItem_AudioPickupId] != -1)
        {
            Audio_PlayEx(playerid, eItemsData[itemId][e_iItem_AudioPickupId]);
        }

        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpIngredient ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpIngredient(playerid, item_objectId, &newItemAmount)
    {
        new itemUid, itemId, itemAmount;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemAmount = itemAmount);

        if(IsPlayerFullOnItem(playerid, itemId))
        {
            return NEXT_ACTION_NONE;
        }
        
        newItemAmount = (GetPlayerItemAmount(playerid, itemId) + itemAmount);
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new ingrMaxAmount = GetPlayerItemMaxAmount(playerid, itemId);
        new nextAction = (newItemAmount > ingrMaxAmount) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            newItemAmount = (newItemAmount - ingrMaxAmount);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpIngredient: charUid: %d pickup itemUid: %d (nextAction: %d)", GetPlayerCharUid(playerid), itemUid, nextAction);
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //----------------- OnPlayerPickUpHealItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpHealItem(playerid, item_objectId)
    {
        new itemUid, itemId, itemAmount;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemAmount = itemAmount);

        if(itemId == _:ITEM_IDX_MEDKIT)
        {
            if(IsPlayerFullOnItem(playerid, itemId))
            {
                return NEXT_ACTION_NONE;
            }

            new newItemAmount = GetPlayerItemAmount(playerid, itemId) + itemAmount;
            GivePlayerItem(playerid, itemId);
            SetPlayerItemAmount(playerid, itemId, newItemAmount);
        }
        else
        {
            SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) + HEAL_ITEM_RECOVER_HP);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpHealItem: charUid: %d pickup itemUid: %d", GetPlayerCharUid(playerid), itemUid);
        return NEXT_ACTION_DESTROY;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpMeleeItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpMeleeItem(playerid, item_objectId, &droppedMeleeItemIdx, &droppedMeleeCnd, &droppedMeleeExtCnd)
    {
        new playerMeleeWeaponItemId = GetPlayerMeleeItemId(playerid);

        new nextAction = (playerMeleeWeaponItemId != -1) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            droppedMeleeItemIdx = playerMeleeWeaponItemId;
            droppedMeleeCnd = GetPlayerItemAmount(playerid, playerMeleeWeaponItemId);
            droppedMeleeExtCnd = GetPlayerMeleeExtraCnd(playerid, playerMeleeWeaponItemId);
            RemovePlayerItem(playerid, playerMeleeWeaponItemId);
        }

        new itemUid, itemId, meleeCnd, meleeExtCnd;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemAmount = droppedMeleeCnd, .itemExtAmount = meleeExtCnd);

        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, meleeCnd, meleeExtCnd);

        // If player currently has melee weapon equipped > Hide it.
        if(GetPlayerEquippedItemId(playerid) == playerMeleeWeaponItemId && playerMeleeWeaponItemId != -1)
        {
            HideItem(playerid);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpMeleeItem: charUid: %d pickup itemUid: %d (nextAction: %d)", GetPlayerCharUid(playerid), itemUid, nextAction);
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------- OnPlayerPickUpThrowableItem --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpThrowableItem(playerid, item_objectId, &droppedThrowableItemIdx)
    {
        new itemUid, itemId;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId);

        if(HasPlayerGotItem(playerid, itemId))
        {
            return NEXT_ACTION_NONE;
        }

        new playerThrowableItemId = GetPlayerThrowableItemId(playerid);

        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, 1);

        if(GetPlayerEquippedItemId(playerid) == playerThrowableItemId)
        {
            EquipItem(playerid, itemId);
        }
        else if(GetPlayerLastEquippedItemId(playerid) == playerThrowableItemId)
        {
            ePlayerInfo[playerid][e_iLastEquippedItemId] = itemId;                
        }

        new nextAction = (playerThrowableItemId != -1) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

        if(nextAction == NEXT_ACTION_UPDATE)
        {
            droppedThrowableItemIdx = playerThrowableItemId;
            RemovePlayerItem(playerid, playerThrowableItemId);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpThrowableItem: charUid: %d pickup itemUid: %d (nextAction: %d)", GetPlayerCharUid(playerid), itemUid, nextAction);
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------- OnPlayerPickUpWeaponOrAmmo --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpWeaponOrAmmo(playerid, item_objectId, &droppedAmmoAmount)
    {
        new itemUid, itemId, itemAmount;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemAmount = itemAmount);

        if(IsPlayerFullOnItem(playerid, itemId))
        {
            return NEXT_ACTION_NONE;
        }

        new newItemAmount = GetPlayerItemAmount(playerid, itemId) + itemAmount;
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new nextAction;
        if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
        {
            new playerWeapon_MaxAmmo = GetPlayerItemMaxAmount(playerid, itemId);
            nextAction = (newItemAmount > playerWeapon_MaxAmmo) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

            if(nextAction == NEXT_ACTION_UPDATE)
            {
                droppedAmmoAmount = (newItemAmount - playerWeapon_MaxAmmo);
            }
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpWeaponOrAmmo: charUid: %d pickup itemUid: %d (nextAction: %d)", GetPlayerCharUid(playerid), itemUid, nextAction);
        return nextAction;
    }

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
        {
            OnPlayerPickUpItem(playerid, GetPlayerFocusedObjectId(playerid));
        }
    }
	return 1;
}