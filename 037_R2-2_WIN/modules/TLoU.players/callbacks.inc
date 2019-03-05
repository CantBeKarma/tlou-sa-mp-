#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//#region Main initiation callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//-------------------- OnPlayerConnect --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	hook OnPlayerConnect(playerid) 
	{
		if(IsPlayerNPC(playerid))
		{
			return 1;
		}

		wait_ms(300);

		Bit_Vet(ePlayerFlag[e_bLogged], playerid);
		SetCameraBehindPlayer(playerid);
		SpawnPlayer(playerid);
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------ OnPlayerDisconnect ------------------- 
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	hook OnPlayerDisconnect(playerid, reason)
	{
		if(IsPlayerNPC(playerid))
		{
			return 1;
		}
		
		SavePlayerData(playerid);
		
		ResetEnum_ePlayerInfo(playerid);
		ResetBits_ePlayerFlag(playerid);
		ResetEnum_eCharsInfo(playerid);
		ResetEnum_ePlayerBpInfo(playerid);
		ResetEnum_ePlayerAudio(playerid);
		ResetEnum_ePlayerWeapon(playerid);
		ResetEnum_ePlayerWeapMenu(playerid);
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- OnPlayerSpawn ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	hook OnPlayerSpawn(playerid) 
	{
		if(!IsPlayerLogged(playerid))
		{
			OnPlayerFirstSpawn(playerid);
			return 0;
		}

		OnPlayerRespawn(playerid);
		return 1;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerFirstSpawn ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerFirstSpawn(playerid)
{
	if(!IsUsingSAMPP(playerid))
	{
		new String:pName = str_new_buf(MAX_PLAYER_NAME);
		str_resize(pName, GetPlayerName_s(playerid, pName, MAX_PLAYER_NAME));

		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "Player %s [%d] connected (possibly?) without a tlou_client.asi file. Kicked.", _:pName, playerid);
		Kick(playerid);
		return 0;
	}

	OnPlayerPrepareToSign(playerid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerRespawn --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerRespawn(playerid)
{
	Bit_Vet(ePlayerFlag[e_bIsDead], playerid);

	SCMF(playerid, -1, "Check: /icmd createitem and dialog responses");
	SCMF(playerid, -1, "Check: /checkzdiff");
	SCMF(playerid, -1, "Check: Don't allow fake shooting when out of ammo (you need another player to test this!)");

	new Float:charHealth = GetPlayerHealthEx(playerid);
	SetPlayerHealthEx(playerid, (charHealth <= 0.0) ? (25.0) : (charHealth));

	UnfreezePlayer(playerid);
	Enable_OnPlayerLookAt(playerid);
	AttachBackpackToPlayer(playerid);

	HideDeathProcedureTds(playerid);

	wait_ms(150);
	FadePlayerScreen(playerid, FADE_OUT);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerDeathEx --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDeathEx(playerid, issuerid, weaponid, bodypart)
{
	if(issuerid == INVALID_PLAYER_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s [%d] has killed himself", \
				PlayerName(playerid), playerid);
	}
	else
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s [%d] has been killed by %s [%d] using weapon id: %d to body part: %d", \
				PlayerName(playerid), playerid, PlayerName(issuerid), issuerid, weaponid, bodypart);
	}

	FreezePlayer(playerid, true, -1);
	HidePlayerWeapon(playerid);
	Disable_OnPlayerLookAt(playerid);
	PlayDeathAnimation(playerid, bodypart);
	PlayDeathProcedureForPlayer(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerUpdate --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
	{
		return 0;
	}
	
    // Don't allow "fake shooting" when out of ammo.
	// Thanks to YSF and new function "SetPlayerSyncKeys" I am able to sync all the keys that player 
	// presses but KEY_FIRE. So that way people won't see "fake-shooting" when player is out of ammo for the weapon.
    if(IsPlayerAimingWeapon(playerid)
    && IsBulletWeapon(CurrentPlayerWeaponId[playerid]) && CurrentPlayerWeaponClipSize[playerid] <= 0)
    {
		new keys, updown, leftright;
		GetPlayerKeys(playerid, keys, updown, leftright);
		SetPlayerSyncKeys(playerid, keys & ~KEY_FIRE, updown, leftright);
    }

	// If player is bunny hopping, he doesn't see jumping animation when pressing
	// jump key but other player's do, so below code doesn't synchronize player jumping key.
	if(IsPlayerBunnyHopping(playerid))
	{
		new keys, updown, leftright;
		GetPlayerKeys(playerid, keys, updown, leftright);
		SetPlayerSyncKeys(playerid, keys & ~KEY_JUMP, updown, leftright);
	}
 
//#region walking when A/W/S/D | custom-walking
	// if(!IsPlayerUsingBackpack(playerid) && !IsPlayerUsingWeaponMenu(playerid) 
	// && !IsPlayerAimingWeapon(playerid)
	// && !IsPlayerUnableToPerformAction(playerid)
	// && GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_2IDLE 
	// && GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_IN
	// && GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_LOOP)
	// {
	// 	new keys, updown, leftright;
	// 	GetPlayerKeys(playerid, keys, updown, leftright);

	// 	new playerWeaponId = CurrentPlayerWeaponId[playerid];

	// 	if(updown & KEY_UP || updown & KEY_DOWN || leftright & KEY_LEFT || leftright & KEY_RIGHT)
	// 	{
	// 		if(keys & KEY_SPRINT)
	// 		{
	// 			if(playerWeaponId == WEAPON_SHOTGUN
	// 			|| playerWeaponId == WEAPON_ASSAULT
	// 			|| playerWeaponId == WEAPON_RIFLE)
	// 			{
	// 				if(GetPlayerAnimationIndex(playerid) != PED_RUN_ARMED)
	// 					ApplyAnimation(playerid, "PED", "RUN_ARMED", 4.1, 1, 1, 1, 1, 1);
	// 			}
	// 			else 
	// 			{
	// 				if(GetPlayerAnimationIndex(playerid) != PED_SPRINT_CIVI)
	// 					ApplyAnimation(playerid, "PED", "SPRINT_CIVI", 4.1, 1, 1, 1, 1, 1);
	// 			}
	// 		}
	// 		else
	// 		{
	// 			if(playerWeaponId == WEAPON_SHOTGUN
	// 			|| playerWeaponId == WEAPON_ASSAULT
	// 			|| playerWeaponId == WEAPON_RIFLE)
	// 			{
	// 				if(GetPlayerAnimationIndex(playerid) != PED_WALK_ARMED)
	// 					ApplyAnimation(playerid, "PED", "WALK_ARMED", 4.1, 1, 1, 1, 1, 1);
	// 			}
	// 			else
	// 			{
	// 				if(GetPlayerAnimationIndex(playerid) != PED_WALK_PLAYER)
	// 					ApplyAnimation(playerid, "PED", "WALK_PLAYER", 4.1, 1, 1, 1, 1, 1);
	// 			}
	// 		}
	// 	}
	// 	else
	// 	{
	// 		if(playerWeaponId == WEAPON_SHOTGUN
	// 		|| playerWeaponId == WEAPON_ASSAULT
	// 		|| playerWeaponId == WEAPON_RIFLE)
	// 		{
	// 			if(GetPlayerAnimationIndex(playerid) != PED_IDLE_ARMED)
	// 				ApplyAnimation(playerid, "PED", "IDLE_ARMED", 4.1, 1, 1, 1, 0, 0);
	// 		}
	// 		else 
	// 		{
	// 			if(GetPlayerAnimationIndex(playerid) != PED_IDLE_STANCE)
	// 				ApplyAnimation(playerid, "PED", "IDLE_STANCE", 4.1, 1, 1, 1, 0, 0);
	// 		}
	// 	}
	// }
//#endregion

	// new animLib[32], animName[32];
	// GetAnimationName(GetPlayerAnimationIndex(playerid), animLib, sizeof(animLib), animName, sizeof(animName));
	// SCMF(playerid, -1, "%s | %s | %d", animLib, animName, GetPlayerAnimationIndex(playerid));
	return 1;
}

//#region Health managment callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------- OnPlayerStartedHealing ----------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerStartedHealing(playerid)
	{
		KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
		Audio_PlayEx(playerid, AUDIO_BANDAGE_USE, .loop = true);

		new Float:oldHealth = GetPlayerHealthEx(playerid);
		new Float:destHealth = oldHealth + 20.0;

		Bit_Let(ePlayerFlag[e_bIsHealing], playerid);
		HealMyself(playerid, oldHealth, destHealth);
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_LOOP", 4.0, 1, 0, 0, 0, 0);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------ OnPlayerIsHealing --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerIsHealing(playerid, Float:oldHealth, Float:destHealth)
	{
		new Float:currentHealth = GetPlayerHealthEx(playerid);
		if(currentHealth >= 100.0 || currentHealth >= destHealth)
		{
			Bit_Vet(ePlayerFlag[e_bIsHealing], playerid);
			OnPlayerHealed(playerid, oldHealth, currentHealth);
		}
		else
		{
			new healSpeed = 
				(Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 0) ? (500) 
				: (Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 1) ? (300) 
				: (100);

			SetPlayerHealthEx(playerid, currentHealth + 2.0);

			SetTimerEx("HealMyself", healSpeed, false, "dff", playerid, oldHealth, destHealth);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------- OnPlayerCanceledHealing ----------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerCanceledHealing(playerid, Float:oldHealth)
	{
		ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);

		Audio_StopEx(playerid);

		SetPlayerHealthEx(playerid, oldHealth);
		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0, 1);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//-------------------- OnPlayerHealed ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerHealed(playerid, Float:oldHealth, Float:newHealth)
	{
		ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);

		Audio_StopEx(playerid);
		Audio_PlayEx(playerid, AUDIO_BANDAGE_FINISH);

		new newMedkitAmount = GetPlayerWeaponAmmo(playerid, WEAPON_MEDKIT)-1;
		SetPlayerWeaponAmmo(playerid, WEAPON_MEDKIT, newMedkitAmount);

		if(newMedkitAmount <= 0)
		{
			HidePlayerWeapon(playerid);
		}

		ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------- OnPlayerHealthChange ------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerHealthChange(playerid, Float:oldHealth, Float:newHealth)
	{
		AdjustPlayerHealthBar(playerid);
		ShowHud(playerid);
		
		if(newHealth <= 30.0)
		{
			// Flash HUD background if health is lower than 30.0.
			if(!IsPlayerHudFlashing(playerid))
			{
				Bit_Let(ePlayerFlag[e_bIsHudBgFlashing], playerid);
				HudTd_Bg_FlashRed(playerid, 0, 0.0);
			}
		}
		else
		{
			if(newHealth < oldHealth)
			{
				// Fade HUD background from RED to BLACK.
				HudTd_Bg_RedToBlack(playerid, 0.0);
			}
		}
		return 0;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerTargetPlayer ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTargetPlayer(playerid, newTargetId, oldTargetId)
{
	if(oldTargetId != INVALID_PLAYER_ID && newTargetId == INVALID_PLAYER_ID)
	{
		ShowPlayerCrosshair(playerid);
	}

	// If target player is in the same team that aimer is
	// hide player crosshair. Otherwise show it back.
	new targetTeamId = GetPlayerTeam(newTargetId);
	if(targetTeamId != FACTION_NONE && GetPlayerTeam(playerid) == targetTeamId)
	{
		HidePlayerCrosshair(playerid);
	}
	else
	{
		ShowPlayerCrosshair(playerid);
	}
	return 1;
}

//#region OnPlayerLook(...) callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------- OnPlayerLookAtPlayer ------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerLookAtPlayer(playerid, lookingAt_playerId)
	{
		if(GetPlayerShivs[playerid] <= 0
		|| IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			return 1;
		}
		
		new Float:x, Float:y, Float:z;
		GetPlayerPos(lookingAt_playerId, x, y, z);

		new 
			virtualWorld = GetPlayerVirtualWorld(playerid),
			interiorId = GetPlayerInterior(playerid);

		// If player is stunned - other players are allowed to shiv that player from any angle.
		if(IsPlayerStunned(lookingAt_playerId))
		{
			ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel("SHIV\n{FF0000}(~k~~PED_FIREWEAPON~)", \
																COLOR_WHITE_TRANS, x, y, z, SHIV_LABEL_DRAW_DIST, lookingAt_playerId, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);

			Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
			return 1;
		}

		// If player isn't stunned but the other player is behind him.
		GetXYBehindPlayer(lookingAt_playerId, x, y, -1.3);
		if(!IsPlayerInRangeOfPoint(playerid, 0.6, x, y, z))
		{
			return 1;
		}

		ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel("SHIV\n{FF0000}(~k~~PED_FIREWEAPON~)", \
															COLOR_WHITE_TRANS, x, y, z, SHIV_LABEL_DRAW_DIST, lookingAt_playerId, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);

		Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------- OnPlayerLookAtNPC -------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public OnPlayerLookAtNPC(playerid, lookingAt_NpcId)
	{
		if(GetPlayerShivs[playerid] <= 0 || FCNPC_GetType(lookingAt_NpcId) == NPC_TYPE:TYPE_BLOATER)
		{
			return 1;
		}

		// If NPC isn't in IDLE state, playing anims or stunned.
		if(!FCNPC_IsIdle(lookingAt_NpcId) && !FCNPC_IsPlayingAnim(lookingAt_NpcId) && !FCNPC_IsStunned(lookingAt_NpcId))
		{
			if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
			{
				DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
				Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
			}
			return 1;
		}

		if(IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			return 1;
		}

		new Float:x, Float:y, Float:z;
		GetPlayerPos(lookingAt_NpcId, x, y, z);

		new 
			virtualWorld = GetPlayerVirtualWorld(playerid),
			interiorId = GetPlayerInterior(playerid);

		// If NPC is stunned show the shiv label from any angle.
		if(FCNPC_IsStunned(lookingAt_NpcId))
		{
			ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel("SHIV\n{FF0000}(~k~~PED_FIREWEAPON~)", \
															COLOR_WHITE_TRANS, x, y, z, SHIV_LABEL_DRAW_DIST, lookingAt_NpcId, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);

			Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
			return 1;
		}

		GetXYBehindPlayer(lookingAt_NpcId, x, y, -1.3);
		if(!IsPlayerInRangeOfPoint(playerid, 0.6, x, y, z))
		{
			return 1;
		}

		ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel("SHIV\n{FF0000}(~k~~PED_FIREWEAPON~)", \
															COLOR_WHITE_TRANS, x, y, z, SHIV_LABEL_DRAW_DIST, lookingAt_NpcId, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);

		Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
		return 0;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerGetStunned -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetStunned(playerid)
{
    Disable_OnPlayerLookAt(playerid);
    //ApplyCallbackAnim(playerid, "PED", "GAS_CWR");

	ApplyAnimation(playerid, "PED", "GAS_CWR", 4.1, 1, 1, 1, 0, 0);
	wait_ms(3000);

	//ApplyCallbackAnim(playerid, "PED", "IDLE_TIRED");
	ApplyAnimation(playerid, "PED", "IDLE_TIRED", 4.1, 1, 1, 1, 0, 0);
	wait_ms(2000);

	ClearAnims(playerid);

	Enable_OnPlayerLookAt(playerid);
	Bit_Vet(ePlayerFlag[e_bIsStunned], playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerKeyStateChange ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
	{
		return 1;
	}

	if(newkeys & KEY_SPRINT)
	{
		// If textdraw "Press ... to continue" (death procedure) is displayed
		// and player has pressed his defined key - respawn that player.
		if(IsPlayerDead(playerid) 
		&& IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1]))
		{
			SpawnPlayer(playerid);
			return 0;
		}
	}
	
	if(!Bit_Get(ePlayerFlag[e_bHoldingKey], playerid)
	&& !IsPlayerAimingWeapon(playerid) && IsPlayerLogged(playerid)
	&& !IsPlayerDead(playerid) && !IsPlayerStunned(playerid))
	{
		Bit_Let(ePlayerFlag[e_bHoldingKey], playerid);
		ePlayerInfo[playerid][e_iTickCounter_holdingKey] = GetConnectedTime(playerid);
		ePlayerInfo[playerid][e_iTimer_holdingKey] = repeat Callback_HoldKeyTimer(playerid, newkeys);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPlayerDeath -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDeath(playerid, killerid, reason)
{
	if(IsPlayerNPC(playerid))
	{
		return 1;
	}

	if(killerid != INVALID_PLAYER_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerDeath callback has been called for: [%d]. Has been killed by: [%d] - VERIFY WHY!!!", playerid, killerid);
		return 1;
	}
			
	Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "OnPlayerDeath callback has been called for [%d]. VERIFY WHY!!!", playerid);
	return 1;
}