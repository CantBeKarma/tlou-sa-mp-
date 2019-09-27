// Create 2D sprite for player at specified position. Sprite is created only in dynamic areas.
Sprite2D_Create(playerid, item_objectId)
{
    new bool:itemCreated;
    ReadItemRelation(item_objectId, .itemCreated = itemCreated);

    if(!itemCreated)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Tried to create 2D sprite for object which is not created (doesn't contain data set?)");
        return false;
    }

    new iter = Iter_Alloc(SpritesCreated<playerid>);
    if(iter == INVALID_ITERATOR_SLOT)
	{
        //Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Sprite2D_Create: Unable to create 2D sprite for: %s (UID: %d | GUID: %d) (iter count: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), Iter_Count(SpritesCreated<playerid>));
		return false;
	}

	new spriteObjectId = Sprite2D_CreateTransObject(playerid, item_objectId, iter);
    if(spriteObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Unable to create item's SPRITE dynamic object!");
        return false;
    }

    if(!ApplyObjectCustomMaterial(item_objectId, spriteObjectId))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Object's material wasn't changed! (SetDynamicObjectMaterial failure)");
        return false;
    }

    new spriteTextObjectId = Sprite2D_CreateAmountText(playerid, item_objectId, iter);

	ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][iter] = repeat UpdateSpriteTimer(playerid, spriteObjectId, spriteTextObjectId);
    ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][iter] = item_objectId;

    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
	return true;
}

// Destroy created 2D sprite.
Sprite2D_Destroy(playerid, item_objectId)
{
    foreach(new iter : SpritesCreated<playerid>)
    {
        if(ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][iter] == item_objectId)
        {
            stop ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][iter];
            DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][iter]);
            DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][iter]);
            Iter_Remove(SpritesCreated<playerid>, iter);
            return true;
        }
    }
    return false;
}

// Find 2D Sprite index (to use it for static table list) of ingredients.
Sprite2D_FindIngrSpriteIdx(ingr, amount)
{
    for(new i = 0; i != MAX_2D_INGR_SPRITES; i++)
    {
        if(ingr == _:Sprites2D_Ingredients[i][e_iIngrId]
        && amount == Sprites2D_Ingredients[i][e_iIngrValue])
        {
            return i;
        }
    }
    return -1;
}

//#region Creating transparent objects and applying textures on them

    // Creates transparent object which's texture is going to be changed (2D Sprite)
    Sprite2D_CreateTransObject(playerid, item_objectId, iter)
    {
        new Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
        ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

        new spriteObjectId = INVALID_OBJECT_ID;
        spriteObjectId = ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][iter] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.4, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST);

        return spriteObjectId;
    }

    // Creates transparent object for displaying text (to display item's amount)
    Sprite2D_CreateAmountText(playerid, item_objectId, iter)
    {
        new itemId, itemAmount;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = itemAmount);

        if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            return INVALID_OBJECT_ID;
        }

        new Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
        ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

        new spriteTextObjectId = INVALID_OBJECT_ID;
        spriteTextObjectId = ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][iter] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.3, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST);

        new spriteTextStr[10];
        if(IsItemType(itemId, ITEM_TYPE_WEAPON))
        {
            if(IsPlayerFullOnItem(playerid, itemId))
            {
                format(spriteTextStr, sizeof(spriteTextStr), "FULL");
            }
            else
            {
                format(spriteTextStr, sizeof(spriteTextStr), "%d", itemAmount);
            }
        }
        else if(IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            new itemExtAmount;
            ReadItemRelation(item_objectId, .itemExtAmount = itemExtAmount);

            new bool:isItemMeleeUpgraded = (itemExtAmount > 0);

            format(spriteTextStr, sizeof(spriteTextStr), "%d/%d", \
                    (isItemMeleeUpgraded) ? (itemAmount + itemExtAmount) : (itemAmount), \
                    (isItemMeleeUpgraded) ? GetMeleeItemMaxCND(itemId, .extraCnd = false) + GetMeleeItemMaxCND(itemId, .extraCnd = true) : GetMeleeItemMaxCND(itemId, .extraCnd = false));
        }

        SetDynamicObjectMaterialText(spriteTextObjectId, 0, spriteTextStr, .fontsize = SPRITE_FONTSIZE_ITEM_AMOUNT, .fontcolor = 0xFFFF0000);

        return spriteTextObjectId;
    }

    // Injects material (texture) onto created transparent object.
    ApplyObjectCustomMaterial(item_objectId, spriteObjectId)
    {
        new itemId, itemTypeId, itemAmount;
        ReadItemRelation(item_objectId, .itemId = itemId, .itemTypeId = itemTypeId, .itemAmount = itemAmount);

        if(itemTypeId == ITEM_TYPE_ITEM && !IsItemType(itemId, ITEM_TYPE_INGREDIENT))
        {
            SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, SPRITES2D_TXD_FILE_NAME, Sprites2D_Items[itemId]);
        }
        else if(itemTypeId == ITEM_TYPE_ARTIFACT || itemTypeId == ITEM_TYPE_FIREFLY_PEND)
        {
            SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, SPRITES2D_TXD_FILE_NAME, Sprites2D_Collectibles[itemTypeId]);
        }
        else
        {
            SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, SPRITES2D_TXD_FILE_NAME, Sprites2D_Ingredients[Sprite2D_FindIngrSpriteIdx(itemId, itemAmount)][e_sIngrSprite]);
        }
        return IsDynamicObjectMaterialUsed(spriteObjectId, 0);
    }

//#endregion