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
            if(HasPlayerGotAnyItemsToPickUp(playerid))
            {
                Sprite2D_DestroyAll(playerid);
            }
            Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_AREA, 0);
        }
        else
        {
            Enable_OnPlayerLookAt(playerid);            
            Streamer_ToggleItemUpdate(playerid, STREAMER_TYPE_AREA, 1);
            Streamer_Update(playerid, STREAMER_TYPE_AREA);

            if(IsPlayerInAnyDynamicArea(playerid))
            {
                new areas[MAX_SPRITES_AT_ONE_TIME];
                GetPlayerDynamicAreas(playerid, areas, sizeof(areas));

                for(new areaid = 0; areaid != MAX_SPRITES_AT_ONE_TIME; areaid++)
                {
                    if(areas[areaid] == 0)
                    {
                        continue;
                    }

                    new areaObjectId = Streamer_GetIntData(STREAMER_TYPE_AREA, areas[areaid], E_STREAMER_EXTRA_ID);
                    if(areaObjectId > 0)
                    {
                        Sprite2D_Create(playerid, areaObjectId);
                    }
                }
            }
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