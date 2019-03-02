//#region Create/Reset/Clear data

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

	// Reset all enum of the NPC data.
	ResetEnum_eNpcInfo(npcid)
	{
		static sBlank[e_NPC_DATA];
		eNpcInfo[npcid] = sBlank;
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESET] Enum Data has been reset for NPC ID: %d", npcid);
	}

	// Reset all enum of the NPC flags.
	ResetEnum_eNpcFlag(npcid)
	{
		Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESET] Bits Data has been reset for NPC ID: %d", npcid);
	}

//#endregion

//#region FCNPC : Types management

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

//#endregion

//#region FCNPC : Status management

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

		if(FCNPC_IsDeadEx(npcid) && status != NPC_STATUS:STATUS_IDLE)
		{
			return false;
		}

		eNpcInfo[npcid][e_iCurrentStatus] = status;
		SCMF_ALL(-1, "Setting status: %d", _:status);

		switch(status)
		{
			case STATUS_NONE:
			{
				FCNPC_ClearAnims(npcid);
				FCNPC_StopUpdateTimer(npcid);
			}

			case STATUS_IDLE: {}
			case STATUS_ROAM:
			{
				new Float:x, Float:y, Float:z;
				FCNPC_GetPosition(npcid, x, y, z);
				FCNPC_OnRoam(npcid, (x + random(2)), (y + random(2)), z);
			}

			case STATUS_CHASE: {}
			case STATUS_ATTACK: {}
			case STATUS_DISTRACTED: {}
			case STATUS_BURNING: {}
			case STATUS_STUNNED: {}

			case STATUS_DEAD:
			{
				FCNPC_ClearAnims(npcid);
				FCNPC_StopUpdateTimer(npcid);
			}

			case STATUS_PLAYING_ANIM: {}
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

//#endregion

//#region Timer updates

	FCNPC_StartUpdateTimer(npcid)
	{
		if(FCNPC_IsTimerRunning(npcid))
		{
			return;
		}

		SCMF_ALL(-1, "Starting timer");
		Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);
	}

	FCNPC_StopUpdateTimer(npcid)
	{
		if(!FCNPC_IsTimerRunning(npcid))
		{
			return;
		}

		SCMF_ALL(-1, "Stopping timer");
		Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		stop eNpcInfo[npcid][e_iUpdateTimer];
	}

//#endregion

//#region Animations

	// Custom SetAnimation for FCNPC.
	FCNPC_SetAnimCallback(npcid, animationid)
	{
		if(FCNPC_IsPlayingAnim(npcid) || FCNPC_IsDeadEx(npcid))
		{
			return;
		}

		new tickCounter = -11;
		switch(animationid)
		{
			case KISSING_GF_CARARGUE_02: 	   	tickCounter = -40;
			case PED_GETUP: 				   	tickCounter = -20;
			case PED_FLEE_LKAROUND_01: 	   		tickCounter = -30;
			case KNIFE_KILL_KNIFE_PED_DAMAGE: 	tickCounter = -35;
		}
		
		FCNPC_ClearAnims(npcid);
		eNpcInfo[npcid][e_iPlayingAnim_Tick] = tickCounter;
		eNpcInfo[npcid][e_iPlayingAnim_Index] = animationid;
		FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_PLAYING_ANIM);
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

	// Plays idle anims on NPC.
	FCNPC_PlayIdleActions(npcid)
	{
		if(!FCNPC_IsIdle(npcid) || FCNPC_IsRoaming(npcid) || FCNPC_IsPlayingAnim(npcid) || FCNPC_IsDeadEx(npcid)
		|| (GetTickCount() - eNpcInfo[npcid][e_iTickCounter_IdleActions]) < TICK_RATE_FCNPC_IDLE)
		{
			return;
		}
		eNpcInfo[npcid][e_iTickCounter_IdleActions] = GetTickCount();

		switch(random(2))
		{
			case 0: FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_ROAM);
			case 1:	FCNPC_SetAnimCallback(npcid, NpcIdleAnimsList[random(sizeof(NpcIdleAnimsList))]);
		}
	}

	// Clear NPCs anims and packets data.
	FCNPC_ClearAnims(npcid)
	{
		if(FCNPC_IsPlayingAnim(npcid))
		{
			eNpcInfo[npcid][e_iPlayingAnim_Tick] = 0;
			eNpcInfo[npcid][e_iPlayingAnim_Index] = 0;
		}

		FCNPC_ResetAnimation(npcid);
		FCNPC_ClearAnimations(npcid);
	}	

	// Plays death animation for dead NPC only
	// Optionally specify a bodypart parameter so it's gonna use different pre-defined animations.
	FCNPC_PlayDeathAnimation(npcid, weaponid, bodypart = -1)
	{
		if(!FCNPC_IsDeadEx(npcid) 
		|| FCNPC_GetAnimationIndex(npcid) == BASEBALL_BAT_HIT_3) // Got shot in stomach by shotgun
		{
			return;
		}

		if(weaponid == WEAPON_SHIV)
		{
			FCNPC_SetAnimCallback(npcid, KNIFE_KILL_KNIFE_PED_DAMAGE);
			return;
		}

		FCNPC_SetAnimation(npcid, \
			(bodypart == BODY_PART_HEAD) ? (PED_KO_SHOT_FACE) : (PED_KO_SHOT_STOM), \
				4.1, 0, 1, 1, 1, 0);
	}

	// Plays 'dead' animation when hit/shot by player.
	// Applies only if NPC is dead.
	FCNPC_PlayDeadAnimIfDead(npcid)
	{
		if(!FCNPC_IsDeadEx(npcid))
		{
			return;
		}

		switch(FCNPC_GetAnimationIndex(npcid))
		{
			case 
				BASEBALL_BAT_HIT_3, 
				PED_KO_SHOT_FACE, 
				KNIFE_KILL_KNIFE_PED_DIE:
				{
					FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0);
				}

			default: 
			{
				FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT_F", 4.1, 0, 1, 1, 1, 0);
			}
		}
	}

	// Plays damage animations on NPC when shot.
	FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId, Float:distanceBetweenPlayers)
	{
		if(FCNPC_IsDeadEx(npcid)
		|| (GetTickCount() - eNpcInfo[npcid][e_iTickCounter_damageAnim]) < TICK_RATE_DAM_ANIM)
		{
			return;
		}
		eNpcInfo[npcid][e_iTickCounter_damageAnim] = GetTickCount();
		
		FCNPC_ClearAnims(npcid);

		switch(bodypart)
		{
			case BODY_PART_LEFT_ARM:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3)], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_RIGHT_ARM: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 3], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_LEFT_LEG:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 6], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_RIGHT_LEG: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 9], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_TORSO:
			{
				if(issuerWeaponId == WEAPON_SHOTGUN && distanceBetweenPlayers <= SHOTGUN_TORSO_SHOT_DISTANCE)
				{
					FlyBackwardsWhenShot(npcid);
				}
				else
				{
					FCNPC_SetAnimation(npcid, NpcDamageAnims[random(4) + 12], 4.1, 0, 1, 1, 0, 0);
				}
			}
		}
	}

	// Check if animation has finished. 
	// If it hasn't increase the counter until it reaches '0'
	bool:FCNPC_CheckIfAnimationFinished(npcid, animIdx)
	{
		if(FCNPC_GetAnimationIndex(npcid) != animIdx || eNpcInfo[npcid][e_iPlayingAnim_Tick] >= 0)
		{
			return true;
		}
		else
		{
			eNpcInfo[npcid][e_iPlayingAnim_Tick]++;
			return false;
		}
	}

