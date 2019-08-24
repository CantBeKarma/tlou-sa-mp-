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
    if(!IsPlayerAimingWeapon(playerid) || Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid))
    {
        return 1;
    }

    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);

    new currentWeaponId = CurrentPlayerWeaponId[playerid];
    
    // If player was aiming arrow and moved.
    if(ePlayerWeapon[playerid][e_bAimingArrow])
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
        if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
        {
            ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);
        }
        else if(currentWeaponId == WEAPON_BOW)
        {
            Bow_PullString(playerid);
        }
    }

    // Player is not moving.
    if(updown == 0 && leftright == 0)
    {
        if(keys & KEY_FIRE)
        {
            if(IsProjectileWeapon(currentWeaponId) || IsThrowableWeapon(currentWeaponId))
            {
                Projectile_Throw(playerid, currentWeaponId);
            }
            else if(currentWeaponId == WEAPON_BOW)
            {
                Arrow_FireArrow(playerid);
            }
        }
    }

    if(IsPlayerAimingWeapon(playerid) && !Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid))
    {
        SetAngleBasedOnCam(playerid);
        defer Key_AimingCustomWeapon(playerid);
    }
    return 0;
}