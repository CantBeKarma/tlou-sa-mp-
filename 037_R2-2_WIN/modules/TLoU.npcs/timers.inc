//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnInfectedUpdate --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer FCNPC_OnInfectedUpdate[TIMER_FCNPC_UPDATE](npcid)
{
	switch(FCNPC_GetStatus(npcid))
	{
		case STATUS_IDLE:			FCNPC_OnIdle(npcid);
		case STATUS_PLAYING_ANIM: 	CLF(#FCNPC_OnPlayingAnim, "dd", npcid, eNpcInfo[npcid][e_iPlayingAnim_Index]);
		case STATUS_BURNING:		CLF(#FCNPC_Burn, "dd", npcid, eNpcInfo[npcid][e_iFireCreatorId]);
		case STATUS_CHASE:			FCNPC_ChasePlayer(npcid, eNpcInfo[npcid][e_iChaseId]);
	}
	return 0;
}

// Respawn after some time (add delay to respawn).
timer FCNPC_DelayRespawn[TIMER_FCNPC_RESPAWN](npcid)
{
    return FCNPC_Respawn(npcid);
}

forward FCNPC_OnIdle(npcid);
public FCNPC_OnIdle(npcid)
{
	new chaseId = GetAvailablePlayerToChase(npcid);
	if(chaseId != INVALID_PLAYER_ID)
	{
		FCNPC_SetStatus(npcid, STATUS_CHASE, chaseId);
	}
	else
	{
		FCNPC_PlayIdleActions(npcid);
	}
	return 1;
}

GetAvailablePlayerToChase(npcid)
{
	new Float:x, Float:y, Float:z;
	FCNPC_GetPosition(npcid, x, y, z);

	foreach(new playerid : Player)
	{
		if(IsPlayerCrouched(playerid)
		|| eNpcInfo[npcid][e_iChaseId] == playerid
		|| GetPlayerSpeed(playerid) == 0.0
		|| !IsPlayerInRangeOfPoint(playerid, RANGE_NPC_START_CHASE, x, y, z))
		{
			continue;
		}

		return playerid;
	}
	return INVALID_PLAYER_ID;
}