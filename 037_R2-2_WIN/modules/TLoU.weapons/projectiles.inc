#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartAimingProjectile -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingProjectile(playerid, weaponId)
{
    KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
    
    ShowPlayerCrosshair(playerid);
    ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);

    // If player was holding ANY key before aiming projectile - force change the old holding key to KEY_AIM
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
        HidePlayerWeapon(playerid, CurrentPlayerWeaponId[playerid]);
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
        HidePlayerWeapon(playerid, CurrentPlayerWeaponId[playerid]);
    }

    ApplyAnimation(playerid, "GRENADE", "WEAPON_THROWU", 3.0, 0, 1, 1, 0, 0);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnProjectileUpdate --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnProjectileUpdate(projid)
{
    new Float:x, Float:y, Float:z;
    GetProjectilePos(projid, x, y, z);
    SetDynamicObjectPos(eServerInfo[e_iProjectilesObjectId][projid], x, y, z);

    GetProjectileRot(projid, x, y, z);
    SetDynamicObjectRot(eServerInfo[e_iProjectilesObjectId][projid], x, y, z);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ OnProjectileStop ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnProjectileStop(projid, Float:x, Float:y, Float:z)
{
    if(Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID) == WEAPON_SMOKE_BOMB)
    {
        OnProjectileDestroy(projid, x, y, z);
    }
    else if(Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID) == WEAPON_NAIL_BOMB)
    {
        eProjectileData[projid][e_iNailBmb_Timer] = repeat Update_Nailbomb(projid, x, y, z);
        eProjectileData[projid][e_iNailBmbSmoke_ObjectId] = CreateDynamicObject(OBJ_MODEL_NAILBOMB_SMOKE, x + 0.06, y, z - 1.5, 0.0, 0.0, 0.0);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnProjectileDestroy --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnProjectileDestroy(projid, Float:x, Float:y, Float:z)
{
    new audioId = -1;
    switch(Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID))
    {
        case WEAPON_BOTTLE:     audioId = AUDIO_BOTTLE_BREAK;
        case WEAPON_BRICK:      audioId = AUDIO_BRICK_BREAK;
        case WEAPON_MOLOTOV:
        {
            audioId = AUDIO_MOLOTOV_EXPLODE;
            CreateFire(x, y, z);
        }

        case WEAPON_NAIL_BOMB:
        {
            audioId = AUDIO_NAILBMB_EXPLODE;
            stop eProjectileData[projid][e_iNailBmb_Timer];
            
            CreateDynamicObject(OBJ_MODEL_EXPLOSION, x, y, z, 0.0, 0.0, 0.0);
            DestroyDynamicObject(eProjectileData[projid][e_iNailBmbSmoke_ObjectId]);
        }

        case WEAPON_SMOKE_BOMB:
        {
            audioId = AUDIO_SMKBMB_EXPLODE;
        
            eProjectileData[projid][e_iSmkBmb_ObjectId] = CreateDynamicObject(OBJ_MODEL_SMOKE, x, y, z, 0.0, 0.0, 0.0);
            defer Destroy_SmokebombEffect(eProjectileData[projid][e_iSmkBmb_ObjectId]);
        }
    }

    // Destroy projectile's data.
    DestroyProjectile(projid);
    DestroyDynamicObject(eServerInfo[e_iProjectilesObjectId][projid]);
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, E_STREAMER_EXTRA_ID, projid, -1);

    if(audioId == -1)
        return 1;
    
    // Configure audio distance of the projectile.
    new Float:audioDistance = 
        ( audioId == AUDIO_NAILBMB_EXPLODE ? DIST3D_EXPLODE_RANGE
        : audioId == AUDIO_MOLOTOV_EXPLODE ? DIST3D_EXPLODE_RANGE
        : audioId == AUDIO_SMKBMB_EXPLODE  ? DIST3D_EXPLODE_RANGE
        : DIST3D_THROWABLE_BREAK_RANGE );
    
    // Play 3D Sound effect for nearby players.
    foreach(new playerid : Player)
    {
        if(IsPlayerInRangeOfPoint(playerid, audioDistance, x, y, z))
        {
            Audio3D_Play(playerid, audioId, x, y, z, DIST3D_EXPLODE_RANGE);
        }
        Streamer_UpdateEx(playerid, x, y, z, .type = STREAMER_TYPE_OBJECT);

        if(audioId == AUDIO_SMKBMB_EXPLODE)
        {
            // Stun player if he's in smokebomb range.
            if(IsPlayerInRangeOfPoint(playerid, SMOKEBMB_AFFECT_RANGE, x, y, z))
                StunPlayer(playerid);
        }
    }
    
    foreach(new npcid : FCNPC)
    {
        if(Bit_Get(eNpcFlag[e_bIsStunned], npcid) || FCNPC_IsBurning(npcid) || FCNPC_IsDeadEx(npcid))
            continue;
        
        if(audioId == AUDIO_SMKBMB_EXPLODE)
        {
            // Stun NPC if he's in smokebomb range.
            if(IsPlayerInRangeOfPoint(npcid, SMOKEBMB_AFFECT_RANGE, x, y, z))
            {
                FCNPC_Stun(npcid);
                continue;
            }
        }

        // Distract FCNPC with the sound effects.
        if(IsPlayerInRangeOfPoint(npcid, audioDistance + 5.0, x, y, z))
        {
            FCNPC_Distract(npcid, x, y, z);
        }
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnProjectileCollide --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnProjectileCollide(projid, type, Float:x, Float:y, Float:z, extraid)
{
    if(Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID) == WEAPON_BOTTLE
    || Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID) == WEAPON_BRICK
    || Streamer_GetIntData(STREAMER_TYPE_OBJECT, eServerInfo[e_iProjectilesObjectId][projid], E_STREAMER_EXTRA_ID) == WEAPON_MOLOTOV)
    {
        OnProjectileDestroy(projid, x, y, z);
    }
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerHoldingKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerHoldingKey(playerid, key)
{
    if(key & KEY_AIM)
    {
		if(Bit_Get(ePlayerFlag[e_bAiming], playerid))
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
//------------------- OnPlayerKeyStateChange ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;
    
    if(newkeys & KEY_FIRE)
    {
        // Throw projectiles & sissy throw of a nailbomb (close throw).
        new currentWeaponId = CurrentPlayerWeaponId[playerid];
        if(Bit_Get(ePlayerFlag[e_bAiming], playerid))
        {
            if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
            {
                Projectile_Throw(playerid, CurrentPlayerWeaponId[playerid]);
            }
        }
        else
        {
            if(!IsPlayerUsingBackpack(playerid) 
            && !IsPlayerUsingWeaponMenu(playerid) && !IsPlayerPlayingActions(playerid))
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

// Function assigned to do the work related with projectiles throw.
Projectile_Throw(playerid, weaponId)
{
    // Validation.
	if(GetPlayerWeaponAmmo(playerid, weaponId) <= 0 
    || GetProjectilePoolSize() >= MAX_THROWABLES //|| !IsProjectileWeapon(weaponId) || !IsThrowableWeapon(weaponId)
    || GetConnectedTime(playerid) - (ePlayerWeapon[playerid][e_iTickCounter_projectileThrow]) < TICK_RATE_PROJECTILE_THROW)
	{
		return 1;
	}
    ePlayerWeapon[playerid][e_iTickCounter_projectileThrow] = GetConnectedTime(playerid);
	
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

	new projId = CreateProjectile(dest_projX, dest_projY, dest_projZ, \
                    (MAX_PROJECTILE_SPEED * fVX), (MAX_PROJECTILE_SPEED * fVY), (MAX_PROJECTILE_SPEED * fVZ) + 5.0, \
                    0.0, 0.0, 0.0, \
                    PROJ_PHYS_SPHERECOL_RADIUS, PROJ_PHYS_GROUND_FRICTION, PROJ_PHYS_COLL_FRICTION, PROJ_PHYS_AIR_ASSISTANCE, PROJ_PHYS_GRAVITY, PROJ_PHYS_PLAYER_COL_RADIUS, PROJ_PHYS_COLLIDE_SIMULATION, PROJ_PHYS_MASS);

    if(projId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Throw] Unable to create projectile (ID: %d) for player: %s (ID: %d). Weapon equipped: %d", projId, PlayerName(playerid), playerid, weaponId);
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

	eServerInfo[e_iProjectilesObjectId][projId] = projObjectId;
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, weaponId);

    // Update.
	foreach(new i : Player)
	{
		Streamer_UpdateEx(i, dest_projX, dest_projY, dest_projZ, .type = STREAMER_TYPE_OBJECT);
	}

    OnPlayerThrowProjectile(playerid, weaponId);
	return 0;
}

// Drop Nailbomb near player (sissy throw).
Projectile_Drop_Nailbomb(playerid)
{
    // Validation.
	if(GetPlayerWeaponAmmo(playerid, WEAPON_NAIL_BOMB) <= 0 
    || GetProjectilePoolSize() >= MAX_THROWABLES || CurrentPlayerWeaponId[playerid] != WEAPON_NAIL_BOMB
    || GetConnectedTime(playerid) - (ePlayerWeapon[playerid][e_iTickCounter_projectileThrow]) < TICK_RATE_PROJECTILE_THROW)
	{
		return 1;
	}
    ePlayerWeapon[playerid][e_iTickCounter_projectileThrow] = GetConnectedTime(playerid);

    new Float:x, Float:y, Float:z, Float:ang;
	GetPlayerPos(playerid, x, y, z);
	GetPlayerFacingAngle(playerid, ang);

    new projId = CreateProjectile(x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, \
                5.0 * floatsin(-ang, degrees), 5.0 * floatcos(-ang, degrees), 3.0, \
                0.0, 0.0, 0.0, \
                PROJ_PHYSDROP_SPHERECOL_RADIUS, PROJ_PHYSDROP_GROUND_FRICTION, PROJ_PHYSDROP_COLL_FRICTION, PROJ_PHYSDROP_AIR_ASSISTANCE, PROJ_PHYSDROP_GRAVITY, PROJ_PHYSDROP_PLAYER_COL_RADIUS, PROJ_PHYSDROP_COLLIDE_SIMULATION, PROJ_PHYSDROP_MASS);

    if(projId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Drop_Nailbomb] Unable to create projectile (ID: %d) for player: %s (ID: %d)", projId, PlayerName(playerid), playerid);
        return 1;
    }

    // Object creation.
    new projObjectId = CreateDynamicObject(ITEM_MODEL_BOMB, x, y - 0.5 * floatcos(-(ang + 90.0), degrees), z, 93.7, 120.0, ang + 60.0);
    if(projObjectId == INVALID_OBJECT_ID)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "[Projectile_Drop_Nailbomb] Invalid object Id. (Player data: %s [%d])", PlayerName(playerid), playerid);
        return 1;
    }

    eServerInfo[e_iProjectilesObjectId][projId] = projObjectId;
    Streamer_SetIntData(STREAMER_TYPE_OBJECT, projObjectId, E_STREAMER_EXTRA_ID, WEAPON_NAIL_BOMB);

    // Update.
	foreach(new i : Player)
	{
		Streamer_Update(i, STREAMER_TYPE_OBJECT);
	}

	OnPlayerDropNailbomb(playerid);
    return 0;
}