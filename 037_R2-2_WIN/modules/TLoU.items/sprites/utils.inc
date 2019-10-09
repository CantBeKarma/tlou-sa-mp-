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
            ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][iter] = INVALID_OBJECT_ID;
            Iter_Remove(SpritesCreated<playerid>, iter);
            return true;
        }
    }
    return false;
}

// Destroy all 2D sprites that player was seeing.
Sprite2D_DestroyAll(playerid)
{
    foreach(new iter : SpritesCreated<playerid>)
    {
        stop ePlayerSpriteInfo[playerid][e_iTimer_UpdateSprite][iter];
        DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSprite_ObjectId][iter]);
        DestroyDynamicObject(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][iter]);
        ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][iter] = INVALID_OBJECT_ID;
    }

    Iter_Clear(SpritesCreated<playerid>);
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

// Enabling 2D sprites updates for player
Sprite2D_EnableUpdate(playerid)
{
    Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_AREA, 1);
    Streamer_Update(playerid, STREAMER_TYPE_AREA);

    if(IsPlayerInAnyDynamicArea(playerid))
    {
        new areas[MAX_SPRITES_AT_ONE_TIME];
        GetPlayerDynamicAreas(playerid, areas, sizeof(areas));

        for(new areaid = 0; areaid != MAX_SPRITES_AT_ONE_TIME; areaid++)
        {
            if(areas[areaid] == 0)
            {
                continue;
            }

            new areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, areas[areaid], E_STREAMER_EXTRA_ID);
            if(areaObjectId > 0)
            {
                Sprite2D_Create(playerid, areaObjectId);
            }
        }
    }
}

