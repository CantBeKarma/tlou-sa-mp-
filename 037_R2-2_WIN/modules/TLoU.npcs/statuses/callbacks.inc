//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnStatusChanged(npcid, NPC_STATUS:oldStatusId, NPC_STATUS:newStatusId, statusIssuerId)
{
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
            FCNPC_StopUpdateTimer(npcid);
            FCNPC_StopMoving(npcid);
            FCNPC_DestroyRoamArea(npcid);
        }

        case STATUS_IDLE:
        {
            FCNPC_PlayIdleActions(npcid);
        }

        case STATUS_FALL:
        {
            FCNPC_StopUpdateTimer(npcid);
            FCNPC_StopMoving(npcid);
            FCNPC_SetAnimCallback(npcid, BASEBALL_BAT_HIT_3, .freeze = true);
        }

        case STATUS_BURNING:
        {
            FCNPC_StopMoving(npcid);
            SPAO(npcid, OBJECT_MODEL_BURNING);
        }

        case STATUS_STUNNED:
        {
            FCNPC_StopUpdateTimer(npcid);
            FCNPC_StopMoving(npcid);
            CLF(#FCNPC_Stun, "d", npcid);
        }
        
        case STATUS_PLAYING_ANIM:
        {
            FCNPC_StopMoving(npcid);
        }
    }
    return 0;
}

// STATUS_DISABLED,
// STATUS_IDLE,
// STATUS_FALL
// STATUS_CHASE,
// STATUS_ATTACK,
// STATUS_BURNING,
// STATUS_STUNNED,
// STATUS_DEAD,
// STATUS_PLAYING_ANIM