// Return current FCNPCs animation index.
FCNPC_GetAnimationIndex(npcid)
{
    new animationid;
    if(FCNPC_GetAnimation(npcid, animationid))
    {
        return animationid;
    }
    return -1;
}

// Custom SetAnimation for FCNPC.
FCNPC_SetAnimCallback(npcid, animationid, bool:freeze = false)
{
    if(FCNPC_ValidateStatusChange(npcid, STATUS_PLAYING_ANIM) == false)
    {
        return false;
    }

    new tickCounter = -6;
    switch(animationid)
    {
        case KISSING_GF_CARARGUE_02: 	   	tickCounter = -10; // IDLE
        case PED_GAS_CWR:					tickCounter = -4;  // STUNNED
        case PED_IDLE_TIRED:				tickCounter = -4;  // STUNNED
        case BASEBALL_BAT_HIT_3:			tickCounter = -8;  // SHOT BY SHOTGUN
        case PED_GETUP: 				   	tickCounter = -4;  // GET UP
    }
    
    eNpcInfo[npcid][e_iPlayingAnim_Tick] = tickCounter;
    eNpcInfo[npcid][e_iPlayingAnim_Index] = animationid;
    
    FCNPC_SetStatus(npcid, STATUS_PLAYING_ANIM);
    FCNPC_SetAnimation(npcid, animationid, 4.1, 0, 1, 1, freeze, 0);
    return true;
}

// Plays idle anims on NPC.
FCNPC_PlayIdleActions(npcid)
{
    if(FCNPC_IsIdle(npcid))
    {
        switch(RandomEx(0, 12))
        {
            case 4, 8: FCNPC_RandomMoveInDynamicArea(npcid);
            default: FCNPC_SetAnimCallback(npcid, NpcIdleAnimsList[random(sizeof(NpcIdleAnimsList))]);
        }
    }
}

// Clear NPCs anims and packets data.
FCNPC_ClearAnims(npcid)
{
    if(FCNPC_IsPlayingAnim(npcid))
    {
        eNpcInfo[npcid][e_iPlayingAnim_Tick] = 0;
        eNpcInfo[npcid][e_iPlayingAnim_Index] = 0;
    }

    FCNPC_ResetAnimation(npcid);
    FCNPC_ClearAnimations(npcid);
}

// Plays death animation for dead NPC only
// Optionally specify a bodypart parameter so it's gonna use different pre-defined animations.
function FCNPC_PlayDeathAnimation(npcid, weaponid, bodypart)
{
    if(!FCNPC_IsDeadEx(npcid))
    {
        return false;
    }

    switch(weaponid)
    {
        case WEAPON_SHOTGUN, WEAPON_NAIL_BOMB:
        {
            if(FCNPC_GetAnimationIndex(npcid) != BASEBALL_BAT_HIT_3)
			{
                FCNPC_SetAnimation(npcid, BASEBALL_BAT_HIT_3, 4.1, 0, 1, 1, 1, 0);
			}
        }

        case WEAPON_SHIV:
        {
            FCNPC_SetAnimation(npcid, KNIFE_KILL_KNIFE_PED_DAMAGE, 4.1, 0, 1, 1, 0, 0);
            wait_ms(1700);

            FCNPC_SetAnimation(npcid, KNIFE_KILL_KNIFE_PED_DIE, 4.1, 0, 1, 1, 1, 0);
        }

        case WEAPON_MOLOTOV:
        {
            FCNPC_SetAnimation(npcid, PED_FLOOR_HIT_F, 4.1, 0, 1, 1, 1, 0);
        }

        default:
        {
            FCNPC_SetAnimation(npcid, \
                (bodypart == BODY_PART_HEAD) ? (PED_KO_SHOT_FACE) : (PED_KO_SHOT_STOM), \
                    4.1, 0, 1, 1, 1, 0);
        }
    }
    return true;
}

// Plays 'dead' animation when hit/shot by player.
// Applies only if NPC is dead.
FCNPC_PlayDeadAnimIfDead(npcid)
{
    if(!FCNPC_IsDeadEx(npcid))
    {
        return;
    }

    switch(FCNPC_GetAnimationIndex(npcid))
    {
        case 
            BASEBALL_BAT_HIT_3, 
            PED_KO_SHOT_FACE, 
            KNIFE_KILL_KNIFE_PED_DIE:
            {
                FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0);
            }

        default: 
        {
            FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT_F", 4.1, 0, 1, 1, 1, 0);
        }
    }
}

// Plays damage animations on NPC when shot.
bool:FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId = -1, Float:distanceBetweenPlayers = 0.0)
{
    if(FCNPC_IsUnableToPerformAction(npcid)
    || (GetTickCount() - eNpcInfo[npcid][e_iTickCounter_damageAnim]) < TICK_RATE_DAM_ANIM)
    {
        return false;
    }
    eNpcInfo[npcid][e_iTickCounter_damageAnim] = GetTickCount();
    
    FCNPC_ClearAnims(npcid);

    switch(bodypart)
    {
        case BODY_PART_LEFT_ARM:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3)], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_ARM: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 3], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_LEFT_LEG:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 6], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_LEG: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 9], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_TORSO:
        {
            if(issuerWeaponId == WEAPON_SHOTGUN && distanceBetweenPlayers <= SHOTGUN_TORSO_SHOT_DISTANCE)
            {
                FCNPC_SetStatus(npcid, STATUS_FALL);
            }
            else
            {
                FCNPC_SetAnimation(npcid, NpcDamageAnims[random(4) + 12], 4.1, 0, 1, 1, 0, 0);
            }
        }
    }
    return true;
}

// Check if animation has finished. 
// If it hasn't increase the counter until it reaches '0'
bool:FCNPC_HasCallbackAnimFinished(npcid, animIdx)
{
    return (FCNPC_GetAnimationIndex(npcid) != animIdx || eNpcInfo[npcid][e_iPlayingAnim_Tick] >= 0);
}