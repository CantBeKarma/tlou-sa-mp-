#include	<YSI\y_hooks>

//#region Aiming and throwing callbacks
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------- OnPlayerStartAimingProjectile -------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerStartAimingProjectile(playerid, itemId)
    {
        Key_AimingCustomWeapon(playerid);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------- OnPlayerStopAimingProjectile -------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerStopAimingProjectile(playerid)
    {
        if(!Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid))
        {
            ClearAnims(playerid);
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerThrowProjectile ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerThrowProjectile(playerid, itemId)
    {
        new newProjectileAmount = GetPlayerItemAmount(playerid, itemId) - 1;
        SetPlayerItemAmount(playerid, itemId, newProjectileAmount);
        
        if(newProjectileAmount <= 0)
        {
            HideItem(playerid);
        }

        CLF(#ApplyCallbackAnim, "dss", playerid, "GRENADE", "WEAPON_THROW");
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerDropNailbomb -----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerDropNailbomb(playerid)
    {
        new newProjectileAmount = GetPlayerItemAmount(playerid, _:ITEM_IDX_NAIL_BOMB) - 1;
        SetPlayerItemAmount(playerid, _:ITEM_IDX_NAIL_BOMB, newProjectileAmount);

        if(newProjectileAmount <= 0)
        {
            HideItem(playerid);
        }

        ApplyAnimation(playerid, "GRENADE", "WEAPON_THROWU", 3.0, 0, 1, 1, 0, 0);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerKeyStateChange -----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
    {
        if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
        {
            return 1;
        }

        if(newkeys & KEY_FIRE)
        {
            if(IsPlayerIdle(playerid) && GetPlayerEquippedItemId(playerid) == _:ITEM_IDX_NAIL_BOMB)
            {
                Projectile_Drop_Nailbomb(playerid);
            }
        }
        return 1;
    }
//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnThrowableDestroy -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnThrowableDestroy(projid, throwerid, hitid, itemId, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(itemId, projid, x, y, z);

    if(hitid != INVALID_PLAYER_ID && IsPlayerNPC(hitid))
    {
        SCMF(throwerid, -1, "You hitted NPCID: %d with throwable!", hitid);
    }

    foreach(new npcid : FCNPC)
    {
        if(npcid == hitid)
        {
            continue;
        }

        FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_DISTRACTED, x, y);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnMolotovExplode --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnMolotovExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(_:ITEM_IDX_MOLOTOV, projid, x, y, z, throwerid);

    if(IsPlayerNPC(hitid) && !FCNPC_IsBurning(hitid))
    {
        FCNPC_SetStatus(hitid, NPC_STATUS:STATUS_BURNING);
    }

    // foreach(new npcid : FCNPC)
    // {
    //     if(FCNPC_Distract(npcid, x, y, z) == 1)
    //         continue;
    // }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnSmokebombExplode -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnSmokebombExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(_:ITEM_IDX_SMOKE_BOMB, projid, x, y, z);
    StunNearbyPlayersAndNpcs(x, y, z, throwerid);

    // foreach(new npcid : FCNPC)
    // {
    //     FCNPC_Distract(npcid, x, y, z);
    // }
    return 0;
}

//#region Nailbomb Callbacks
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------- OnNailbombInitiate ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnNailbombInitiate(nailBmbIdx, throwerid, Float:x, Float:y, Float:z)
    {
        eProjectileData[nailBmbIdx][e_iNailBmb_SmokeObjectId] = CreateDynamicObject(OBJ_MODEL_NAILBOMB_SMOKE, x + 0.06, y, z - 1.5, 0.0, 0.0, 0.0);
        eProjectileData[nailBmbIdx][e_iNailBmb_Timer] = repeat OnNailbombUpdate(nailBmbIdx, throwerid, x, y, z);        
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------------- OnNailbombUpdate -------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    timer OnNailbombUpdate[TICK_RATE_NAILBMB_UPDATE](nailBmbIdx, throwerid, Float:x, Float:y, Float:z)
    {
        for(new i = 0; i != MAX_PLAYERS; i++)
        {
            if(!IsPlayerConnected(i) || i == throwerid
            || !IsPlayerInRangeOfPoint(i, NAILBOMB_INITIATE_RANGE, x, y, z))
            {
                continue;
            }

            stop eProjectileData[nailBmbIdx][e_iNailBmb_Timer];
            
            if(IsPlayerNPC(i))
            {
                FCNPC_OnTriggerNailbomb(i, throwerid, nailBmbIdx, x, y, z);
            }
            else
            {
                OnPlayerTriggerNailbomb(i, throwerid, nailBmbIdx, x, y, z);
            }
            break;
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerTriggerNailbomb ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerTriggerNailbomb(triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
    {
        ApplyAnimation(triggererid, "PED", "EV_STEP", 4.1, 0, 1, 1, 0, 0, 0);
        Audio3D_Play(triggererid, AUDIO_NAILBMB_TRIGGER, x, y, z, 10.0);
        defer OnNailbombExplode(triggererid, throwerid, nailBmbIdx, x, y, z);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------------- OnNailbombExplode ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    timer OnNailbombExplode[NAILBMB_TRIGGERED_EXPLODE](triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
    {
        if(IsPlayerInRangeOfPoint(triggererid, NAILBOMB_EXPLODE_RANGE, x, y, z))
        {
            if(GetPlayerEquippedItemId(triggererid) != -1)
            {
                if(!IsItemType(GetPlayerEquippedItemId(triggererid), ITEM_TYPE_BULLET_WEAPON))
                {
                    HideItem(triggererid);
                }
                else
                {
                    if(!Bit_Get(ePlayerFlag[e_bShotNailbomb], triggererid))
                    {
                        HideItem(triggererid);
                    }
                }
            }

            SetPlayerStatus(triggererid, PLAYER_STATUS:PLAYER_STATUS_FALL);
            SetPlayerHealthEx(triggererid, GetPlayerHealthEx(triggererid) - DAMAGE_AMOUNT_NAIL_BOMB, throwerid, _:ITEM_IDX_NAIL_BOMB);

            if(!IsPlayerDead(triggererid))
            {
                defer DelayedTimer_AnimGetUp[1000](triggererid);
            }
        }

        CreateProjectileDestroyEffect(_:ITEM_IDX_NAIL_BOMB, nailBmbIdx, x, y, z);
        return 0;
    }
//#endregion

//#region Projectile internal callbacks
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //----------------------- OnProjectileUpdate --------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnProjectileUpdate(projid)
    {
        new Float:x, Float:y, Float:z;
        GetProjectilePos(projid, x, y, z);
        SetDynamicObjectPos(eProjectileData[projid][e_iProjectile_ObjectId], x, y, z);

        GetProjectileRot(projid, x, y, z);
        SetDynamicObjectRot(eProjectileData[projid][e_iProjectile_ObjectId], x, y, z);
        return 1;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------------- OnProjectileCollide --------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid)
    {
        // If I hitted myself (during throwing) ignore collision.
        new throwerid = eProjectileData[projid][e_iProjectile_ThrowerId];
        if(type == PROJECTILE_COLLIDE_PLAYER && extraid == throwerid)
        {
            return 1;
        }

        new itemId = Streamer_GetIntData(STREAMER_TYPE_OBJECT, eProjectileData[projid][e_iProjectile_ObjectId], E_STREAMER_EXTRA_ID);

        switch(ITEM_IDX:itemId)
        {
            case ITEM_IDX_BOTTLE, ITEM_IDX_BRICK:
            {
                OnThrowableDestroy(projid, throwerid, (type == PROJECTILE_COLLIDE_PLAYER) ? (extraid) : (INVALID_PLAYER_ID), itemId, x, y, z);
            }

            case ITEM_IDX_MOLOTOV:
            {
                OnMolotovExplode(projid, throwerid, (type == PROJECTILE_COLLIDE_PLAYER) ? (extraid) : (INVALID_PLAYER_ID), x, y, z);
            }

            case ITEM_IDX_SMOKE_BOMB:
            {
                if(type == PROJECTILE_COLLIDE_PLAYER)
                {
                    OnSmokebombExplode(projid, throwerid, extraid, x, y, z);
                }
            }

            case ITEM_IDX_NAIL_BOMB:
            {
                if(type == PROJECTILE_COLLIDE_PLAYER)
                {
                    OnNailbombExplode(projid, throwerid, extraid, x, y, z);
                }
            }
        }
        return 1;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------------ OnProjectileStop ---------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnProjectileStop(projid, Float:x, Float:y, Float:z)
    {
        new throwerid = eProjectileData[projid][e_iProjectile_ThrowerId];
        new itemId = Streamer_GetIntData(STREAMER_TYPE_OBJECT, eProjectileData[projid][e_iProjectile_ObjectId], E_STREAMER_EXTRA_ID);

        if(itemId == _:ITEM_IDX_SMOKE_BOMB)
        {
            OnSmokebombExplode(projid, throwerid, INVALID_PLAYER_ID, x, y, z);
        }
        else if(itemId == _:ITEM_IDX_NAIL_BOMB)
        {
            InitiateNailbomb(projid, throwerid, x, y, z);
        }
        return 1;
    }
//#endregion

//#region Projectiles utils
    // Function assigned to do the work related with projectiles throw.
    Projectile_Throw(playerid, itemId)
    {
        // Validation.
        if(GetPlayerItemAmount(playerid, itemId) <= 0 || GetProjectilePoolSize() >= MAX_THROWABLES || Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid)
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
//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    Iter_Clear(NailBmbsList);
    return 1;
}