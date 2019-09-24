// Create 2D sprite for player at specified position. Sprite is created only in dynamic areas.
Sprite2D_Create(playerid, areaId, itemId, Float:x, Float:y, Float:z, virtualWorld, interiorId)
{
    new i = Iter_Free(SpritesCreated<playerid>);
    if(i == INVALID_ITERATOR_SLOT)
	{
		return false;
	}

    Iter_Add(SpritesCreated<playerid>, areaId);

	new objectId = ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, x, y, z + 0.5, 0.0, 0.0, 0.0, virtualWorld, interiorId, playerid, ITEM_SPRITE_STREAM_DIST);
    SetDynamicObjectMaterial(objectId, 0, ITEM_MODEL_TRANS_NO_COLL, "doNotUse", eItemsSpritesEx[itemId]);
	ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][i] = repeat UpdateSpriteTimer(playerid, objectId);

    SCMF(playerid, -1, "Created sprite for areaId: %d", areaId);
	return true;
}

// Destroy created 2D sprite.
Sprite2D_Destroy(playerid, areaId)
{
    if(Iter_Contains(SpritesCreated<playerid>, areaId))
    {
        SCMF(playerid, -1, "Destroying sprite for areaId: %d", areaId);

        foreach(new internalAreaId : SpritesCreated<playerid>)
        {
            if(internalAreaId == areaId)
            {
                stop ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][internalAreaId];

                if(IsValidDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i]))
                {
                    DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][i]);
                }

                Iter_Remove(SpritesCreated<playerid>, areaId);
                SCMF(playerid, -1, "Stopped timer for areaId: %d (internalAreaId: %d)", areaId, internalAreaId);
                break;
            }
        }
    }
    return Iter_Contains(SpritesCreated<playerid>, areaId);
}