//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnIdle -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnIdle(npcid)
{
	new chaseId = GetAvailablePlayerToChase(npcid);
	if(chaseId != INVALID_PLAYER_ID 
	&& FCNPC_GetAnimationIndex(npcid) != PED_GETUP 
	&& FCNPC_GetAnimationIndex(npcid) != BASEBALL_BAT_HIT_3)
	{
		FCNPC_SetStatus(npcid, STATUS_CHASE, chaseId);
	}
	else
	{
		FCNPC_PlayIdleActions(npcid);
	}
	return 1;
}

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
	CLF(#FCNPC_PlayDeathAnimation, "ddd", npcid, weaponid, bodypart);
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
		new 
            fireCreatorId = INVALID_PLAYER_ID,
            areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, areaid, E_STREAMER_EXTRA_ID);

		ReadFireRelation(areaObjectId, .fireCreatorId = fireCreatorId);
		FCNPC_SetStatus(npcid, STATUS_BURNING, fireCreatorId);
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
//------------------------ FCNPC_OnUpdate -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnUpdate(npcid)
{
	return 1;
}