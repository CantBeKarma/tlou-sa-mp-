IsPlayerAimingBow(playerid)	return (ePlayerWeapon[playerid][e_bAimingArrow]);

// Pull player's bow string.
Bow_PullString(playerid)
{
	if(IsPlayerAimingBow(playerid))
	{
		return;
	}

	ApplyAnimation(playerid, "TLoU", "BOW_AIM_IN", 3.1, 0, 1, 1, 1, 1);
	Audio_PlayEx(playerid, AUDIO_BOW_PULL);

	SPAO(playerid, ITEM_MODEL_ARROW);
	SPAO(playerid, ITEM_MODEL_BOW, true);

	ePlayerWeapon[playerid][e_bAimingArrow] = true;
	ePlayerWeapon[playerid][e_iTickCounter_bowString] = GetConnectedTime(playerid);
}

// Release player's bow string.
Bow_ReleaseString(playerid)
{
	Audio_StopEx(playerid);
	Audio_PlayEx(playerid, AUDIO_BOW_CANCEL);
	
	SPAO(playerid, ITEM_MODEL_BOW, true);
	RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);

	// normal = -0.071999, 0.002999, 0.069999, -72.899917, -171.399917, 1.300000, 0.758000, 0.627999, 1.000000
	// crouched = -0.090999, -0.061999, -0.011000, -5.899932, -171.399917,  1.700000, 0.758000, 0.627999, 1.000000

	if(!IsPlayerCrouched(playerid))
	{
		ClearAnims(playerid);
	}

	ePlayerWeapon[playerid][e_bAimingArrow] = false;
	ePlayerWeapon[playerid][e_iTickCounter_bowString] = 0;
}

// Fire arrow.
Arrow_FireArrow(playerid)
{
	// Validation.
	if(GetPlayerItemAmount(playerid, _:ITEM_IDX_BOW) <= 0
	|| GetConnectedTime(playerid) - (ePlayerWeapon[playerid][e_iTickCounter_projectileThrow]) < TICK_RATE_PROJECTILE_THROW)
	{
		return 1;
	}

	// Physics.
	new Float:pCamX, Float:pCamY, Float:pCamZ;
	GetPlayerCameraPos(playerid, pCamX, pCamY, pCamZ);

	new Float:pCamVecX, Float:pCamVecY, Float:pCamVecZ;
	GetPlayerCameraFrontVector(playerid, pCamVecX, pCamVecY, pCamVecZ);

	new Float:pPosX, Float:pPosY, Float:pPosZ, Float:pAngle;
	GetPlayerPos(playerid, pPosX, pPosY, pPosZ);
	GetPlayerFacingAngle(playerid, pAngle);
	GetXYInFrontOfPlayer(playerid, pPosX, pPosY, 1.2);

	new stretch_time = (GetConnectedTime(playerid) - ePlayerWeapon[playerid][e_iTickCounter_bowString]);
	new Float:distance = (stretch_time >= MAX_ARROW_STRETCH_TIME) ? (MAX_ARROW_DISTANCE) : ((stretch_time / 1000.0) * 15);

	new Float:dest_arrowX = pCamX + floatmul((distance / 100) * 3, floatcos(pAngle, degrees)) + floatmul(pCamVecX, distance);
	new Float:dest_arrowY = pCamY + floatmul((distance / 100) * 4.2, floatsin(pAngle, degrees)) + floatmul(pCamVecY, distance);
	new Float:dest_arrowZ = pCamZ + ((distance / 100) * 10.5) + floatmul(pCamVecZ, distance);

	new arrowObjectId = CreateDynamicObject(ITEM_MODEL_ARROW, pPosX, pPosY, pPosZ + 0.5, 0.0, 0.0, pAngle - 79.0);
	if(arrowObjectId == INVALID_OBJECT_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "Arrow_FireArrow: Unable to create arrow for player: %s (UID: %d | GUID: %d)", PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));
		return 1;
	}

	MoveDynamicObject(arrowObjectId, dest_arrowX, dest_arrowY, dest_arrowZ, (distance * 1.5));

	foreach(new i : Player)
	{
		Streamer_UpdateEx(i, pPosX, pPosY, pPosZ, .type = STREAMER_TYPE_OBJECT);
	}

	ePlayerWeapon[playerid][e_bAimingArrow] = false;
	Bit_Let(ePlayerFlag[e_bShootingCustomWeapon], playerid);
	ePlayerWeapon[playerid][e_iTickCounter_projectileThrow] = GetConnectedTime(playerid);
	
	OnPlayerShotArrow(playerid);
	OnArrowObjectUpdate(playerid, arrowObjectId);
	return 0;
}

// Check player in range of arrow object and return his ID (or NPC ID)
GetEntityInRangeOfArrow(Float:x, Float:y, Float:z, shooterId)
{
    for(new i = 0; i != MAX_PLAYERS; i++)
    {
        if(!IsPlayerConnected(i) || i == shooterId
        || !IsPlayerInRangeOfPoint(i, ARROW_RANGE_DAMAGE, x, y, z))
        {
            continue;
        }

        if(IsPlayerNPC(i))
        {
            if(FCNPC_IsUnableToPerformAction(i))
            {
                continue;
            }
        }
        else
        {
            if(IsPlayerDead(i)
            || IsPlayerBurning(i)
            || IsPlayerStunned(i)
            || IsPlayerFall(i))
            {
                continue;
            }
        }
        return i;
    }
    return INVALID_PLAYER_ID;
}

// Recreate arrow item once stopped moving.
Random_CreateArrowItem(Float:x, Float:y, Float:z)
{
    if(RandomEx(0, 2) == 1)
    {
        CA_FindZ_For2DCoord(x, y, z);
        CreateOrUpdateItem(_:ITEM_IDX_ARROW, 1, .x = x, .y = y, .z = z);
    }
}