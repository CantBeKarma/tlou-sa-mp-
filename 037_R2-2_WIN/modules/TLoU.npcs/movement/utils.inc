//#region OLD movement code

	// // Put NPC in the chase state after given playerid (chaseid).
	// FCNPC_ChasePlayer(npcid, chaseid)
	// {
	// 	if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]))
	// 	{
	// 		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
	// 	}

	// 	FCNPC_SetStatus(npcid, NPC_STATUS:CHASE);

	// 	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID || eNpcInfo[npcid][e_iChasedId] != chaseid)
	// 	{
	// 		eNpcInfo[npcid][e_iChasedId] = chaseid;
	// 	}

	// 	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);

	// 	new npcType = FCNPC_GetType(npcid);
	// 	FCNPC_GoToPlayer(npcid, chaseid, MOVE_TYPE_AUTO, NpcMovementSpeed[npcType], false, NPC_RUN_OFFSET, true);
	// 	FCNPC_OnChasePlayer(npcid, chaseid);
	// }

	// // Put NPC in the chase state by path to the specific point (not using native GoTo).
	// FCNPC_ChasePlayerByPath(npcid, chaseid)
	// {
	// //	if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]) && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING_BY_PATH)
	// //		return 1;

	// 	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID || eNpcInfo[npcid][e_iChasedId] != chaseid)
	// 		eNpcInfo[npcid][e_iChasedId] = chaseid;
		
	// 	static Float:z;

	// 	new Float:npcPosX, Float:npcPosY;
	// 	FCNPC_GetPosition(npcid, npcPosX, npcPosY, z);
		
	// 	new Float:chasePosX, Float:chasePosY;
	// 	GetPlayerPos(chaseid, chasePosX, chasePosY, z);

	// 	PathFinder_FindWay(npcid, npcPosX, npcPosY, chasePosX, chasePosY, PATHFINDER_Z_DIFF, PATHFINDER_STEP_SIZE, PATHFINDER_STEP_LIMIT, PATHFINDER_MAX_STEPS);
	// 	return 0;
	// }

//#endregion

// Stops NPC from moving and clear animations used.
FCNPC_StopMoving(npcid)
{
	if(!FCNPC_IsMoving(npcid))
	{
		return;
	}

	FCNPC_Stop(npcid);
	FCNPC_ClearAnims(npcid);
}

FCNPC_GoToPlayerOnGroundCol(npcid, playerid, walkOrRun = FCNPC_MOVE_WALK, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250)
{
	if(FCNPC_IsDeadEx(npcid))
	{
		return false;
	}

	new Float:speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)];
	if(walkOrRun == FCNPC_MOVE_WALK)
	{
		speed = (speed / 2.0);
	}
	
	static Float:radius = NPC_RUN_OFFSET;
	static pathfinding = FCNPC_MOVE_PATHFINDING_NONE;
	static Float:min_distance = 0.0;
	static Float:climbing = 2.0;
	static Float:cut_size = 1.0;

	new 
		Float:pX, Float:pY, Float:pZ,
		Float:nX, Float:nY, Float:nZ;

	GetPlayerPos(playerid, pX, pY, pZ);
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	if(GetDistanceBetweenPoints1D(nZ, pZ) > climbing)
	{
		pZ = nZ;
	}

	new Float:tX, Float:tY, Float:tZ;
	if(MovePointColCutLineEx(nX, nY, nZ, pX, pY, pZ, tX, tY, tZ, cut_size))
    {
		FCNPC_GoTo(npcid, tX, tY, tZ, type, speed, mode, pathfinding, radius, set_angle, min_distance, stopdelay);

		if(walkOrRun == FCNPC_MOVE_WALK)
		{
			FCNPC_ApplyAnimation(npcid, "TLoU", "INF_WALK", 4.1, 1, 1, 1, 0, 0);
		}
		else if(walkOrRun == FCNPC_MOVE_RUN)
		{
			FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
		}
		return true;
	}
	return false;
}

FCNPC_RandomMoveInDynamicArea(npcid, areaid, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250)
{
	if(FCNPC_IsDeadEx(npcid) || !IsValidDynamicArea(areaid))
	{
		return false;
	}

	new Float:speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)];
	static Float:radius = NPC_RUN_OFFSET;
	static pathfinding = FCNPC_MOVE_PATHFINDING_NONE;
	static Float:min_distance = 0.0;
	static Float:climbing = 2.0;

	new 
		Float:tX, Float:tY, Float:tZ,
		Float:tmp, 
		count = 0;

	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	do 
	{
		if(count >= 100)
		{
			return 0;
		}

		Random_PointInDynamicArea(areaid, tX, tY, tZ);
		CA_FindZ_For2DCoord(tX, tY, tZ);
		tZ += 1.0;
		count++;
	} 
	while 
	(
		IsPointInWater(tX, tY) 
		|| CA_RayCastLine(nX, nY, nZ, tX, tY, tZ, tmp, tmp, tmp) 
		|| GetDistanceBetweenPoints1D(tZ, nZ) > climbing 
		|| !IsPointInDynamicArea(areaid, tX, tY, tZ)
	);

	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_WALK", 4.1, 1, 1, 1, 0, 0);
	return FCNPC_GoTo(npcid, tX, tY, tZ, type, speed, mode, pathfinding, radius, set_angle, min_distance, stopdelay);
}