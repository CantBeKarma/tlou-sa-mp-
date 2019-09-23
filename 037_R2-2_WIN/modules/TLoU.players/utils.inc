// Reset all player data in enumerator.
ResetEnum_ePlayerInfo(playerid)
{
    static sBlank[e_PLAYER_DATA];
    ePlayerInfo[playerid] = sBlank;
	
	Disable_OnPlayerLookAt(playerid);
	stop ePlayerInfo[playerid][e_iTimer_holdingKey];
	stop ePlayerInfo[playerid][e_iTimer_lookingAt];

	print("ResetEnum_ePlayerInfo");
}

// Reset all player flags.
ResetBits_ePlayerFlag(playerid)
{
	Bit_Vet(ePlayerFlag[e_bLogged], playerid);
	Bit_Vet(ePlayerFlag[e_bDoingCrouchBug], playerid);
	Bit_Vet(ePlayerFlag[e_bDoingBunnyHop], playerid);
	Bit_Vet(ePlayerFlag[e_bCacheLoaded], playerid);
	Bit_Vet(ePlayerFlag[e_bHoldingKey], playerid);
	Bit_Vet(ePlayerFlag[e_bKeyPress], playerid);
	Bit_Vet(ePlayerFlag[e_bIsCrafting], playerid);
	Bit_Vet(ePlayerFlag[e_bIsCheckingColls], playerid);
	Bit_Vet(ePlayerFlag[e_bIsViewingColl], playerid);
	Bit_Vet(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
	Bit_Vet(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
	Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
	Bit_Vet(ePlayerFlag[e_bShootingCustomWeapon], playerid);
	Bit_Vet(ePlayerFlag[e_bShotNailbomb], playerid);

	print("ResetBits_ePlayerFlag");
}

//#region Utils

	IsPlayerBurning(playerid) 				return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_BURNING);
	IsPlayerStunned(playerid)				return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_STUNNED);
	IsPlayerFall(playerid)					return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_FALL);
	IsPlayerDead(playerid)					return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_DEAD);
	IsPlayerHealing(playerid)				return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_HEALING);
	IsPlayerUsingBackpack(playerid)			return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_USING_BACKPACK);
	IsPlayerUsingWeaponMenu(playerid)		return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_USING_WEAPMENU);
	IsPlayerAimingWeapon(playerid)			return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_AIMING);
	IsPlayerPlayingCallbackAnim(playerid)	return (GetPlayerStatus(playerid) == PLAYER_STATUS:PLAYER_STATUS_PLAYING_ANIM);
	
	IsPlayerCrafting(playerid)				return Bit_Get(ePlayerFlag[e_bIsCrafting], playerid);
	IsPlayerUpgradingSkill(playerid)		return Bit_Get(ePlayerFlag[e_bIsUpgradingSkill], playerid);
	IsPlayerCheckingCollectibles(playerid)	return Bit_Get(ePlayerFlag[e_bIsCheckingColls], playerid);
	IsPlayerViewingCollectible(playerid)	return Bit_Get(ePlayerFlag[e_bIsViewingColl], playerid);
	IsPlayerAimOrShootCustomItem(playerid)	return Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid);
	HasPlayerShotNailbomb(playerid)			return Bit_Get(ePlayerFlag[e_bShotNailbomb], playerid);
	IsPlayerCrouched(playerid)				return (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK);
	IsPlayerLookingAtPlayer(playerid)		return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && GetPlayerFocusedPlayerId(playerid) != INVALID_PLAYER_ID);
	IsPlayerLookingAtItem(playerid)			return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && GetPlayerFocusedObjectId(playerid) != INVALID_OBJECT_ID);
	IsPlayerAllowedToShiv(playerid)			return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && IsPlayerLookingAtPlayer(playerid));

	IsPlayerLookingTimerRunning(playerid)	return Bit_Get(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	IsPlayerLogged(playerid)				return Bit_Get(ePlayerFlag[e_bLogged], playerid);
	IsPlayerCacheLoaded(playerid)			return Bit_Get(ePlayerFlag[e_bCacheLoaded], playerid);
	IsPlayerBunnyHopping(playerid)			return Bit_Get(ePlayerFlag[e_bDoingBunnyHop], playerid);
	IsPlayerCrouchBugging(playerid)			return Bit_Get(ePlayerFlag[e_bDoingCrouchBug], playerid);

	IsPlayerIdle(playerid)
	{
		return !IsPlayerAimingWeapon(playerid)
			&& !IsPlayerUsingBackpack(playerid) 
			&& !IsPlayerUsingWeaponMenu(playerid)
			&& !IsPlayerUnableToPerformAction(playerid);
	}

	GetPlayerGuid(playerid)
	{
		return ePlayerInfo[playerid][e_iGuid];
	}

//#endregion

// Frezee player with timer to unfreeze (optional).
function FreezePlayer(playerid, bool:camMove, time)
{
	if(camMove)
	{
		TogglePlayerFrozen(playerid, true);
	}
	else
	{
		TogglePlayerControllable(playerid, false);
	}

	if(time == 0)
	{
		UnfreezePlayer(playerid);
		return;
	}
	
	if(time > 0)
	{
		SetTimerEx("FreezePlayer", 1000, false, "ddd", playerid, camMove, time-1);
	}
}

// Unfreeze player if necessary.
UnfreezePlayer(playerid)
{
	TogglePlayerFrozen(playerid, false);

	if(!IsPlayerControllable(playerid))
	{
		TogglePlayerControllable(playerid, true);
	}
}

// Enable OnPlayerLookAt callback timer if it isn't running.
function Enable_OnPlayerLookAt(playerid)
{
	if(IsPlayerLookingTimerRunning(playerid))
	{
		return;
	}
	
	EnablePlayerCameraTarget(playerid, true);
	GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
	GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
	Bit_Let(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	ePlayerInfo[playerid][e_iTimer_lookingAt] = repeat Timer_OnPlayerIsLooking(playerid);
}

// Disable OnPlayerLookAt callback.
Disable_OnPlayerLookAt(playerid)
{
	if(!IsPlayerLookingTimerRunning(playerid))
	{
		return;
	}

	EnablePlayerCameraTarget(playerid, false);
	GetPlayerFocusedPlayerId(playerid) = INVALID_PLAYER_ID;
	GetPlayerFocusedObjectId(playerid) = INVALID_OBJECT_ID;
	Bit_Vet(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	stop ePlayerInfo[playerid][e_iTimer_lookingAt]; 

	if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
	{
		DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
	}
}

// Main-menu theme with The Last of Us logo + prompt.
ConnectionIntroProcedure(playerid)
{
	if(IsPlayerLogged(playerid))
	{
		return;
	}

    wait_ms(1000);
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Logo]);

    wait_ms(2500);
    PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Logo]);

    wait_ms(2500);
    FadePlayerScreen(playerid, FADE_OUT);

    wait_ms(2600);
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_MainMenu_Prompt]);
}

