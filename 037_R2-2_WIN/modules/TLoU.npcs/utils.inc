function NPC_TYPE:FCNPC_GetType(npcid)
{
	return NPC_TYPE:eNpcInfo[npcid][e_iType];
}

// Create specified amount of infected.
FCNPC_CreateInfected()
{
	new str_fcnpcName[9];
	for(new i = MAX_PLAYERS-1, maxNpcs = MAX_FCNPCS; maxNpcs > 0; i--, maxNpcs--)
	{
  		format(str_fcnpcName, sizeof(str_fcnpcName), "FCNPC_%d", i);
		FCNPC_Create(str_fcnpcName);
	}
}

// Reset all enum of the NPC data.
ResetEnum_eNpcInfo(npcid)
{
    static sBlank[e_NPC_DATA];
    eNpcInfo[npcid] = sBlank;
}

// Reset all enum of the NPC flags.
ResetEnum_eNpcFlag(npcid)
{
	Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);
	Bit_Vet(eNpcFlag[e_bIsBurning], npcid);
	Bit_Vet(eNpcFlag[e_bIsStunned], npcid);
	Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
}

// Clear NPCs anims and packets data.
FCNPC_ClearAnims(npcid)
{
	FCNPC_ResetAnimation(npcid);
	FCNPC_ClearAnimations(npcid);
}

// -> Prepare data for NPC on spawn.
FCNPC_ResetAllData(npcid)
{
	ResetEnum_eNpcInfo(npcid);
	ResetEnum_eNpcFlag(npcid);

	FCNPC_ClearAnims(npcid);
    FCNPC_SetInterior(npcid, 0);
	FCNPC_SetVirtualWorld(npcid, 0);
	FCNPC_SetStatus(npcid, NPC_STATUS:IDLE);

	SetPlayerTeam(npcid, FACTION_NONE);
	RemovePlayerAttachedObj(npcid, eCharsInfo[npcid][e_iAttachSlot_Dynamic]);

	if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]))
	{
		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
	}

	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESET] Data has been reset for NPC ID: %d", npcid);
}

FCNPC_GetStatus(npcid)
{
	return eNpcInfo[npcid][e_iCurrentStatus];
}

FCNPC_SetStatus(npcid, NPC_STATUS:status)
{
	if(!FCNPC_IsTimerRunning(npcid)|| eNpcInfo[npcid][e_iCurrentStatus] == status)
	{
		return;
	}

	eNpcInfo[npcid][e_iCurrentStatus] = status;

	switch(NPC_STATUS:status)
	{
		case NONE:	ResetEnum_eNpcFlag(npcid);
	}
}

FCNPC_SetType(npcid, NPC_TYPE:type)
{
	if(eNpcInfo[npcid][e_iType] == type)
		return;

	new skinid = 0;
	switch(NPC_TYPE:type)
	{
		case RUNNER:	skinid = NpcSkins[random(7)];
		case STALKER:	skinid = NpcSkins[random(6) + 7];
		case CLICKER:	skinid = NpcSkins[13];
		case BLOATER:	skinid = NpcSkins[14];
	}

	if(skinid == 0)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "[TYPE]: NPC ID: %d | TYPE: %d | Couldn't find skin model for the specified type!", npcid, _:type);
		return;
	}
	
	eNpcInfo[npcid][e_iType] = type;
	FCNPC_SetSkin(npcid, skinid);
}

// Return current FCNPC health.
Float:FCNPC_GetHealthEx(npcid)
{
	return eNpcInfo[npcid][e_fHealth];
}

// Return whenever NPC is dead.
FCNPC_IsDeadEx(npcid)
{
	return (FCNPC_GetHealthEx(npcid) <= 0.0);
}

// Set FCNPC dest. health value (cannot be lower than 0.0).
FCNPC_SetHealthEx(npcid, Float:health)
{
	eNpcInfo[npcid][e_fHealth] = (health <= 0.0 ? 0.0 : health);
}

// Custom SetAnimation for FCNPC.
FCNPC_SetAnimCallback(npcid, animationid)
{
	new tickCounter = -11;
	switch(animationid)
	{
		case KISSING_GF_CARARGUE_02: 	   	tickCounter = -45;
		case PED_GETUP: 				   	tickCounter = -16;
		case PED_FLEE_LKAROUND_01: 	   		tickCounter = -34;
		case KNIFE_KILL_KNIFE_PED_DAMAGE: 	tickCounter = -35;
	}
	
	eNpcInfo[npcid][e_iAnimTick] = tickCounter;
	eNpcInfo[npcid][e_iAnimIdx] = animationid;
	Bit_Let(eNpcFlag[e_bAnimPlaying], npcid);
	FCNPC_SetAnimation(npcid, animationid, 4.1, 0, 1, 1, 0, 0);
}

// Return current FCNPCs animation index.
FCNPC_GetAnimationIndex(npcid)
{
	new animationid;
	if(FCNPC_GetAnimation(npcid, animationid))
	{
		return animationid;
	}
	return -1;
}

// Check if NPC is burning.
FCNPC_IsBurning(npcid)
{
	return Bit_Get(eNpcFlag[e_bIsBurning], npcid);
}

// Check if NPC is stunned
FCNPC_IsStunned(npcid)
{
	return Bit_Get(eNpcFlag[e_bIsStunned], npcid);
}

