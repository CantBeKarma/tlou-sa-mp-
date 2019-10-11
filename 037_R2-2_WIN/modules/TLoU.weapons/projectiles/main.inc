#include <TLoU.weapons\projectiles\forwards>
#include <TLoU.weapons\projectiles\callbacks>

#include <YSI\y_hooks>

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

    if(type == PROJECTILE_COLLIDE_PLAYER && IsPlayerNPC(extraid) && FCNPC_IsUnableToPerformAction(extraid))
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
                OnNailbombExplode(extraid, throwerid, projid, x, y, z);
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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    Iter_Clear(NailBmbsList);
    return 1;
}