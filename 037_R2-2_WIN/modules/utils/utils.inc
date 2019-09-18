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

// Attach : attach backpack to player's back (happens during respawn).
AttachBackpackToPlayer(playerid, bool:isInFrontOfPlayer = false)
{
	switch(GetPlayerSkinEx(playerid))
	{
		case SKIN_JOEL:
			SPAO(playerid, OBJECT_MODEL_JOELBP, isInFrontOfPlayer);

		case SKIN_ELLIE, SKIN_ELLIE2:
			SPAO(playerid, OBJECT_MODEL_ELLIEBP, isInFrontOfPlayer);

		case SKIN_TESS:
			SPAO(playerid, OBJECT_MODEL_TESSBP, isInFrontOfPlayer);

		default:
			SPAO(playerid, OBJECT_MODEL_BILLBP, isInFrontOfPlayer);
	}
}

// Attach : attach weapons to player (happens during respawn).
AttachItemsToPlayer(playerid, itemId = -1)
{
	if(itemId == -1)
	{
		for(new i = 0; i != MAX_ITEMS_DATA; i++)
		{
			if(!IsItemType(i, ITEM_TYPE_WEAPON) && !IsItemType(i, ITEM_TYPE_MELEE))
			{
				continue;
			}

			if(!HasPlayerGotItem(playerid, i))
			{
				continue;
			}

			SPAO(playerid, GetItemModel(i));
		}
	}
	else
	{
		if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
		{
			return false;
		}

		if(!HasPlayerGotItem(playerid, itemId))
		{
			return false;
		}

		SPAO(playerid, GetItemModel(itemId));
	}

	return true;
}

// Gets a free available attachment slot.
GetFreeAttachmentSlot(playerid)
{
    for(new i = 0; i != MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
        if(!IsPlayerAttachedObjectSlotUsed(playerid, i))
			return i;
    }
    return -1;
}

// Check if attached object slot is used - if so, remove attached object.
RemovePlayerAttachedObj(playerid, slotId)
{
    if(IsPlayerAttachedObjectSlotUsed(playerid, slotId) && slotId != -1)
	{
        RemovePlayerAttachedObject(playerid, slotId);
	}
}

// Attach models to player (extraInfo is used if there are more than one option for a specific model).
SPAO(playerid, modelId, bool:extraInfo = false)
{
	if(modelId == -1)
	{
		return;	
	}
		
	new 
		slotIndex = -1, 
		attachmentIndex = -1;

	for(new i = 0; i != MAX_ATTACHMENTS_DATA; i++)
	{
		if(eAttachmentsData[i][e_iModelId] != modelId || eAttachmentsData[i][e_bExtraInfo] != extraInfo)
			continue;

		if(eAttachmentsData[i][e_iSkinId] == GetPlayerSkinEx(playerid) || eAttachmentsData[i][e_iSkinId] == -1)
		{
			attachmentIndex = i;

			slotIndex = GetFreeAttachmentSlot(playerid);
			break;
		}
	}

//#region Slot assignment based on model id

	if(slotIndex == -1 || attachmentIndex == -1)
		return;

	switch(modelId)
	{
		case 
			OBJECT_MODEL_JOELBP,
			OBJECT_MODEL_ELLIEBP,
			OBJECT_MODEL_TESSBP,
			OBJECT_MODEL_BILLBP:
			{
				if(eCharsInfo[playerid][e_iAttachSlot_Backpack] != -1)
					slotIndex = eCharsInfo[playerid][e_iAttachSlot_Backpack];
				else
					eCharsInfo[playerid][e_iAttachSlot_Backpack] = slotIndex;		
			}

		case 
			ITEM_MODEL_PIPE,
			ITEM_MODEL_SCSR_PIPE,
			ITEM_MODEL_HATCHET,
			ITEM_MODEL_MACHETE:
			{
				if(eCharsInfo[playerid][e_iAttachSlot_Melee] != -1)
					slotIndex = eCharsInfo[playerid][e_iAttachSlot_Melee];
				else
					eCharsInfo[playerid][e_iAttachSlot_Melee] = slotIndex;
			}

		case ITEM_MODEL_9MM, ITEM_MODEL_SHORTY:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_9mm] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_9mm];
			else
				eCharsInfo[playerid][e_iAttachSlot_9mm] = slotIndex;
		}

		case ITEM_MODEL_REVOLVER:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_Revolver] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_Revolver];
			else
				eCharsInfo[playerid][e_iAttachSlot_Revolver] = slotIndex;

		}

		case ITEM_MODEL_SHOTGUN, ITEM_MODEL_ASSAULT:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_LongGunOne] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_LongGunOne];
			else
				eCharsInfo[playerid][e_iAttachSlot_LongGunOne] = slotIndex;

		}

		case ITEM_MODEL_RIFLE, ITEM_MODEL_BOW:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_LongGunTwo] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_LongGunTwo];
			else
				eCharsInfo[playerid][e_iAttachSlot_LongGunTwo] = slotIndex;
		}

		default: 
		{
			if(eCharsInfo[playerid][e_iAttachSlot_Dynamic] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_Dynamic];
			else
				eCharsInfo[playerid][e_iAttachSlot_Dynamic] = slotIndex;
		}
	}
	
//#endregion

	SetPlayerAttachedObject(playerid, slotIndex, eAttachmentsData[attachmentIndex][e_iModelId], eAttachmentsData[attachmentIndex][e_iBoneId], \
		eAttachmentsData[attachmentIndex][e_fOffsetX], eAttachmentsData[attachmentIndex][e_fOffsetY], eAttachmentsData[attachmentIndex][e_fOffsetZ], \
		eAttachmentsData[attachmentIndex][e_fRotX],    eAttachmentsData[attachmentIndex][e_fRotY],    eAttachmentsData[attachmentIndex][e_fRotZ], \
		eAttachmentsData[attachmentIndex][e_fScaleX],  eAttachmentsData[attachmentIndex][e_fScaleY],  eAttachmentsData[attachmentIndex][e_fScaleZ]);
}

// Detach weapon models from player's body.
DetachWeaponFromPlayersBody(playerid, itemId)
{
	if(!IsItemType(itemId, ITEM_TYPE_BULLET_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
	{
		return false;
	}

    // Depends on the weapon Id - if new weapon is equipped then remove its attached object from player's body.
    new slotId = -1;
    switch(ITEM_IDX:itemId)
    {
        case 
			ITEM_IDX_9MM, 
			ITEM_IDX_SHORTY:        
			{
				slotId = eCharsInfo[playerid][e_iAttachSlot_9mm];
			}

        case ITEM_IDX_REVOLVER:
		{
			slotId = eCharsInfo[playerid][e_iAttachSlot_Revolver];
		}
        
        case 
            ITEM_IDX_SHOTGUN,
            ITEM_IDX_ASSAULT_RIFLE:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_LongGunOne];
            }
            
        case 
            ITEM_IDX_RIFLE,
            ITEM_IDX_BOW:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_LongGunTwo];
            }

        case 
            ITEM_IDX_PIPE,
            ITEM_IDX_SCSR_PIPE,
            ITEM_IDX_HATCHET,
            ITEM_IDX_MACHETE:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_Melee];
            }
        
        default:
        {
            // No need for seeking of dynamic object (e.g. projectiles, throwables, shiv)
            // since they are not attached to player's body at all.
            // so in fact, they are not going to be removed so we stop the code here from going any further.

            // slotId = eCharsInfo[playerid][e_iAttachSlot_Dynamic];
            return true;
        }
    }

    RemovePlayerAttachedObj(playerid, slotId);
	return true;
}

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