// Check if NPC's timer is running.
FCNPC_IsTimerRunning(npcid)
{
	return Bit_Get(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
}

// Check if NPC is not: stunned, burning, dead and its timer is running.
FCNPC_IsAllowedToDoAction(npcid)
{
	return
		!FCNPC_IsStunned(npcid) 
		&& !FCNPC_IsBurning(npcid) 
		&& !FCNPC_IsDeadEx(npcid) 
		&& FCNPC_IsTimerRunning(npcid);
}

// Put NPC in a fire state.
FCNPC_SetOnFire(npcid)
{
	if(!FCNPC_IsTimerRunning(npcid) || FCNPC_IsBurning(npcid))
		return;

	Bit_Let(eNpcFlag[e_bIsBurning], npcid);
	FCNPC_OnBurn(npcid);
}

// Puts NPC in a stunned state.
FCNPC_Stun(npcid)
{
	if(!FCNPC_IsTimerRunning(npcid) || FCNPC_IsStunned(npcid) || FCNPC_IsDeadEx(npcid))
		return false;

	Bit_Let(eNpcFlag[e_bIsStunned], npcid);

	FCNPC_OnGetStunnedBySmokeBomb(npcid);
	FCNPC_StopUpdateTimer(npcid);

	FCNPC_SetAnimation(npcid, PED_GAS_CWR, 4.1, 1, 1, 1, 0, 0);
	wait_ms(3000);

	FCNPC_SetAnimation(npcid, PED_IDLE_TIRED, 4.1, 1, 1, 1, 0, 0);
	wait_ms(2000);
	
	if(!FCNPC_IsDeadEx(npcid))
	{
		FCNPC_ClearAnims(npcid);
		FCNPC_StartUpdateTimer(npcid);
	}
	Bit_Vet(eNpcFlag[e_bIsStunned], npcid);
	return true;
}

// Put NPC in the chase state after given playerid (chaseid).
FCNPC_ChasePlayer(npcid, chaseid)
{
	if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]))
	{
		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
	}

	FCNPC_SetStatus(npcid, NPC_STATUS:CHASE);

	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID || eNpcInfo[npcid][e_iChasedId] != chaseid)
	{
		eNpcInfo[npcid][e_iChasedId] = chaseid;
	}

	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);

	new npcType = FCNPC_GetType(npcid);
	FCNPC_GoToPlayer(npcid, chaseid, MOVE_TYPE_AUTO, NpcMovementSpeed[npcType], false, NPC_RUN_OFFSET, true);
	FCNPC_OnChasePlayer(npcid, chaseid);
}

// Put NPC in the chase state by path to the specific point (not using native GoTo).
FCNPC_ChasePlayerByPath(npcid, chaseid)
{
//	if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]) && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING_BY_PATH)
//		return 1;

	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID || eNpcInfo[npcid][e_iChasedId] != chaseid)
		eNpcInfo[npcid][e_iChasedId] = chaseid;
	
	static Float:z;

	new Float:npcPosX, Float:npcPosY;
	FCNPC_GetPosition(npcid, npcPosX, npcPosY, z);
	
	new Float:chasePosX, Float:chasePosY;
	GetPlayerPos(chaseid, chasePosX, chasePosY, z);

	PathFinder_FindWay(npcid, npcPosX, npcPosY, chasePosX, chasePosY, PATHFINDER_Z_DIFF, PATHFINDER_STEP_SIZE, PATHFINDER_STEP_LIMIT, PATHFINDER_MAX_STEPS);
	return 0;
}

// Distract NPC (force it to go to the specific location).
FCNPC_Distract(npcid, Float:x, Float:y, Float:z, bool:force = false)
{
	if(!FCNPC_IsAllowedToDoAction(npcid) || !IsPlayerInRangeOfPoint(npcid, RANGE_NPC_DISTRACT, x, y, z))
	{
		return 1;
	}

	if(force)
	{
		FCNPC_StopUpdateTimer(npcid);
//		eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_DISTRACTED;
		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
		FCNPC_OnGetDistracted(npcid, x, y, z);
		return 0;
	}

//    if (eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_IDLE)
//    || (eNpcInfo[npcid][e_iType] == NPC_TYPE_CLICKER && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING)
//	|| (eNpcInfo[npcid][e_iType] == NPC_TYPE_CLICKER && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING_BY_PATH))
   // {
		FCNPC_StopUpdateTimer(npcid, true);
//        eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_DISTRACTED;

		FCNPC_OnGetDistracted(npcid, x, y, z);
	//}
	return 0;
}

// Start NPC update timer.
FCNPC_StartUpdateTimer(npcid)
{
	if(FCNPC_IsTimerRunning(npcid))
		return;

	SCMF_ALL(-1, "NPC timer started");
//	eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;

	Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
	eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);

	if(FCNPC_IsStunned(npcid))
		Bit_Vet(eNpcFlag[e_bIsStunned], npcid);
	
	if(FCNPC_IsBurning(npcid))
		Bit_Vet(eNpcFlag[e_bIsBurning], npcid);

	if(Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
		Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);
}

// Stop NPC update timer.
FCNPC_StopUpdateTimer(npcid, bool:force = false)
{
	if(force)
	{
		stop eNpcInfo[npcid][e_iUpdateTimer];
		FCNPC_Stop(npcid);
		FCNPC_ClearAnims(npcid);
		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
//		eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;
	}

	if(!FCNPC_IsTimerRunning(npcid))
		return;
		
	SCMF_ALL(-1, "NPC timer stopped");
	stop eNpcInfo[npcid][e_iUpdateTimer];
//	eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;

	if(Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
		Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);

	if(FCNPC_IsMoving(npcid))
	{
		FCNPC_Stop(npcid);
		FCNPC_ClearAnims(npcid);
	}

	Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
}