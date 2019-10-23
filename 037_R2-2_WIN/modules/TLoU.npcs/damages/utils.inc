// Attacks player (updating callback - applied in TLoU.npcs\timers.inc)
FCNPC_AttackPlayer(npcid, playerid)
{
	if(IsPlayerDead(playerid) || IsPlayerBurning(playerid)
	|| IsPlayerStunned(playerid) || IsPlayerFall(playerid) 
    || CA_IsPlayerNearWater(playerid)
	|| GetDistanceBetweenPlayers(npcid, playerid) > RANGE_NPC_ATTACK
	|| (GetTickCount() - eNpcInfo[npcid][e_iTickCounter_damageAnim]) < TICK_RATE_ATTACK_AFTER_DAM)
    {
		FCNPC_OnStopAttackingPlayer(npcid, playerid);
        return false;
    }

	FCNPC_OnAttackPlayer(npcid, playerid);
	return true;
}