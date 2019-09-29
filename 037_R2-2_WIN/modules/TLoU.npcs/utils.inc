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
	NPC_TYPE:FCNPC_GetType(npcid) return eNpcInfo[npcid][e_iType];

	// Set NPC to the specific type and change its skin automatically.
	FCNPC_SetType(npcid, NPC_TYPE:type)
	{
		if(FCNPC_GetType(npcid) == type)
		{
			return true;
		}

		new skinid = FCNPC_SetSkinBasedOnType(npcid);
		if(skinid == 0)
		{
			return false;
		}
		
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[TYPE]: NPC ID: %d has changed its type from %d (%s) to %d (%s)", npcid, _:eNpcInfo[npcid][e_iType], NpcTypeNames[_:eNpcInfo[npcid][e_iType]], _:type, NpcTypeNames[_:type]);
		eNpcInfo[npcid][e_iType] = type;
		return true;
	}

//#endregion

//#region Timer updates

	FCNPC_StartUpdateTimer(npcid)
	{
		if(FCNPC_IsTimerRunning(npcid))
		{
			return false;
		}

		SCMF_ALL(-1, "Starting timer");
		Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);
		return true;
	}

	FCNPC_StopUpdateTimer(npcid)
	{
		if(!FCNPC_IsTimerRunning(npcid))
		{
			return false;
		}

		SCMF_ALL(-1, "Stopping timer");
		Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
		stop eNpcInfo[npcid][e_iUpdateTimer];
		return true;
	}

//#endregion

//#region Animations

	// Custom SetAnimation for FCNPC.
	FCNPC_SetAnimCallback(npcid, animationid, bool:freeze = false)
	{
		if(FCNPC_IsDeadEx(npcid) && animationid != KNIFE_KILL_KNIFE_PED_DAMAGE // Is dead and not playing 'knifed' animation.
		|| FCNPC_IsDeadEx(npcid) && animationid != BASEBALL_BAT_HIT_3) // Is dead and not playing 'fly back' animation when shot by shotgun.
		{
			return;
		}

		new tickCounter = -6;
		switch(animationid)
		{
			// IDLE
			case KISSING_GF_CARARGUE_02: 	   	tickCounter = -10; 
			case PED_FLEE_LKAROUND_01: 	   		tickCounter = -10;
			
			case PED_GAS_CWR:					tickCounter = -4;  // STUNNED
			case PED_IDLE_TIRED:				tickCounter = -4;  // STUNNED
			case KNIFE_KILL_KNIFE_PED_DAMAGE: 	tickCounter = -30; // KNIFED
			case BASEBALL_BAT_HIT_3:			tickCounter = -8;  // SHOT BY SHOTGUN
			case PED_GETUP: 				   	tickCounter = -4;  // GET UP
		}

		SCMF_ALL(-1, "Anim callback | tickcounter = %d", tickCounter);
		
		//FCNPC_ClearAnims(npcid);
		eNpcInfo[npcid][e_iPlayingAnim_Tick] = tickCounter;
		eNpcInfo[npcid][e_iPlayingAnim_Index] = animationid;

		if(!FCNPC_IsStunned(npcid))
		{
			FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_PLAYING_ANIM);		
		}
		FCNPC_SetAnimation(npcid, animationid, 4.1, 0, 1, 1, freeze, 0);
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

		if(weaponid == WEAPON_MOLOTOV)
		{
			FCNPC_SetAnimation(npcid, PED_FLOOR_HIT_F, 4.1, 0, 1, 1, 1, 0);
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
	FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId = -1, Float:distanceBetweenPlayers = 0.0)
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

//#region Areas management

	// Create NPC roam area.
	CreateNpcRoamArea(npcid)
	{
		if(IsValidDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]))
		{
			return false;
		}

		new Float:x, Float:y, Float:z;
		FCNPC_GetPosition(npcid, x, y, z);

		eNpcInfo[npcid][e_iRoamAreaId] = CreateDynamicCircle(x, y, AREA_ROAM_SIZE);
		return true;
	}

	// Destroy NPC roam area.
	DestroyNpcRoamArea(npcid)
	{
		if(!IsValidDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]))
		{
			return false;
		}

		DestroyDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]);
		return true;
	}

//#endregion

//#region Utils

	FCNPC_IsNpcDisabled(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_IDLE);
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
			FCNPC_SetStatus(npcid, STATUS_DEAD);
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

	// Set NPC new random skin based on its type.
	FCNPC_SetSkinBasedOnType(npcid)
	{
		new
			skinid = 0,
			NPC_TYPE:type = FCNPC_GetType(npcid);

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

	// Check if FCNPC is in a fire area.
	FCNPC_IsInFireArea(npcid, npcAreaId)
	{
		if(!IsPlayerNPC(npcid))
		{
			return false;
		}

		foreach(new i : ServerFires)
		{
			if(npcAreaId == eFireData[i][e_iFireAreaId])
			{
				return IsPlayerInDynamicArea(npcid, i);
			}
		}
		return false;
	}
	
	// Prepares FCNPC for (re)spawn (first spawn or not are both supported).
	FCNPC_PrepareForSpawn(npcid, bool:firstSpawn = false)
	{
		if(firstSpawn)
		{
			PreloadAnimations(npcid);
			SetPlayerTeam(npcid, FACTION_NONE);
		}
		else if(!firstSpawn)
		{
			FCNPC_SetStatus(npcid, STATUS_IDLE);
			RemovePlayerAttachedObj(npcid, eCharsInfo[npcid][e_iAttachSlot_Dynamic]);
		}

		FCNPC_ClearAnims(npcid);
		FCNPC_SetHealthEx(npcid, 100.0);
		CreateNpcRoamArea(npcid);
	}

//#endregion

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