// Nailbomb update timer - explodes when player is near it.
timer Update_NailBombTimer[TICK_RATE_NAILBMB_UPDATE](projid, Float:x, Float:y, Float:z)
{
    // for loop so it's going to apply for both players and NPCs.
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i)) 
            continue;

        if(IsPlayerInRangeOfPoint(i, NAILBMB_EXPLODE_RANGE, x, y, z))
        {
            OnProjectileDestroy(projid, WEAPON_NAIL_BOMB, x, y, z);
            break;
        }
    }
}

// Destroy SmokeBomb smoke effect and reset projectile's data.
timer DestroySmokeBombData[TICK_RATE_SMOKEBMB_EFFECT](projid)
{
    if(IsValidDynamicObject(eProjectileData[projid][e_iSmkBmbSmoke_ObjectId]))
    {
        DestroyDynamicObject(eProjectileData[projid][e_iSmkBmbSmoke_ObjectId]);
    }
    ResetProjectileData(projid);
}