//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- FCNPC_OnRespawn -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnRespawn(npcid)
{
	FCNPC_PrepareForSpawn(npcid);
	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnRespawn: Sucessfully re-spawned NPC (ID: %d)", npcid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnDie --------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnDie(npcid, killerid, weaponid, bodypart)
{
	DestroyNpcRoamArea(npcid);
	FCNPC_PlayDeathAnimation(npcid, weaponid, bodypart);
	defer FCNPC_DelayRespawn(npcid);

	if(killerid == INVALID_PLAYER_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnDie: NPC (ID: %d) has killed itself", npcid);

		if(weaponid != _:ITEM_IDX_NAIL_BOMB)
		{
			weaponid = WEAPON_COLLISION;
		}
	}
	else
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnDie: NPC (ID: %d) has been killed by %s (UID: %d | GUID: %d) using weapon id: %d to body part: %d", \
				npcid, PlayerName(killerid), GetPlayerCharUid(killerid), GetPlayerGuid(killerid), weaponid, bodypart);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- FCNPC_OnInfectedUpdate -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer FCNPC_OnInfectedUpdate[TIMER_FCNPC_UPDATE](npcid)
{
	switch(FCNPC_GetStatus(npcid))
	{
		case STATUS_IDLE:			FCNPC_OnIdle(npcid);
		// case STATUS_PLAYING_ANIM:	FCNPC_OnPlayingAnim(npcid, eNpcInfo[npcid][e_iPlayingAnim_Index]);
		// case STATUS_BURNING:		FCNPC_OnBurning(npcid);
		// case STATUS_ATTACK:			FCNPC_OnAttackPlayer(npcid, eNpcInfo[npcid][e_iChaseId]);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnIdle -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnIdle(npcid)
{
	GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~IDLE: NPCID (%d)", npcid);

	FCNPC_PlayIdleActions(npcid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnReachDestination ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnReachDestination(npcid)
{
	if(FCNPC_IsRoaming(npcid))
	{
		SCMF_ALL(-1, "NPC reached destination (while roaming).");
		FCNPC_RandomMoveInDynamicArea(npcid, eNpcInfo[npcid][e_iRoamAreaId]);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- FCNPC_OnEnterDynArea ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnEnterDynArea(npcid, areaid)
{
	if(FCNPC_IsDeadEx(npcid) || FCNPC_IsBurning(npcid))
	{
		return 1;
	}

	if(FCNPC_IsInFireArea(npcid, areaid))
	{
		FCNPC_SetStatus(npcid, STATUS_BURNING);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnLeaveDynArea --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnLeaveDynArea(npcid, areaid)
{
	if(FCNPC_IsDeadEx(npcid) || FCNPC_IsBurning(npcid))
	{
		return 1;
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnStartBurning --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnStartBurning(npcid)
{
	SPAO(npcid, OBJECT_MODEL_BURNING);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnBurning ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnBurning(npcid)
{
	FCNPC_SetAnimation(npcid, PED_FLOOR_HIT_F, 4.1, 1, 1, 1, 0, 0);
	FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - FIRE_DAMAGE_AMOUNT, .weaponid = WEAPON_MOLOTOV);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnTriggerNailbomb -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnTriggerNailbomb(npcid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
{
	ApplyAnimation(npcid, "PED", "EV_STEP", 4.1, 0, 1, 1, 0, 0, 0);
	Audio3D_Play(throwerid, AUDIO_NAILBMB_TRIGGER, x, y, z, 10.0);
	defer OnNailbombExplode(npcid, throwerid, nailBmbIdx, x, y, z);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- FCNPC_OnGetExplodedByNailbomb -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetExplodedByNailbomb(npcid, throwerid)
{
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnGetStunned ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetStunned(npcid)
{
	FCNPC_SetAnimation(npcid, PED_GAS_CWR, 4.1, 1, 1, 1, 0, 0);
	wait_ms(3000);

	FCNPC_SetAnimation(npcid, PED_IDLE_TIRED, 4.1, 1, 1, 1, 0, 0);
	wait_ms(2000);

	FCNPC_OnShakeOffStun(npcid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnShakeOffStun --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnShakeOffStun(npcid)
{
	FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnGetDistracted ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetDistracted(npcid, Float:x, Float:y, Float:z)
{
	DestroyNpcRoamArea(npcid);
	FCNPC_SetAnimCallback(npcid, PED_FLEE_LKAROUND_01);

	eNpcInfo[npcid][e_fDistractedX] = x;
	eNpcInfo[npcid][e_fDistractedY] = y;
	eNpcInfo[npcid][e_fDistractedZ] = z;
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnUpdate -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnUpdate(npcid)
{
	if(FCNPC_IsDeadEx(npcid))
	{
		return 1;
	}

	foreach(new playerid : Player)
	{
		if(!IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]) 
		|| IsDynamicObjectMoving(ePlayerWeapon[playerid][e_iArrowObjectId]))
		{
			continue;
		}

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(ePlayerWeapon[playerid][e_iArrowObjectId], x, y, z);

		if(!IsPlayerInRangeOfPoint(npcid, ARROW_RANGE_DAMAGE, x, y, z))
		{
			continue;
		}

		DestroyPlayerArrow(playerid);
		FCNPC_OnGetArrowShot(npcid, playerid);
		break;
	}
	return 1;
}