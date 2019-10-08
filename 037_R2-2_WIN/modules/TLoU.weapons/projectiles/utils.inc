// Function assigned to do the work related with projectiles throw.
Projectile_Throw(playerid, itemId)
{
    // Validation.
    if(GetPlayerItemAmount(playerid, itemId) <= 0 || GetProjectilePoolSize() >= MAX_THROWABLES || IsPlayerAimOrShootCustomItem(playerid)
    || GetConnectedTime(playerid) - (ePlayerWeapon[playerid][e_iTickCounter_projectileThrow]) < TICK_RATE_PROJECTILE_THROW)
    {
        return 1;
    }
    
    // Physics.
    new Float:fPX, Float:fPY, Float:fPZ;
    GetPlayerCameraPos(playerid, fPX, fPY, fPZ);

    new Float:fVX, Float:fVY, Float:fVZ;
    GetPlayerCameraFrontVector(playerid, fVX, fVY, fVZ);

    new Float:pAngle;
    GetPlayerFacingAngle(playerid, pAngle);

    const Float:fScale = 4.1;
    new Float:dest_projX = fPX + floatmul(0.8, floatcos(pAngle, degrees)) + floatmul(fVX, fScale);
    new Float:dest_projY = fPY + floatmul(0.7, floatsin(pAngle, degrees)) + floatmul(fVY, fScale);
    new Float:dest_projZ = fPZ + floatmul(fVZ, fScale);

    new projid = CreateProjectile(dest_projX, dest_projY, dest_projZ, \
                    (MAX_PROJECTILE_SPEED * fVX), (MAX_PROJECTILE_SPEED * fVY), (MAX_PROJECTILE_SPEED * fVZ) + 5.0, \
                    0.0, 0.0, 0.0, \
                    PROJ_PHYS_SPHERECOL_RADIUS, PROJ_PHYS_GROUND_FRICTION, PROJ_PHYS_COLL_FRICTION, PROJ_PHYS_AIR_ASSISTANCE, PROJ_PHYS_GRAVITY, PROJ_PHYS_PLAYER_COL_RADIUS, PROJ_PHYS_COLLIDE_SIMULATION, PROJ_PHYS_MASS);

    if(projid == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Throw] Unable to create projectile (ID: %d) for player: %s (ID: %d). Item equipped: %d", projid, PlayerName(playerid), playerid, itemId);
        return 1;
    }

    // Object creation.
    new projObjectId = INVALID_OBJECT_ID;
    projObjectId = CreateDynamicObject(GetItemModel(itemId), dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);

    if(projObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Throw] Invalid object Id for item id: %d (Player data: %s [%d])", itemId, PlayerName(playerid), playerid);
        return 1;
    }

    // Update.
    foreach(new i : Player)
    {
        Streamer_Update(i, STREAMER_TYPE_OBJECT);
    }

    eProjectileData[projid][e_iProjectile_ThrowerId] = playerid;
    eProjectileData[projid][e_iProjectile_ObjectId] = projObjectId;
    ePlayerWeapon[playerid][e_iTickCounter_projectileThrow] = GetConnectedTime(playerid);
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, itemId);
    
    Bit_Let(ePlayerFlag[e_bShootingCustomWeapon], playerid);
    OnPlayerThrowProjectile(playerid, itemId);
    return 0;
}

// Drop Nailbomb near player (sissy throw).
Projectile_Drop_Nailbomb(playerid)
{
    // Validation.
    if(GetPlayerItemAmount(playerid, _:ITEM_IDX_NAIL_BOMB) <= 0 || GetProjectilePoolSize() >= MAX_THROWABLES
    || GetConnectedTime(playerid) - (ePlayerWeapon[playerid][e_iTickCounter_projectileThrow]) < TICK_RATE_PROJECTILE_THROW)
    {
        return 1;
    }

    new Float:x, Float:y, Float:z, Float:ang;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, ang);

    new projid = CreateProjectile(x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, \
                5.0 * floatsin(-ang, degrees), 5.0 * floatcos(-ang, degrees), 3.0, \
                0.0, 0.0, 0.0, \
                PROJ_PHYSDROP_SPHERECOL_RADIUS, PROJ_PHYSDROP_GROUND_FRICTION, PROJ_PHYSDROP_COLL_FRICTION, PROJ_PHYSDROP_AIR_ASSISTANCE, PROJ_PHYSDROP_GRAVITY, PROJ_PHYSDROP_PLAYER_COL_RADIUS, PROJ_PHYSDROP_COLLIDE_SIMULATION, PROJ_PHYSDROP_MASS);

    if(projid == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Drop_Nailbomb] Unable to create projectile (ID: %d) for player: %s (ID: %d)", projid, PlayerName(playerid), playerid);
        return 1;
    }

    // Object creation.
    new projObjectId = CreateDynamicObject(GetItemModel(_:ITEM_IDX_NAIL_BOMB), x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, 93.7, 120.0, ang + 60.0);
    if(projObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Drop_Nailbomb] Invalid object Id. (Player data: %s [%d])", PlayerName(playerid), playerid);
        return 1;
    }

    // Update.
    foreach(new i : Player)
    {
        Streamer_Update(i, STREAMER_TYPE_OBJECT);
    }

    eProjectileData[projid][e_iProjectile_ThrowerId] = playerid;
    eProjectileData[projid][e_iProjectile_ObjectId] = projObjectId;
    ePlayerWeapon[playerid][e_iTickCounter_projectileThrow] = GetConnectedTime(playerid);
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, _:ITEM_IDX_NAIL_BOMB);
    
    OnPlayerDropNailbomb(playerid);
    return 0;
}

