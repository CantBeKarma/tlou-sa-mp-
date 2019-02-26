// Player pressing keys in weapon menu / using weapon menu.
timer Key_WeaponMenu[50](playerid)
{
    if(IsPlayerUsingWeaponMenu(playerid))
    {
        defer Key_WeaponMenu(playerid);
    }

    new keys, updown, leftright;
    GetPlayerKeys(playerid, keys, updown, leftright);

    if(updown != ePlayerWeapMenu[playerid][e_iWeapMenu_KeysUpDown])
    {
        if(!updown)
		{
		    ePlayerWeapMenu[playerid][e_iWeapMenu_KeysUpDown] = 0;
		    return 1;
		}

        ePlayerWeapMenu[playerid][e_iWeapMenu_KeysUpDown] = updown;
        
        if(IsPlayerSwitchingWeapons(playerid))
            OnPlayerUD_SwapWeaps(playerid, updown);
        else 
            OnPlayerUD_WeaponMenu(playerid, updown);
    }

    if(leftright != ePlayerWeapMenu[playerid][e_iWeapMenu_KeysLeftRight])
    {
        if(!leftright)
		{
		    ePlayerWeapMenu[playerid][e_iWeapMenu_KeysLeftRight] = 0;
		    return 1;
		}

        if(IsPlayerSwitchingWeapons(playerid)) 
            return 1;

        ePlayerWeapMenu[playerid][e_iWeapMenu_KeysLeftRight] = leftright;
        OnPlayerLR_WeaponMenu(playerid, leftright);
    }
    return 0;
}