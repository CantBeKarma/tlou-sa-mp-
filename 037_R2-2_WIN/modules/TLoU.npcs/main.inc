#include    <TLoU.npcs\forwards>
#include    <TLoU.npcs\timers>
#include    <TLoU.npcs\callbacks>
#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    SendRconCommandf("maxnpc %d", MAX_FCNPCS);
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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- FCNPC_OnCreate ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnCreate(npcid)
{
    if(!FCNPC_SetType(npcid, NPC_TYPE:RandomEx(_:TYPE_RUNNER, _:TYPE_CLICKER)))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_NPC], WARNING, "FCNPC_OnCreate: Unable to create NPC (couldn't set NPC type)");
        return 1;
    }

    Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnCreate: Sucessfully created NPC (ID: %d | TYPE: %s)", npcid, NpcTypeNames[_:FCNPC_GetType(npcid)]);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- FCNPC_OnDestroy -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnDestroy(npcid)
{
    Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnDestroy: Sucessfully destroyed NPC (ID: %d)", npcid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnSpawn ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnSpawn(npcid)
{
	FCNPC_PrepareForSpawn(npcid, .firstSpawn = true);
	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_OnSpawn: Sucessfully spawned NPC (ID: %d)", npcid);
	return 1;
}

//#region Hooking OnPlayer(Enter/Leave)DynArea for FCNPCs

	hook OnPlayerEnterDynArea(playerid, areaid)
	{
		if(IsPlayerNPC(playerid))
		{
			FCNPC_OnEnterDynArea(playerid, areaid);
		}
		return 0;
	}

	hook OnPlayerLeaveDynArea(playerid, areaid)
	{
		if(IsPlayerNPC(playerid))
		{
			FCNPC_OnLeaveDynArea(playerid, areaid);
		}
		return 0;
	}

//#endregion