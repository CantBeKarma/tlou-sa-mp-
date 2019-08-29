#include    <TLoU.npcs\timers>
#include    <TLoU.npcs\callbacks>
#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    SendRconCommandf("maxnpc %d", MAX_FCNPCS);

    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Creating FCNPCs...");
    //FCNPC_CreateInfected();
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeExit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    foreach(new npcid : FCNPC)
	{
	    FCNPC_Destroy(npcid);
	}
    return 1;
}