// Player pressing keys in backpack / using backpack.
timer Key_BackpackMenu[50](playerid)
{
    if(IsPlayerUsingBackpack(playerid))
    {
        defer Key_BackpackMenu(playerid);
    }

    if(((GetConnectedTime(playerid) - ePlayerBpInfo[playerid][e_iTickCounter_backpackKeys]) < 450) || IsPlayerHoldingKey(playerid, KEY_FIRE))
    {
        return 1;
	}

    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);

    if(updown != ePlayerBpInfo[playerid][e_iBackpackKeysUpDown])
    {
        if(!updown)
		{
		    ePlayerBpInfo[playerid][e_iBackpackKeysUpDown] = 0;
		    return 1;
		}

        ePlayerBpInfo[playerid][e_iBackpackKeysUpDown] = updown;
        OnPlayerUseBackpack(playerid, PlayerBackpackCategory(playerid), updown);
    }

    if(leftright != ePlayerBpInfo[playerid][e_iBackpackKeysLeftRight])
    {
        if(!leftright)
		{
		    ePlayerBpInfo[playerid][e_iBackpackKeysLeftRight] = 0;
		    return 1;
		}

        ePlayerBpInfo[playerid][e_iBackpackKeysLeftRight] = leftright;
        OnPlayerSwitchBpCtgry(playerid, leftright);
    }

    return 0;
}