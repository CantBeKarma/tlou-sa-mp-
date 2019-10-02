//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnStatusChanged(npcid, NPC_STATUS:oldStatusId, NPC_STATUS:newStatusId, statusIssuerId)
{
    if(newStatusId == STATUS_DISABLED
    || newStatusId == STATUS_IDLE
    || newStatusId == STATUS_FALL
    || newStatusId == STATUS_BURNING
    || newStatusId == STATUS_STUNNED
    || newStatusId == STATUS_DEAD
    || newStatusId == STATUS_PLAYING_ANIM)
    {
        FCNPC_StopMoving(npcid);
    }

    if(newStatusId == STATUS_CHASE
    || newStatusId == STATUS_ATTACK
    || newStatusId == STATUS_BURNING
    || newStatusId == STATUS_PLAYING_ANIM)
    {
        FCNPC_StartUpdateTimer(npcid);
    }

    switch(newStatusId)
    {
        case 
            STATUS_DISABLED,
            STATUS_DEAD:
        {
            FCNPC_DestroyRoamArea(npcid);
            FCNPC_StopUpdateTimer(npcid);
        }

        case STATUS_IDLE:
        {
            FCNPC_PlayIdleActions(npcid);
        }

        case STATUS_FALL:
        {
            FCNPC_SetAnimCallback(npcid, BASEBALL_BAT_HIT_3, .freeze = true);
        }
    }
    return 0;
}

// STATUS_DISABLED,
// STATUS_IDLE,
// STATUS_FALL
// STATUS_CHASE,
// STATUS_ATTACK,
// STATUS_DISTRACTED,
// STATUS_BURNING,
// STATUS_STUNNED,
// STATUS_DEAD,
// STATUS_PLAYING_ANIM