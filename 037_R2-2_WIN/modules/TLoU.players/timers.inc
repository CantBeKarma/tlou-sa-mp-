// Timer which calls an animation of getting up. 
timer DelayedTimer_AnimGetUp[TIMER_GETUP_DELAY](playerid)
{
    if(Bit_Get(ePlayerFlag[e_bDoingCrouchBug], playerid))
    {
        Bit_Vet(ePlayerFlag[e_bDoingCrouchBug], playerid);
    }

    if(IsPlayerNPC(playerid)) 
		FCNPC_SetAnimCallback(playerid, PED_GETUP); // Start UpdateTimer when anim. finishes.
	else 
		ApplyAnimation(playerid, "PED", "GETUP", 4.1, 0, 1, 1, 0, 0);
}

// Enable player jump action (anti-bunny hopping).
timer DelayedTimer_EnableJumping[TIMER_BUNNYHOP_DELAY](playerid)
{
	if(Bit_Get(ePlayerFlag[e_bDoingBunnyHop], playerid))
	{
		Bit_Vet(ePlayerFlag[e_bDoingBunnyHop], playerid);
		TogglePlayerAction(playerid, PLAYER_ACTION_JUMP, true);
	}
}

// Check for how long player's been holding key for.
timer Callback_HoldKeyTimer[100](playerid, key)
{
	if(IsPlayerHoldingKey(playerid, key))
	{
		Bit_Let(ePlayerFlag[e_bKeyPress], playerid);
		CallLocalFunction("OnPlayerHoldingKey", "dd", playerid, key);
	}
    else 
    {
        if(Bit_Get(ePlayerFlag[e_bKeyPress], playerid))
       	{
			CallLocalFunction("OnPlayerReleaseKey", "ddd", playerid, key, GetConnectedTime(playerid) - ePlayerInfo[playerid][e_iTickCounter_holdingKey]);
       	}
        stop ePlayerInfo[playerid][e_iTimer_holdingKey];

		Bit_Vet(ePlayerFlag[e_bKeyPress], playerid);
        Bit_Vet(ePlayerFlag[e_bHoldingKey], playerid);
    }
}

// Check what is player looking at the current moment (players & items supported).
timer Timer_OnPlayerIsLooking[TIMER_LOOKAT_RATE](playerid)
{
//#region Looking at items...
	new lookingAt_objectId = GetPlayerCameraTargetDynObject(playerid);
	if(!IsValidDynamicObject(lookingAt_objectId))
	{
        if(IsPlayerLookingAtItem(playerid))
			GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;

		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
	}
	else
	{
		if(!Streamer_IsInArrayData(STREAMER_TYPE_OBJECT, lookingAt_objectId, E_STREAMER_EXTRA_ID, eObjData[e_bObj_Created]))
		{
			return 1;
		}

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(lookingAt_objectId, x, y, z);

		if(IsPlayerInRangeOfPoint(playerid, ITEM_LABEL_DRAW_DIST, x, y, z))
		{
			if(GetPlayerFocusedObjectId(playerid) != lookingAt_objectId)
			{
				// If I was looking at player but I do look at object now (reset lookingAt object data).
				if(IsPlayerLookingAtPlayer(playerid))
				{
					GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
				
					if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
						DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
				}

				GetPlayerFocusedObjectId(playerid) = lookingAt_objectId;
				OnPlayerLookAtItem(playerid, lookingAt_objectId);
			}
		}
	}
//#endregion

//#region Looking at player... (and not looking at any item).
	if(IsPlayerLookingAtItem(playerid))
	{
		return 1;
	}

	new lookingAt_playerId = GetPlayerCameraTargetPlayer(playerid);
	if(lookingAt_playerId == INVALID_PLAYER_ID)
	{
		if(IsPlayerLookingAtPlayer(playerid))
		{
			GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;

			if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
	}
	else
	{
		new Float:distance = GetDistanceBetweenPlayers(playerid, lookingAt_playerId);
		if(distance > SHIV_LABEL_DRAW_DIST)
		{
			if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		else
		{
			// If looking at dead NPC or dead player.
			if(IsPlayerNPC(lookingAt_playerId)  && FCNPC_IsDeadEx(lookingAt_playerId)
			|| !IsPlayerNPC(lookingAt_playerId) && IsPlayerDead(lookingAt_playerId))
			{
				if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
					DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
			}
			else
			{
				GetPlayerFocusedPlayerId(playerid) = lookingAt_playerId;

				if(IsPlayerNPC(lookingAt_playerId)) 
					OnPlayerLookAtNPC(playerid, lookingAt_playerId);
				else 
					OnPlayerLookAtPlayer(playerid, lookingAt_playerId);
			}
		}
	}
//#endregion
    return 0;
}