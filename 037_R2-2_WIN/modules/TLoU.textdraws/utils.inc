// Set TextDraw string information and show it.
PlayerTdSetString(playerid, PlayerText:textdraw, text[])
{
	PlayerTextDrawSetString(playerid, textdraw, text);
	PlayerTextDrawShow(playerid, textdraw);
}

// Set TextDraw color and show it.
PlayerTdSetColor(playerid, PlayerText:textdraw, color, bool:isBox = false)
{
	new currentColor = PlayerTextDrawGetColor(playerid, textdraw);
	if(currentColor != color)
	{
        if(isBox) PlayerTextDrawBoxColor(playerid, textdraw, color);
        else PlayerTextDrawColor(playerid, textdraw, color);
        
		PlayerTextDrawShow(playerid, textdraw);
	}
}

// Show textdraw if it isn't visible.
PlayerTdShow(playerid, PlayerText:textdrawId)
{
    if(!IsPlayerTextDrawVisible(playerid, textdrawId))
    {
        PlayerTextDrawShow(playerid, textdrawId);
    }
}

// Hide PLAYER textdraw if it's visible.
PlayerTdHide(playerid, PlayerText:textdrawId)
{
    if(IsPlayerTextDrawVisible(playerid, textdrawId))
    {
        PlayerTextDrawHide(playerid, textdrawId);
    }
}

// Show GLOBAL textdraw if it isn't visible.
TdShowForPlayer(playerid, Text:textdrawId)
{
    if(!IsTextDrawVisibleForPlayer(playerid, textdrawId))
    {
        TextDrawShowForPlayer(playerid, textdrawId);
    }
}

// Show GLOBAL textdraw if it isn't visible.
TdHideForPlayer(playerid, Text:textdrawId)
{
    if(IsTextDrawVisibleForPlayer(playerid, textdrawId))
    {
        TextDrawHideForPlayer(playerid, textdrawId);
    }
}

function Delay_PlayerTdHide(playerid, PlayerText:textdrawId, time)
{
    if(time > 0) 
        SetTimerEx("Delay_PlayerTdHide", 1000, false, "ddd", playerid, _:textdrawId, time - 1);
	else if(time <= 0) 
        PlayerTdHide(playerid, textdrawId);
}