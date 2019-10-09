// Function creates a fire objects and configures settings.
CreateFire(Float:x, Float:y, Float:z, fireCreatorId = INVALID_PLAYER_ID)
{
    new fireId = Iter_Alloc(ServerFires);
    if(fireId == INVALID_ITERATOR_SLOT)
    {
        return 1;
    }

    // Below magic trick is required, because server checks if player is in a fire area
    // from ID of 1 instead of 0 example: CreateDynamic[...] IDs start from 1, where Iterator ID starts from 0
    if(fireId == 0)
    {
        fireId = Iter_Alloc(ServerFires);
        Iter_Remove(ServerFires, 0);

        if(fireId == INVALID_ITERATOR_SLOT)
        {
            return 1;
        }
    }

    new firesCount = RandomEx(MIN_FIRES_PER_MOLOTOV, MAX_FIRES_PER_MOLOTOV);
    for(new i = 0; i != firesCount; i++)
    {
        if(i == 0)
        {
            eFireData[fireId][e_fFirePos_X] = x;
            eFireData[fireId][e_fFirePos_Y] = y;

            CreateDynamicObject(OBJ_MODEL_FIRE_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);
            eFireData[fireId][e_iFireAreaId] = CreateDynamicCircle(x, y, FIRE_RANGE);
            eFireData[fireId][e_iFireCreatorId] = fireCreatorId;
        }
        else
        {
            eFireData[fireId][e_fFirePos_X] = x + random(3);
            eFireData[fireId][e_fFirePos_Y] = y + random(3);
        }

        eFireData[fireId][e_fFirePos_Z] = z - 2.5;
        eFireData[fireId][e_iFireObjectId][i] = CreateDynamicObject(FIRE_MODEL_ID, eFireData[fireId][e_fFirePos_X], eFireData[fireId][e_fFirePos_Y], eFireData[fireId][e_fFirePos_Z], 0.0, 0.0, 0.0);
    }

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }

    defer DestroyFire(fireId);
    return 0;
}

// Destroys fire after certain amount of time.
timer DestroyFire[FIRE_DESTROY_DELAY](fireId)
{    
    if(Iter_Contains(ServerFires, fireId))
    {
        Iter_Remove(ServerFires, fireId);
    }

    if(IsValidDynamicArea(eFireData[fireId][e_iFireAreaId]))
    {
        DestroyDynamicArea(eFireData[fireId][e_iFireAreaId]); 
    }

    for(new i = 0; i != MAX_FIRES_PER_MOLOTOV; i++)
    {
        if(!IsValidDynamicObject(eFireData[fireId][e_iFireObjectId][i]))
        {
            break;
        }
        DestroyDynamicObject(eFireData[fireId][e_iFireObjectId][i]);
    }
}

// Check if player's area is fire area.
IsPlayerInFireArea(playerAreaId)
{
    return Iter_Contains(ServerFires, playerAreaId);
}

// Puts player on fire.
PutPlayerOnFire(playerid, areaid)
{
    if(IsPlayerBurning(playerid))
    {
        return false;
    }

    SetPlayerStatus(playerid, PLAYER_STATUS_BURNING);
    ePlayerInfo[playerid][e_iTimer_Burning] = repeat OnPlayerBurn(playerid, eFireData[areaid][e_iFireCreatorId]);
    return true;
}

// Puts the fire out of player.
timer PutOutFireOnPlayer[FIRE_PUT_OUT_DELAY](playerid)
{
    if(!IsPlayerBurning(playerid))
    {
        return false;
    }
    
    stop ePlayerInfo[playerid][e_iTimer_Burning];
    SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
    return true;
}