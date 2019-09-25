// Create 2D sprite for player at specified position. Sprite is created only in dynamic areas.
Sprite2D_Create(playerid, item_objectId)
{
    new itemAreaId, bool:itemCreated;
    ReadItemRelation(item_objectId, .itemAreaId = itemAreaId, .itemCreated = itemCreated);

    if(!itemCreated || itemAreaId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Tried to create 2D sprite for object which is not created (isCreated: %d) OR its area is invalid (itemAreaId: %d)", itemCreated, itemAreaId);
        return false;
    }

    new i = GetFreeSpriteIndex(playerid);
    if(i == -1)
	{
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Sprite2D_Create: Unable to create 2D sprite for: %s (UID: %d | GUID: %d). Iter index is: %d", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), i);
		return false;
	}

    new Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
    ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

	new spriteObjectId = INVALID_OBJECT_ID;
    spriteObjectId = ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.4, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST, .areaid = itemAreaId);

    if(spriteObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Unable to create item's SPRITE dynamic object!");
        return false;
    }

    new itemId, itemTypeId, itemAmount;
    ReadItemRelation(item_objectId, .itemId = itemId, .itemTypeId = itemTypeId, .itemAmount = itemAmount);

    if(itemTypeId == ITEM_TYPE_ITEM && !IsItemType(itemId, ITEM_TYPE_INGREDIENT))
    {
        SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, "doNotUse", Sprites2D_Items[itemId]);
    }
    else if(itemTypeId == ITEM_TYPE_ARTIFACT || itemTypeId == ITEM_TYPE_FIREFLY_PEND)
    {
        SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, "doNotUse", Sprites2D_Collectibles[itemTypeId]);
    }
    else
    {
        SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, "doNotUse", Sprites2D_Ingredients[Sprite2D_FindIngrSpriteIdx(itemId, itemAmount)][e_sIngrSprite]);
    }

    if(!IsDynamicObjectMaterialUsed(spriteObjectId, 0))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Object's material wasn't changed! (SetDynamicObjectMaterial failure)");
        return false;
    }

    new spriteTextObjectId = INVALID_OBJECT_ID;

    if(GetItemType(itemId) == ITEM_TYPE_WEAPON
    || GetItemType(itemId) == ITEM_TYPE_MELEE)
    {
        spriteTextObjectId = ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][i] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.3, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST, .areaid = itemAreaId);

        new spriteTextStr[10];   
        if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
        {
            format(spriteTextStr, sizeof(spriteTextStr), "%d", itemAmount);
        }
        else if(GetItemType(itemId) == ITEM_TYPE_MELEE)
        {
            new itemExtAmount;
            ReadItemRelation(item_objectId, .itemExtAmount = itemExtAmount);

            new bool:isItemMeleeUpgraded = (itemExtAmount > 0);

            format(spriteTextStr, sizeof(spriteTextStr), "%d/%d", \
                    (isItemMeleeUpgraded) ? (itemAmount + itemExtAmount) : (itemAmount), \
                    (isItemMeleeUpgraded) ? GetMeleeItemMaxCND(itemId, .extraCnd = false) + GetMeleeItemMaxCND(itemId, .extraCnd = true) : GetMeleeItemMaxCND(itemId, .extraCnd = false));
        }

        SetDynamicObjectMaterialText(spriteTextObjectId, 0, spriteTextStr, .fontsize = SPRITE_FONTSIZE_ITEM_AMOUNT, .fontcolor = 0xFFFF0000);
    }

	ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][i] = repeat UpdateSpriteTimer(playerid, spriteObjectId, spriteTextObjectId);
    ePlayerSpriteInfo[playerid][e_iSprite_AreaId][i] = itemAreaId;
    ePlayerSpriteInfo[playerid][e_bSprite_Created][i] = true;

    Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    SCMF(playerid, -1, "Created sprite for areaId: %d", itemAreaId);
	return true;
}

// Destroy created 2D sprite.
Sprite2D_Destroy(playerid, areaId)
{
    new bool:destroyed = false;
    for(new i = 0; i != MAX_SPRITES_AT_ONE_TIME; i++)
    {
        if(ePlayerSpriteInfo[playerid][e_bSprite_Created][i] && ePlayerSpriteInfo[playerid][e_iSprite_AreaId][i] == areaId)
        {
            stop ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][i];

            DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i]);
            DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][i]);
            ePlayerSpriteInfo[playerid][e_bSprite_Created][i] = false;
            
            SCMF(playerid, -1, "Destroying sprite for areaId: %d", areaId);

            destroyed = true;
        }
    }
    return destroyed;
}

CMD:mycreated(playerid, params[])
{
    for(new i = 0; i != MAX_SPRITES_AT_ONE_TIME; i++)
    {
        if(ePlayerSpriteInfo[playerid][e_bSprite_Created][i] == true)
        {
            SCMF(playerid, -1, "created objects for indexes: %d", i);
        }
    }
    return 1;
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

GetFreeSpriteIndex(playerid)
{
    for(new i = 0; i != MAX_SPRITES_AT_ONE_TIME; i++)
    {
        if(ePlayerSpriteInfo[playerid][e_bSprite_Created][i] == false)
        {
            return i;
        }
    }
    return -1;
}