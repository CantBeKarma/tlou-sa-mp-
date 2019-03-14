// Timer which calls an animation of getting up. 
timer DelayedTimer_AnimGetUp[TIMER_GETUP_DELAY](playerid)
{
	if(IsPlayerCrouchBugging(playerid))
    {
        Bit_Vet(ePlayerFlag[e_bDoingCrouchBug], playerid);
    }

	ApplyAnimation(playerid, "PED", "GETUP", 4.1, 0, 1, 1, 0, 0);
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
timer Callback_HoldKeyTimer[100](playerid, key)
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
//#region Looking at items...

	new playerTargetObjectId = GetPlayerCameraTargetDynObject(playerid);

	// If looking at invalid objectId and player WAS looking at item.
	if(!IsValidDynamicObject(playerTargetObjectId) && IsPlayerLookingAtItem(playerid))
	{
		GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
		DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
	}

	// If player is looking at valid Dynamic objectId.
	if(IsValidDynamicObject(playerTargetObjectId))
	{
		// Check if player's target objectId is different than the old one
		// and it has a flag of "item created" inside.
		if(GetPlayerFocusedObjectId(playerid) != playerTargetObjectId
		&& Streamer_IsInArrayData(STREAMER_TYPE_OBJECT, playerTargetObjectId, E_STREAMER_EXTRA_ID, eObjData[e_bObj_Created]))
		{
			// If player's distance to the item is higher than allowed.
			new Float:x, Float:y, Float:z, vW, intId;
			ReadItemRelation(playerTargetObjectId, .itemPosX = x, .itemPosY = y, .itemPosZ = z, .itemVw = vW, .itemIntId = intId);
			if(!IsPlayerInRangeOfPoint(playerid, ITEM_LABEL_DRAW_DIST, x, y, z)
			|| GetPlayerVirtualWorld(playerid) != vW || GetPlayerInterior(playerid) != intId)
			{
				return 1;
			}

			// If I was looking at player but I do look at object now (reset lookingAt object data).
			if(IsPlayerLookingAtPlayer(playerid))
			{
				GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
			}

			GetPlayerFocusedObjectId(playerid) = playerTargetObjectId;
			OnPlayerLookAtItem(playerid, playerTargetObjectId);
		}
	}

//#endregion

//#region Looking at player...

	// If player is looking at the item.
	if(IsPlayerLookingAtItem(playerid))
	{
		return 1;
	}

	new playerTargetPlayerId = GetPlayerCameraTargetPlayer(playerid);

	// If player looks at invalid player and WAS looking at the player recently.
	if(!IsValidPlayerId(playerTargetPlayerId) && IsPlayerLookingAtPlayer(playerid))
	{
		GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
		DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
	}

	if(IsValidPlayerId(playerTargetPlayerId))
	{
		// If player is looking at dead NPC or dead player.
		if( IsPlayerNPC(playerTargetPlayerId) && FCNPC_IsDeadEx(playerTargetPlayerId)
		|| !IsPlayerNPC(playerTargetPlayerId) && IsPlayerDead(playerTargetPlayerId) )
		{
			if(IsPlayerLookingAtPlayer(playerid))
			{
				GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
			}
			return 1;
		}

		// If player's distance between eachother is higher than allowed.
		if(GetDistanceBetweenPlayers(playerid, playerTargetPlayerId) > SHIV_LABEL_DRAW_DIST)
		{
			return 1;
		}

		GetPlayerFocusedPlayerId(playerid) = playerTargetPlayerId;
		if(IsPlayerNPC(playerTargetPlayerId))
		{
			OnPlayerLookAtNPC(playerid, playerTargetPlayerId);
		}	
		else
		{
			OnPlayerLookAtPlayer(playerid, playerTargetPlayerId);
		}
	}

//#endregion
    return 0;
}