//#endregion

//#region Utils

	FCNPC_IsBurning(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_BURNING);
	FCNPC_IsStunned(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_STUNNED);
	FCNPC_IsPlayingAnim(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_PLAYING_ANIM);
	FCNPC_IsIdle(npcid)				return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_IDLE);
	FCNPC_IsRoaming(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_ROAM);
	FCNPC_IsDistracted(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_DISTRACTED);
	FCNPC_IsDeadEx(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_DEAD);
	FCNPC_IsTimerRunning(npcid)		return Bit_Get(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
	Float:FCNPC_GetHealthEx(npcid)	return eNpcInfo[npcid][e_fHealth];

	// Set FCNPC dest. health value (cannot be lower than 0.0).
	FCNPC_SetHealthEx(npcid, Float:health, killerid = INVALID_PLAYER_ID, weaponid = -1, bodypart = -1)
	{
		if(npcid == INVALID_PLAYER_ID || FCNPC_IsDeadEx(npcid))
		{
			return;
		}

		eNpcInfo[npcid][e_fHealth] = (health <= 0.0 ? 0.0 : health);

		if(FCNPC_GetHealthEx(npcid) <= 0.0)
		{
			FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_DEAD);
			FCNPC_OnDeathEx(npcid, killerid, weaponid, bodypart);
		}
	}

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

	// NPC goes to the specific location with a custom animation of running.
	FCNPC_RunTo(npcid, Float:x, Float:y, Float:z, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, mode = FCNPC_MOVE_MODE_AUTO, pathfinding = FCNPC_MOVE_PATHFINDING_AUTO, Float:radius = 0.0, bool:set_angle = true, Float:min_distance = 0.0, stopdelay = 250)
	{
		if(FCNPC_IsDeadEx(npcid))
		{
			return;
		}
		
		speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)];
		radius = NPC_RUN_OFFSET;

		FCNPC_GoTo(npcid, x, y, z, type, speed, mode, pathfinding, radius, set_angle, min_distance, stopdelay);
		FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	}

	// NPC goes to the specific location with a custiom animation of walking.
	FCNPC_WalkTo(npcid, Float:x, Float:y, Float:z, type = FCNPC_MOVE_TYPE_AUTO, Float:speed = FCNPC_MOVE_SPEED_AUTO, mode = FCNPC_MOVE_MODE_AUTO, pathfinding = FCNPC_MOVE_PATHFINDING_AUTO, Float:radius = 0.0, bool:set_angle = true, Float:min_distance = 0.0, stopdelay = 250)
	{
		if(FCNPC_IsDeadEx(npcid))
		{
			return;
		}

		speed = NpcMovementSpeed[_:FCNPC_GetType(npcid)] / 2;
		radius = NPC_RUN_OFFSET;

		FCNPC_GoTo(npcid, x, y, z, type, speed, mode, pathfinding, radius, set_angle, min_distance, stopdelay);
		FCNPC_ApplyAnimation(npcid, "TLoU", "INF_WALK", 4.1, 1, 1, 1, 0, 0);
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

		SetPlayerSkin(npcid, skinid);
		return skinid;
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