// Play that procedure when player dies.
function PlayDeathProcedureForPlayer(playerid)
{
	if(!IsPlayerDead(playerid))
	{
		return;
	}

	Audio_PlayEx(playerid, AUDIO_DEATH_SFX);
	wait_ms(1200);

	if(IsPlayerHudFlashing(playerid))
    {
        Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
    }

    HideHud(playerid);
	HideItem(playerid);
	ShowPlayerBlackScreen(playerid);
	
	wait_ms(1600);

	PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], CombatTips[random(sizeof(CombatTips))]);
	wait_ms(1800);

	PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1]);
}

// Creates firefly pendant on player's death.
CreatePlayerPendant(playerid)
{
	static fireflyPendantItemId = 2;
	static fireflyPendantName[16] = "Firefly Pendant";
	new string[MAX_PLAYER_NAME + 24];
	
	new year, month, day;
	getdate(year, month, day);

	format(string, sizeof(string), "%s~n~%02d-%02d-%02d", PlayerName(playerid), year, month, day);
	CreateCollectible(fireflyPendantItemId, fireflyPendantName, string, .creatorId = playerid);
}

// Create player nickname label over player's head.
CreatePlayerLabel(playerid)
{
    ePlayerInfo[playerid][e_iPlayerLabel] = CreateDynamic3DTextLabel(PlayerName(playerid), COLOR_WHITE_TRANS, 0.0, 0.0, 0.12, 10.0, playerid, INVALID_VEHICLE_ID, true);
}

// Destroys player's nickname label if created.
DestroyPlayerLabel(playerid)
{
	if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iPlayerLabel]))
	{
		DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iPlayerLabel]);
	}
}

// Hide death procedure textdraws.
HideDeathProcedureTds(playerid)
{
	PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0]);
	PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1]);
}

// Define player spawn info with SetPlayerTeam fix.
function SetSpawnInfoEx(playerid, skin, Float:x, Float:y, Float:z, Float:rotation)
{
	// SetPlayerTeam(playerid, FACTION_NONE);
	SetPlayerSkinEx(playerid, skin);
	SetSpawnInfo(playerid, NO_TEAM, skin, Float:x, Float:y, Float:z, Float:rotation, 0, 0, 0, 0, 0, 0);
}

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

// Stops syncing a key for specific player when function is called.
DesynchronizePlayerKey(playerid, keyDef)
{
	new keys, updown, leftright;
	GetPlayerKeys(playerid, keys, updown, leftright);
	SetPlayerSyncKeys(playerid, keys & ~keyDef, updown, leftright);
}

// Check what's player's current speed.
GetPlayerSpeed(playerid)
{
	new Float:x, Float:y, Float:z, Float:result;
   	GetPlayerVelocity(playerid, x, y, z);
    result = floatsqroot(floatpower(floatabs(x), 2.0) + floatpower(floatabs(y), 2.0) + floatpower(floatabs(z), 2.0)) * 179.28625;
    return floatround(result);
}

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

// Get player's IP.
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

// Force change the old holding key to the specified key definition.
// ForceChangeHoldingKeyDef(playerid, key)
// {
// 	if(!Bit_Get(ePlayerFlag[e_bHoldingKey], playerid))
// 	{
// 		return;
// 	}

// 	stop ePlayerInfo[playerid][e_iTimer_holdingKey];
// 	ePlayerInfo[playerid][e_iTickCounter_holdingKey] = GetConnectedTime(playerid);
// 	ePlayerInfo[playerid][e_iTimer_holdingKey] = repeat Callback_HoldKeyTimer(playerid, key);
// }