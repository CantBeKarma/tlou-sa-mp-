// Create 2D sprite for player at specified position. Sprite is created only in dynamic areas.
Sprite2D_Create(playerid, item_objectId)
{
    new itemAreaId, bool:itemCreated;
    ReadItemRelation(item_objectId, .itemAreaId = itemAreaId, .itemCreated = itemCreated);

    if(!itemCreated || itemAreaId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Tried to create 2D sprite for object which is not created (isCreated: %d) OR itss area is invalid (itemAreaId: %d)", itemCreated, itemAreaId);
        return false;
    }

    new i = Iter_Free(SpritesCreated<playerid>);
    if(i == INVALID_ITERATOR_SLOT || Iter_Contains(SpritesCreated<playerid>, itemAreaId))
	{
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Sprite2D_Create: Unable to create 2D sprite for: %s (UID: %d | GUID: %d). Iter_Free? %d | Contains area already? %d", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), i, Iter_Contains(SpritesCreated<playerid>, itemAreaId));
		return false;
	}

    new Float:itemPosX, Float:itemPosY, Float:itemPosZ, itemVw, itemIntId;
    ReadItemRelation(item_objectId, .itemPosX = itemPosX, .itemPosY = itemPosY, .itemPosZ = itemPosZ, .itemVw = itemVw, .itemIntId = itemIntId);

	new spriteObjectId = INVALID_OBJECT_ID;
    spriteObjectId = ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.5, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST);

    if(spriteObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Sprite2D_Create: Unable to create item's SPRITE dynamic object!");
        return false;
    }

    new itemId, itemAmount;
    ReadItemRelation(item_objectId, .itemId = itemId, .itemAmount = itemAmount);

    SetDynamicObjectMaterial(spriteObjectId, 0, ITEM_MODEL_TRANS_NO_COLL, "doNotUse", (GetItemType(itemId) == ITEM_TYPE_INGREDIENT) ? (eIngrGameWorldSprites[Sprite2D_FindIngrSpriteIdx(itemId, itemAmount)])  : (eItemGameWorldSprite[itemId]));

    new spriteTextObjectId = INVALID_OBJECT_ID;

    if(GetItemType(itemId) == ITEM_TYPE_WEAPON
    || GetItemType(itemId) == ITEM_TYPE_MELEE)
    {
        spriteTextObjectId = ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][i] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.2, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST);

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

        SetDynamicObjectMaterialText(spriteTextObjectId, 0, spriteTextStr, 10);
    }

	ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][i] = repeat UpdateSpriteTimer(playerid, spriteObjectId, spriteTextObjectId);

    Iter_Add(SpritesCreated<playerid>, itemAreaId);
    SCMF(playerid, -1, "Created sprite for areaId: %d", itemAreaId);
	return true;
}

// Destroy created 2D sprite.
Sprite2D_Destroy(playerid, areaId)
{
    if(!Iter_Contains(SpritesCreated<playerid>, areaId))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Sprite2D_Create: Unable to destroy 2D Sprite for: %s (UID: %d | GUID: %d). Iterator doesn't contain areaId (%d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), areaId);
        return false;
    }

    SCMF(playerid, -1, "Destroying sprite for areaId: %d", areaId);

    foreach(new i : SpritesCreated<playerid>)
    {
        if(i == areaId)
        {
            stop ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][i];

            if(IsValidDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i]))
            {
                DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i]);
            }

            if(IsValidDynamicObject(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][i]))
            {
                DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][i]);
            }

            Iter_Remove(SpritesCreated<playerid>, areaId);
            SCMF(playerid, -1, "Stopped timer for areaId: %d (index: %d)", areaId, i);
            break;
        }
    }
    return !Iter_Contains(SpritesCreated<playerid>, areaId);
}

// Crafting : Find ingredient sprite (texture) for specified amount (used internally in CraftTd_DisplayIngredients())
Sprite2D_FindIngrSpriteIdx(ingr, amount)
{
    for(new i = 0; i != MAX_2D_INGR_SPRITES; i++)
    {
        if(ingr == _:eIngrGameWorldSprites[i][e_iIngrId]
        && amount == eIngrGameWorldSprites[i][e_iIngrValue])
        {
            return i;
        }
    }
    return -1;
}