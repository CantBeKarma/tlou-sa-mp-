// Respawn after some time (add delay to respawn).
timer FCNPC_DelayRespawn[TIMER_FCNPC_RESPAWN](npcid)
{
    return FCNPC_Respawn(npcid);
}