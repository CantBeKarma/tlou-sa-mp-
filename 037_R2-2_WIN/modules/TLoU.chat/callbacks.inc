#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerText ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerText(playerid, text[])
{
	if(!IsPlayerLogged(playerid)
	|| IsPlayerDead(playerid)
	|| IsPlayerUsingAnim(playerid, ANIM_COLLECTION_SWIM_UNDERWATER))
	{
	    return 0;
	}

	if(TextHasRestrictedCharacters(text))
	{
		SendChatMessage(playerid, COLOR_RED, "WARNING: Invalid characters in text!");
		return 0;
	}

	if(strlen(text) < 1 || strlen(text) > 128)
	{
		SendChatMessage(playerid, COLOR_RED, "WARNING: Invalid message length!");
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
		else if(strfind(text, "!!", true, msgLength) != -1)
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
	SendFadingMessage(playerid, msgDistance, COLOR_CHAT_FADE_FROM, COLOR_CHAT_FADE_TO, msgFormat);

	Log(eLogs[e_Log_Index][LOG_INDEX_CHAT], INFO, msgFormat);
	return 1; 
}