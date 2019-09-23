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

// Disable ambients noise (wind noise)
DisableDefaultAmbientNoise(playerid)
{
	return PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
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

// Replaces certain characters in a string provided with a replacement string.
strreplace(string[], const search[], const replacement[], bool:ignorecase = false, pos = 0, limit = -1, maxlength = sizeof(string)) 
{
    // No need to do anything if the limit is 0.
    if (limit == 0)
        return 0;
    
    new
             sublen = strlen(search),
             replen = strlen(replacement),
        bool:packed = ispacked(string),
             maxlen = maxlength,
             len = strlen(string),
             count = 0
    ;
    
    // "maxlen" holds the max string length (not to be confused with "maxlength", which holds the max. array size).
    // Since packed strings hold 4 characters per array slot, we multiply "maxlen" by 4.
    if (packed)
        maxlen *= 4;
    
    // If the length of the substring is 0, we have nothing to look for..
    if (!sublen)
        return 0;
    
    // In this line we both assign the return value from "strfind" to "pos" then check if it's -1.
    while (-1 != (pos = strfind(string, search, ignorecase, pos)))
	{
        // Delete the string we found
        strdel(string, pos, pos + sublen);
        
        len -= sublen;
        
        // If there's anything to put as replacement, insert it. Make sure there's enough room first.
        if (replen && len + replen < maxlen) {
            strins(string, replacement, pos, maxlength);
            
            pos += replen;
            len += replen;
        }
        
        // Is there a limit of number of replacements, if so, did we break it?
        if (limit != -1 && ++count >= limit)
            break;
    }
	return 1;
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