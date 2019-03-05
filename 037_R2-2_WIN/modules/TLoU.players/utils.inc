#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

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
	Bit_Vet(ePlayerFlag[e_bAiming], playerid);
	Bit_Vet(ePlayerFlag[e_bAnimPlaying], playerid);
	Bit_Vet(ePlayerFlag[e_bCacheLoaded], playerid);
	Bit_Vet(ePlayerFlag[e_bHoldingKey], playerid);
	Bit_Vet(ePlayerFlag[e_bKeyPress], playerid);
	Bit_Vet(ePlayerFlag[e_bIsCrafting], playerid);
	Bit_Vet(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
	Bit_Vet(ePlayerFlag[e_bLookingAtTimerRunning], playerid);
	Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
	Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
	Bit_Vet(ePlayerFlag[e_bIsHealing], playerid);
	Bit_Vet(ePlayerFlag[e_bIsBurning], playerid);
	Bit_Vet(ePlayerFlag[e_bIsStunned], playerid);

	print("ResetBits_ePlayerFlag");
}

// Get player health value.
Float:GetPlayerHealthEx(playerid)		return eCharsInfo[playerid][e_fHealth];

// Set player health (avoid going over 100 health points or lower than 0.0).
SetPlayerHealthEx(playerid, Float:health, killerid = INVALID_PLAYER_ID, weaponid = -1, bodypart = -1)
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
		OnPlayerDeathEx(playerid, killerid, weaponid, bodypart);
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
		return;

	if(IsPlayerHoldingKey(playerid, KEY_FIRE))
	{
		new Float:currentHealth = GetPlayerHealthEx(playerid);
		if(currentHealth >= 100.0 || currentHealth >= destHealth)
		{
			Bit_Vet(ePlayerFlag[e_bIsHealing], playerid);
			OnPlayerHealed(playerid, oldHealth, currentHealth);
		}
		else
		{
			new healSpeed = (Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 0) ? (500) : (Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 1) ? (300) : (100);
			SetPlayerHealthEx(playerid, currentHealth + 2.0);

			SetTimerEx("HealMyself", healSpeed, false, "dff", playerid, oldHealth, destHealth);
		}
	}
	else
	{
		Bit_Vet(ePlayerFlag[e_bIsHealing], playerid);
		OnPlayerCanceledHealing(playerid, oldHealth);
	}
}

//#region Utils

	IsPlayerDead(playerid)					return (GetPlayerHealthEx(playerid) <= 0.0);
	IsPlayerStunned(playerid)				return Bit_Get(ePlayerFlag[e_bIsStunned], playerid);
	IsPlayerHealing(playerid)				return Bit_Get(ePlayerFlag[e_bIsHealing], playerid);
	IsPlayerLookingAtPlayer(playerid)		return GetPlayerFocusedPlayerId(playerid) != INVALID_PLAYER_ID;
	IsPlayerLookingAtItem(playerid)			return GetPlayerFocusedObjectId(playerid) != INVALID_OBJECT_ID;
	IsPlayerAimingWeapon(playerid)			return Bit_Get(ePlayerFlag[e_bAiming], playerid);
	IsPlayerLookingTimerRunning(playerid)	return Bit_Get(ePlayerFlag[e_bLookingAtTimerRunning], playerid);

//#endregion

// Frezee player with timer to unfreeze (optional).
function FreezePlayer(playerid, bool:camMove, time)
{
	if(camMove) TogglePlayerFrozen(playerid, true);
	else TogglePlayerControllable(playerid, false);

	if(time > 0) SetTimerEx("FreezePlayer", 1000, false, "ddd", playerid, camMove, time-1);
	else if(time == 0) UnfreezePlayer(playerid);
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

// Stuns player.
StunPlayer(playerid)
{
	if(IsPlayerStunned(playerid))
	{
		return false;
	}

	Bit_Let(ePlayerFlag[e_bIsStunned], playerid);
	OnPlayerGetStunned(playerid);
	return true;
}

// Enable OnPlayerLookAt callback timer if it isn't running.
Enable_OnPlayerLookAt(playerid)
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
PlayDeathProcedureForPlayer(playerid)
{
	Audio_PlayEx(playerid, AUDIO_DEATH_SFX);
	wait_ms(1200);

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

// Plays death animation for player (based on the current position e.g.: in vehicle or on foot).
PlayDeathAnimation(playerid, bodypart)
{
	if(!IsPlayerInAnyVehicle(playerid))
	{
		switch(bodypart)
		{
			case BODY_PART_HEAD: ApplyAnimation(playerid, "PED", "KO_SHOT_FACE", 4.1, 0, 1, 1, 1, 0, 1);
			default:
			{
				// If player wasn't killed stealthly or 
				// shot in stomach by shotgun (Flew backwards)
				if(GetPlayerAnimationIndex(playerid) != BASEBALL_BAT_HIT_3 
				&& GetPlayerAnimationIndex(playerid) != KNIFE_KILL_KNIFE_PED_DIE)
				{
					ApplyAnimation(playerid, "PED", "KO_SHOT_STOM", 4.1, 0, 1, 1, 1, 0, 1);
				}
			}
		}
	}
	else
	{
		new 
			vehicleid = GetPlayerVehicleID(playerid),
			modelid = GetVehicleModel(vehicleid);

		switch(modelid)
		{
			case 448, 461, 462, 463, 468, 471, 481, 509, 510, 521, 522, 523, 581, 586: 
			{
				new Float:x, Float:y, Float:z;
				GetVehiclePos(vehicleid, x, y, z);
				RemovePlayerFromVehicle(playerid);
				SetPlayerPos(playerid, x, y, z);
				ApplyAnimation(playerid, "PED", "BIKE_FALL_OFF", 4.0, 0, 1, 1, 0, 0, 1);
			}

			default:
				ApplyAnimation(playerid, "PED", "CAR_DEAD_LHS", 4.0, 0, 1, 1, 1, 0, 1);
		}
	}
}

// Force change the old holding key to the specified key definition.
ForceChangeHoldingKeyDef(playerid, key)
{
	if(Bit_Get(ePlayerFlag[e_bHoldingKey], playerid))
	{
		stop ePlayerInfo[playerid][e_iTimer_holdingKey];
		ePlayerInfo[playerid][e_iTickCounter_holdingKey] = GetConnectedTime(playerid);
		ePlayerInfo[playerid][e_iTimer_holdingKey] = repeat Callback_HoldKeyTimer(playerid, key);
	}
}