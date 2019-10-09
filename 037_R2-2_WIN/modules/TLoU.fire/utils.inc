// Function creates a fire objects and configures settings.
CreateFire(Float:x, Float:y, Float:z, fireCreatorId = INVALID_PLAYER_ID)
{
    new firesCount = RandomEx(MIN_FIRES_PER_MOLOTOV, MAX_FIRES_PER_MOLOTOV);
    new firesObjectIds[MAX_FIRES_PER_MOLOTOV];

    for(new i = 0; i != firesCount; i++)
    {
        if(i == 0)
        {
            eFireData[e_fFirePos_X] = x;
            eFireData[e_fFirePos_Y] = y;
            eFireData[e_iFireCreatorId] = fireCreatorId;

            CreateDynamicObject(OBJ_MODEL_FIRE_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);            
        }
        else
        {
            eFireData[e_fFirePos_X] = x + random(3);
            eFireData[e_fFirePos_Y] = y + random(3);
        }

        eFireData[e_fFirePos_Z] = z - 2.5;
        firesObjectIds[i] = eFireData[e_iFireObjectId][i] = CreateDynamicObject(FIRE_MODEL_ID, eFireData[e_fFirePos_X], eFireData[e_fFirePos_Y], eFireData[e_fFirePos_Z], 0.0, 0.0, 0.0);

        if(i == 0)
        {
            eFireData[e_iFireAreaId] = CreateDynamicSphere(x, y, z, FIRE_RANGE);
            Streamer_SetIntData(STREAMER_TYPE_AREA, eFireData[e_iFireAreaId], E_STREAMER_EXTRA_ID, eFireData[e_iFireObjectId][0]);
        }
    }

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }

    defer DestroyFire(firesObjectIds, eFireData[e_iFireAreaId]);
    return 0;
}

// Destroys fire after certain amount of time.
timer DestroyFire[FIRE_DESTROY_DELAY](firesObjectIds[], fireAreaId)
{
    DestroyDynamicArea(fireAreaId);

    for(new i = 0; i != MAX_FIRES_PER_MOLOTOV; i++)
    {
        if(!IsValidDynamicObject(firesObjectIds[i]))
        {
            break;
        }
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
    if(IsPlayerBurning(playerid))
    {
        return false;
    }

    if(SetPlayerStatus(playerid, PLAYER_STATUS_BURNING, fireCreatorId) == true)
    {
        ePlayerInfo[playerid][e_iTimer_Burning] = repeat OnPlayerBurn(playerid, fireCreatorId);
    }
    return true;
}

// Puts the fire out of player.
timer PutOutFireOnPlayer[FIRE_PUT_OUT_DELAY](playerid)
{
    if(!IsPlayerBurning(playerid))
    {
        return false;
    }
    
    if(SetPlayerStatus(playerid, PLAYER_STATUS_NONE) == true)
    {
        stop ePlayerInfo[playerid][e_iTimer_Burning];
    }
    return true;
}