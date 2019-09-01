#include	<YSI\y_hooks>

// Reset all player data in enumerator (called in OnPlayerDisconnect)
ResetEnum_ePlayerInfo(playerid)
{
    static sBlank[e_PLAYER_DATA];
    ePlayerInfo[playerid] = sBlank;
	
	Disable_OnPlayerLookAt(playerid);
	stop ePlayerInfo[playerid][e_iTimer_holdingKey];
	stop ePlayerInfo[playerid][e_iTimer_lookingAt];

	print("ResetEnum_ePlayerInfo");
}

// Reset all player flags (called in OnPlayerDisconnect)
ResetBits_ePlayerFlag(playerid)
{
	Bit_Vet(ePlayerFlag[e_bLogged], playerid);
	Bit_Vet(ePlayerFlag[e_bDoingCrouchBug], playerid);
	Bit_Vet(ePlayerFlag[e_bDoingBunnyHop], playerid);
	Bit_Vet(ePlayerFlag[e_bCacheLoaded], playerid);
	Bit_Vet(ePlayerFlag[e_bHoldingKey], playerid);
	Bit_Vet(ePlayerFlag[e_bKeyPress], playerid);
	Bit_Vet(ePlayerFlag[e_bIsCrafting], playerid);
	Bit_Vet(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
	Bit_Vet(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
	Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
	Bit_Vet(ePlayerFlag[e_bShootingCustomWeapon], playerid);
	Bit_Vet(ePlayerFlag[e_bShotNailbomb], playerid);

	print("ResetBits_ePlayerFlag");
}

//#region Status management

	PLAYER_STATUS:GetPlayerStatus(playerid)	return ePlayerInfo[playerid][e_iCurrentStatus];

	SetPlayerStatus(playerid, PLAYER_STATUS:status, statusIssuerId = INVALID_PLAYER_ID)
	{
		if(GetPlayerStatus(playerid) == status)
		{
			return;
		}

		if(IsPlayerDead(playerid) && status != PLAYER_STATUS_NONE)
		{
			return;
		}

		switch(status)
		{
			case PLAYER_STATUS_NONE, PLAYER_STATUS_DEAD:
			{
				if(IsPlayerAimingWeapon(playerid) || Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid))
				{
					OnPlayerStopAimingWeapon(playerid);
				}

				if(IsPlayerUsingBackpack(playerid))
				{
					new bool:successfullyClosed = HideBackpack(playerid, (statusIssuerId != INVALID_PLAYER_ID) ? (true) : (false));
					if(!successfullyClosed)
					{
						return;
					}

					OnPlayerCloseBackpack(playerid);
				}

				if(IsPlayerUsingWeaponMenu(playerid))
				{
					new bool:successfullyClosed = HideWeaponMenu(playerid);
					if(!successfullyClosed)
					{
						return;
					}

					OnPlayerCloseWeaponMenu(playerid);
				}

				if(IsPlayerBurning(playerid))
				{
					OnPlayerStopBurning(playerid);
				}

				if(Bit_Get(ePlayerFlag[e_bShotNailbomb], playerid))
				{
					Bit_Vet(ePlayerFlag[e_bShotNailbomb], playerid);
				}
			}

			case PLAYER_STATUS_FALL:
			{
				if(IsPlayerAimingWeapon(playerid) || Bit_Get(ePlayerFlag[e_bShootingCustomWeapon], playerid))
				{
					OnPlayerStopAimingWeapon(playerid);
				}

				if(IsPlayerUsingBackpack(playerid))
				{
					new bool:successfullyClosed = HideBackpack(playerid, (statusIssuerId != INVALID_PLAYER_ID) ? (true) : (false));
					if(!successfullyClosed)
					{
						return;
					}

					OnPlayerCloseBackpack(playerid);
				}

				if(IsPlayerUsingWeaponMenu(playerid))
				{
					new bool:successfullyClosed = HideWeaponMenu(playerid);
					if(!successfullyClosed)
					{
						return;
					}

					OnPlayerCloseWeaponMenu(playerid);
				}

				ApplyAnimation(playerid, "BASEBALL", "BAT_HIT_3", 4.1, 0, 1, 1, 1, 0);
			}

			case PLAYER_STATUS_USING_BACKPACK:
			{
				new bool:successfullyOpened = ShowBackpack(playerid);
				if(!successfullyOpened)
				{
					return;
				}

				ePlayerInfo[playerid][e_iCurrentStatus] = status;
				OnPlayerOpenBackpack(playerid, PlayerBackpackCategory(playerid));
				Disable_OnPlayerLookAt(playerid);
				return;
			}

			case PLAYER_STATUS_USING_WEAPMENU:
			{
				if(IsPlayerUsingBackpack(playerid))
				{
					return;
				}

				new bool:successfullyOpened = ShowWeaponMenu(playerid);
				if(!successfullyOpened)
				{
					return;
				}

				ePlayerInfo[playerid][e_iCurrentStatus] = status;
				OnPlayerOpenWeaponMenu(playerid);
				Disable_OnPlayerLookAt(playerid);
				return;
			}

			case PLAYER_STATUS_AIMING: {}
			case PLAYER_STATUS_PLAYING_ANIM: {}

			case PLAYER_STATUS_STUNNED:
			{
				if(IsPlayerBurning(playerid))
				{
					return;
				}

				CLF(#OnPlayerGetStunned, "d", playerid);
			}

			case PLAYER_STATUS_BURNING:
			{
				OnPlayerStartBurning(playerid, statusIssuerId);
			}
		}

		if(status != PLAYER_STATUS_NONE)
		{
			Disable_OnPlayerLookAt(playerid);
		}
		else
		{
			Enable_OnPlayerLookAt(playerid);
		}

		ePlayerInfo[playerid][e_iCurrentStatus] = status;
	}

//#endregion

//#region Health management utils

	// Get player health value.
	Float:GetPlayerHealthEx(playerid) return eCharsInfo[playerid][e_fHealth];

	// Set player health (avoid going over 100 health points or lower than 0.0).
	SetPlayerHealthEx(playerid, Float:health, killerid = INVALID_PLAYER_ID, itemId = -1, bodypart = -1)
	{
		if(playerid == INVALID_PLAYER_ID || IsPlayerDead(playerid))
		{
			return;
		}

		new Float:oldHealth = eCharsInfo[playerid][e_fHealth];
		new Float:newHealth = eCharsInfo[playerid][e_fHealth] = (health <= 0.0 ? 0.0 : health >= 100.0 ? 100.0 : health);

		OnPlayerHealthChange(playerid, oldHealth, newHealth);

		if(GetPlayerHealthEx(playerid) <= 0.0)
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_DEAD);
			OnPlayerDie(playerid, killerid, itemId, bodypart);
		}
	}

	// Changes player health bar length
	// (calculates its length so it doesn't go over its limit).
	AdjustPlayerHealthBar(playerid)
	{
		PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], pb_percent(528.9, 35.3, 100.0, GetPlayerHealthEx(playerid)), 0.0);
	}

	// (recursive) Function which makes player heal himself with a medkit.
	function HealMyself(playerid, Float:oldHealth, Float:destHealth)
	{
		if(!IsPlayerHealing(playerid))
		{
			return;
		}

		if(IsPlayerHoldingKey(playerid, KEY_FIRE))
		{
			OnPlayerIsHealing(playerid, oldHealth, destHealth);
		}
		else
		{
			OnPlayerCanceledHealing(playerid, oldHealth);
		}
	}

