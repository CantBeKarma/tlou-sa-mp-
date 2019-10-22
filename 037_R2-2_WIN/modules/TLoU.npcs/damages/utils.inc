// Attacks player (updating callback - applied in TLoU.npcs\timers.inc)
FCNPC_AttackPlayer(npcid, playerid)
{
	if(IsPlayerDead(playerid) || IsPlayerBurning(playerid)
	|| IsPlayerStunned(playerid) || IsPlayerFall(playerid) 
    || CA_IsPlayerNearWater(playerid)
	|| GetDistanceBetweenPlayers(npcid, playerid) > RANGE_NPC_ATTACK)
    {
		FCNPC_OnStopAttackingPlayer(npcid, playerid);
        return false;
    }

	FCNPC_OnAttackPlayer(npcid, playerid);
	return true;
}