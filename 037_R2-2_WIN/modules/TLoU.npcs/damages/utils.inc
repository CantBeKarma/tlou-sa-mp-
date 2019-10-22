// Attacks player (updating callback - applied in TLoU.npcs\timers.inc)
FCNPC_AttackPlayer(npcid, playerid)
{
	if(IsPlayerDead(playerid) || IsPlayerBurning(playerid)
	|| IsPlayerStunned(playerid) || IsPlayerFall(playerid) 
    || CA_IsPlayerNearWater(playerid))
    {
		FCNPC_OnStopAttackingPlayer(npcid, playerid);
        return false;
    }

	new Float:x, Float:y, Float:z;
	FCNPC_GetPosition(npcid, x, y, z);

    if(!IsPlayerInRangeOfPoint(playerid, RANGE_NPC_ATTACK, x, y, z))
	{
		FCNPC_OnStopAttackingPlayer(npcid, playerid);
		return false;
	}

	FCNPC_OnAttackPlayer(npcid, playerid);
	return true;
}