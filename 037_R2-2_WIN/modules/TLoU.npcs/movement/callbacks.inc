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
		SCMF_ALL(-1, "NPC reached destition (while NOT roaming)");
		FCNPC_CreateRoamArea(npcid);
		FCNPC_SetStatus(npcid, STATUS_IDLE);
	}
	return 1;
}

// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// //--------------------- FCNPC_OnChasePlayer ---------------------
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// public FCNPC_OnChasePlayer(npcid, chaseid)
// {
// 	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~NATIVE CHASE", 1000, 5);

// 	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
// 	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

// 	new Float:npcPosZ;
// 	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

// 	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
// 	{
// 		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
// 		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
// 		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

// 		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
// 	}
// 	return 1;
// }

// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// //------------------ FCNPC_OnChasePlayerByPath ------------------
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
// public FCNPC_OnChasePlayerByPath(npcid, chaseid, pathid)
// {
// 	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~PATH CHASE", 1000, 5);

// 	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
// 	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

// 	new Float:npcPosZ;
// 	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

// 	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
// 	{
// 		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
// 		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
// 		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

// 		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
// 	}
// 	return 0;
// }

// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// //----------------------- OnPathCalculated ----------------------
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// public OnPathCalculated(routeid, success, Float:nodesX[], Float:nodesY[], Float:nodesZ[], nodesSize)
// {
// 	if(!success)
// 	{
// 		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't calculate path for NPCID: %d (tried to create %d nodes)", routeid, nodesSize);
// 		return 1;
// 	}

// 	eNpcInfo[routeid][FCNPC_MovePathId] = FCNPC_CreateMovePath();
// 	if(eNpcInfo[routeid][FCNPC_MovePathId] == FCNPC_INVALID_MOVEPATH_ID)
// 	{
// 		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't create Move Path for NPCID: %d", routeid);
// 		return 1;
// 	}

// 	FCNPC_StopUpdateTimer(routeid);
// //	eNpcInfo[routeid][e_iCurrentStatus] = NPC_STATUS_CHASING_BY_PATH;

// 	new 
// 		i,
// 		Last_i = -1;

// 	new 
// 		Float:Last_A = -1,
// 		Float:Last_Z = -1;

// 	while(i < nodesSize)
// 	{
// 		if(i == 0)
// 		{
// 			Last_i = i;
// 			Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
// 			Last_Z = (nodesZ[i+1]-nodesZ[i]);
// 			i++;
// 		}
// 		else if(i < nodesSize-1)
// 		{
// 			if(Last_A == floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i])) && CompareZ(Last_Z, (nodesZ[i+1]-nodesZ[i]), 0.3))
// 			{
// 				i++;
// 			}
// 			else
// 			{
// 				Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
// 				Last_Z = (nodesZ[i+1]-nodesZ[i]);

// 				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[i+1], nodesY[i+1], nodesZ[i+1]+1.0);
// 				Last_i = i+1;
// 				i++;
// 			}
// 		}
// 		else
// 		{
// 			if(Last_i != nodesSize-1)
// 			{
// 				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[nodesSize-1], nodesY[nodesSize-1], nodesZ[nodesSize-1]+1.0);
// 			}
// 			i++;
// 		}
// 	}

// 	FCNPC_GoByMovePath(routeid, eNpcInfo[routeid][FCNPC_MovePathId], .speed = NpcMovementSpeed[ eNpcInfo[routeid][e_iType] ]);
// 	FCNPC_ApplyAnimation(routeid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
// 	return 1;
// }

// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// //----------------- FCNPC_OnFinishMovePathPoint -----------------
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// public FCNPC_OnFinishMovePathPoint(npcid, pathid, pointid)
// {
// 	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID)
// 	{
// 		FCNPC_DestroyMovePath(pathid);
// 		FCNPC_StartUpdateTimer(npcid);
// 		return 1;
// 	}

// 	new chaseId = eNpcInfo[npcid][e_iChasedId];
// 	FCNPC_OnChasePlayerByPath(npcid, chaseId, pathid);

// 	// If FCNPC got close enough to player - use native chase and destroy the path.
// 	new Float:distanceToPlayer = GetDistanceBetweenPlayers(npcid, chaseId);
// 	if(distanceToPlayer <= RANGE_NPC_CHASE)
// 	{
// 		FCNPC_ChasePlayer(npcid, chaseId);
// 		return 0;
// 	}

// 	// If NPC is chasing player by path but player
// 	// has moved more than 15.0 meters from last node in path
// 	// destroy old path for NPC and create a new one to move by.
// 	new Float:x, Float:y, Float:z, 
// 		lastNode = FCNPC_GetNumberMovePoint(pathid) - 1;

// 	FCNPC_GetMovePoint(pathid, lastNode, x, y, z);

// 	if(GetPlayerDistanceFromPoint(chaseId, x, y, z) > 10.0)
// 	{
// 		SCMF(chaseId, -1, "You tried to run away. Creating new path...");
// 		FCNPC_DestroyMovePath(pathid);
// 		FCNPC_ChasePlayerByPath(npcid, chaseId);
// 	}
// 	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
// 	return 0;
// }

// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// //------------------- FCNPC_OnFinishMovePath --------------------
// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// public FCNPC_OnFinishMovePath(npcid, pathid)
// {
// 	FCNPC_DestroyMovePath(pathid);
// 	FCNPC_StartUpdateTimer(npcid);	
// 	return 0;
// }