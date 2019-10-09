// Get the current status of an NPC.
NPC_STATUS:FCNPC_GetStatus(npcid)
{
    return eNpcInfo[npcid][e_iCurrentStatus];
}

// Sets player status (e.g. dead). 
// Status change can be caused by other player, for example when player shot NPC
FCNPC_SetStatus(npcid, NPC_STATUS:status, statusIssuerId = INVALID_PLAYER_ID)
{
    new NPC_STATUS:oldStatusId = FCNPC_GetStatus(npcid);

    if(FCNPC_ValidateStatusChange(npcid, status, statusIssuerId) == false)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_NPC], WARNING, "FCNPC_SetStatus: Status couldn't be changed for NPCID: %d : OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", npcid, _:oldStatusId, _:status);
        return false;
    }
    
    eNpcInfo[npcid][e_iCurrentStatus] = status;
    FCNPC_OnStatusChanged(npcid, oldStatusId, status, statusIssuerId);
    return true;
}

bool:FCNPC_ValidateStatusChange(npcid, NPC_STATUS:newStatusId, statusIssuerId = INVALID_PLAYER_ID)
{
    if(newStatusId == STATUS_DISABLED
    || newStatusId == STATUS_DEAD)
    {
        return true;
    }

    if(FCNPC_GetStatus(npcid) == newStatusId 
    || FCNPC_IsDeadEx(npcid) && newStatusId != STATUS_IDLE)
    {
        return false;
    }

    if(newStatusId == STATUS_CHASE
    || newStatusId == STATUS_ATTACK
    || newStatusId == STATUS_PLAYING_ANIM)
    {
        if(FCNPC_IsBurning(npcid) 
        || FCNPC_IsStunned(npcid))
        {
            return false;
        }
    }

    new bool:success = true;
    
    switch(newStatusId)
    {
        case STATUS_STUNNED:
        {
            if(FCNPC_IsBurning(npcid))
            {
                success = false;
            }
        }
        
        case STATUS_FALL:
        {
            if(FCNPC_IsBurning(npcid) 
            || FCNPC_GetType(npcid) == TYPE_BLOATER)
            {
                success = false;
            }
        }
        
        case STATUS_ATTACK:
        {
            if(statusIssuerId == INVALID_PLAYER_ID || IsPlayerDead(statusIssuerId))
            {
                success = false;
            }
        }
    }
    return success;
}