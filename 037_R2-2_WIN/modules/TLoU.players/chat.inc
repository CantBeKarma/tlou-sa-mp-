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

	strcpy(msgType, \
				  (strfind(text, "?", true, msgLength) != -1) ? ("asks") 
				: (strfind(text, "!", true, msgLength) != -1) ? ("shouts") 
				: (strfind(text, ".", true, 0) != -1) ? ("whispers") 
				: ("says"), \
				sizeof(msgType));
	
	switch(YHash(msgType))
	{
		case _I<whispers>: msgDistance = 2.0, strdel(text, 0, 1);
		case _I<shouts>:   msgDistance = 25.0;
		default: 		   msgDistance = 10.0;
	}

	new msgFormat[170];
	format(msgFormat, sizeof(msgFormat), "%s %s: %c%s%c", PlayerName(playerid), msgType, toupper(text[0]), text[1], punctuation);
	ProxMessage(msgDistance, playerid, msgFormat);

	Log(eLogs[e_Log_Index][LOG_INDEX_CHAT], INFO, msgFormat);
	return 1;
}

// Hide player's chat panel.
function HideChatPlanel(playerid)
{
    for(new i = 0; i != MAX_CHAT_LINES; i++)
	{
        if(IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i]))
            PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i]);
	}
}

// Determine if illegal character has been used in a chat.
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
	format(PlayerCustomChatText[playerid][chatLineIdx], 128, "%s", text);
	PlayerTextDrawColor     (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], color);
	PlayerTextDrawSetString (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx], PlayerCustomChatText[playerid][chatLineIdx]);
	PlayerTextDrawShow      (playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][chatLineIdx]);
}

// Custom send message which sends message in the chat panel
SendChatMessage(playerid, color, text[])
{
	static newLine[12] = "... ~n~ ...";

	new 
		messagePartOne[64] = EOS,
		messagePartTwo[64] = EOS,
		bool:splitInTwoLines = false;

	if(strlen(text) > 64)
	{
		splitInTwoLines = true;

		strcpy(messagePartOne, substr(text, 0, 63), sizeof(messagePartOne));
	    strins(text, newLine, 63, 128);
		strcpy(messagePartOne, substr(text, 63, 128), sizeof(messagePartOne));
	}

	new chatLineIdx = 0;
	for(new i = 0; i != MAX_CHAT_LINES; i++)
	{
		if(IsPlayerCustomChatFull[playerid] == true)
		{
			chatLineIdx = (splitInTwoLines == true) ? (MAX_CHAT_LINES-2) : (MAX_CHAT_LINES-1);
			break;
		}

		if(strlen(PlayerCustomChatText[playerid][i]) > 0 
			|| PlayerCustomChatText[playerid][i][0] != EOS)
		{
			continue;
		}

		if(i == MAX_CHAT_LINES-1)
		{
			IsPlayerCustomChatFull[playerid] = true;
			chatLineIdx = MAX_CHAT_LINES-1;
			break;
		}

		chatLineIdx = i;
	}

	if(!splitInTwoLines)
	{
		FormatChatMessage(playerid, chatLineIdx, color, text);
	}
	else
	{
		FormatChatMessage(playerid, chatLineIdx, color, messagePartOne);
		FormatChatMessage(playerid, chatLineIdx+1, color, messagePartTwo);
	}
}

// Send message based on distance to nearby players.
ProxMessage(Float:msgDistance, playerid, message[])
{
    new Float:distance;
	foreach(new person : Player)
	{
	    if((GetPlayerVirtualWorld(person) == GetPlayerVirtualWorld(playerid)) && (GetPlayerInterior(person) == GetPlayerInterior(playerid)))
    	{
            distance = GetDistanceBetweenPlayers(playerid, person);

            if(distance < (msgDistance / 16.0))		SendChatMessage(person, 0xE6E6E6E6, message);
            else if(distance < (msgDistance / 8.0))	SendChatMessage(person, 0xC8C8C8C8, message);
            else if(distance < (msgDistance / 4.0))	SendChatMessage(person, 0xAAAAAAAA, message);
            else if(distance < (msgDistance / 2.0))	SendChatMessage(person, 0x8C8C8C8C, message);
            else if(distance <  msgDistance)		SendChatMessage(person, 0x6E6E6E6E, message);
		}
	}
}

// SubString - Custom string extend function.
substr(const sSource[], iStart, iLength = sizeof sSource)
{
	new
		sReturn[128];

	if(iLength < 0)
	{
		strmid(sReturn, sSource, iStart, strlen(sSource) + iLength);
		return sReturn;
	}
	else
	{
		strmid(sReturn, sSource, iStart, (iStart + iLength));
		return sReturn;
	}
}