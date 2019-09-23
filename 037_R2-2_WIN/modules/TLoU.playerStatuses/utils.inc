forward bool:ValidateStatusChange(playerid, PLAYER_STATUS:newStatusId, statusIssuerId);

// Gets player's current status ID.
PLAYER_STATUS:GetPlayerStatus(playerid)
{
    return ePlayerInfo[playerid][e_iCurrentStatus];
}

// Sets player status (e.g. dead). 
// Status change can be caused by other player, for example when player shot another player
// and playerid was using a backpack, it will forcibly close backpack for player automatically.
SetPlayerStatus(playerid, PLAYER_STATUS:status, statusIssuerId = INVALID_PLAYER_ID)
{
    new PLAYER_STATUS:oldStatusId = GetPlayerStatus(playerid);

    if(ValidateStatusChange(playerid, status, statusIssuerId) == false)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SetPlayerStatus: Status couldn't be changed for: %s (GUID: %d) : OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", PlayerName(playerid), GetPlayerGuid(playerid), _:oldStatusId, _:status);
        return false;
    }
    
    ePlayerInfo[playerid][e_iCurrentStatus] = status;
    OnPlayerStatusChanged(playerid, oldStatusId, status, statusIssuerId);
    return true;
}

// Checks if player can change from his current status to a new one 
// based on the current actions that player is performing.
bool:ValidateStatusChange(playerid, PLAYER_STATUS:newStatusId, statusIssuerId)
{
    if(GetPlayerStatus(playerid) == newStatusId || IsPlayerDead(playerid) && newStatusId != PLAYER_STATUS_NONE)
    {
        return false;
    }

    new bool:success = true;

    if(newStatusId == PLAYER_STATUS_NONE
    || newStatusId == PLAYER_STATUS_DEAD
    || newStatusId == PLAYER_STATUS_FALL)
    {
        if(IsPlayerAimingWeapon(playerid) || IsPlayerAimOrShootCustomItem(playerid))
        {
            OnPlayerStopAimingWeapon(playerid);
        }

        if(IsPlayerUsingBackpack(playerid))
        {
            success = HideBackpack(playerid, .forceClose = (statusIssuerId != INVALID_PLAYER_ID));
            if(success)
            {
                OnPlayerCloseBackpack(playerid);
            }
        }

        if(IsPlayerUsingWeaponMenu(playerid))
        {
            success = HideWeaponMenu(playerid);
            if(success)
            {
                OnPlayerCloseWeaponMenu(playerid);
            }
        }
    }

    if(!success)
    {
        return false;
    }

    switch(newStatusId)
    {
        case PLAYER_STATUS_USING_BACKPACK:
        {
            if(IsPlayerUsingWeaponMenu(playerid))
            {
                success = false;
            }
        }

        case PLAYER_STATUS_USING_WEAPMENU:
        {
            if(IsPlayerUsingBackpack(playerid))
            {
                success = false;
            }
        }

        case PLAYER_STATUS_STUNNED:
        {
            if(IsPlayerBurning(playerid))
            {
                success = false;
            }
        }
    }
    return success;
}