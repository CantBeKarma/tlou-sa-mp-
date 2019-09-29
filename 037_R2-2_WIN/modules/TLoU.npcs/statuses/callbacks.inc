//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnStatusChanged(npcid, NPC_STATUS:oldStatusId, NPC_STATUS:newStatusId, statusIssuerId)
{
    switch(newStatusId)
    {
        case 
            STATUS_DISABLED,
            STATUS_DEAD:
        {
            FCNPC_StopUpdateTimer(npcid);
        }

        case STATUS_ROAM:
        {
            FCNPC_RandomMoveInDynamicArea(npcid, eNpcInfo[npcid][e_iRoamAreaId]);
        }

        // case STATUS_DISTRACTED:
        // {
        //     FCNPC_OnGetDistracted(npcid, roamX, roamY, z);
        // }

        // case STATUS_BURNING:
        // {
        //     FCNPC_OnStartBurning(npcid);
        // }

        // case STATUS_STUNNED:
        // {
        //     CLF(#FCNPC_OnGetStunned, "d", npcid);
        // }
    }

    if(newStatusId != STATUS_ROAM 
    && newStatusId != STATUS_CHASE)
    {
        FCNPC_StopMoving(npcid);
    }

    if(newStatusId != STATUS_DISABLED 
    && newStatusId != STATUS_DEAD)
    {
        FCNPC_StartUpdateTimer(npcid);
    }
    return 0;
}