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
    //ShowPlayerItemLabel(playerid, item_objectId);
	return 0;
}

//#region PickUp management callbacks (specific items type and main PickUp Item callback)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerTryPickupItem ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerTryPickupItem(playerid, item_objectId)
    {
        new itemUid, itemId, itemTypeId;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemTypeId = itemTypeId);

        if(itemTypeId == ITEM_TYPE_ARTIFACT || itemTypeId == ITEM_TYPE_FIREFLY_PEND)
        {
            OnPlayerPickUpCollectible(playerid, itemId, itemUid, itemTypeId, item_objectId);
            return 0;
        }

        if(IsPlayerFullOnItem(playerid, itemId)
        && (IsItemType(itemId, ITEM_TYPE_INGREDIENT) || IsItemType(itemId, ITEM_TYPE_MEDKIT)
        || IsItemType(itemId, ITEM_TYPE_WEAPON) || IsItemType(itemId, ITEM_TYPE_THROWABLE)
        || IsItemType(itemId, ITEM_TYPE_PROJECTILE)))
        {
            return NEXT_ACTION_NONE;
        }

        new itemAmount, itemExtAmount;
        if(IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            ReadItemRelation(item_objectId, .itemUid = itemUid, .itemAmount = itemAmount, .itemExtAmount = itemExtAmount);
        }
        else
        {
            ReadItemRelation(item_objectId, .itemUid = itemUid, .itemAmount = itemAmount);
        }

        OnPlayerPickUpItem(playerid, itemId, itemAmount, itemExtAmount, item_objectId, itemUid);
        return 1;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerPickUpItem -------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpItem(playerid, itemId, itemAmount, itemExtAmount, item_objectId, itemUid)
    {
        new nextAction = NEXT_ACTION_NONE;

        new
            dropItemId,
            dropItemAmount = 1,
            dropItemExtCnd = 0;

        switch(GetItemType(itemId))
        {
            case ITEM_TYPE_INGREDIENT:  nextAction = OnPlayerPickUpIngredient(playerid, itemId, itemAmount, dropItemAmount);
            case ITEM_TYPE_HEAL:        nextAction = OnPlayerPickUpHealItem(playerid, itemId, itemAmount);
            case ITEM_TYPE_MELEE:       nextAction = OnPlayerPickUpMeleeItem(playerid, itemId, itemAmount, itemExtAmount, dropItemId, dropItemAmount, dropItemExtCnd);
            case ITEM_TYPE_THROWABLE:   nextAction = OnPlayerPickUpThrowableItem(playerid, itemId, dropItemId);
            case ITEM_TYPE_PROJECTILE:  nextAction = OnPlayerPickUpProjectileItem(playerid, itemId, itemAmount);
            case ITEM_TYPE_WEAPON:      nextAction = OnPlayerPickUpWeapon(playerid, itemId, itemAmount, dropItemAmount);
        }

        switch(nextAction)
        {
            case NEXT_ACTION_NONE:
            {
                return NEXT_ACTION_NONE;
            }

            case NEXT_ACTION_DESTROY:
            {
                DestroyItem(item_objectId);
            }

            case NEXT_ACTION_UPDATE:
            {
                new Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
                ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

                DestroyItem(item_objectId, .update = true);

                new recreateItemId = (IsItemType(itemId, ITEM_TYPE_MELEE) || IsItemType(itemId, ITEM_TYPE_THROWABLE) ? (dropItemId) : (itemId));

                CreateOrUpdateItem(recreateItemId, dropItemAmount, dropItemExtCnd, playerid, itemPosX, itemPosY, itemPosZ, itemVw, itemIntId, true, itemUid);
            }
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

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpItem: charUid: %d pickup itemUid: %d (nextAction: %d)", GetPlayerCharUid(playerid), itemUid, nextAction);
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpIngredient ---------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpIngredient(playerid, itemId, itemAmount, &dropItemAmount)
    {
        new newItemAmount = (GetPlayerItemAmount(playerid, itemId) + itemAmount);
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new 
            ingrMaxAmount = GetPlayerItemMaxAmount(playerid, itemId),
            nextAction = (newItemAmount > ingrMaxAmount) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

        if(nextAction == NEXT_ACTION_UPDATE)
        {
            dropItemAmount = (newItemAmount - ingrMaxAmount);
        }
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //----------------- OnPlayerPickUpHealItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpHealItem(playerid, itemId, itemAmount)
    {
        if(IsItemType(itemId, ITEM_TYPE_MEDKIT))
        {
            new newItemAmount = (GetPlayerItemAmount(playerid, itemId) + itemAmount);
            GivePlayerItem(playerid, itemId);
            SetPlayerItemAmount(playerid, itemId, newItemAmount);
        }
        else
        {
            SetPlayerHealthEx(playerid, (GetPlayerHealthEx(playerid) + HEAL_ITEM_RECOVER_HP) + random(5));
        }
        return NEXT_ACTION_DESTROY;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpMeleeItem ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpMeleeItem(playerid, itemId, itemAmount, itemExtAmount, &dropItemId, &dropItemAmount, &dropItemExtCnd)
    {
        new playerMeleeWeaponItemId = GetPlayerMeleeItemId(playerid);

        new nextAction = (playerMeleeWeaponItemId != -1) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            dropItemId = playerMeleeWeaponItemId;
            dropItemAmount = GetPlayerItemAmount(playerid, playerMeleeWeaponItemId);
            dropItemExtCnd = GetPlayerMeleeExtraCnd(playerid, playerMeleeWeaponItemId);

            RemovePlayerItem(playerid, playerMeleeWeaponItemId);
        }

        if(GetPlayerEquippedItemId(playerid) == playerMeleeWeaponItemId && playerMeleeWeaponItemId != -1)
        {
            HideItem(playerid);
        }

        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, itemAmount, itemExtAmount);
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------- OnPlayerPickUpThrowableItem --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpThrowableItem(playerid, itemId, &dropItemId)
    {
        new playerThrowableItemId = GetPlayerThrowableItemId(playerid);

        new nextAction = (playerThrowableItemId != -1) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);
        if(nextAction == NEXT_ACTION_UPDATE)
        {
            dropItemId = playerThrowableItemId;
            RemovePlayerItem(playerid, playerThrowableItemId);
        }

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
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------- OnPlayerPickUpProjectileItem --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpProjectileItem(playerid, itemId, itemAmount)
    {
        new newItemAmount = (GetPlayerItemAmount(playerid, itemId) + itemAmount);
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        return NEXT_ACTION_DESTROY;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerPickUpWeapon -----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpWeapon(playerid, itemId, itemAmount, &dropItemAmount)
    {
        new newItemAmount = (GetPlayerItemAmount(playerid, itemId) + itemAmount);
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, newItemAmount);

        new 
            playerWeapon_MaxAmmo = GetPlayerItemMaxAmount(playerid, itemId),
            nextAction = (newItemAmount > playerWeapon_MaxAmmo) ? (NEXT_ACTION_UPDATE) : (NEXT_ACTION_DESTROY);

        if(nextAction == NEXT_ACTION_UPDATE)
        {
            dropItemAmount = (newItemAmount - playerWeapon_MaxAmmo);
        }
        return nextAction;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerPickUpCollectible --------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpCollectible(playerid, itemId, itemUid, itemTypeId, item_objectId)
    {
        if(itemTypeId == ITEM_TYPE_ARTIFACT)
        {
            Iter_Add(ArtifactsOwned<playerid>, itemUid);
        }
        else if(itemTypeId == ITEM_TYPE_FIREFLY_PEND)
        {
            Iter_Add(PendantsOwned<playerid>, itemUid);
        }
        else
        {
            return NEXT_ACTION_NONE;
        }

        DB_GiveCharacterCollectible(playerid, itemUid);
        DestroyItem(item_objectId);

        if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
        {
            GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
            DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
        }

        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);

        Audio_PlayEx(playerid, eCollectiblesData[itemId][e_iCollectible_AudioPickupId]);

        return NEXT_ACTION_DESTROY;
    }

//#endregion

//#region Items management callbacks (Create/Destroy/Update)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemCreate ----------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemCreate(item_objectId, itemId, itemType, Float:x, Float:y, Float:z, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId)
    {
        eObjData[e_iObj_ObjectId] = item_objectId;
        eObjData[e_iObj_Uid] = cache_insert_id();
        eObjData[e_iObj_ItemId] = itemId;
        eObjData[e_iObj_TypeId] = itemType;
        eObjData[e_fObj_X] = x;
        eObjData[e_fObj_Y] = y;
        eObjData[e_fObj_Z] = z;
        eObjData[e_iObj_Vw] = virtualWorld;
        eObjData[e_iObj_Int] = interiorId;
        eObjData[e_iObj_ItemAmount] = itemAmount;
        eObjData[e_iObj_MeleeUpgradedCnd] = meleeExtCnd;

        if(!isnull(itemTitle) && !isnull(innerMsg))
        {
            strcpy(eObjData[e_sObj_Title], itemTitle, sizeof(eObjData[e_sObj_Title]));
            strcpy(eObjData[e_sObj_InnerMessage], innerMsg, sizeof(eObjData[e_sObj_InnerMessage]));
        }

        eObjData[e_iObj_SpriteAreaId] = CreateDynamicSphere(x, y, z, ITEM_SPRITE_SPHERE_SIZE, virtualWorld, interiorId);
        eObjData[e_bObj_Created] = true;

        Streamer_SetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

        if(creatorId != INVALID_PLAYER_ID)
        {
            Streamer_Update(creatorId, STREAMER_TYPE_OBJECT);
            Streamer_Update(creatorId, STREAMER_TYPE_AREA);
        }

        SCMF_ALL(-1, "Created item of objectid: %d (var: %d)", item_objectId, eObjData[e_iObj_ObjectId]);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemDestroy ---------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemDestroy(item_objectId, bool:updating)
    {
        if(!updating)
        {
            DB_DestroyItem(eObjData[e_iObj_Uid]);
        }

        DestroyDynamicArea(eObjData[e_iObj_SpriteAreaId]);

        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_ObjectId]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Uid]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_ItemId]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_X]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_Y]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, _:eObjData[e_fObj_Z]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Vw]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_Int]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_ItemAmount]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_MeleeUpgradedCnd]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_sObj_Title]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_sObj_InnerMessage]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_bObj_Created]);
        Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData[e_iObj_SpriteAreaId]);

        DestroyDynamicObject(item_objectId);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemUpdate ----------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemUpdate(item_objectId, itemId, itemType, Float:x, Float:y, Float:z, virtualWorld, interiorId, newItemAmount, newMeleeExtCnd, updateItemUid, updaterId)    
    {
        eObjData[e_iObj_ObjectId] = item_objectId;
        eObjData[e_iObj_Uid] = updateItemUid;
        eObjData[e_iObj_ItemId] = itemId;
        eObjData[e_iObj_TypeId] = itemType;
        eObjData[e_fObj_X] = x;
        eObjData[e_fObj_Y] = y;
        eObjData[e_fObj_Z] = z;
        eObjData[e_iObj_Vw] = virtualWorld;
        eObjData[e_iObj_Int] = interiorId;
        eObjData[e_iObj_ItemAmount] = newItemAmount;
        eObjData[e_iObj_MeleeUpgradedCnd] = newMeleeExtCnd;
        eObjData[e_sObj_Title][0] = EOS;
        eObjData[e_sObj_InnerMessage][0] = EOS;
        eObjData[e_iObj_SpriteAreaId] = CreateDynamicSphere(x, y, z, ITEM_SPRITE_SPHERE_SIZE, virtualWorld, interiorId);
        eObjData[e_bObj_Created] = true;

        Streamer_SetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

        if(updaterId != INVALID_PLAYER_ID)
        {
            Streamer_Update(updaterId, STREAMER_TYPE_OBJECT);
            Streamer_Update(updaterId, STREAMER_TYPE_AREA);
        }

        SCMF_ALL(-1, "Updated item. objectid: %d (var: %d)", item_objectId, eObjData[e_iObj_ObjectId]);
        return 0;
    }

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerEnterDynamicArea ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerEnterDynArea(playerid, areaid)
{
	if(areaid == eObjData[e_iObj_SpriteAreaId])
	{
        SCMF(playerid, -1, "You've entered area of itemId: %d (UID: %d) (objectid: %d)", eObjData[e_iObj_ItemId], eObjData[e_iObj_Uid], eObjData[e_iObj_ObjectId]);
        Sprite2D_Create(playerid, eObjData[e_iObj_ObjectId]);
	}
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerLeaveDynArea ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerLeaveDynArea(playerid, areaid)
{
	if(areaid == eObjData[e_iObj_SpriteAreaId])
	{
        SCMF(playerid, -1, "You've left area of itemId: %d (UID: %d)", eObjData[e_iObj_ItemId], eObjData[e_iObj_Uid]);
		Sprite2D_Destroy(playerid, areaid);
	}
    return 1;
}

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
            OnPlayerTryPickupItem(playerid, GetPlayerFocusedObjectId(playerid));
        }
    }
	return 1;
}