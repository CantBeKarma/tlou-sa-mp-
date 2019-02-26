// Apply animation when shot in close-range with shotgun.
FlyBackwardsWhenShot(damagedid)
{
    if(IsPlayerNPC(damagedid))
    {
        if(eNpcInfo[damagedid][e_iType] == NPC_TYPE_BLOATER) 
            return 1;

        FCNPC_StopUpdateTimer(damagedid);
        FCNPC_SetAnimation(damagedid, BASEBALL_BAT_HIT_3, 4.1, 0, 1, 1, 1, 0);

        if(!FCNPC_IsDeadEx(damagedid))
            defer DelayedTimer_AnimGetUp[1000](damagedid);
    }
    else 
    {
        ApplyAnimation(damagedid, "BASEBALL", "BAT_HIT_3", 4.0, 0, 1, 1, 1, 0, 1);

        if(!IsPlayerDead(damagedid))
            defer DelayedTimer_AnimGetUp[1000](damagedid);
    }
    return 0;
}

// Plays damage animations on player.
PlayPlayerDamageAnimations(damagedId, bodypart, issuerWeaponId, Float:distance)
{
    if(IsPlayerUnableToPerformAction(damagedId))
        return;

    if((GetConnectedTime(damagedId) - ePlayerInfo[damagedId][e_iTickCounter_damageAnim]) < TICK_RATE_DAM_ANIM)
        return;

    switch(bodypart)
    {
        case BODY_PART_LEFT_ARM:  ApplyAnimation(damagedId, "PED", DamageAnimations[random(3)], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_ARM: ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 3], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_LEFT_LEG:  ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 6], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_LEG: ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 9], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_TORSO:
        {
            if(issuerWeaponId == WEAPON_SHOTGUN && distance <= SHOTGUN_TORSO_SHOT_DISTANCE)
            {
                FlyBackwardsWhenShot(damagedId);
            }
            else
            {
                ApplyAnimation(damagedId, "PED", DamageAnimations[random(4) + 12], 4.1, 0, 1, 1, 0, 0);
            }
        }
    }
    ePlayerInfo[damagedId][e_iTickCounter_damageAnim] = GetConnectedTime(damagedId);
}