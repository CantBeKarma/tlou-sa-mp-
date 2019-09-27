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

//#region PickUp management callbacks (specific items type and main PickUp Item callback)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerTryPickupItem ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerTryPickupItem(playerid)
    {
        foreach(new sprite : SpritesCreated<playerid>)
        {
            new itemId, item_objectId = ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][sprite];
            ReadItemRelation(item_objectId, .itemId = itemId);

            if(IsPlayerFullOnItem(playerid, itemId)
            && (IsItemType(itemId, ITEM_TYPE_INGREDIENT) || IsItemType(itemId, ITEM_TYPE_MEDKIT)
            || IsItemType(itemId, ITEM_TYPE_WEAPON) || IsItemType(itemId, ITEM_TYPE_THROWABLE)
            || IsItemType(itemId, ITEM_TYPE_PROJECTILE)))
            {
                continue;
            }

            new nextAction = OnPlayerPickUpItem(playerid, item_objectId);
            if(nextAction == NEXT_ACTION_NONE)
            {
                continue;
            }
            else
            {
                Sprite2D_UpdateTextNearbyItems(playerid, item_objectId);
                break;
            }
        }
        return 1;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerPickUpItem -------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerPickUpItem(playerid, item_objectId)
    {
        new nextAction = NEXT_ACTION_NONE;

        new itemUid, itemId, itemTypeId;
        ReadItemRelation(item_objectId, .itemUid = itemUid, .itemId = itemId, .itemTypeId = itemTypeId);

        if(itemTypeId == ITEM_TYPE_ARTIFACT || itemTypeId == ITEM_TYPE_FIREFLY_PEND)
        {
            nextAction = OnPlayerPickUpCollectible(playerid, itemUid, itemTypeId);
        }
        else
        {
            new itemAmount, itemExtAmount;
            ReadItemRelation(item_objectId, .itemAmount = itemAmount, .itemExtAmount = itemExtAmount);
            
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

            if(nextAction == NEXT_ACTION_UPDATE)
            {
                new 
                    Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId,
                    recreateItemId = (IsItemType(itemId, ITEM_TYPE_MELEE) || IsItemType(itemId, ITEM_TYPE_THROWABLE) ? (dropItemId) : (itemId));

                ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

                DestroyItem(item_objectId, .update = true);
                CreateOrUpdateItem(recreateItemId, dropItemAmount, dropItemExtCnd, playerid, itemPosX, itemPosY, itemPosZ, itemVw, itemIntId, true, itemUid);
            }
        }

        if(nextAction == NEXT_ACTION_NONE)
        {
            return NEXT_ACTION_NONE;
        }
        else if(nextAction == NEXT_ACTION_DESTROY)
        {
            DestroyItem(item_objectId);
        }

        Audio_PlayEx(playerid, 
                          (itemTypeId == ITEM_TYPE_ARTIFACT || itemTypeId == ITEM_TYPE_FIREFLY_PEND) 
                        ? (eCollectiblesData[itemId][e_iCollectible_AudioPickupId]) 
                        : (eItemsData[itemId][e_iItem_AudioPickupId])
                    );

        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerPickUpItem: Player %s (UID: %d | GUID: %d) picked up itemUid: %d (nextAction: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), itemUid, nextAction);
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
    public OnPlayerPickUpCollectible(playerid, itemUid, itemTypeId)
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
        return NEXT_ACTION_DESTROY;
    }

//#endregion

//#region Items management callbacks (Create/Destroy/Update)

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemCreate ----------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemCreate(item_objectId, itemId, itemType, Float:x, Float:y, Float:z, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId)
    {
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
        Streamer_SetIntData(STREAMER_TYPE_AREA, eObjData[e_iObj_SpriteAreaId], E_STREAMER_EXTRA_ID, item_objectId);

        if(creatorId != INVALID_PLAYER_ID)
        {
            Streamer_Update(creatorId, STREAMER_TYPE_OBJECT);
            Streamer_Update(creatorId, STREAMER_TYPE_AREA);
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //--------------------- OnItemDestroy ---------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnItemDestroy(item_objectId, bool:updating)
    {
        if(!updating)
        {
            new itemUid;
            ReadItemRelation(item_objectId, itemUid);
            DB_DestroyItem(itemUid);
        }

        new itemAreaId;
        ReadItemRelation(item_objectId, .itemAreaId = itemAreaId);
        DestroyDynamicArea(itemAreaId);

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
        Streamer_SetIntData(STREAMER_TYPE_AREA, eObjData[e_iObj_SpriteAreaId], E_STREAMER_EXTRA_ID, item_objectId);

        if(updaterId != INVALID_PLAYER_ID)
        {
            Streamer_Update(updaterId, STREAMER_TYPE_OBJECT);
            Streamer_Update(updaterId, STREAMER_TYPE_AREA);
        }
        return 0;
    }

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerEnterDynamicArea ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerEnterDynArea(playerid, areaid)
{
    new areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);
    if(areaObjectId > 0)
	{
        Sprite2D_Create(playerid, areaObjectId);
	}
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerLeaveDynArea ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerLeaveDynArea(playerid, areaid)
{
    new areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);
    if(areaObjectId > 0)
	{
		Sprite2D_Destroy(playerid, areaObjectId);
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
    
    if(RELEASED(KEY_NO))
    {
        if(HasPlayerGotAnyItemsToPickUp(playerid) 
        && !IsPlayerUsingBackpack(playerid)
        && !IsPlayerUsingBackpackAnims(playerid))
        {
            OnPlayerTryPickupItem(playerid);
        }
    }
	return 1;
}