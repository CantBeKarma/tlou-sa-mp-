// Function creates a fire objects and configures settings.
CreateFire(Float:x, Float:y, Float:z, fireCreatorId = INVALID_PLAYER_ID)
{
    new fireId = Iter_Alloc(ServerFires);
    if(fireId == INVALID_ITERATOR_SLOT)
        return 1;

    new firesAmount = RandomEx(MIN_FIRES_PER_MOLOTOV, MAX_FIRES_PER_MOLOTOV);
    for(new i = 0; i != firesAmount; i++)
    {
        if(i == 0)
        {
            eFireData[fireId][e_fFirePos_X] = x;
            eFireData[fireId][e_fFirePos_Y] = y;

            // Additional explosion effect (just once).
            CreateDynamicObject(OBJ_MODEL_FIRE_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);
        }
        else
        {
            eFireData[fireId][e_fFirePos_X] = x + random(3);
            eFireData[fireId][e_fFirePos_Y] = y + random(3);
        }

        eFireData[fireId][e_fFirePos_Z] = z - 2.5;

        eFireData[fireId][e_iFireObjectId][i] = CreateDynamicObject(FIRE_MODEL_ID, eFireData[fireId][e_fFirePos_X], eFireData[fireId][e_fFirePos_Y], eFireData[fireId][e_fFirePos_Z], 0.0, 0.0, 0.0);
        eFireData[fireId][e_iFireAreaId] = CreateDynamicSphere(x, y, z, FIRE_RANGE);
        eFireData[fireId][e_iFireCreatorId] = fireCreatorId;
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
IsPlayerInFireArea(playerid, playerAreaId)
{
    foreach(new i : ServerFires)
    {
        if(playerAreaId == eFireData[i][e_iFireAreaId])
        {
            return IsPlayerInDynamicArea(playerid, i);
        }
    }
    return false;
}

// Puts player on fire state.
SetPlayerOnFire(playerid, fireCreatorId = INVALID_PLAYER_ID)
{
    if(IsPlayerBurning(playerid))
    {
        return;
    }

    SPAO(playerid, OBJECT_MODEL_BURNING);
    Bit_Let(ePlayerFlag[e_bIsBurning], playerid);
    OnPlayerStartBurning(playerid, fireCreatorId);
}

// Puts the fire out of player.
timer PutOutFireOnPlayer[FIRE_PUT_OUT_DELAY](playerid)
{
    if(!IsPlayerBurning(playerid))
    {
        return;
    }

    RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
    Bit_Vet(ePlayerFlag[e_bIsBurning], playerid);
    OnPlayerStopBurning(playerid);
}