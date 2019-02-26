// Destroy SmokeBomb smoke effect and reset projectile's data.
timer DestroySmokeBombData[TICK_RATE_SMOKEBMB_EFFECT](projid)
{
    if(IsValidDynamicObject(eProjectileData[projid][e_iSmkBmbSmoke_ObjectId]))
    {
        DestroyDynamicObject(eProjectileData[projid][e_iSmkBmbSmoke_ObjectId]);
    }
    ResetProjectileData(projid);

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }
}