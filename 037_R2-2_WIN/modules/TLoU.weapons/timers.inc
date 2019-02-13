// Smokebomb defer timer - destroy smokebomb effect.
timer Destroy_SmokebombEffect[TICK_RATE_SMOKEBMB_EFFECT](objectid)
{
    if(IsValidDynamicObject(objectid))
        DestroyDynamicObject(objectid);
}

// Nailbomb update timer - explodes when player is near it.
timer Update_Nailbomb[TICK_RATE_NAILBMB_UPDATE](projid, Float:x, Float:y, Float:z)
{
    // for loop so it's going to apply for both players and NPCs.
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i)) continue;

        if(IsPlayerInRangeOfPoint(i, NAILBMB_EXPLODE_RANGE, x, y, z))
        {
            OnProjectileDestroy(projid, x, y, z);
            break;
        }
    }
}