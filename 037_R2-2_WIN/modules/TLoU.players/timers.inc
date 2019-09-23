// Timer which calls an animation of getting up. 
timer DelayedTimer_AnimGetUp[TIMER_GETUP_DELAY](playerid)
{
	if(IsPlayerCrouchBugging(playerid))
    {
        Bit_Vet(ePlayerFlag[e_bDoingCrouchBug], playerid);
    }

	if(HasPlayerShotNailbomb(playerid))
	{
		Bit_Vet(ePlayerFlag[e_bShotNailbomb], playerid);
	}

	ApplyAnimation(playerid, "PED", "GETUP", 4.1, 0, 1, 1, 0, 0);
	SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
		
	return true;
}

// Enable player jump action (anti-bunny hopping).
timer DelayedTimer_EnableJumping[TIMER_BUNNYHOP_DELAY](playerid)
{
	if(!IsPlayerBunnyHopping(playerid))
	{
		return;
	}

	Bit_Vet(ePlayerFlag[e_bDoingBunnyHop], playerid);
	TogglePlayerAction(playerid, PLAYER_ACTION_JUMP, true);
}

// Check for how long player's been holding key for.
timer Callback_HoldKeyTimer[TIMER_HOLDKEY_RATE](playerid, key)
{
	if(IsPlayerHoldingKey(playerid, key))
	{
		Bit_Let(ePlayerFlag[e_bKeyPress], playerid);
		CLF(#OnPlayerHoldingKey, "dd", playerid, key);
	}
    else 
    {
        if(Bit_Get(ePlayerFlag[e_bKeyPress], playerid))
       	{
			CLF(#OnPlayerReleaseKey, "ddd", playerid, key, GetConnectedTime(playerid) - ePlayerInfo[playerid][e_iTickCounter_holdingKey]);
       	}
        stop ePlayerInfo[playerid][e_iTimer_holdingKey];

		Bit_Vet(ePlayerFlag[e_bKeyPress], playerid);
        Bit_Vet(ePlayerFlag[e_bHoldingKey], playerid);
    }
}

// Check what is player looking at the current moment (players & items supported).
timer Timer_OnPlayerIsLooking[TIMER_LOOKAT_RATE](playerid)
{
	if(!IsPlayerLookingTimerRunning(playerid))
	{
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 0;
	}

//#region Looking at items...
	
	// Looking at invalid object.
	new lookingAt_objectId = GetPlayerCameraTargetDynObject(playerid);
	if(!IsValidDynamicObject(lookingAt_objectId))
	{
        if(IsPlayerLookingAtItem(playerid))
		{
			GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
				
			if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
			{
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
			}
		}
	}

	// Check if object that player is looking at has a flag of created item.
	if(IsValidDynamicObject(lookingAt_objectId) && !Streamer_IsInArrayData(STREAMER_TYPE_OBJECT, lookingAt_objectId, E_STREAMER_EXTRA_ID, eObjData[e_bObj_Created]))
	{
		//SCMF(playerid, -1, "This isn't an item.");
		return 1;
	}

	if(GetPlayerFocusedObjectId(playerid) == lookingAt_objectId)
	{
		//SCMF(playerid, -1, "Already am looking at it.");
		return 1;
	}

	// If player's distance to the item is higher than allowed.
	new Float:x, Float:y, Float:z;
	GetDynamicObjectPos(lookingAt_objectId, x, y, z);
	if(!IsPlayerInRangeOfPoint(playerid, ITEM_LABEL_DRAW_DIST, x, y, z))
	{
		//SCMF(playerid, -1, "Not in range of the item!");
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 1;
	}

	// If I was looking at player but I do look at object now (reset lookingAt object data).
	if(IsPlayerLookingAtPlayer(playerid))
	{
		//SCMF(playerid, -1, "I was looking at the player. Reseting its data.");
		GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
	
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}		
	}

	GetPlayerFocusedObjectId(playerid) = lookingAt_objectId;
	OnPlayerLookAtItem(playerid, lookingAt_objectId);

//#endregion

//#region Looking at player...

	// If player is looking at the item.
	if(IsPlayerLookingAtItem(playerid))
	{
		GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
		return 1;
	}

	// If player looks at invalid player.
	new lookingAt_playerId = GetPlayerCameraTargetPlayer(playerid);
	if(lookingAt_playerId == INVALID_PLAYER_ID)
	{
		if(!IsPlayerLookingAtPlayer(playerid))
		{
			return 1;
		}

		GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;

		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 1;
	}

	// If player is looking at dead NPC or dead player.
	if(IsPlayerNPC(lookingAt_playerId) && FCNPC_IsDeadEx(lookingAt_playerId)
	|| !IsPlayerNPC(lookingAt_playerId) && IsPlayerDead(lookingAt_playerId))
	{
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 1;
	}

	// If player's distance between eachother is higher than allowed.
	new Float:distance = GetDistanceBetweenPlayers(playerid, lookingAt_playerId);
	if(distance > SHIV_LABEL_DRAW_DIST)
	{
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 1;
	}

	GetPlayerFocusedPlayerId(playerid) = lookingAt_playerId;
	if(IsPlayerNPC(lookingAt_playerId))
	{
		OnPlayerLookAtNPC(playerid, lookingAt_playerId);
	}	
	else
	{
		OnPlayerLookAtPlayer(playerid, lookingAt_playerId);
	}

//#endregion
    return 0;
}