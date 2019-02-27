#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//#region Aiming and throwing callbacks
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------- OnPlayerStartAimingProjectile -------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerStartAimingProjectile(playerid, weaponId)
    {
        ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);
        ForceChangeHoldingKeyDef(playerid, KEY_AIM);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------- OnPlayerStopAimingProjectile -------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerStopAimingProjectile(playerid)
    {
        if(GetPlayerAnimationIndex(playerid) != GRENADE_WEAPON_THROW)
        {
            ClearAnims(playerid);
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerThrowProjectile ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerThrowProjectile(playerid, weaponId)
    {
        new newProjectileAmount = GetPlayerWeaponAmmo(playerid, weaponId)-1;
        SetPlayerWeaponAmmo(playerid, weaponId, newProjectileAmount);

        if(newProjectileAmount <= 0)
        {
            HidePlayerWeapon(playerid);
        }

        ApplyCallbackAnim(playerid, "GRENADE", "WEAPON_THROW");
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------ OnPlayerDropNailbomb -----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerDropNailbomb(playerid)
    {
        new newProjectileAmount = GetPlayerWeaponAmmo(playerid, WEAPON_NAIL_BOMB)-1;
        SetPlayerWeaponAmmo(playerid, WEAPON_NAIL_BOMB, newProjectileAmount);

        if(newProjectileAmount <= 0)
        {
            HidePlayerWeapon(playerid);
        }

        ApplyAnimation(playerid, "GRENADE", "WEAPON_THROWU", 3.0, 0, 1, 1, 0, 0);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //------------------- OnPlayerHoldingKey ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    hook OnPlayerHoldingKey(playerid, key)
    {
        if(key & KEY_AIM)
        {
            if(IsPlayerAimingWeapon(playerid))
            {
                new currentWeaponId = CurrentPlayerWeaponId[playerid];
                if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
                {
                    SetAngleBasedOnCam(playerid);
                }
            }
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerKeyStateChange -----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
    {
        if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid))
            return 1;
        
        if(newkeys & KEY_FIRE)
        {
            // Throw projectiles & sissy throw of a nailbomb (close throw).
            new currentWeaponId = CurrentPlayerWeaponId[playerid];
            if(IsPlayerAimingWeapon(playerid))
            {
                if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
                {
                    Projectile_Throw(playerid, CurrentPlayerWeaponId[playerid]);
                }
            }
            else
            {
                if(!IsPlayerUsingBackpack(playerid) 
                && !IsPlayerUsingWeaponMenu(playerid) && !IsPlayerUnableToPerformAction(playerid))
                {
                    if(currentWeaponId == WEAPON_NAIL_BOMB)
                    {
                        Projectile_Drop_Nailbomb(playerid);
                    }
                }
            }
        }
        return 1;
    }
//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnThrowableDestroy -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnThrowableDestroy(projid, throwerid, hitid, weaponid, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(weaponid, projid, x, y, z);

    if(hitid != INVALID_PLAYER_ID && IsPlayerNPC(hitid))
    {
        SCMF(throwerid, -1, "You hitted NPCID: %d with throwable!", hitid);
    }

    // foreach(new npcid : FCNPC)
    // {
    //     if(FCNPC_Distract(npcid, x, y, z) == 1)
    //         continue;
    // }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnMolotovExplode --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnMolotovExplode(projid, throwerid, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(WEAPON_MOLOTOV, projid, x, y, z);

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
    CreateProjectileDestroyEffect(WEAPON_SMOKE_BOMB, projid, x, y, z);
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
            || !IsPlayerInRangeOfPoint(i, NAILBMB_EXPLODE_RANGE, x, y, z))
            {
                continue;
            }

            if(IsPlayerNPC(i))
            {
                FCNPC_OnTriggerNailbomb(i, throwerid, nailBmbIdx, x, y, z);
            }
            else
            {
                OnPlayerTriggerNailbomb(i, throwerid, nailBmbIdx, x, y, z);
            }

            stop eProjectileData[nailBmbIdx][e_iNailBmb_Timer];
            break;
        }
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //---------------- OnPlayerTriggerNailbomb ----------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    public OnPlayerTriggerNailbomb(triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
    {
        Audio3D_Play(triggererid, AUDIO_NAILBMB_TRIGGER, x, y, z, 10.0);
        defer OnNailbombExplode(triggererid, throwerid, nailBmbIdx, x, y, z);
        return 0;
    }

    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    //-------------------- OnNailbombExplode ------------------
    //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
    timer OnNailbombExplode[NAILBMB_TRIGGERED_EXPLODE](triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
    {
        CreateProjectileDestroyEffect(WEAPON_NAIL_BOMB, nailBmbIdx, x, y, z);
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

        new weaponId = Streamer_GetIntData(STREAMER_TYPE_OBJECT, eProjectileData[projid][e_iProjectile_ObjectId], E_STREAMER_EXTRA_ID);

        switch(weaponId)
        {
            case WEAPON_BOTTLE, WEAPON_BRICK:
            {
                OnThrowableDestroy(projid, throwerid, (type == PROJECTILE_COLLIDE_PLAYER) ? (extraid) : (INVALID_PLAYER_ID), weaponId, x, y, z);
            }

            case WEAPON_MOLOTOV:
            {
                OnMolotovExplode(projid, throwerid, x, y, z);
            }

            case WEAPON_SMOKE_BOMB:
            {
                if(type == PROJECTILE_COLLIDE_PLAYER)
                {
                    OnSmokebombExplode(projid, throwerid, extraid, x, y, z);
                }
            }

            case WEAPON_NAIL_BOMB:
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
        new weaponId = Streamer_GetIntData(STREAMER_TYPE_OBJECT, eProjectileData[projid][e_iProjectile_ObjectId], E_STREAMER_EXTRA_ID);

        if(weaponId == WEAPON_SMOKE_BOMB)
        {
            OnSmokebombExplode(projid, throwerid, INVALID_PLAYER_ID, x, y, z);
        }
        else if(weaponId == WEAPON_NAIL_BOMB)
        {
            InitiateNailbomb(projid, throwerid, x, y, z);
        }
        return 1;
    }
//#endregion

//#region Projectiles utils
    // Function assigned to do the work related with projectiles throw.
    Projectile_Throw(playerid, weaponId)
    {
        // Validation.
        if(GetPlayerWeaponAmmo(playerid, weaponId) <= 0 || GetProjectilePoolSize() >= MAX_THROWABLES
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
            Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Throw] Unable to create projectile (ID: %d) for player: %s (ID: %d). Weapon equipped: %d", projid, PlayerName(playerid), playerid, weaponId);
            return 1;
        }

        // Object creation.
        new projObjectId = INVALID_OBJECT_ID;
        switch(weaponId)
        {
            case WEAPON_NAIL_BOMB:  projObjectId = CreateDynamicObject(ITEM_MODEL_BOMB,    dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);
            case WEAPON_MOLOTOV:    projObjectId = CreateDynamicObject(ITEM_MODEL_MOLOTOV, dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);
            case WEAPON_SMOKE_BOMB: projObjectId = CreateDynamicObject(ITEM_MODEL_SMKBMB,  dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);
            case WEAPON_BOTTLE:     projObjectId = CreateDynamicObject(ITEM_MODEL_BOTTLE,  dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);
            case WEAPON_BRICK:      projObjectId = CreateDynamicObject(ITEM_MODEL_BRICK,   dest_projX, dest_projY, dest_projZ + 0.5, 0, 0, 0);
        }

        if(projObjectId == INVALID_OBJECT_ID)
        {
            Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Throw] Invalid object Id for weapon id: %d (Player data: %s [%d])", weaponId, PlayerName(playerid), playerid);
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
        Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, weaponId);
        
        OnPlayerThrowProjectile(playerid, weaponId);
        return 0;
    }

    // Drop Nailbomb near player (sissy throw).
    Projectile_Drop_Nailbomb(playerid)
    {
        // Validation.
        if(GetPlayerWeaponAmmo(playerid, WEAPON_NAIL_BOMB) <= 0 || GetProjectilePoolSize() >= MAX_THROWABLES
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
        new projObjectId = CreateDynamicObject(ITEM_MODEL_BOMB, x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, 93.7, 120.0, ang + 60.0);
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
        Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, WEAPON_NAIL_BOMB);
        
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

    // Creates projectile destroy effect based on the weaponId.
    CreateProjectileDestroyEffect(weaponId, projid, Float:x, Float:y, Float:z)
    {
        switch(weaponId)
        {
            case WEAPON_MOLOTOV:
            {
                CreateFire(x, y, z);
            }

            case WEAPON_NAIL_BOMB:
            {
                if(IsValidDynamicObject(eProjectileData[projid][e_iNailBmb_SmokeObjectId]))
                {
                    DestroyDynamicObject(eProjectileData[projid][e_iNailBmb_SmokeObjectId]);
                }

                if(IsValidDynamicObject(eProjectileData[projid][e_iNailBmb_ObjectId]))
                {
                    DestroyDynamicObject(eProjectileData[projid][e_iNailBmb_ObjectId]);
                }
                
                Iter_Remove(NailBmbsList, projid);
                CreateDynamicObject(OBJ_MODEL_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);
            }

            case WEAPON_SMOKE_BOMB:
            {
                new smkBmbSmokeObjectId = CreateDynamicObject(OBJ_MODEL_SMOKE, x, y, z, 0.0, 0.0, 0.0);
                defer DestroySmkBmbSmokeObject(smkBmbSmokeObjectId);
            }
        }

        if(IsValidDynamicObject(eProjectileData[projid][e_iProjectile_ObjectId]) && weaponId != WEAPON_NAIL_BOMB)
        {
            ResetProjectileData(projid);
            DestroyDynamicObject(eProjectileData[projid][e_iProjectile_ObjectId]);
        }
        
        foreach(new playerid : Player)
        {
            Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
            PlayPlayer3D_ProjectileSFX(playerid, weaponId, x, y, z);
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