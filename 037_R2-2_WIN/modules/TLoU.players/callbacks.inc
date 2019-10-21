#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerRespawn --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerRespawn(playerid)
{
	SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);

	new Float:charHealth = GetPlayerHealthEx(playerid);
	SetPlayerHealthEx(playerid, (charHealth <= 0.0) ? (MIN_HEALTH_AFTER_DEATH) : (charHealth));

	AttachBackpackToPlayer(playerid);
	AttachItemsToPlayer(playerid);

	wait_ms(150);
	CLF(#FadePlayerScreen, "dd", playerid, FADE_OUT);
	UnfreezePlayer(playerid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnPlayerDie ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDie(playerid, issuerid, itemId, bodypart)
{
	if(issuerid == INVALID_PLAYER_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (UID: %d | GUID: %d) has killed himself", \
				PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid));

		if(itemId != _:ITEM_IDX_NAIL_BOMB)
		{
			itemId = WEAPON_COLLISION;
		}
	}
	else
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (UID: %d | GUID: %d) has been killed by %s (UID: %d | GUID: %d) using weapon id: %d to body part: %d", \
				PlayerName(playerid), GetPlayerCharUid(playerid), GetPlayerGuid(playerid), GetPlayerCharUid(issuerid), GetPlayerGuid(issuerid), itemId, bodypart);
	}

	FreezePlayer(playerid, true, -1);
	CreatePlayerPendant(playerid);
	CLF(#PlayDeathAnimation, "ddd", playerid, bodypart, itemId);
	CLF(#PlayDeathProcedureForPlayer, "d", playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerUpdate --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate@000(playerid)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
	{
		return 0;
	}
	
    // Don't allow "fake shooting" when out of ammo.
	// Thanks to YSF and new function "SetPlayerSyncKeys" I am able to sync all the keys that player 
	// presses but KEY_FIRE. So that way people won't see "fake-shooting" when player is out of ammo for the weapon.
    if(IsPlayerAimingWeapon(playerid)
	&& IsItemType(GetPlayerEquippedItemId(playerid), ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON)
	&& GetPlayerItemClipAmount(playerid, GetPlayerEquippedItemId(playerid)) <= 0)
    {
		DesynchronizePlayerKey(playerid, KEY_FIRE);
    }

	// If player is bunny hopping, he doesn't see jumping animation when pressing
	// jump key but other player's do, so below code doesn't synchronize player jumping key.
	if(IsPlayerBunnyHopping(playerid))
	{
		DesynchronizePlayerKey(playerid, KEY_JUMP);
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

	// 	new playerWeaponId = ...

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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerGetStunned -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetStunned(playerid)
{
	ApplyAnimation(playerid, "PED", "GAS_CWR", 4.1, 1, 1, 1, 0, 0);
	wait_ms(3000);

	if(GetPlayerAnimationIndex(playerid) == PED_GAS_CWR)
	{
		ApplyAnimation(playerid, "PED", "IDLE_TIRED", 4.1, 1, 1, 1, 0, 0);
		wait_ms(2000);

		SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerShakeOffStun ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShakeOffStun(playerid)
{
	ClearAnims(playerid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerGetHitByThrowable ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetHitByThrowable(playerid, issuerId, itemId)
{
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerGetArrowShot -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetArrowShot(playerid, damagerid)
{
	ShowPlayerHitMarker(damagerid, BODY_PART_TORSO);
	ApplyAnimation(playerid, "PED", "HIT_GUN_BUTT", 4.1, 0, 1, 1, 0, 350);
	Bit_Vet(ePlayerFlag[e_bShootingCustomWeapon], damagerid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------- OnPlayerGetExplodedByNailbomb ------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetExplodedByNailbomb(playerid, nailbombCreatorId)
{
    if(GetPlayerEquippedItemId(playerid) != -1)
    {
        if(!IsItemType(GetPlayerEquippedItemId(playerid), ITEM_TYPE_BULLET_WEAPON))
        {
            HideItem(playerid);
        }
        else
        {
            if(!HasPlayerShotNailbomb(playerid))
            {
                HideItem(playerid);
            }
        }
    }

    SetPlayerStatus(playerid, PLAYER_STATUS_FALL);
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - DAMAGE_AMOUNT_NAIL_BOMB, nailbombCreatorId, _:ITEM_IDX_NAIL_BOMB);

    if(!IsPlayerDead(playerid))
    {
        defer DelayedTimer_AnimGetUp[1000](playerid);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerLookAtPlayer ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerLookAtPlayer(playerid, lookingAt_playerId)
{
	if(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV) <= 0
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
	if(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV) <= 0 || FCNPC_GetType(lookingAt_NpcId) == TYPE_BLOATER)
	{
		return 1;
	}

	new Float:x, Float:y, Float:z;
	GetPlayerPos(lookingAt_NpcId, x, y, z);

	// If FCNPC is not stunned, it is required for player to be placed behind NPC.
	//GetXYBehindPlayer(lookingAt_NpcId, x, y, -1.1);

	if(!FCNPC_IsStunned(lookingAt_NpcId)
	&& !IsPlayerBehindPlayer(playerid, lookingAt_NpcId, 0.5))
	//&& !IsPlayerInRangeOfPoint(playerid, 0.5, x, y, z))
	{
		DestroyDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]);
	}
	else
	{
		if(!IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]))
		{
			new 
				virtualWorld = GetPlayerVirtualWorld(playerid),
				interiorId = GetPlayerInterior(playerid);

			ePlayerInfo[playerid][e_iLookingAt_Label] = CreateDynamic3DTextLabel("SHIV\n{FF0000}(~k~~PED_FIREWEAPON~)", COLOR_WHITE_TRANS, x, y, z, SHIV_LABEL_DRAW_DIST, lookingAt_NpcId, INVALID_VEHICLE_ID, 0, virtualWorld, interiorId, playerid);
		}
	}

	Streamer_Update(playerid, STREAMER_TYPE_3D_TEXT_LABEL);
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
			HideDeathProcedureTds(playerid);
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