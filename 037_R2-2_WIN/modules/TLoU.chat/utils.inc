// Send message based on distance to nearby players.
SendFadingMessage(playerid, Float:range, start_colour, end_colour, const message[])
{
    if (!IsPlayerConnected(playerid) || range < 0.0) 
	{
        return 0;
    }

    new 
		Float:ratio, Float:x, Float:y, Float:z, 
		vw = GetPlayerVirtualWorld(playerid),
		int = GetPlayerInterior(playerid);

    GetPlayerPos(playerid, x, y, z);

    foreach (new i : Player) 
	{
        if (GetPlayerVirtualWorld(i) != vw || GetPlayerInterior(i) != int || (ratio = (GetPlayerDistanceFromPoint(i, x, y, z) / range)) > 1.0) 
		{
            continue;
        }

		SendChatMessage(i, InterpolateColours(start_colour, end_colour, ratio), message);
    }
    return 1;
}

// Custom send message which sends message in the chat panel
SendChatMessage(playerid, color, const text[])
{
	new 
		firstChatLineIdx = INVALID_ITERATOR_SLOT,
		secondChatLineIdx = INVALID_ITERATOR_SLOT,
		msgPartOne[64 + 4] = EOS,
		msgPartTwo[64 + 4] = EOS,
		bool:splitInTwoLines = (strlen(text) > 64) ? true : false;

	firstChatLineIdx = Iter_Alloc(PlayerChatMessages<playerid>);

	if(splitInTwoLines)
	{
		secondChatLineIdx = Iter_Alloc(PlayerChatMessages<playerid>);
		SplitStringIntoTwoStrings(text, msgPartOne, msgPartTwo, 63, true);
	}

	// Chat isn't full.
	if(firstChatLineIdx != INVALID_ITERATOR_SLOT)
	{
		// If my new chat line is split into two lines and script has allocated
		// my new message Idx on the last one (9).
		// We need to move all the chat lines up, so we got enough space for two lines
		// and send my new splitted message in the beginning lines (Idx: 0 & 1).
		if(splitInTwoLines && firstChatLineIdx == 9)
		{
			firstChatLineIdx = 1;
			MovePlayerChatLinesUpByOne(playerid, splitInTwoLines);
		}
	}
	// Chat is full | Move all messages up by one.
	else
	{
		firstChatLineIdx = MovePlayerChatLinesUpByOne(playerid, splitInTwoLines);
	}

	// Actual message formatting and showing.
	if(!splitInTwoLines)
	{
		FormatChatMessage(playerid, firstChatLineIdx, color, text);
	}
	else
	{
		if(firstChatLineIdx == 1 && strlen(PlayerCustomChatTextString[playerid][9]) > 0)
		{
			if(strfind(PlayerCustomChatTextString[playerid][firstChatLineIdx], "...") != -1)
			{
				MovePlayerChatLinesUpByOne(playerid);
				MovePlayerChatLinesUpByOne(playerid);
			}

			FormatChatMessage(playerid, 0, color, msgPartTwo);
			FormatChatMessage(playerid, 1, color, msgPartOne);
		}
		else
		{
			FormatChatMessage(playerid, firstChatLineIdx, color, msgPartTwo);
			FormatChatMessage(playerid, secondChatLineIdx, color, msgPartOne);
		}
	}
	ShowChatPanel(playerid);
}

// Format custom chat message.
FormatChatMessage(playerid, chatLineIdx, color, const text[])
{
	format(PlayerCustomChatTextString[playerid][chatLineIdx], 128, "%s", text);
	PlayerTextDrawColor     (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], color);
	PlayerTextDrawSetString (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], PlayerCustomChatTextString[playerid][chatLineIdx]);
}

// Rotate all the current chat lines and texts upwards by one.
MovePlayerChatLinesUpByOne(playerid, bool:splitInTwoLines = false)
{
	foreach(new msgIdx : Reverse(PlayerChatMessages<playerid>))
	{
		// If my new message is split into two lines and my iterator is at
		// index message of 1 (0, 1...)
		if(splitInTwoLines && msgIdx == 1)
		{
			break;
		}

		// Get each line: index and color below current (for example (9) index gets (8) data)
		new lineBelowIdx = msgIdx-1;
		if(lineBelowIdx <= -1)
			break;

		new lineBelowColor = PlayerTextDrawGetColor(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][lineBelowIdx]);

		// Format current msgIdx (9) with message color of msgIdx below (8) and change the message string of current (9) to message string below (8).
		FormatChatMessage(playerid, msgIdx, lineBelowColor, PlayerCustomChatTextString[playerid][lineBelowIdx]);
	}
	return (splitInTwoLines) ? (1) : (0);
}

//#region Chat position & Alignment

	// Changes player's chat position (4 options to pick from).
	SetPlayerChatPosition(playerid, CP_POSITION:positionType)
	{
		new 
			Float:chatPosTopStart_Y = 2.5,
			Float:chatPosBottomStart_Y = 430.0;

		for(new i = 0; i != MAX_CHAT_LINES; i++)
		{
			switch(positionType)
			{
				case CP_POS_TOP_LEFT:
				{
					PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 15.0, chatPosTopStart_Y);
					chatPosTopStart_Y += 10.0;
				}

				case CP_POS_BOTTOM_LEFT:
				{
					PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 15.0, chatPosBottomStart_Y);
					chatPosBottomStart_Y -= 10.0;
				}
			}
		}
		ShowChatPanel(playerid);
	}

	// Align player chat to his needs (left/right/center).
	SetPlayerChatPanelAlignment(playerid, CP_ALIGNMENT:alignType)
	{
		if(alignType == CP_ALIGN_UNKNOWN)
		{
			return;
		}

		for(new i = 0; i != MAX_CHAT_LINES; i++)
		{
			PlayerTextDrawAlignment(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], _:alignType);
		}
		ShowChatPanel(playerid);
	}

//#endregion

// Show player his chat panel.
ShowChatPanel(playerid)
{
	stop ePlayerInfo[playerid][e_iTimer_hideChat];
	foreach(new i : PlayerChatMessages<playerid>)
	{
		PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i]);
	}

	ePlayerInfo[playerid][e_iTimer_hideChat] = defer HideChatPlanel(playerid);
}

// Hide player's chat panel.
timer HideChatPlanel[TIMER_HIDECHAT_DELAY](playerid)
{
	foreach(new i : Reverse(PlayerChatMessages<playerid>))
	{
		PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i]);
	}
}

// Determine if illegal character has been used in a text string.
TextHasRestrictedCharacters(const text[])
{
	for(new i = 0; i != sizeof(RestrictedTextCharactersList); i++)
	{
		if(strfind(text, RestrictedTextCharactersList[i], true) != -1)
		{
			return true;
		}
	}
	return false;
}