// Disabling player's 2D sprites updates and destroying all current 2D sprites created for player.
Sprite2D_DisableUpdate(playerid)
{
    if(HasPlayerGotAnyItemsToPickUp(playerid))
    {
        Sprite2D_DestroyAll(playerid);
    }
    Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_AREA, 0);
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
        spriteTextObjectId = ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][iter] = CreateDynamicObject(ITEM_MODEL_TRANS_NO_COLL, itemPosX, itemPosY, itemPosZ + 0.5, 0.0, 0.0, 0.0, itemVw, itemIntId, playerid, ITEM_SPRITE_STREAM_DIST);

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

    // This function seeks for items nearby player of the same itemId.
    // Once item was found, it will check if player is full on this item and if he should see "FULL" text instead of an amount.
    Sprite2D_UpdateTextNearbyItems(playerid, pickedUp_itemObjectId)
    {
        new pickedUpItemId;
        ReadItemRelation(pickedUp_itemObjectId, .itemId = pickedUpItemId);

        foreach(new iter : SpritesCreated<playerid>)
        {
            new iterItemId;
            ReadItemRelation(ePlayerSpriteInfo[playerid][e_iSprite_CreatedForObjectId][iter], .itemId = iterItemId);

            if(iterItemId == pickedUpItemId)
            {
                if(!IsItemType(iterItemId, ITEM_TYPE_WEAPON))
                {
                    continue;
                }

                if(IsPlayerFullOnItem(playerid, pickedUpItemId))
                {
                    SetDynamicObjectMaterialText(ePlayerSpriteInfo[playerid][e_iSpriteText_ObjectId][iter], 0, "FULL", .fontsize = SPRITE_FONTSIZE_ITEM_AMOUNT, .fontcolor = 0xFFFF0000);
                }
            }
        }
        return 1;
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


#define     INVALID_ROTATION            (-1000.0)
#define     INVALID_VIRTUAL_WORLD       (-2000)
#define     VERTICAL_CAMERA_RADIUS      (55.0) // 27.5 * 2
#define     HORIZONTAL_CAMERA_RADIUS    (70.0) // 35.0 * 2

enum Item3D_Type 
{
	item_player,
	item_npc,
	item_actor,
	item_object,
	item_vehicle,
	item_dynamic_object,
	item_dynamic_pickup,
	item_dynamic_cp,
	item_dynamic_racecp,
	item_dynamic_mapicon,
	item_dynamic_3dtext,
	item_dynamic_actor,
	item_dynamic_vehicle,
	item_fcnpc
}

enum element_orientation 
{
	o_left,
	o_right,
	o_up,
	o_down,
	o_front,
	o_back
}

CMD:onscreen(playerid, params[])
{
    SCMF(playerid, -1, "%s", OnPlayerScreen(playerid, 49, item_fcnpc) ? "NPC ID: 49 is on your screen!" : "NPC ID: 49 is NOT on your screen!");
    return 1;
}

OnPlayerScreen(playerid, targetid, Item3D_Type:target_type = item_player, element_orientation:orientation = o_front, Float:rx = INVALID_ROTATION, Float:rz = INVALID_ROTATION, \
                    Float:vrx = VERTICAL_CAMERA_RADIUS, Float:vrz = HORIZONTAL_CAMERA_RADIUS, bool:testLOS = true, bool:testVW = true)
{
	new 
        Float:x, Float:y, Float:z,
        Float:tx, Float:ty, Float:tz,
        Float:prx, Float:prz;

	GetPlayerCameraPos(playerid, x, y, z);
	GetPlayerCameraRotation(playerid, prx, prz);
	
	switch(orientation)
    {
		case o_left:	prx = CompressRotation(prx + 90.0);
		case o_right:	prx = CompressRotation(prx - 90.0);
		case o_back:	prx = CompressRotation(prx + 180.0);
	}
	
	if(rx != INVALID_ROTATION)
    {
        prx = rx;
    }

	if(rz != INVALID_ROTATION)
    {
        prz = rz;
    }
	
	if(!GetItemPos(targetid, target_type, tx, ty, tz))
    {
        return 0;
    }
	
	if(testVW)
    {
		if(GetItemVirtualWorld(targetid, target_type) != -1 
        && GetItemVirtualWorld(targetid, target_type) != GetPlayerVirtualWorld(playerid))
        {
            return 0;
        }
	}
	return PointInSphericalSectorEx(tx, ty, tz, x, y, z, prx, prz, 300.0, vrx, vrz, testLOS);
}

GetItemPos(elementid, Item3D_Type:element_type, &Float:x, &Float:y, &Float:z)
{
	switch(element_type)
    {
		case item_player, item_npc: GetPlayerPos(elementid, x, y, z);
		case item_actor:			GetActorPos(elementid, x, y, z);
		case item_object:			GetObjectPos(elementid, x, y, z);
		case item_vehicle:			GetVehiclePos(elementid, x, y, z);
        case item_dynamic_object:	GetDynamicObjectPos(elementid, x, y, z);
        case item_dynamic_pickup: 	Streamer_GetItemPos(STREAMER_TYPE_PICKUP, elementid, x, y, z);
        case item_dynamic_cp: 		Streamer_GetItemPos(STREAMER_TYPE_CP, elementid, x, y, z);
        case item_dynamic_racecp:	Streamer_GetItemPos(STREAMER_TYPE_RACE_CP, elementid, x, y, z);
        case item_dynamic_mapicon:	Streamer_GetItemPos(STREAMER_TYPE_MAP_ICON, elementid, x, y, z);
        case item_dynamic_3dtext: 	Streamer_GetItemPos(STREAMER_TYPE_3D_TEXT_LABEL, elementid, x, y, z);
        case item_dynamic_actor:	GetDynamicActorPos(elementid, x, y, z);
        case item_fcnpc: 			FCNPC_GetPosition(elementid, x, y, z);
	}
    return 1;
}

GetItemVirtualWorld(elementid, Item3D_Type:element_type)
{
	switch(element_type)
    {
		case item_player, item_npc: return GetPlayerVirtualWorld(elementid);
		case item_actor:            return GetActorVirtualWorld(elementid);
		
        case item_vehicle:          return GetVehicleVirtualWorld(elementid);
        case item_dynamic_object:   return Streamer_GetIntData(STREAMER_TYPE_OBJECT, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_pickup:   return Streamer_GetIntData(STREAMER_TYPE_PICKUP, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_cp:       return Streamer_GetIntData(STREAMER_TYPE_CP, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_racecp:   return Streamer_GetIntData(STREAMER_TYPE_RACE_CP, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_mapicon:  return Streamer_GetIntData(STREAMER_TYPE_MAP_ICON, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_3dtext:   return Streamer_GetIntData(STREAMER_TYPE_3D_TEXT_LABEL, elementid, E_STREAMER_WORLD_ID);
        case item_dynamic_actor:    return GetDynamicActorVirtualWorld(elementid);
		case item_fcnpc:            return FCNPC_GetVirtualWorld(elementid);
	}
	return INVALID_VIRTUAL_WORLD;
}

PointInSphericalSectorEx(Float:px, Float:py, Float:pz, Float:x, Float:y, Float:z, Float:rx, Float:rz, Float:radius, Float:vrx, Float:vrz, bool:testLOS = true)
{
	if(GetDistanceBetweenPoints3D(px, py, pz, x, y, z) > radius)
    {
        return 0;
    }
	
    if(testLOS)
    {
        new Float:x2, Float:y2, Float:z2;
        if(CA_RayCastLine(x, y, z, px, py, pz, x2, y2, z2))
        {
            return 0;
        }
    }
	
	vrx /= 2.0;
	vrz /= 2.0;
	
	new Float:trx, Float:trz;
	if(!GetRotationFor2Point3D(x, y, z, px, py, pz, trx, trz))
    {
        return 0;
    }
	return (IsRotationTest(trx, rx - vrx, rx + vrx) && IsRotationTest(trz, rz - vrz, rz + vrz));
}

function IsRotationTest(Float:rotation, Float:r_min, Float:r_max)
{
	rotation = CompressRotation(rotation);
	r_min = CompressRotation(r_min);
	r_max = CompressRotation(r_max);

	if(r_min > r_max)
    {
		if((rotation >= 0.0 && rotation <= r_max) || (rotation >= r_min && rotation <= 360.0)) 
        {
            return 1;
        }
	} 
    else 
    {
		if(rotation >= r_min && rotation <= r_max)
        {
            return 1;
        }
	}
	return 0;
}

function GetPlayerCameraRotation(playerid, &Float:rx, &Float:rz)
{
	new Float:mx, Float:my, Float:mz;
	GetPlayerCameraFrontVector(playerid, mx, my, mz);
	rx = CompressRotation(-(acos(mz)-90.0));
	rz = CompressRotation((atan2(my,mx)-90.0));
}

GetRotationFor2Point3D(Float:x,Float:y,Float:z,Float:tx,Float:ty,Float:tz,&Float:rx,&Float:rz)
{
	new Float:radius = GetDistanceBetweenPoints3D(x,y,z,tx,ty,tz);
	return (radius <= 0.0) ? 0 : (0x1 & (1 | _:(rx = CompressRotation(-(acos((tz-z)/radius)-90.0)), rz = CompressRotation(atan2(ty-y,tx-x)-90.0))));
}