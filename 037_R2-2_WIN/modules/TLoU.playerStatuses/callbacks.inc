//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStatusChanged(playerid, PLAYER_STATUS:oldStatusId, PLAYER_STATUS:newStatusId, statusIssuerId)
{
    new bool:success = true;

    if(newStatusId == PLAYER_STATUS_NONE
    || newStatusId == PLAYER_STATUS_DEAD
    || newStatusId == PLAYER_STATUS_FALL)
    {
        if(IsPlayerAimingWeapon(playerid) || IsPlayerAimOrShootCustomItem(playerid))
        {
            OnPlayerStopAimingWeapon(playerid);
        }
    }

    switch(newStatusId)
    {
        case PLAYER_STATUS_NONE:
        {
            if(IsPlayerBurning(playerid))
            {
                OnPlayerStopBurning(playerid);
            }

            if(IsPlayerStunned(playerid))
            {
                OnPlayerShakeOffStun(playerid);
            }
        }

        case PLAYER_STATUS_FALL:
        {
            ApplyAnimation(playerid, "BASEBALL", "BAT_HIT_3", 4.1, 0, 1, 1, 1, 0);
        }

        case PLAYER_STATUS_USING_BACKPACK:
        {
            success = ShowBackpack(playerid);
            if(success)
            {
                OnPlayerOpenBackpack(playerid, PlayerBackpackCategory(playerid));
            }
        }

        case PLAYER_STATUS_USING_WEAPMENU:
        {
            success = ShowWeaponMenu(playerid);
            if(success)
            {
                OnPlayerOpenWeaponMenu(playerid);
            }
        }

        case PLAYER_STATUS_STUNNED:
        {
            CLF(#OnPlayerGetStunned, "d", playerid);
        }

        case PLAYER_STATUS_BURNING:
        {
            OnPlayerStartBurning(playerid, statusIssuerId);
        }
    }

    if(success)
    {
        if(newStatusId != PLAYER_STATUS_NONE)
        {
            Disable_OnPlayerLookAt(playerid);
        }
        else
        {
            Enable_OnPlayerLookAt(playerid);
        }

        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerStatusChanged: Player: %s (GUID: %d) changed status: OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", PlayerName(playerid), GetPlayerGuid(playerid), _:oldStatusId, _:newStatusId);
    }
    else
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerStatusChanged: Status couldn't be changed for: %s (GUID: %d) : OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", PlayerName(playerid), GetPlayerGuid(playerid), _:oldStatusId, _:newStatusId);
    }
    return 0;
}