// Destroy SmokeBomb smoke effect and reset projectile's data.
timer DestroySmkBmbSmokeObject[TICK_RATE_SMOKEBMB_EFFECT](objectId)
{
    if(!Iter_Contains(SmkBmbSmokesList, objectId))
        return;

    if(!IsValidDynamicObject(objectId))
        return;

    DestroyDynamicObject(objectId);

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }
}