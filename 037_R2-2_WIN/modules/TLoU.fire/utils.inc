// Function creates a fire objects and configures settings.
CreateFire(Float:x, Float:y, Float:z, fireCreatorId = INVALID_PLAYER_ID)
{
    new firesCount = RandomEx(MIN_FIRES_PER_MOLOTOV, MAX_FIRES_PER_MOLOTOV);
    new firesObjectIds[MAX_FIRES_PER_MOLOTOV];

    new 
        virtualWorld = 0, 
        interiorId = 0;
        
    if(fireCreatorId != INVALID_PLAYER_ID)
    {
        virtualWorld = GetPlayerVirtualWorld(fireCreatorId);
        interiorId = GetPlayerInterior(fireCreatorId);
    }

    z -= 2.2;
    for(new i = 0; i != firesCount; i++)
    {
        if(i > 0)
        {
            x = x + random(2);
            y = y + random(2);
        }

        firesObjectIds[i] = CreateDynamicObject(FIRE_MODEL_ID, x, y, z, 0.0, 0.0, 0.0, virtualWorld, interiorId);
        
        if(i == 0)
        {
            CreateDynamicObject(OBJ_MODEL_FIRE_EXPLOSION, x, y, z, 0.0, 0.0, 0.0, virtualWorld, interiorId);

            eFireData[e_iFireAreaId] = CreateDynamicSphere(x, y, z, FIRE_RANGE, virtualWorld, interiorId);
            eFireData[e_iFireCreatorId] = fireCreatorId;
            eFireData[e_bFireCreated] = true;

            Streamer_SetArrayData(STREAMER_TYPE_OBJECT, firesObjectIds[0], E_STREAMER_EXTRA_ID, eFireData);
            Streamer_SetIntData(STREAMER_TYPE_AREA, eFireData[e_iFireAreaId], E_STREAMER_EXTRA_ID, firesObjectIds[0]);
        }
    }

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
        Streamer_Update(playerid, STREAMER_TYPE_AREA);
    }

    defer DestroyFire(firesObjectIds, firesCount, eFireData[e_iFireAreaId]);
    return 0;
}

// Destroys fire after certain amount of time.
timer DestroyFire[FIRE_DESTROY_DELAY](firesObjectIds[], firesObjectIdsLen, fireAreaId)
{
    DestroyDynamicArea(fireAreaId);
    Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, firesObjectIds[0], E_STREAMER_EXTRA_ID, eFireData[e_iFireAreaId]);
    Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, firesObjectIds[0], E_STREAMER_EXTRA_ID, eFireData[e_iFireCreatorId]);
    Streamer_RemoveArrayData(STREAMER_TYPE_OBJECT, firesObjectIds[0], E_STREAMER_EXTRA_ID, eFireData[e_bFireCreated]);

    for(new i = 0; i < firesObjectIdsLen; i++)
    {
        DestroyDynamicObject(firesObjectIds[i]);
    }
}

// Check if player's area is fire area.
IsPlayerInFireArea(playerAreaId)
{
    new areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, playerAreaId, E_STREAMER_EXTRA_ID);
    return (GetDynamicObjectModel(areaObjectId) == FIRE_MODEL_ID);
}

// Puts player on fire.
PutPlayerOnFire(playerid, fireCreatorId = INVALID_PLAYER_ID)
{
    if(SetPlayerStatus(playerid, PLAYER_STATUS_BURNING, fireCreatorId) == true)
    {
        ePlayerInfo[playerid][e_iTimer_Burning] = repeat OnPlayerBurn(playerid, fireCreatorId);
    }
}

// Puts the fire out of player.
timer PutOutFireOnPlayer[FIRE_PUT_OUT_DELAY](playerid)
{
    if(SetPlayerStatus(playerid, PLAYER_STATUS_NONE) == true)
    {
        stop ePlayerInfo[playerid][e_iTimer_Burning];
    }
}

// Reads single fire data relation and returns its value to variable sent by reference.
// May be used to return many relations/data.
ReadFireRelation(fire_objectId, &fireAreaId = -1, &fireCreatorId = -1, &fireCreated = -1)
{
    if(!IsValidDynamicObject(fire_objectId))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "ReadFireRelation: Tried to read invalid object ID data");
        return 1;
    }

    Streamer_GetArrayData(STREAMER_TYPE_OBJECT, fire_objectId, E_STREAMER_EXTRA_ID, eFireData);

    if(fireAreaId != -1)
    {
        fireAreaId = eFireData[e_iFireAreaId];
    }

    if(fireCreatorId != -1)
    {
        fireCreatorId = eFireData[e_iFireCreatorId];
    }

    if(fireCreated != -1)
    {
        fireCreated = eFireData[e_bFireCreated];
    }

    return 0;
}