//#endregion

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
	IsPlayerCrouched(playerid)				return (GetPlayerSpecialAction(playerid) == SPECIAL_ACTION_DUCK);
	IsPlayerLookingAtPlayer(playerid)		return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && GetPlayerFocusedPlayerId(playerid) != INVALID_PLAYER_ID);
	IsPlayerLookingAtItem(playerid)			return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && GetPlayerFocusedObjectId(playerid) != INVALID_OBJECT_ID);
	IsPlayerAllowedToShiv(playerid)			return (IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && IsPlayerLookingAtPlayer(playerid));

	IsPlayerLookingTimerRunning(playerid)	return Bit_Get(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	IsPlayerLogged(playerid)				return Bit_Get(ePlayerFlag[e_bLogged], playerid);
	IsPlayerCacheLoaded(playerid)			return Bit_Get(ePlayerFlag[e_bCacheLoaded], playerid);
	IsPlayerBunnyHopping(playerid)			return Bit_Get(ePlayerFlag[e_bDoingBunnyHop], playerid);
	IsPlayerCrouchBugging(playerid)			return Bit_Get(ePlayerFlag[e_bDoingCrouchBug], playerid);
	IsPlayerHudFlashing(playerid)			return Bit_Get(ePlayerFlag[e_bIsHudBgFlashing], playerid);

	IsPlayerIdle(playerid)
	{
		return !IsPlayerAimingWeapon(playerid)
			&& !IsPlayerUsingBackpack(playerid) 
			&& !IsPlayerUsingWeaponMenu(playerid)
			&& !IsPlayerUnableToPerformAction(playerid);
	}

//#endregion

// Stops HideHud timer and starts it again.
RestartHideHudTimer(playerid)
{
	if(IsPlayerAimingWeapon(playerid))
	{
		return;
	}
    stop ePlayerInfo[playerid][e_iTimer_HideHud];
    ePlayerInfo[playerid][e_iTimer_HideHud] = defer HideHud(playerid);
}

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

	HideItem(playerid);
	ShowPlayerBlackScreen(playerid);
	wait_ms(1600);

	PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], CombatTips[random(sizeof(CombatTips))]);
	wait_ms(1800);

	PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1]);
}

// Hide death procedure textdraws.
HideDeathProcedureTds(playerid)
{
	PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0]);
	PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1]);
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