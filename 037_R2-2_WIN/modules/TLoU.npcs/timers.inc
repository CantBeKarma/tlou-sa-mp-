//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnInfectedUpdate --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer FCNPC_OnInfectedUpdate[TIMER_FCNPC_UPDATE](npcid)
{
	switch(FCNPC_GetStatus(npcid))
	{
		case STATUS_PLAYING_ANIM: 	CLF(#FCNPC_OnPlayingAnim, "dd", npcid, eNpcInfo[npcid][e_iPlayingAnim_Index]);
		case STATUS_BURNING:		CLF(#FCNPC_Burn, "dd", npcid, eNpcInfo[npcid][e_iFireCreatorId]);
	}
	return 0;
}

// Respawn after some time (add delay to respawn).
timer FCNPC_DelayRespawn[TIMER_FCNPC_RESPAWN](npcid)
{
    return FCNPC_Respawn(npcid);
}