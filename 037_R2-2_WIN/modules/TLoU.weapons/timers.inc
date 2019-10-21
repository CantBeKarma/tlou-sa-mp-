// Destroy SmokeBomb smoke effect and reset projectile's data.
timer DestroySmkBmbSmokeObject[TICK_RATE_SMOKEBMB_EFFECT](objectId)
{
    if(!IsValidDynamicObject(objectId))
        return;

    DestroyDynamicObject(objectId);

    foreach(new playerid : Player)
    {
        Streamer_Update(playerid, STREAMER_TYPE_OBJECT);
    }
}

// Movement animation while aiming a custom weapon
// (Firing/throwing custom weapons)
timer Key_AimingCustomWeapon[100](playerid)
{
    if(!IsPlayerAimingWeapon(playerid) || IsPlayerAimOrShootCustomItem(playerid))
    {
        return 1;
    }

    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);

    new equippedItemId = GetPlayerEquippedItemId(playerid);
    
    // If player was aiming arrow and moved.
    if(IsPlayerAimingBow(playerid))
    {
        if(updown != 0 || leftright != 0)
        {
            Bow_ReleaseString(playerid);
        }
    }

    // Moving & idle animations.
    if(updown == KEY_UP)            ApplyAnimation(playerid, "PED", "GUNMOVE_FWD", 4.1, 1, 1, 1, 0, 0);
    else if(updown == KEY_DOWN)     ApplyAnimation(playerid, "PED", "GUNMOVE_BWD", 4.1, 1, 1, 1, 0, 0);
    else if(leftright == KEY_LEFT)  ApplyAnimation(playerid, "PED", "GUNMOVE_L", 4.1, 1, 1, 1, 0, 0);
    else if(leftright == KEY_RIGHT) ApplyAnimation(playerid, "PED", "GUNMOVE_R", 4.1, 1, 1, 1, 0, 0);
    else
    {
        if(GetPlayerAnimationIndex(playerid) != PED_GETUP
        && GetPlayerAnimationIndex(playerid) != BASEBALL_BAT_HIT_3)
        {
            if(IsItemType(equippedItemId, ITEM_TYPE_THROWABLE) || IsItemType(equippedItemId, ITEM_TYPE_PROJECTILE))
            {
                ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);
            }
            else if(equippedItemId == _:ITEM_IDX_BOW)
            {
                Bow_PullString(playerid);
            }
        }
    }

    // Player is not moving.
    if(updown == 0 && leftright == 0)
    {
        if(keys & KEY_FIRE)
        {
            if(IsItemType(equippedItemId, ITEM_TYPE_THROWABLE) || IsItemType(equippedItemId, ITEM_TYPE_PROJECTILE))
            {
                Projectile_Throw(playerid, equippedItemId);
            }
            else if(equippedItemId == _:ITEM_IDX_BOW)
            {
                Arrow_FireArrow(playerid);
            }
        }
    }

    if(IsPlayerAimingWeapon(playerid) && !IsPlayerAimOrShootCustomItem(playerid))
    {
        SetAngleBasedOnCam(playerid);
        defer Key_AimingCustomWeapon(playerid);
    }
    return 0;
}