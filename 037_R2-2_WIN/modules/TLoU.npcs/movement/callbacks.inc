//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnReachDestination ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnReachDestination(npcid)
{
    FCNPC_ClearAnims(npcid);
	if(FCNPC_IsIdle(npcid))
	{
		SCMF_ALL(-1, "NPC reached destination (while roaming).");
		FCNPC_PlayIdleActions(npcid);
	}
	else
	{
		if(eNpcInfo[npcid][FCNPC_MovePathId] == FCNPC_INVALID_MOVEPATH_ID)
		{
			SCMF_ALL(-1, "NPC reached destition (while NOT roaming)");
			FCNPC_CreateRoamArea(npcid);
			FCNPC_SetStatus(npcid, STATUS_IDLE);
		}
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnChasePlayer ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnChasePlayer(npcid, chaseId)
{
	GameTextForPlayerf(chaseId, 1000, 5, "~n~~n~~n~~n~~n~~g~NPC %d is chasing you!", npcid);
	if(GetDistanceBetweenPlayers(npcid, chaseId) <= RANGE_NPC_ATTACK)
	{
		FCNPC_SetStatus(npcid, STATUS_ATTACK, chaseId);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnLostPlayer ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnLostPlayer(npcid, chaseId)
{
	GameTextForPlayerf(chaseId, 3000, 5, "~n~~n~~n~~n~~n~~r~NPC %d lost you!", npcid);
	
	if(GetDistanceBetweenPlayers(npcid, chaseId) <= RANGE_NPC_ATTACK)
	{
		FCNPC_SetStatus(npcid, STATUS_ATTACK, chaseId);
	}
	else
	{
		FCNPC_SetStatus(npcid, STATUS_IDLE);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ FCNPC_OnChasePlayerByPath ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
public FCNPC_OnChasePlayerByPath(npcid, chaseId, pathid)
{
	GameTextForPlayer(chaseId, "~n~~n~~n~~n~~n~PATH CHASE", 1000, 5);

	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);
	
	// If FCNPC got close enough to player - use native chase and destroy the path.
	if(IsPlayerInRangeOfPoint(chaseId, RANGE_NPC_LOOSE_PLAYER, nX, nY, nZ) && !FCNPC_IsBlockedByCollision(npcid))
	{
		FCNPC_StopMoving(npcid);
		FCNPC_DestroyMovePathFinder(npcid);
		FCNPC_SetStatus(npcid, STATUS_CHASE, chaseId);
		SCMF_ALL(COLOR_GREEN, "FCNPC_OnChasePlayerByPath > IsPlayerInRangeOfPoint");
		return 0;
	}

	// If NPC is chasing player by path but player
	// has moved more than 15.0 meters from last node in path
	// destroy old path for NPC and create a new one to move by.
	new Float:x, Float:y, Float:z, 
		lastNode = FCNPC_GetNumberMovePathPoint(pathid) - 1;

	FCNPC_GetMovePathPoint(pathid, lastNode, x, y, z);

	if(GetPlayerDistanceFromPoint(chaseId, x, y, z) > 10.0)
	{
		SCMF(chaseId, COLOR_GREEN, "You tried to run away. Creating new path...");
		FCNPC_DestroyMovePath(pathid);
		FCNPC_CalculatePathToPlayer(npcid, chaseId);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- FCNPC_OnFinishMovePathPoint -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnFinishMovePathPoint(npcid, pathid, pointid)
{
	SCMF_ALL(COLOR_GREEN, "FCNPC_OnFinishMovePathPoint");
	new chaseId = eNpcInfo[npcid][e_iChaseId];
	FCNPC_OnChasePlayerByPath(npcid, chaseId, pathid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnFinishMovePath --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnFinishMovePath(npcid, pathid)
{
	SCMF_ALL(COLOR_GOLD, "FCNPC_OnFinishMovePath");
	FCNPC_DestroyMovePathFinder(npcid);
	FCNPC_SetStatus(npcid, STATUS_IDLE);
	return 0;
}

FCNPC_DestroyMovePathFinder(npcid)
{
	FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
	eNpcInfo[npcid][FCNPC_MovePathId] = FCNPC_INVALID_MOVEPATH_ID;
}