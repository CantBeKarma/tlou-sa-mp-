//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerStatusChanged ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStatusChanged(playerid, PLAYER_STATUS:oldStatusId, PLAYER_STATUS:newStatusId, statusIssuerId)
{
    new bool:success = true;

    if(newStatusId == PLAYER_STATUS_NONE
    || newStatusId == PLAYER_STATUS_DEAD
    || newStatusId == PLAYER_STATUS_FALL
    || newStatusId == PLAYER_STATUS_BURNING
    || newStatusId == PLAYER_STATUS_STUNNED)
    {
        if(oldStatusId == PLAYER_STATUS_AIMING || IsPlayerAimOrShootCustomItem(playerid))
        {
            OnPlayerStopAimingWeapon(playerid);
        }
    }

    switch(newStatusId)
    {
        case PLAYER_STATUS_NONE:
        {
            if(oldStatusId == PLAYER_STATUS_BURNING)
            {
                OnPlayerStopBurning(playerid);
            }

            if(oldStatusId == PLAYER_STATUS_STUNNED)
            {
                OnPlayerShakeOffStun(playerid);
            }
        }

        case PLAYER_STATUS_FALL:
        {
            ApplyAnimation(playerid, "BASEBALL", "BAT_HIT_3", 4.1, 0, 1, 1, 1, 0);
            defer DelayedTimer_AnimGetUp[1000](playerid);
        }

        case PLAYER_STATUS_USING_BACKPACK:
        {
            ShowBackpack(playerid);
            OnPlayerOpenBackpack(playerid, PlayerBackpackCategory(playerid));
        }

        case PLAYER_STATUS_USING_WEAPMENU:
        {
            ShowWeaponMenu(playerid);
            OnPlayerOpenWeaponMenu(playerid);
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
            Sprite2D_DisableUpdate(playerid);
        }
        else
        {
            Enable_OnPlayerLookAt(playerid);            
            Sprite2D_EnableUpdate(playerid);
        }
        //Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "OnPlayerStatusChanged: Player: %s (GUID: %d) changed status: OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", PlayerName(playerid), GetPlayerGuid(playerid), _:oldStatusId, _:newStatusId);
    }
    else
    {
        ePlayerInfo[playerid][e_iCurrentStatus] = oldStatusId;
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerStatusChanged: Status couldn't be changed for: %s (GUID: %d) : OLD_STATUS_ID = %d | NEW_STATUS_ID = %d", PlayerName(playerid), GetPlayerGuid(playerid), _:oldStatusId, _:newStatusId);
    }
    return 0;
}