// Initiates nailbomb in the "idle" state.
InitiateNailbomb(projid, throwerid, Float:x, Float:y, Float:z)
{
    ResetProjectileData(projid);

    new nailBmbIdx = Iter_Alloc(NailBmbsList);
    if(nailBmbIdx == INVALID_ITERATOR_SLOT)
        return;

    eProjectileData[nailBmbIdx][e_iNailBmb_ObjectId] = eProjectileData[projid][e_iProjectile_ObjectId];
    OnNailbombInitiate(nailBmbIdx, throwerid, x, y, z);

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }
}

// Finds Nailbomb Index based on its objectid.
FindNailbombIdx(objectid)
{
    foreach(new nailBmbIdx : NailBmbsList)
    {
        if(objectid == eProjectileData[nailBmbIdx][e_iNailBmb_ObjectId])
        {
            return nailBmbIdx;
        }
    }
    return INVALID_ITERATOR_SLOT;
}

// Creates projectile destroy effect based on the itemId.
CreateProjectileDestroyEffect(itemId, projid, Float:x, Float:y, Float:z, throwerid = INVALID_PLAYER_ID)
{
    switch(ITEM_IDX:itemId)
    {
        case ITEM_IDX_MOLOTOV:
        {
            CreateFire(x, y, z, throwerid);
        }

        case ITEM_IDX_NAIL_BOMB:
        {
            if(IsValidDynamicObject(eProjectileData[projid][e_iNailBmb_SmokeObjectId]))
            {
                DestroyDynamicObject(eProjectileData[projid][e_iNailBmb_SmokeObjectId]);
            }

            if(IsValidDynamicObject(eProjectileData[projid][e_iNailBmb_ObjectId]))
            {
                DestroyDynamicObject(eProjectileData[projid][e_iNailBmb_ObjectId]);
            }
            
            // If contains means nailbomb WAS in idle state
            // else means nailbomb collided while thrown.
            if(Iter_Contains(NailBmbsList, projid))
            {
                Iter_Remove(NailBmbsList, projid);
            }
            else
            {
                ResetProjectileData(projid);
            }
            CreateDynamicObject(OBJ_MODEL_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);
        }

        case ITEM_IDX_SMOKE_BOMB:
        {
            new smkBmbSmokeObjectId = CreateDynamicObject(OBJ_MODEL_SMOKE, x, y, z, 0.0, 0.0, 0.0);
            defer DestroySmkBmbSmokeObject(smkBmbSmokeObjectId);
        }
    }

    if(IsValidDynamicObject(eProjectileData[projid][e_iProjectile_ObjectId]) && itemId != _:ITEM_IDX_NAIL_BOMB)
    {
        ResetProjectileData(projid);
        DestroyDynamicObject(eProjectileData[projid][e_iProjectile_ObjectId]);
    }
    
    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
        PlayPlayer3D_ProjectileSFX(playerid, itemId, x, y, z);
    }
}

// Destroy projectile's data.
ResetProjectileData(projid)
{
    DestroyProjectile(projid);
    eProjectileData[projid][e_iProjectile_ThrowerId] = INVALID_PLAYER_ID;
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, E_STREAMER_EXTRA_ID, projid, -1);
}

// Stuns all nearby players and FCNPCs that are in range of x, y, z coordinates.
// Optional: throwerid - so he won't get stunned if he's the initiator of the projectile.
StunNearbyPlayersAndNpcs(Float:x, Float:y, Float:z, throwerid = INVALID_PLAYER_ID)
{
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || i == throwerid
        || !IsPlayerInRangeOfPoint(i, SMOKEBMB_AFFECT_RANGE, x, y, z))
        {
            continue;
        }

        if(IsPlayerNPC(i))
        {
			FCNPC_SetStatus(i, STATUS_STUNNED);
        }
        else
        {
			SetPlayerStatus(i, PLAYER_STATUS_STUNNED);
        }
    }
}