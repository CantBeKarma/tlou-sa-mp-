//#region Reset/Clear data

	// Reset all enum of the NPC data.
	ResetEnum_eNpcInfo(npcid)
	{
		static sBlank[e_NPC_DATA];
		eNpcInfo[npcid] = sBlank;
	}

	// Reset all enum of the NPC flags.
	ResetEnum_eNpcFlag(npcid)
	{
		Bit_Vet(eNpcFlag[e_bPlayingAnim], npcid);
		Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
	}

	// Clear NPCs anims and packets data.
	FCNPC_ClearAnims(npcid)
	{
		FCNPC_ResetAnimation(npcid);
		FCNPC_ClearAnimations(npcid);

		if(FCNPC_IsPlayingAnim(npcid))
		{
			eNpcInfo[npcid][e_iPlayingAnim_Tick] = tickCounter;
			eNpcInfo[npcid][e_iPlayingAnim_Index] = animationid;
			Bit_Vet(eNpcFlag[e_bPlayingAnim], npcid);
		}
	}

//#endregion

// Create specified amount of infected based on 'MAX_FCNPCS'
FCNPC_CreateInfected()
{
	new npcName[9];
	for(new i = MAX_PLAYERS-1, maxNpcs = MAX_FCNPCS; maxNpcs > 0; i--, maxNpcs--)
	{
  		format(npcName, sizeof(npcName), "FCNPC_%d", i);
		FCNPC_Create(npcName);
	}
}

//#region FCNPC : Types and Statuses

	// Get the type of an NPC.
	NPC_TYPE:FCNPC_GetType(npcid)
	{
		return eNpcInfo[npcid][e_iType];
	}

	// Set NPC to the specific type and change its skin automatically.
	FCNPC_SetType(npcid, NPC_TYPE:type)
	{
		if(FCNPC_GetType(npcid) == type)
		{
			return true;
		}

		new skinid = FCNPC_SetSkinBasedOnType(npcid, type);
		if(skinid == 0)
		{
			return false;
		}
		
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[TYPE]: NPC ID: %d has changed its type from %d (%s) to %d (%s)", npcid, _:eNpcInfo[npcid][e_iType], NpcTypeNames[_:eNpcInfo[npcid][e_iType]], _:type, NpcTypeNames[_:type]);
		eNpcInfo[npcid][e_iType] = type;
		return true;
	}

	// Get the current status of an NPC.
	NPC_STATUS:FCNPC_GetStatus(npcid)
	{
		return eNpcInfo[npcid][e_iCurrentStatus];
	}

	// Set NPC a new status.
	FCNPC_SetStatus(npcid, NPC_STATUS:status)
	{
		if(FCNPC_GetStatus(npcid) == status)
		{
			return true;
		}
		eNpcInfo[npcid][e_iCurrentStatus] = status;

		switch(status)
		{
			case STATUS_NONE:
			{
				FCNPC_ClearAnims(npcid);
				FCNPC_StopUpdateTimer(npcid);
			}

			case STATUS_IDLE:
			{
				FCNPC_PlayIdleAnims(npcid);
			}

			case STATUS_ROAM: {}
			case STATUS_CHASE: {}
			case STATUS_ATTACK: {}
			case STATUS_DISTRACTED: {}
			case STATUS_BURNING: {}
			case STATUS_STUNNED: {}
			case STATUS_DEAD: {}
		}

		if(status != STATUS_ROAM && status != STATUS_CHASE)
		{
			FCNPC_StopMoving(npcid);
		}

		if(status != STATUS_NONE && status != STATUS_DEAD)
		{
			FCNPC_StartUpdateTimer(npcid);
		}
		return true;
	}

	// Set NPC new random skin based on its type.
	FCNPC_SetSkinBasedOnType(npcid, NPC_TYPE:type)
	{
		new skinid = 0;
		switch(type)
		{
			case TYPE_RUNNER:	skinid = NpcSkinIds[random(7)];
			case TYPE_STALKER:	skinid = NpcSkinIds[random(6) + 7];
			case TYPE_CLICKER:	skinid = NpcSkinIds[13];
			case TYPE_BLOATER:	skinid = NpcSkinIds[14];
		}

		if(skinid == 0)
		{
			Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "[SKIN]: NPC ID: %d | TYPE: %d | Couldn't find skin model for the specified type!", npcid, _:type);
			return 0;
		}

		FCNPC_SetSkin(npcid, skinid);
		return skinid;
	}
	
	FCNPC_PlayIdleAnims(npcid)
	{
		if(FCNPC_GetStatus(npcid) != NPC_STATUS:STATUS_IDLE)
		{
			return;	
		}

		if(!FCNPC_IsPlayingAnim(npcid))
		{
			FCNPC_SetAnimCallback(npcid, NpcIdleAnimsList[random(sizeof(NpcIdleAnimsList))]);
		}
	}

