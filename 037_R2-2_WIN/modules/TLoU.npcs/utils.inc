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

//#region Timer updates

	FCNPC_StartUpdateTimer(npcid)
	{
		if(!FCNPC_IsTimerRunning(npcid))
		{
			SCMF_ALL(-1, "Starting timer");
			Bit_Let(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
			eNpcInfo[npcid][e_iUpdateTimer] = repeat FCNPC_OnInfectedUpdate(npcid);
		}
	}

	FCNPC_StopUpdateTimer(npcid)
	{
		if(FCNPC_IsTimerRunning(npcid))
		{
			SCMF_ALL(-1, "Stopping timer");
			Bit_Vet(eNpcFlag[e_bIsUpdateTimerRunning], npcid);
			stop eNpcInfo[npcid][e_iUpdateTimer];
		}
	}

//#endregion

//#region Areas management

	// Create NPC roam area.
	FCNPC_CreateRoamArea(npcid)
	{
		if(!IsValidDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]))
		{
			new Float:x, Float:y, Float:z;
			FCNPC_GetPosition(npcid, x, y, z);

			eNpcInfo[npcid][e_iRoamAreaId] = CreateDynamicCircle(x, y, AREA_ROAM_SIZE);
		}
	}

	// Destroy NPC roam area.
	FCNPC_DestroyRoamArea(npcid)
	{
		if(IsValidDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]))
		{
			DestroyDynamicArea(eNpcInfo[npcid][e_iRoamAreaId]);
		}
	}

//#endregion

function FCNPC_Distract(npcid, Float:x, Float:y, Float:z)
{
	if(!IsPlayerInRangeOfPoint(npcid, RANGE_NPC_DISTRACT, x, y, z))
	{
		return false;
	}

	FCNPC_StopUpdateTimer(npcid);
	FCNPC_StopMoving(npcid);
	FCNPC_DestroyRoamArea(npcid);

	FCNPC_SetAnimation(npcid, PED_FLEE_LKAROUND_01, 4.1, 0, 1, 1, 0, 0);
    wait_ms(1700);

	FCNPC_GoToOnGroundCol(npcid, x, y, z, FCNPC_MOVE_RUN);
	return true;
}

forward NPC_STATUS:FCNPC_GetStatus(npcid);
forward NPC_TYPE:FCNPC_GetType(npcid);

//#region Utils

	FCNPC_IsDisabled(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_DISABLED);
	FCNPC_IsIdle(npcid)				return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_IDLE);
	FCNPC_IsBurning(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_BURNING);
	FCNPC_IsStunned(npcid)			return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_STUNNED);
	FCNPC_IsPlayingAnim(npcid)		return (FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_PLAYING_ANIM);
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
			FCNPC_OnDie(npcid, killerid, weaponid, bodypart);
		}
	}

	// Check if FCNPC is in a fire area.
	FCNPC_IsInFireArea(npcid, npcAreaId)
	{
		if(!IsPlayerNPC(npcid) || FCNPC_IsDisabled(npcid))
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
		FCNPC_CreateRoamArea(npcid);
	}

//#endregion