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
    forward OnPlayerPickUpIngredient(playerid, item_objectId, &newItemAmount);
    public OnPlayerPickUpIngredient(playerid, item_objectId, &newItemAmount)
    {
        new itemId, itemAmount;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = itemAmount);

        // itemId = ingredientId (this is the ONLY case!)
        if(IsPlayerFullOnItem(playerid, itemid))
        {
            return NEXT_ACTION_NONE;
        }
        
        newItemAmount = Craft_IngrAmount(playerid, itemId) + itemAmount;
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new nextAction = (newItemAmount > 300) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            newItemAmount = (newItemAmount-300);
        }
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //----------------- OnPlayerPickUpHealItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    forward OnPlayerPickUpHealItem(playerid, item_objectId);
    public OnPlayerPickUpHealItem(playerid, item_objectId)
    {
        new itemId, itemAmount, itemWeaponId;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = itemAmount, .itemWeaponId = itemWeaponId);

        if(itemWeaponId == WEAPON_MEDKIT)
        {
            if(IsPlayerWeaponFullOnAmmo(playerid, WEAPON_MEDKIT))
            {
                return NEXT_ACTION_NONE;
            }

            new newItemAmount = GetPlayerWeaponAmmo(playerid, WEAPON_MEDKIT) + itemAmount;
            SetPlayerItemAmount(playerid, itemId, newItemAmount);
        }
        else
        {
            SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) + HEAL_ITEM_RECOVER_HP);
        }
        return NEXT_ACTION_DESTROY;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpMeleeItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    forward OnPlayerPickUpMeleeItem(playerid, item_objectId, &droppedMeleeItemIdx, &droppedMeleeCnd, &droppedMeleeExtCnd);
    public OnPlayerPickUpMeleeItem(playerid, item_objectId, &droppedMeleeItemIdx, &droppedMeleeCnd, &droppedMeleeExtCnd)
    {
        new playerMeleeWeapon = GetPlayerMelee[playerid];

        new nextAction = (playerMeleeWeapon != NO_MELEE) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            new playerMeleeItemIdx = droppedMeleeItemIdx = FindItemIdxByWeaponId(playerMeleeWeapon);
            if(playerMeleeItemIdx == -1)
            {
                return NEXT_ACTION_NONE;
            }

            droppedMeleeCnd = GetPlayerMeleeCnd[playerid];
            droppedMeleeExtCnd = (IsPlayerMeleeUpgraded[playerid] ? GetPlayerMeleeExtCnd[playerid] : 0);
        }

        new itemId, meleeCnd, meleeExtCnd;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = meleeCnd, .itemExtAmount = meleeExtCnd);
        SetPlayerItemAmount(playerid, itemId, meleeCnd, meleeExtCnd);

        // If player currently has melee weapon equipped > Hide it.
        if(CurrentPlayerWeaponId[playerid] == playerMeleeWeapon && playerMeleeWeapon != NO_MELEE)
        {
            HideItem(playerid);
        }
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------- OnPlayerPickUpThrowableItem --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    forward OnPlayerPickUpThrowableItem(playerid, item_objectId, &droppedThrowableItemIdx);
    public OnPlayerPickUpThrowableItem(playerid, item_objectId, &droppedThrowableItemIdx)
    {
        new itemId, itemWeaponId;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemWeaponId = itemWeaponId);

        if(HasPlayerGotThrowableItem(playerid, itemWeaponId))
        {
            return NEXT_ACTION_NONE;
        }

        new playerThrowable = GetPlayerThrowable[playerid];
        new nextAction = HasPlayerGotThrowableItem(playerid) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

        if(nextAction == NEXT_ACTION_UPDATE)
        {
            new playerThrowableItemIdx = droppedThrowableItemIdx = FindItemIdxByWeaponId(playerThrowable);
            if(playerThrowableItemIdx == -1)
            {
                return NEXT_ACTION_NONE;
            }
        }

        SetPlayerItemAmount(playerid, itemId, 1);

        // If player currently uses throwable item.
        if(IsThrowableWeapon(CurrentPlayerWeaponId[playerid]))
        {
            //SetPlayerWeapon(playerid, itemWeaponId);
        }
        // If player last weapon is his throwable item.
        else if(LastPlayerWeaponId[playerid] == playerThrowable)
        {
            LastPlayerWeaponId[playerid] = itemWeaponId;                
        }
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------- OnPlayerPickUpWeaponOrAmmo --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    forward OnPlayerPickUpWeaponOrAmmo(playerid, item_objectId, &droppedAmmoAmount);
    public OnPlayerPickUpWeaponOrAmmo(playerid, item_objectId, &droppedAmmoAmount)
    {
        new itemId, itemAmount, itemWeaponId;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = itemAmount, .itemWeaponId = itemWeaponId);

        if(IsPlayerWeaponFullOnAmmo(playerid, itemWeaponId))
        {
            return NEXT_ACTION_NONE;
        }

        new newItemAmount = GetPlayerWeaponAmmo(playerid, itemWeaponId) + itemAmount;
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new nextAction;
        if(IsBulletWeapon(itemWeaponId) || itemWeaponId == WEAPON_BOW)
        {
            new playerWeapon_MaxAmmo = GetPlayerWeaponLvl_MaxAmmo(playerid, itemWeaponId);
            nextAction = (newItemAmount > playerWeapon_MaxAmmo) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

            if(nextAction == NEXT_ACTION_UPDATE)
            {
                droppedAmmoAmount = (newItemAmount-playerWeapon_MaxAmmo);
            }
        }
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