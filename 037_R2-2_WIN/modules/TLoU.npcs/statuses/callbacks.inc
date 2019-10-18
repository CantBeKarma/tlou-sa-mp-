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

    switch(newStatusId)
    {
        case 
            STATUS_DISABLED,
            STATUS_DEAD:
        {
            FCNPC_DestroyRoamArea(npcid);
        }

        case STATUS_IDLE:
        {
            FCNPC_PlayIdleActions(npcid);
        }

        case STATUS_FALL:
        {
            FCNPC_SetAnimCallback(npcid, BASEBALL_BAT_HIT_3, .freeze = true);
        }

        case STATUS_BURNING:
        {
            SPAO(npcid, OBJECT_MODEL_BURNING);
            eNpcInfo[npcid][e_iFireCreatorId] = statusIssuerId;
        }

        case STATUS_STUNNED:
        {
            CLF(#FCNPC_Stun, "d", npcid);
        }

        case STATUS_CHASE:
        {
            FCNPC_GoToPlayerOnGroundCol(npcid, statusIssuerId, FCNPC_MOVE_RUN);
        }
    }
    return 0;
}