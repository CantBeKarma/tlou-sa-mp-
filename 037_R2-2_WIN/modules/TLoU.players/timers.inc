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
			CLF(#OnPlayerReleaseKey, "ddd", playerid, key, GetHoldingKeyTime(playerid));
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
		GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 0;
	}

	//------------------- LOOKING AT PLAYER/NPC ------------------
	// If player looks at invalid player.
	new lookingAtId = GetPlayerCameraTargetPlayer(playerid);
	if(lookingAtId == INVALID_PLAYER_ID)
	{
		if(IsPlayerLookingAtPlayer(playerid))
		{
			GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
			if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
			{
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
			}
		}
		return 1;
	}

	// If players distance between eachother is higher than allowed
	// OR player is looking at dead player
	// OR player is looking at dead/disabled NPC
	if(GetDistanceBetweenPlayers(playerid, lookingAtId) > SHIV_LABEL_DRAW_DIST
	|| !IsPlayerNPC(lookingAtId) && IsPlayerDead(lookingAtId)
	|| IsPlayerNPC(lookingAtId) && (FCNPC_IsDeadEx(lookingAtId) || FCNPC_IsDisabled(lookingAtId) || FCNPC_IsBurning(lookingAtId)))
	{
		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
		}
		return 1;
	}

	GetPlayerFocusedPlayerId(playerid) = lookingAtId;
	if(IsPlayerNPC(lookingAtId))
	{
		OnPlayerLookAtNPC(playerid, lookingAtId);
	}	
	else
	{
		OnPlayerLookAtPlayer(playerid, lookingAtId);
	}
    return 0;
}