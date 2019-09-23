#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnThrowableDestroy -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnThrowableDestroy(projid, throwerid, hitid, itemId, Float:x, Float:y, Float:z)
{
    CreateProjectileDestroyEffect(itemId, projid, x, y, z);

    if(hitid != INVALID_PLAYER_ID)
    {
        if(IsPlayerNPC(hitid))
        {
            FCNPC_OnGetHitByThrowable(hitid, throwerid, itemId);
        }
        else
        {
            OnPlayerGetHitByThrowable(hitid, throwerid, itemId);
        }
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
                if(!HasPlayerShotNailbomb(triggererid))
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
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
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