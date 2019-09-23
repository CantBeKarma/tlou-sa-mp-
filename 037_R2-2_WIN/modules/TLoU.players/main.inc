#include <TLoU.players\forwards>
#include <TLoU.players\timers>
#include <TLoU.players\callbacks>
#include <TLoU.players\cache>
#include <TLoU.players\anti-bunnyhop>
#include <TLoU.players\anti-crouchbug>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerConnect --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerConnect(playerid) 
{
    if(IsPlayerNPC(playerid))
    {
        return 1;
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerTLoUClientJoin ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTLoUClientJoin(playerid, bool:has_plugin)
{
    if(has_plugin)
    {
        wait_ms(500);

        Bit_Vet(ePlayerFlag[e_bLogged], playerid);
        SetCameraBehindPlayer(playerid);
        SpawnPlayer(playerid);
    }
    else
    {
        if(!TLoU_IsClientConnected(playerid))
        {
            new String:pName = str_new_buf(MAX_PLAYER_NAME);
            str_resize(pName, GetPlayerName_s(playerid, pName, MAX_PLAYER_NAME));

            Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Player %s connected (possibly) without a tlou_client.asi file. Kicked.", _:pName);
            Kick(playerid);
            return 0;
        }
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerDisconnect ------------------- 
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDisconnect(playerid, reason)
{
    if(IsPlayerNPC(playerid))
    {
        return 1;
    }
    
    SavePlayerCharacterData(playerid);
    DestroyPlayerLabel(playerid);
    
    ResetEnum_ePlayerInfo(playerid);
    ResetBits_ePlayerFlag(playerid);
    ResetEnum_eCharsInfo(playerid);
    ResetEnum_ePlayerBpInfo(playerid);
    ResetEnum_ePlayerAudio(playerid);
    ResetEnum_ePlayerWeapon(playerid);
    ResetEnum_ePlayerWeapMenu(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerSpawn ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerSpawn(playerid) 
{
    if(!IsPlayerLogged(playerid))
    {
        OnPlayerFirstSpawn(playerid);
        return 0;
    }

    OnPlayerRespawn(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerFirstSpawn ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerFirstSpawn(playerid)
{
    if(!TLoU_IsClientConnected(playerid))
    {
        new String:pName = str_new_buf(MAX_PLAYER_NAME);
        str_resize(pName, GetPlayerName_s(playerid, pName, MAX_PLAYER_NAME));

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "Player %s connected (possibly) without a tlou_client.asi file. Kicked.", _:pName);
        Kick(playerid);
        return 0;
    }

    PreparePlayerToSign(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPlayerDeath -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDeath(playerid, killerid, reason)
{
	if(IsPlayerNPC(playerid))
	{
		return 1;
	}

	if(killerid != INVALID_PLAYER_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerDeath callback has been called for: %s (GUID: %d). Killed by: %s (GUID: %d)", PlayerName(playerid), GetPlayerGuid(playerid), PlayerName(killerid), GetPlayerGuid(killerid));
		return 1;
	}
			
	Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerDeath callback has been called for: %s (GUID: %d). VERIFY WHY!!!", PlayerName(playerid), GetPlayerUid(playerid));
	return 1;
}