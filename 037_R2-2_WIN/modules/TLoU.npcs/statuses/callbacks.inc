//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnStatusChanged(npcid, NPC_STATUS:oldStatusId, NPC_STATUS:newStatusId, statusIssuerId)
{
    if(newStatusId == STATUS_CHASE
    || newStatusId == STATUS_ATTACK
    || newStatusId == STATUS_BURNING
    || newStatusId == STATUS_IDLE
    || newStatusId == STATUS_PLAYING_ANIM)
    {
        FCNPC_StartUpdateTimer(npcid);
    }

    if(newStatusId == STATUS_DISABLED
    || newStatusId == STATUS_DEAD
    || newStatusId == STATUS_FALL
    || newStatusId == STATUS_STUNNED)
    {
        FCNPC_StopUpdateTimer(npcid);
    }

    if(newStatusId == STATUS_DISABLED
    || newStatusId == STATUS_DEAD
    || newStatusId == STATUS_FALL
    || newStatusId == STATUS_BURNING
    || newStatusId == STATUS_STUNNED
    || newStatusId == STATUS_PLAYING_ANIM)
    {
        FCNPC_StopMoving(npcid);
    }

    if(newStatusId == STATUS_DEAD
    || newStatusId == STATUS_CHASE
    || newStatusId == STATUS_ATTACK)
    {
        FCNPC_DestroyRoamArea(npcid);
    }

    switch(newStatusId)
    {
        case STATUS_IDLE:
        {
            FCNPC_Stop(npcid);
            FCNPC_CreateRoamArea(npcid);
        }
        
        case STATUS_FALL:
        {
            FCNPC_SetAnimCallback(npcid, BASEBALL_BAT_HIT_3, .freeze = true);
        }

        case STATUS_BURNING:
        {
            SPAO(npcid, OBJECT_MODEL_BURNING);
            eNpcInfo[npcid][e_iFireCreatorId] = statusIssuerId;
            // Further works happening in TLoU.npcs\timers.inc
        }

        case STATUS_STUNNED:
        {
            CLF(#FCNPC_Stun, "d", npcid);
        }

        case STATUS_CHASE, STATUS_ATTACK:
        {
            eNpcInfo[npcid][e_iChaseId] = statusIssuerId;
            // Further works happening in TLoU.npcs\timers.inc
        }
    }
    return 0;
}