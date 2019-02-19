//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerText ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerText(playerid, text[])
{
	if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) 
		|| TextHasRestrictedCharacters(text)
		|| IsPlayerUsingAnim(playerid, ANIM_COLLECTION_SWIM_UNDERWATER) 
		|| IsPlayerDead(playerid))
	{
	    return 0;
	}

	if(strlen(text) < 1 || strlen(text) > 128)
	{
	    return 0;
	}

	new Float:msgDistance, msgType[24],
		msgLength = strlen(text) - 1,
		punctuation = (text[msgLength] != '.') && (text[msgLength] != '!') && (text[msgLength] != '?') ? ('.') : ('\0');

	if(strfind(text, ".", true, 0) == 0)
	{
		strcpy(msgType, "whispers", sizeof(msgType));
		msgDistance = 2.0, strdel(text, 0, 1);
	}
	else
	{
		if(strfind(text, "?", true, msgLength) != -1)
		{
			strcpy(msgType, "asks", sizeof(msgType));
			msgDistance = 10.0;
		}
		else if(strfind(text, "!", true, msgLength) != -1)
		{
			strcpy(msgType, "shouts", sizeof(msgType));
			msgDistance = 25.0;
		}
		else
		{
			strcpy(msgType, "says", sizeof(msgType));
			msgDistance = 10.0;
		}
	}

	new msgFormat[170];
	format(msgFormat, sizeof(msgFormat), "%s %s: %c%s%c", PlayerName(playerid), msgType, toupper(text[0]), text[1], punctuation);
	ProxMessage(msgDistance, playerid, msgFormat);

	Log(eLogs[e_Log_Index][LOG_INDEX_CHAT], INFO, msgFormat);
	return 1; 
}

// Show player his chat panel.
ShowChatPanel(playerid)
{
	stop HideChatPlanel(playerid);
	foreach(new i : PlayerChatMessages<playerid>)
	{
		PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i]);
	}

	defer HideChatPlanel(playerid);
}

// Hide player's chat panel.
timer HideChatPlanel[TIMER_HIDECHAT_DELAY](playerid)
{
	foreach(new i : Reverse(PlayerChatMessages))
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

// Format custom chat message.
FormatChatMessage(playerid, chatLineIdx, color, const text[])
{
	format(PlayerCustomChatTextString[playerid][chatLineIdx], 128, "%s", text);
	PlayerTextDrawColor     (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], color);
	PlayerTextDrawSetString (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], PlayerCustomChatTextString[playerid][chatLineIdx]);
}

// strcpy(msgPartOne, substr(text, 0, 63), sizeof(msgPartOne));
// strins(msgPartOne, dotContinuation, 63, sizeof(msgPartOne));

// strcpy(msgPartTwo, substr(text, 63, 128), sizeof(msgPartTwo));
// strins(msgPartTwo, dotContinuation, 0, sizeof(msgPartTwo));

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
		new lineBelowColor = PlayerTextDrawGetColor(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][lineBelowIdx]);

		// Format current msgIdx (9) with message color of msgIdx below (8) and change the message string of current (9) to message string below (8).
		FormatChatMessage(playerid, msgIdx, lineBelowColor, PlayerCustomChatTextString[playerid][lineBelowIdx]);
	}
	return (splitInTwoLines) ? (1) : (0);
}

// Custom send message which sends message in the chat panel
SendChatMessage(playerid, color, text[])
{
	print("SendChatMessage - START");

	new 
		msgPartOne[64 + 4] = EOS,
		msgPartTwo[64 + 4] = EOS,
		bool:splitInTwoLines = (strlen(text) > 64) ? true : false;

	if(splitInTwoLines)
	{
		print("Splitting my message in TWO strings.");

		SplitStringIntoTwoStrings(text, msgPartOne, msgPartTwo, 63, true);
		print(msgPartOne);
		print(msgPartTwo);
	}

	new chatLineIdx = Iter_Alloc(PlayerChatMessages<playerid>);
	printf("Iter_Alloc: chatLineIdx = %d", chatLineIdx);

	// Chat isn't full.
	if(chatLineIdx != INVALID_ITERATOR_SLOT)
	{
		print("Chat isn't full.");
		// If my new chat line is split into two lines and script has allocated
		// my new message Idx on the last one (9).
		// We need to move all the chat lines up, so we got enough space for two lines.
		if(splitInTwoLines && chatLineIdx == 9)
		{
			print("ChatLineIdx is 9 and my message is supposed to be split into two.");
			chatLineIdx = MovePlayerChatLinesUpByOne(playerid, splitInTwoLines);
			printf("New chatLineIdx = %d", chatLineIdx);
		}
	}
	// Chat is full | Move all messages up by one.
	else
	{
		printf("Chat is full. My chatLineIdx is/was: %d", chatLineIdx);
		chatLineIdx = MovePlayerChatLinesUpByOne(playerid, splitInTwoLines);
		printf("New chatLineIdx = %d", chatLineIdx);
	}

	// Actual message formatting and showing.
	if(!splitInTwoLines)
	{
		printf("Not splitting. Simple message");
		FormatChatMessage(playerid, chatLineIdx, color, text);
	}
	else
	{
		printf("Split in two. Indexes: %d | %d", chatLineIdx+1, chatLineIdx);

		FormatChatMessage(playerid, chatLineIdx+1, color, msgPartOne);
		FormatChatMessage(playerid, chatLineIdx, color, msgPartTwo);
	}
	ShowChatPanel(playerid);
}

// Send message based on distance to nearby players.
ProxMessage(Float:msgDistance, senderId, message[])
{
    new Float:distance;
	foreach(new playerId : Player)
	{
	    if((GetPlayerVirtualWorld(playerId) == GetPlayerVirtualWorld(senderId)) && (GetPlayerInterior(playerId) == GetPlayerInterior(senderId)))
    	{
            distance = GetDistanceBetweenPlayers(senderId, playerId);

            if(distance < (msgDistance / 16.0))		SendChatMessage(playerId, 0xE6E6E6E6, message);
            else if(distance < (msgDistance / 8.0))	SendChatMessage(playerId, 0xC8C8C8C8, message);
            else if(distance < (msgDistance / 4.0))	SendChatMessage(playerId, 0xAAAAAAAA, message);
            else if(distance < (msgDistance / 2.0))	SendChatMessage(playerId, 0x8C8C8C8C, message);
            else if(distance <  msgDistance)		SendChatMessage(playerId, 0x6E6E6E6E, message);
		}
	}
}