//#endregion

//#region Timer updates
	FCNPC_StartUpdateTimer(npcid)
	{
		if(FCNPC_IsTimerRunning(npcid))
			return;

		Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);
	}

	FCNPC_StopUpdateTimer(npcid)
	{
		if(!FCNPC_IsTimerRunning(npcid))
			return;

		Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		stop eNpcInfo[npcid][e_iUpdateTimer];
	}
//#endregion

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
	
	eNpcInfo[npcid][e_iPlayingAnim_Tick] = tickCounter;
	eNpcInfo[npcid][e_iPlayingAnim_Index] = animationid;
	Bit_Let(eNpcFlag[e_bPlayingAnim], npcid);
	FCNPC_SetAnimation(npcid, animationid, 4.1, 0, 1, 1, 0, 0);
}

//#region Utils

	FCNPC_IsBurning(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_BURNING);
	FCNPC_IsStunned(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_STUNNED);
	FCNPC_IsPlayingAnim(npcid)	return Bit_Get(eNpcFlag[e_bPlayingAnim], npcid);
	FCNPC_IsTimerRunning(npcid)	return Bit_Get(eNpcFlag[e_bIsUpdateTimerRunning], npcid);

	// Stops NPC from moving and clear animations used.
	FCNPC_StopMoving(npcid)
	{
		if(!FCNPC_IsMoving(npcid))
			return;

		FCNPC_Stop(npcid);
		FCNPC_ClearAnims(npcid);
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

//#endregion




// // -> Prepare data for NPC on spawn.
// FCNPC_ResetAllData(npcid)
// {
// 	ResetEnum_eNpcInfo(npcid);
// 	ResetEnum_eNpcFlag(npcid);

// 	FCNPC_ClearAnims(npcid);
//     FCNPC_SetInterior(npcid, 0);
// 	FCNPC_SetVirtualWorld(npcid, 0);
// 	FCNPC_SetStatus(npcid, NPC_STATUS:IDLE);

// 	SetPlayerTeam(npcid, FACTION_NONE);
// 	RemovePlayerAttachedObj(npcid, eCharsInfo[npcid][e_iAttachSlot_Dynamic]);

// 	// if(FCNPC_IsValidMovePath(eNpcInfo[npcid][FCNPC_MovePathId]))
// 	// {
// 	// 	FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
// 	// }

// 	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESET] Data has been reset for NPC ID: %d", npcid);
// }






	


// // Return current FCNPC health.
// Float:FCNPC_GetHealthEx(npcid)
// {
// 	return eNpcInfo[npcid][e_fHealth];
// }

// // Return whenever NPC is dead.
// FCNPC_IsDeadEx(npcid)
// {
// 	return (FCNPC_GetHealthEx(npcid) <= 0.0);
// }

// // Set FCNPC dest. health value (cannot be lower than 0.0).
// FCNPC_SetHealthEx(npcid, Float:health)
// {
// 	eNpcInfo[npcid][e_fHealth] = (health <= 0.0 ? 0.0 : health);
// }

// // Check if NPC is not: stunned, burning, dead and its timer is running.
// FCNPC_IsAllowedToDoAction(npcid)
// {
// 	return
// 		!FCNPC_IsStunned(npcid) 
// 		&& !FCNPC_IsBurning(npcid) 
// 		&& !FCNPC_IsDeadEx(npcid) 
// 		&& FCNPC_IsTimerRunning(npcid);
// }

// // Put NPC in a fire state.
// FCNPC_SetOnFire(npcid)
// {
// 	if(!FCNPC_IsTimerRunning(npcid) || FCNPC_IsBurning(npcid))
// 		return;

// 	Bit_Let(eNpcFlag[e_bIsBurning], npcid);
// 	FCNPC_OnBurn(npcid);
// }

// // Puts NPC in a stunned state.
// FCNPC_Stun(npcid)
// {
// 	if(!FCNPC_IsTimerRunning(npcid) || FCNPC_IsStunned(npcid) || FCNPC_IsDeadEx(npcid))
// 		return false;

// 	Bit_Let(eNpcFlag[e_bIsStunned], npcid);

// 	FCNPC_OnGetStunnedBySmokeBomb(npcid);
// 	FCNPC_StopUpdateTimer(npcid);

// 	FCNPC_SetAnimation(npcid, PED_GAS_CWR, 4.1, 1, 1, 1, 0, 0);
// 	wait_ms(3000);

// 	FCNPC_SetAnimation(npcid, PED_IDLE_TIRED, 4.1, 1, 1, 1, 0, 0);
// 	wait_ms(2000);
	
// 	if(!FCNPC_IsDeadEx(npcid))
// 	{
// 		FCNPC_ClearAnims(npcid);
// 		FCNPC_StartUpdateTimer(npcid);
// 	}
// 	Bit_Vet(eNpcFlag[e_bIsStunned], npcid);
// 	return true;
// }

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

// // Distract NPC (force it to go to the specific location).
// FCNPC_Distract(npcid, Float:x, Float:y, Float:z, bool:force = false)
// {
// 	if(!FCNPC_IsAllowedToDoAction(npcid) || !IsPlayerInRangeOfPoint(npcid, RANGE_NPC_DISTRACT, x, y, z))
// 	{
// 		return 1;
// 	}

// 	if(force)
// 	{
// 		FCNPC_StopUpdateTimer(npcid);
// //		eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_DISTRACTED;
// 		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
// 		FCNPC_OnGetDistracted(npcid, x, y, z);
// 		return 0;
// 	}

// //    if (eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_IDLE)
// //    || (eNpcInfo[npcid][e_iType] == NPC_TYPE_CLICKER && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING)
// //	|| (eNpcInfo[npcid][e_iType] == NPC_TYPE_CLICKER && eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_CHASING_BY_PATH))
//    // {
// 		FCNPC_StopUpdateTimer(npcid, true);
// //        eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_DISTRACTED;

// 		FCNPC_OnGetDistracted(npcid, x, y, z);
// 	//}
// 	return 0;
// }

// // Start NPC update timer.
// FCNPC_StartUpdateTimer(npcid)
// {
// 	if(FCNPC_IsTimerRunning(npcid))
// 		return;

// 	SCMF_ALL(-1, "NPC timer started");
// //	eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;

// 	Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
// 	eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);

// 	if(FCNPC_IsStunned(npcid))
// 		Bit_Vet(eNpcFlag[e_bIsStunned], npcid);
	
// 	if(FCNPC_IsBurning(npcid))
// 		Bit_Vet(eNpcFlag[e_bIsBurning], npcid);

// 	if(Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
// 		Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);
// }

// // Stop NPC update timer.
// FCNPC_StopUpdateTimer(npcid, bool:force = false)
// {
// 	if(force)
// 	{
// 		stop eNpcInfo[npcid][e_iUpdateTimer];
// 		FCNPC_Stop(npcid);
// 		FCNPC_ClearAnims(npcid);
// 		FCNPC_DestroyMovePath(eNpcInfo[npcid][FCNPC_MovePathId]);
// //		eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;
// 	}

// 	if(!FCNPC_IsTimerRunning(npcid))
// 		return;
		
// 	SCMF_ALL(-1, "NPC timer stopped");
// 	stop eNpcInfo[npcid][e_iUpdateTimer];
// //	eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;

// 	if(Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
// 		Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);

// 	if(FCNPC_IsMoving(npcid))
// 	{
// 		FCNPC_Stop(npcid);
// 		FCNPC_ClearAnims(npcid);
// 	}

// 	Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
// }