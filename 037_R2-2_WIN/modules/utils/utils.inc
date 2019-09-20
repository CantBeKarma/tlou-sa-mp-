// Define player spawn info with SetPlayerTeam fix.
function SetSpawnInfoEx(playerid, skin, Float:x, Float:y, Float:z, Float:rotation)
{
	// SetPlayerTeam(playerid, FACTION_NONE);
	SetPlayerSkinEx(playerid, skin);
	SetSpawnInfo(playerid, NO_TEAM, skin, Float:x, Float:y, Float:z, Float:rotation, 0, 0, 0, 0, 0, 0);
}

// Clear player chat.
ClearChat(playerid)
{
	for(new i = 0; i != 50; i++)
	{
		SendClientMessage(playerid, -1, " ");
	}
}

// Check what key is player currently holding.
IsPlayerHoldingKey(playerid, holdingKey)
{
    static keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
    return (keys & holdingKey);
}

// Absolute integer value.
// IntAbs(value)
// {
// 	return ((value < 0) ? (-value) : (value));
// }

// Random between two numbers.
RandomEx(min, max)
{
	return random(max - min) + min;
}

// Stops syncing a key for specific player when function is called.
DesynchronizePlayerKey(playerid, keyDef)
{
	new keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
	SetPlayerSyncKeys(playerid, keys & ~keyDef, updown, leftright);
}

// Check if player Id is valid or invalid.
// IsValidPlayerId(playerid)
// {
// 	return playerid != INVALID_PLAYER_ID;
// }

// Extends GetPlayerSkin default function.
GetPlayerSkinEx(playerid)
{
	return eCharsInfo[playerid][e_iSkinId];
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

// Extends SetPlayerSkin default function.
SetPlayerSkinEx(playerid, skinId)
{
    eCharsInfo[playerid][e_iSkinId] = skinId;
    SetPlayerSkin(playerid, skinId);
    return 1;
}

// Check what's player's current speed.
GetPlayerSpeed(playerid)
{
	new Float:x, Float:y, Float:z, Float:result;
   	GetPlayerVelocity(playerid, x, y, z);
    result = floatsqroot(floatpower(floatabs(x), 2.0) + floatpower(floatabs(y), 2.0) + floatpower(floatabs(z), 2.0)) * 179.28625;
    return floatround(result);
}

// Splits string into two strings based on the split point number position (can use dot Continuation symbols too).
SplitStringIntoTwoStrings(const string[], 
	stringPartOne[], 
	stringPartTwo[], 
	splitPointNumber = 64, 
	bool:useDotContinuation = false,
	stringPartOneSize = sizeof(stringPartOne),
	stringPartTwoSize = sizeof(stringPartTwo))
{
	static dotContinuation[3 + 1] = "...";

	strcpy(stringPartOne, substr(string, 0, splitPointNumber), stringPartOneSize);
	strcpy(stringPartTwo, substr(string, splitPointNumber, 128), stringPartTwoSize);

	if(useDotContinuation)
	{
		strins(stringPartOne, dotContinuation, splitPointNumber-1, stringPartOneSize);
		strins(stringPartTwo, dotContinuation, 0, stringPartTwoSize);
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

#if !defined strcpy

	stock strcpy(dest[], src[], sz=sizeof(dest))
	{
		dest[0] = 0;	
		return strcat(dest, src, sz);
	}

#endif

// Set player screen to black and show it.
ShowPlayerBlackScreen(playerid)
{
	new color = PlayerTextDrawGetBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen]);
	if(color != COLOR_BLACK)
	{
    	PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen], COLOR_BLACK);
	}
	PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen]);
}

// Disable ambients noise (wind noise)
DisableDefaultAmbientNoise(playerid)
{
	return PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
}

// Get player's IP.
forward [16]GetPlayerIpAddress(playerid);
stock GetPlayerIpAddress(playerid)
{
	//#pragma unused playerid
    #emit PUSH.C 16
    #emit PUSH.S 16
    //#emit PUSH.S 12
	#emit PUSH.S playerid
    #emit PUSH.C 12
    #emit SYSREQ.C GetPlayerIp
    #emit STACK 16
    #emit RETN
}

// Fade player screen
function FadePlayerScreen(playerid, type)
{
	new color = PlayerTextDrawGetBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen]);

	if(type == FADE_IN)
	{
		color = (((color + 10) >= 255) ? 255 : color + 10);
		if(color == 255)
			type = FADE_NONE;
	}
	else if(type == FADE_OUT)
	{
		color = (((color - 10) <= 0) ? 0 : color - 10);
		if(!color)
			type = FADE_NONE;
	}

	PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen], color);
	PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_BlackScreen]);

	if(type != FADE_NONE)
	{
	    SetTimerEx("FadePlayerScreen", 50, false, "dd", playerid, type);
	}
}

// Face player against player.
SetPlayerToFacePlayer(playerid, targetid)
{
	static  
			Float:x, Float:y, Float:z, Float:angle,
			Float:x2, Float:y2, Float:z2;

	GetPlayerPos(targetid, x, y, z);
	GetPlayerPos(playerid, x2, y2, z2);

	angle = (180.0 - atan2(x2 - x, y2 - y));

	if(IsPlayerNPC(playerid)) FCNPC_SetAngle(playerid, angle + (5.0 * -1));
	else SetPlayerFacingAngle(playerid, angle + (5.0 * -1));
}

