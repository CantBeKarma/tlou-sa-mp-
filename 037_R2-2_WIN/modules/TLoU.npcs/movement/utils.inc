// Check if NPC can chase a player
bool:FCNPC_ValidateChasingPlayer(npcid, playerid)
{
	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	if(!IsPlayerInRangeOfPoint(playerid, RANGE_NPC_LOOSE_PLAYER, nX, nY, nZ)
	|| FCNPC_IsUnableToPerformAction(npcid) 
	|| CA_IsPlayerNearWater(playerid)
	|| (GetTickCount() - eNpcInfo[npcid][e_iTickCounter_damageAnim]) < TICK_RATE_ATTACK_AFTER_DAM)
	{
		return false;
	}

	new Float:pX, Float:pY, Float:pZ;
	GetPlayerPos(playerid, pX, pY, pZ);

	static Float:climbing = 2.0;
	if(GetDistanceBetweenPoints1D(nZ, pZ) > climbing)
	{
		pZ = nZ;
		return false;
	}

	if(FCNPC_IsBlockedByCollision(npcid))
	{
		return false;
	}
	return true;
}

// Check if NPC is blocked by a collision (during a chase on a player).
// If collision has been reached, usually NPC is going to try finding a Path (with PathFinder) to the player or ignore him.
FCNPC_IsBlockedByCollision(npcid)
{
	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	new Float:pX, Float:pY, Float:pZ;
	GetPlayerPos(eNpcInfo[npcid][e_iChaseId], pX, pY, pZ);

	static Float:cut_size = 1.0;
	new Float:tX, Float:tY, Float:tZ;
	if(!MovePointColCutLineEx(nX, nY, nZ, pX, pY, pZ, tX, tY, tZ, cut_size, true))
	{
		return true;
	}
	return false;
}

// Calculates path to the player with PathFinder.
// bool:FCNPC_CalculatePathToPlayer(npcid, playerid)
// {
// 	new Float:npcPosX, Float:npcPosY, Float:npcPosZ;
// 	FCNPC_GetPosition(npcid, npcPosX, npcPosY, npcPosZ);

// 	if(!IsPlayerInRangeOfPoint(playerid, RANGE_NPC_START_CHASE, npcPosX, npcPosY, npcPosZ))
// 	{
// 		return false;
// 	}
	
// 	static Float:z;
// 	new Float:chasePosX, Float:chasePosY;
// 	GetPlayerPos(playerid, chasePosX, chasePosY, z);

// 	PathFinder_FindWay(npcid, npcPosX, npcPosY, chasePosX, chasePosY, PATHFINDER_Z_DIFF, PATHFINDER_STEP_SIZE, PATHFINDER_STEP_LIMIT, PATHFINDER_MAX_STEPS);
// 	return true;
// }

// Starts chasing player (updating)
bool:FCNPC_ChasePlayer(npcid, playerid)
{
	// if(FCNPC_IsBlockedByCollision(npcid))
	// {
	// 	FCNPC_CalculatePathToPlayer(npcid, playerid);
	// 	return true;
	// }

	if(!FCNPC_ValidateChasingPlayer(npcid, playerid))
	{
		FCNPC_OnLostPlayer(npcid, playerid);
		return false;
	}

	FCNPC_GoToPlayerOnGroundCol(npcid, playerid, FCNPC_MOVE_RUN);
	FCNPC_OnChasePlayer(npcid, playerid);
	return true;
}

// Check which player's are available to chase near NPC and make sure they are not stealth etc.
GetAvailablePlayerToChase(npcid)
{
	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	foreach(new playerid : Player)
	{
		if(IsPlayerCrouched(playerid)
		|| GetPlayerSpeed(playerid) == 0.0
		|| !IsPlayerInRangeOfPoint(playerid, RANGE_NPC_START_CHASE, nX, nY, nZ)
		|| IsPlayerDead(playerid) 
		|| IsPlayerBurning(playerid)
		|| IsPlayerStunned(playerid) 
		|| IsPlayerFall(playerid) 
    	|| CA_IsPlayerNearWater(playerid))
		{
			continue;
		}

		return playerid;
	}
	return INVALID_PLAYER_ID;
}

// Stops NPC from moving and clear animations used.
FCNPC_StopMoving(npcid)
{
	if(FCNPC_IsMoving(npcid))
	{
		FCNPC_Stop(npcid);
		FCNPC_ClearAnims(npcid);
	}
}

// This function makes NPC go to player as long as it reaches collision on the way.
// For example when NPC goes to player and has a fence during its way - it will stop moving.
// Movement is set through parameter (WALK or RUN) with animations being applied.
//bool:
FCNPC_GoToPlayerOnGroundCol(npcid, playerid, walkOrRun = FCNPC_MOVE_WALK, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250)
{
	new Float:speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)];
	if(walkOrRun == FCNPC_MOVE_WALK)
	{
		speed = (speed / 2.0);
	}
	
	static Float:radius = NPC_RUN_OFFSET;
	static pathfinding = FCNPC_MOVE_PATHFINDING_NONE;
	static Float:min_distance = 0.0;
	static Float:cut_size = 1.0;

	new 
		Float:pX, Float:pY, Float:pZ,
		Float:nX, Float:nY, Float:nZ;

	GetPlayerPos(playerid, pX, pY, pZ);
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	new Float:tX, Float:tY, Float:tZ;
	if(MovePointColCutLineEx(nX, nY, nZ, pX, pY, pZ, tX, tY, tZ, cut_size, true))
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
	}
}

// This function makes NPC is a copy of FCNPC_GoToPlayer...
// with only difference with it having coordinates specified where NPC is supposed to go
// using ColAndreas (going on ground) AND stopping when hitting a collision (e.g. fence)
FCNPC_GoToOnGroundCol(npcid, Float:posX, Float:posY, Float:posZ, walkOrRun = FCNPC_MOVE_WALK, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250)
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

	new Float:nX, Float:nY, Float:nZ;
	FCNPC_GetPosition(npcid, nX, nY, nZ);

	if(GetDistanceBetweenPoints1D(nZ, posZ) > climbing)
	{
		posZ = nZ;
	}

	new Float:tX, Float:tY, Float:tZ;
	if(MovePointColCutLineEx(nX, nY, nZ, posX, posY, posZ, tX, tY, tZ, cut_size))
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

// Each NPC has its own dynamic area in which it's supposed to be moving while roaming (doing nothing).
// This function finds X, Y, Z in NPCs area id, validates it and moves NPC to the target location
// Movement is random between WALKING and RUNNING with animations applies
FCNPC_RandomMoveInDynamicArea(npcid, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250)
{
	new areaid = eNpcInfo[npcid][e_iRoamAreaId];
	if(FCNPC_IsDeadEx(npcid) || areaid == INVALID_STREAMER_ID)
	{
		return false;
	}

	new walkOrRun = random(2);
	new Float:speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)];
	if(walkOrRun == FCNPC_MOVE_WALK)
	{
		speed = (speed / 2.0);
	}

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

	if(walkOrRun == FCNPC_MOVE_WALK)
	{
		FCNPC_ApplyAnimation(npcid, "TLoU", "INF_WALK", 4.1, 1, 1, 1, 0, 0);
	}
	else if(walkOrRun == FCNPC_MOVE_RUN)
	{
		FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	}
	return FCNPC_GoTo(npcid, tX, tY, tZ, type, speed, mode, pathfinding, radius, set_angle, min_distance, stopdelay);
}