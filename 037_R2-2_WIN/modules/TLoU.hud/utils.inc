IsPlayerHudFlashing(playerid)			return Bit_Get(ePlayerFlag[e_bIsHudBgFlashing], playerid);

// Stops HideHud timer and starts it again.
RestartHideHudTimer(playerid)
{
	if(IsPlayerAimingWeapon(playerid))
	{
		return;
	}
    stop ePlayerInfo[playerid][e_iTimer_HideHud];
    ePlayerInfo[playerid][e_iTimer_HideHud] = defer HideHud(playerid);
}