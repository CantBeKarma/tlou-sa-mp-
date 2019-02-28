// Respawn after some time (add delay to respawn).
timer FCNPC_RespawnDelay[TIMER_FCNPC_RESPAWN](npcid)
{
    return FCNPC_Respawn(npcid);
}

// Custom FCNPC attack animation.
timer FCNPC_AttackPlayer[NPC_ATTACK_DELAY](npcid, chaseid)
{
// 	if(!FCNPC_IsDeadEx(npcid))
// 	{
// 		// if(eNpcInfo[npcid][e_iCurrentStatus] != NPC_STATUS_ATTACKING && GetDistanceBetweenPlayers(npcid, chaseid) <= RANGE_NPC_ATTACK)
// 		// {
// 		// 	FCNPC_StopUpdateTimer(npcid);
// 		// 	//eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_ATTACKING;
// 		// }

// //		if(eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_ATTACKING)
// ///		{
// 			new 
// 				NPC_TYPE:npcType = eNpcInfo[npcid][e_iType],
// 				Float:chaseId_oldHealth = GetPlayerHealthEx(chaseid),
// 				Float:chaseId_newHealth = chaseId_oldHealth - NpcDamageValues[npcType];

// 			SetPlayerToFacePlayer(npcid, chaseid);
// 			FCNPC_SetAnimation(npcid, NpcAttackAnims[random(sizeof(NpcAttackAnims))], 4.1, 1, 0, 0, 0, 0);

// 			FCNPC_OnAttackPlayer(npcid, chaseid, chaseId_oldHealth, chaseId_newHealth);
// 			eNpcInfo[npcid][e_iUpdateTimer] = defer FCNPC_AttackPlayer(npcid, chaseid);
// 	//	}
// 	}
}