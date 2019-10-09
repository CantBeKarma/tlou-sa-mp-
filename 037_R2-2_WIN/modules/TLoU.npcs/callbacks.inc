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
		FCNPC_SetStatus(npcid, STATUS_BURNING, eFireData[areaid][e_iFireCreatorId]);
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