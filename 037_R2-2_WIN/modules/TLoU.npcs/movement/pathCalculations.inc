//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPathCalculated ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPathCalculated(routeid, success, Float:nodesX[], Float:nodesY[], Float:nodesZ[], nodesSize)
{
	if(!success)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], WARNING, "OnPathCalculated: Couldn't calculate path for NPCID: %d (tried to create %d nodes)", routeid, nodesSize);
		return 1;
	}

	eNpcInfo[routeid][FCNPC_MovePathId] = FCNPC_CreateMovePath();
	if(eNpcInfo[routeid][FCNPC_MovePathId] == FCNPC_INVALID_MOVEPATH_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "OnPathCalculated: Couldn't create Move Path for NPCID: %d", routeid);
		return 1;
	}

	FCNPC_SetStatus(routeid, STATUS_DISABLED);
	FCNPC_CalcAndAddPathNodes(routeid, nodesX, nodesY, nodesZ, nodesSize);
	FCNPC_GoByMovePath(routeid, eNpcInfo[routeid][FCNPC_MovePathId], .speed = NpcMovementSpeed[ _:FCNPC_GetType(routeid) ]);
	FCNPC_ApplyAnimation(routeid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	return 1;
}

// Calculates path nodes retrieved in OnPathCalculated and adds its points to NPCs Move path ID.
FCNPC_CalcAndAddPathNodes(npcid, Float:nodesX[], Float:nodesY[], Float:nodesZ[], nodesSize)
{
	new 
		i,
		Last_i = -1;

	new 
		Float:Last_A = -1,
		Float:Last_Z = -1;

	while(i < nodesSize)
	{
		if(i == 0)
		{
			Last_i = i;
			Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
			Last_Z = (nodesZ[i+1]-nodesZ[i]);
			i++;
		}
		else if(i < nodesSize-1)
		{
			if(Last_A == floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i])) && CompareZ(Last_Z, (nodesZ[i+1] - nodesZ[i]), 0.3))
			{
				i++;
			}
			else
			{
				Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
				Last_Z = (nodesZ[i+1]-nodesZ[i]);

				FCNPC_AddPointToMovePath(eNpcInfo[npcid][FCNPC_MovePathId], nodesX[i+1], nodesY[i+1], nodesZ[i+1]+1.0);
				Last_i = i+1;
				i++;
			}
		}
		else
		{
			if(Last_i != nodesSize-1)
			{
				FCNPC_AddPointToMovePath(eNpcInfo[npcid][FCNPC_MovePathId], nodesX[nodesSize-1], nodesY[nodesSize-1], nodesZ[nodesSize-1]+1.0);
			}
			i++;
		}
	}
	
	FCNPC_AddPointToMovePath(eNpcInfo[npcid][FCNPC_MovePathId], nodesX[nodesSize-1] + 5.0, nodesY[nodesSize-1] + 5.0, nodesZ[nodesSize-1]+1.0);
}

// Function is used for PathNodes calculations. Checking if given Z should be added to the Move path ID
// For example if difference between Z1 & Z2 is higher than specified - that node won't be added to the Move path.
CompareZ(Float:fZ, Float:fZ2, Float:difference)
{
	new Float:zdiff = floatabs(fZ - fZ2);
	if(zdiff == 0.0) return 1;
	else if(fZ < 0 && fZ2 < 0)
	{
		if(zdiff <= difference) return 1;
		return 0;
	}
	else if(fZ > 0 && fZ2 > 0)
	{
		if(zdiff <= difference) return 1;
		return 0;
	}
	return 0;
}

// Calculates path to the player with PathFinder.
bool:FCNPC_CalculatePathToPlayer(npcid, playerid)
{
	SCMF_ALL(COLOR_RED, "FCNPC_CalculatePathToPlayer");
	if(eNpcInfo[npcid][FCNPC_MovePathId] != FCNPC_INVALID_MOVEPATH_ID)
	{
		SCMF_ALL(COLOR_RED, "Already calculated!");
		return false;
	}

	new Float:npcPosX, Float:npcPosY, Float:npcPosZ;
	FCNPC_GetPosition(npcid, npcPosX, npcPosY, npcPosZ);

	if(!IsPlayerInRangeOfPoint(playerid, RANGE_NPC_START_CHASE + 20.0, npcPosX, npcPosY, npcPosZ))
	{
		SCMF_ALL(COLOR_RED, "Player not in range!");
		return false;
	}
	
	static Float:z;
	new Float:chasePosX, Float:chasePosY;
	GetPlayerPos(playerid, chasePosX, chasePosY, z);

	PathFinder_FindWay(npcid, npcPosX, npcPosY, chasePosX, chasePosY, PATHFINDER_Z_DIFF, PATHFINDER_STEP_SIZE, PATHFINDER_STEP_LIMIT, PATHFINDER_MAX_STEPS);
	return true;
}