// Set player's facing angle where he's currently looking (useful for custom-weapons).
SetAngleBasedOnCam(playerid)
{
	new Float:pCamX, Float:pCamY, Float:pCamZ;
	GetPlayerCameraPos(playerid, pCamX, pCamY, pCamZ);

	new Float:pCamVecX, Float:pCamVecY, Float:pCamVecZ;
	GetPlayerCameraFrontVector(playerid, pCamVecX, pCamVecY, pCamVecZ);

	new Float:pPosX, Float:pPosY, Float:pPosZ, Float:angle;
	GetPlayerPos(playerid, pPosX, pPosY, pPosZ);

	new Float:dest_angleX, Float:dest_angleY;
	dest_angleX = (pCamX + pCamVecX);
	dest_angleY = (pCamY + pCamVecY);
	angle = atan2(dest_angleX - pPosX, dest_angleY - pPosY);
	
	SetPlayerFacingAngle(playerid, 180.0 - angle);
}

// Check distance between players.
Float:GetDistanceBetweenPlayers(playerid, targetplayerid)
{
    if(!IsPlayerConnected(playerid) || !IsPlayerConnected(targetplayerid)) 
	{
        return -1.00;
    }

	new Float:x, Float:y, Float:z;
	if(IsPlayerNPC(targetplayerid)) FCNPC_GetPosition(targetplayerid, x, y, z);
	else GetPlayerPos(targetplayerid, x, y, z);
	
	return GetPlayerDistanceFromPoint(playerid, x, y, z);
}

// Check if one Z point is higher/lower than second Z point.
CompareZ(Float:fZ, Float:fZ2, Float:difference)
{
	new Float:zdiff = floatabs(fZ-fZ2);
	if(zdiff == 0.0) return true;
	else if(fZ < 0 && fZ2 < 0)
	{
		if(zdiff <= difference) return true;
		return false;
	}
	else if(fZ > 0 && fZ2 > 0)
	{
		if(zdiff <= difference) return true;
		return false;
	}
	return false;
}

#define		INVALID_Z_LEVEL		(-1)
#define		Z_LEVEL_SAME		(0)
#define		Z_LEVEL_ABOVE		(1)
#define		Z_LEVEL_BELOW		(2)

CMD:checkzdiff(playerid, params[])
{
	new targetId;
	if(sscanf(params, "d", targetId))return SCM(playerid, -1, "/checkzdiff (targetId)");
	{
		new Float:z;
		GetPlayerPos(playerid, z, z, z);

		new Float:targetZ;
		FCNPC_GetPosition(targetId, targetZ, targetZ, targetZ);

		SCMF(playerid, -1, "Height diff result : %d", CompareZ_Level(z, targetZ));
		SCMF(playerid, -1, "Actual diff: %f", z-targetZ);
	}
	return 1;
}

CompareZ_Level(Float:fZ, Float:fZ2)//, Float:heightDiff)
{
	new Float:zDiff = fZ-fZ2;

	if(zDiff >= -0.1 && zDiff <= 1.0)
		return Z_LEVEL_SAME;
		
	else if(zDiff >= 1.1)
	//else if(floatcmp(zDiff, heightDiff) == 1)
		return Z_LEVEL_ABOVE;

	else if(zDiff < -0.1)
	//else if(floatcmp(zDiff, heightDiff) == -1)
		return Z_LEVEL_BELOW;
	
	return INVALID_Z_LEVEL;	
}

// Camera interploation when opening the backpack.
MoveCameraNextToPlayer(playerid, Float:distance, Float:offset, move_time, Float:height = 0.0)
{
    new Float:X, Float:Y, Float:Z, Float:Ang;
    GetPlayerPos(playerid, X, Y, Z);
    GetPlayerFacingAngle(playerid, Ang);

    new Float:newX = X + (distance * floatsin(-Ang, degrees));
    new Float:newY = Y + (distance * floatcos(-Ang, degrees));

    SetCameraBehindPlayer(playerid);
    SetPlayerCameraPos(playerid, newX, newY, Z + height);

    X = newX, Y = newY;
    Ang -= 90.0;

    newX += (offset * floatsin(-Ang, degrees));
    newY += (offset * floatcos(-Ang, degrees));
    InterpolateCameraPos(playerid, X, Y, Z, newX, newY, Z, move_time, CAMERA_MOVE);
}

// Check if specified vehicle model is a (motor/quad) bike.
IsVehicleModelBike(model)
{
	switch(model)
	{
		case 448, 461, 462, 463, 468, 471, 481, 509, 510, 521, 522, 523, 581, 586:
		{
			return true;
		}
	}
	return false;
}

// Get XY coords in front of a player with certain distance.
Float:GetXYInFrontOfPlayer(playerid, &Float:x, &Float:y, Float:distance)
{
	new Float:ang;
	GetPlayerPos(playerid, x, y, ang);
	GetPlayerFacingAngle(playerid, ang);

	x += (distance * floatsin(-ang, degrees));
	y += (distance * floatcos(-ang, degrees));
	return ang;
}
Float:GetXYBehindPlayer(playerid, &Float:x, &Float:y, Float:distance)return GetXYInFrontOfPlayer(playerid, x, y, distance);
// Get XY coords behind player with certain distance (for distance substract sign '-' needs to be supplied).

// Stuns all nearby players and FCNPCs that are in range of x, y, z coordinates.
// Optional: throwerid - so he won't get stunned if he's the initiator of the projectile.
StunNearbyPlayersAndNpcs(Float:x, Float:y, Float:z, throwerid = INVALID_PLAYER_ID)
{
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || i == throwerid
        || !IsPlayerInRangeOfPoint(i, SMOKEBMB_AFFECT_RANGE, x, y, z))
        {
            continue;
        }

        if(IsPlayerNPC(i))
        {
			FCNPC_SetStatus(i, NPC_STATUS:STATUS_STUNNED);
        }
        else
        {
			SetPlayerStatus(i, PLAYER_STATUS:PLAYER_STATUS_STUNNED);
        }
    }
}

ShowDialogMessage(playerid, title[], message[])
{
	return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, title, message, "Close");
}