#include	<YSI\y_hooks>

//#region Main initiation FCNPC callbacks


	hook OnPlayerEnterDynArea(playerid, areaid)
	{
		if(IsPlayerNPC(playerid))
		{
			FCNPC_OnEnterDynArea(playerid, areaid);
		}
		return 0;
	}

	hook OnPlayerLeaveDynArea(playerid, areaid)
	{
		if(IsPlayerNPC(playerid))
		{
			FCNPC_OnLeaveDynArea(playerid, areaid);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------------- FCNPC_OnCreate ------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnCreate(npcid)
	{
		//new NPC_TYPE:npcType = RandomEx(NPC_TYPE:RUNNER, NPC_TYPE:BLOATER);
		new bool:sucessfullySetType = bool:FCNPC_SetType(npcid, NPC_TYPE:TYPE_CLICKER);
		if(!sucessfullySetType)
		{
			Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "[CREATE]: Couldn't create NPC ID: %d", npcid);
			return 1;
		}

		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[CREATE]: Sucessfully created: NPC ID: %d | TYPE: %d", npcid, _:FCNPC_GetType(npcid));
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------------- FCNPC_OnDestroy -----------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnDestroy(npcid)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[DESTROY]: NPC ID: %d", npcid);
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------------ FCNPC_OnSpawn ------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnSpawn(npcid)
	{
		FCNPC_PrepareForSpawn(npcid, .firstSpawn = true);
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[SPAWN]: NPC ID: %d", npcid);
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------------- FCNPC_OnRespawn -----------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnRespawn(npcid)
	{
		FCNPC_PrepareForSpawn(npcid);
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESPAWN]: NPC ID: %d", npcid);
		return 1;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnDeathEx ------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnDeathEx(npcid, killerid, weaponid, bodypart)
	{
		DestroyNpcRoamArea(npcid);
		FCNPC_PlayDeathAnimation(npcid, weaponid, bodypart);
		defer FCNPC_DelayRespawn(npcid);

		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[DEATH]: NPC ID: %d | KILLER ID: %d | WEAPON ID: %d | BODYPART: %d", npcid, killerid, weaponid, bodypart);
		return 1;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- FCNPC_OnInfectedUpdate -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer FCNPC_OnInfectedUpdate[TIMER_FCNPC_UPDATE](npcid)
{
	switch(FCNPC_GetStatus(npcid))
	{
		case STATUS_IDLE:			FCNPC_OnIdle(npcid);
		case STATUS_PLAYING_ANIM:	FCNPC_OnPlayingAnim(npcid, eNpcInfo[npcid][e_iPlayingAnim_Index]);
		case STATUS_BURNING:		FCNPC_OnBurning(npcid);
		case STATUS_ATTACK:			FCNPC_OnAttackPlayer(npcid, eNpcInfo[npcid][e_iChaseId]);
	}
	return 0;
}

//#region Idle management callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------------ FCNPC_OnIdle -------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnIdle(npcid)
	{
		GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~IDLE: NPCID (%d)", npcid);

		FCNPC_PlayIdleActions(npcid);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//----------------------- FCNPC_OnRoam --------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnRoam(npcid, Float:target_x, Float:target_y, Float:target_z)
	{
		GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ROAM: NPCID (%d)", npcid);

		switch(random(2))
		{
			case 0: FCNPC_WalkTo(npcid, target_x, target_y, target_z);
			case 1:	FCNPC_RunTo(npcid, target_x, target_y, target_z);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------- FCNPC_OnReachDestination ------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnReachDestination(npcid)
	{
		if(FCNPC_IsRoaming(npcid))
		{
			FCNPC_ClearAnims(npcid);

			if(!IsPlayerInDynamicArea(npcid, eNpcInfo[npcid][e_iRoamAreaId]))
			{
				FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_ROAM, eNpcInfo[npcid][e_iRoamAreaCenterX], eNpcInfo[npcid][e_iRoamAreaCenterY]);
				return 0;
			}

			FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
			return 0;
		}

		return 1;
	}

//#endregion

//#region Animation callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- FCNPC_OnPlayingAnim ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnPlayingAnim(npcid, animIdx)
	{
		GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM PLAYING: NPCID (%d)", npcid);

		if(FCNPC_CheckIfAnimationFinished(npcid, animIdx))
		{
			FCNPC_OnAnimFinish(npcid, animIdx);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnAnimFinish ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnAnimFinish(npcid, animIdx)
	{
		GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM FINISH: NPCID (%d)", npcid);

		switch(animIdx)
		{
			// Knifed by other player.
			case KNIFE_KILL_KNIFE_PED_DAMAGE:
			{
				FCNPC_SetAnimation(npcid, KNIFE_KILL_KNIFE_PED_DIE, 4.1, 0, 1, 1, 1, 0);
			}

			// Shot in stomach by shotgun (flew back on impact).
			case BASEBALL_BAT_HIT_3:
			{
				FCNPC_SetAnimCallback(npcid, PED_GETUP);
			}

			case PED_FLEE_LKAROUND_01:
			{
				if(FCNPC_IsDistracted(npcid))
				{
					SCMF_ALL(-1, "I was distracted. Finished animation, so time to roam to the specific location");
					FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_ROAM, eNpcInfo[npcid][e_fDistractedX], eNpcInfo[npcid][e_fDistractedY]);
				}
			}

			default:
			{
				FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
			}
		}
		//printf("FCNPC_OnAnimFinish(%d, %d)", npcid, animIdx);
		return 1;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnTriggerNailbomb -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnTriggerNailbomb(npcid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
{
	Audio3D_Play(throwerid, AUDIO_NAILBMB_TRIGGER, x, y, z, 10.0);
	defer OnNailbombExplode(npcid, throwerid, nailBmbIdx, x, y, z);
    return 0;
}

//#region Damage management callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnTakeDamage ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnTakeDamage(npcid, issuerid, Float:amount, weaponid, bodypart)
	{
		if(FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_NONE)
		{
			return 0;
		}
		
		if(FCNPC_IsDeadEx(npcid))
		{
			FCNPC_PlayDeadAnimIfDead(npcid);
			return 0;
		}

		new playerItemId = GetPlayerEquippedItemId(issuerid);
		new issuerWeaponId = GetItemNativeWeaponId(playerItemId);
		new Float:distanceBetweenPlayers = 0.0;
		new Float:damageAmount = CalculateDamageAmount(issuerid, npcid, issuerWeaponId, distanceBetweenPlayers, bodypart);

		if(IsItemMeleeWeapon(playerItemId))
		{
			FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, damageAmount);
		}
		else if(IsItemBulletWeapon(playerItemId))
		{
			FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, damageAmount, distanceBetweenPlayers, bodypart);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------- FCNPC_OnGetHitByPlayer --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount)
	{
		// if(FCNPC_IsPlayingAnim(npcid))
		// {
		FCNPC_ClearAnims(npcid);
		//}
		FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - damageAmount, issuerid, issuerWeaponId);

		if(issuerWeaponId == WEAPON_UNARMED)
		{
			return 0;
		}
		
		// Doesn't hide weapon if we are hitting someone.
		RestartHideHudTimer(issuerid);
		DegradePlayerMeleeWeapon(issuerid);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//------------------ FCNPC_OnGetShotByPlayer --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount, Float:distanceBetweenPlayers, bodypart)
	{
		SCMF(issuerid, -1, "You shot FCNPC (%d) with weapon (%d). Damage done: %f to bodypart: %d on distance: %f", npcid, issuerWeaponId, damageAmount, bodypart, distanceBetweenPlayers);

		ShowPlayerHitMarker(issuerid, bodypart);

		if(bodypart == BODY_PART_HEAD)
		{
			if(FCNPC_GetType(npcid) == NPC_TYPE:TYPE_RUNNER
			|| FCNPC_GetType(npcid) == NPC_TYPE:TYPE_STALKER)
			{
				FCNPC_SetHealthEx(npcid, 0.0, issuerid, issuerWeaponId, bodypart);
				return 0;
			}
		}

		FCNPC_SetHealthEx(npcid, (FCNPC_GetHealthEx(npcid) - damageAmount), issuerid, issuerWeaponId, bodypart);
		FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId, distanceBetweenPlayers);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnGetShived ----------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnGetShived(npcid, shiverId)
	{
		EquipItem(shiverId, _:ITEM_IDX_SHIV);
		ApplyCallbackAnim(shiverId, "KNIFE", "KILL_KNIFE_PLAYER");

		FCNPC_SetHealthEx(npcid, 0.0, shiverId, WEAPON_SHIV);
	
		new Float:x, Float:y, Float:z;
		GetPlayerPos(shiverId, x, y, z);
		Audio3D_Play(shiverId, RandomEx(AUDIO_CLICKER_DEATH01, AUDIO_CLICKER_DEATH03), x, y, z, DIST3D_CLICKER_SHIVED, .downmix = true);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- FCNPC_OnGetArrowShot --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnGetArrowShot(npcid, damagerid)
	{
		ShowPlayerHitMarker(damagerid, BODY_PART_TORSO);
		FCNPC_PlayDamageAnimation(npcid, BODY_PART_TORSO);
		FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - PlayerWeaponBodyDamage[damagerid], damagerid, WEAPON_BOW);
		return 1;
	}

//#endregion

//#region Area management

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//-------------------- FCNPC_OnEnterDynArea ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnEnterDynArea(npcid, areaid)
	{
		if(FCNPC_IsDeadEx(npcid) || FCNPC_IsBurning(npcid))
		{
			return 1;
		}

		if(FCNPC_IsInFireArea(npcid, areaid))
		{
			FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_BURNING);
		}
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- FCNPC_OnLeaveDynArea --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnLeaveDynArea(npcid, areaid)
	{
		if(FCNPC_IsDeadEx(npcid) || FCNPC_IsBurning(npcid))
		{
			return 1;
		}

		if(areaid == eNpcInfo[npcid][e_iRoamAreaId])
		{
			SCMF_ALL(-1, "NPCID: %d left its roam area. Heading back!", npcid);
			FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_ROAM, eNpcInfo[npcid][e_iRoamAreaCenterX], eNpcInfo[npcid][e_iRoamAreaCenterY]);
		}
		return 0;
	}

//#endregion

//#region Burn management callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- FCNPC_OnStartBurning --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnStartBurning(npcid)
	{
		SPAO(npcid, OBJECT_MODEL_BURNING);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnBurning ------------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnBurning(npcid)
	{
		FCNPC_SetAnimation(npcid, PED_FLOOR_HIT_F, 4.1, 1, 1, 1, 0, 0);
		FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - FIRE_DAMAGE_AMOUNT, .weaponid = WEAPON_MOLOTOV);
		return 0;
	}

//#endregion

//#region Stunned states callbacks

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//---------------------- FCNPC_OnGetStunned ---------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnGetStunned(npcid)
	{
		FCNPC_SetAnimation(npcid, PED_GAS_CWR, 4.1, 1, 1, 1, 0, 0);
		wait_ms(3000);

		FCNPC_SetAnimation(npcid, PED_IDLE_TIRED, 4.1, 1, 1, 1, 0, 0);
		wait_ms(2000);

		FCNPC_OnShakeOffStun(npcid);
		return 0;
	}

	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	//--------------------- FCNPC_OnShakeOffStun --------------------
	//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	public FCNPC_OnShakeOffStun(npcid)
	{
		FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
		return 0;
	}

//#endregion

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnGetDistracted ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetDistracted(npcid, Float:x, Float:y, Float:z)
{
	DestroyNpcRoamArea(npcid);
	FCNPC_SetAnimCallback(npcid, PED_FLEE_LKAROUND_01);

	eNpcInfo[npcid][e_fDistractedX] = x;
	eNpcInfo[npcid][e_fDistractedY] = y;
	eNpcInfo[npcid][e_fDistractedZ] = z;
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnAttackPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnAttackPlayer(npcid, chaseid)
{
	if(GetDistanceBetweenPlayers(npcid, chaseid) > RANGE_NPC_ATTACK
	|| IsPlayerDead(chaseid))
	{
		FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
		return 1;
	}

	SetPlayerToFacePlayer(npcid, chaseid);
	FCNPC_SetAnimation(npcid, NpcAttackAnims[random(sizeof(NpcAttackAnims))], 4.1, 1, 0, 0, 0, 0);

	OnPlayerGetAttackedByNpc(chaseid, npcid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnUpdate -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnUpdate(npcid)
{
	if(FCNPC_IsDeadEx(npcid))
	{
		return 1;
	}

	foreach(new playerid : Player)
	{
		if(!IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]) 
		|| IsDynamicObjectMoving(ePlayerWeapon[playerid][e_iArrowObjectId]))
		{
			continue;
		}

		new Float:x, Float:y, Float:z;
		GetDynamicObjectPos(ePlayerWeapon[playerid][e_iArrowObjectId], x, y, z);

		if(!IsPlayerInRangeOfPoint(npcid, ARROW_RANGE_DAMAGE, x, y, z))
		{
			continue;
		}

		DestroyPlayerArrow(playerid);
		FCNPC_OnGetArrowShot(npcid, playerid);
		break;
	}
	return 1;
}

//#region FCNPC old movement code

	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// //--------------------- FCNPC_OnChasePlayer ---------------------
	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// public FCNPC_OnChasePlayer(npcid, chaseid)
	// {
	// 	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~NATIVE CHASE", 1000, 5);

	// 	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
	// 	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

	// 	new Float:npcPosZ;
	// 	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

	// 	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
	// 	{
	// 		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
	// 		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
	// 		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

	// 		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
	// 	}
	// 	return 1;
	// }

	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// //------------------ FCNPC_OnChasePlayerByPath ------------------
	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
	// public FCNPC_OnChasePlayerByPath(npcid, chaseid, pathid)
	// {
	// 	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~PATH CHASE", 1000, 5);

	// 	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
	// 	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

	// 	new Float:npcPosZ;
	// 	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

	// 	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
	// 	{
	// 		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
	// 		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
	// 		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

	// 		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
	// 	}
	// 	return 0;
	// }

	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// //----------------------- OnPathCalculated ----------------------
	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// public OnPathCalculated(routeid, success, Float:nodesX[], Float:nodesY[], Float:nodesZ[], nodesSize)
	// {
	// 	if(!success)
	// 	{
	// 		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't calculate path for NPCID: %d (tried to create %d nodes)", routeid, nodesSize);
	// 		return 1;
	// 	}

	// 	eNpcInfo[routeid][FCNPC_MovePathId] = FCNPC_CreateMovePath();
	// 	if(eNpcInfo[routeid][FCNPC_MovePathId] == FCNPC_INVALID_MOVEPATH_ID)
	// 	{
	// 		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't create Move Path for NPCID: %d", routeid);
	// 		return 1;
	// 	}

	// 	FCNPC_StopUpdateTimer(routeid);
	// //	eNpcInfo[routeid][e_iCurrentStatus] = NPC_STATUS_CHASING_BY_PATH;

	// 	new 
	// 		i,
	// 		Last_i = -1;

	// 	new 
	// 		Float:Last_A = -1,
	// 		Float:Last_Z = -1;

	// 	while(i < nodesSize)
	// 	{
	// 		if(i == 0)
	// 		{
	// 			Last_i = i;
	// 			Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
	// 			Last_Z = (nodesZ[i+1]-nodesZ[i]);
	// 			i++;
	// 		}
	// 		else if(i < nodesSize-1)
	// 		{
	// 			if(Last_A == floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i])) && CompareZ(Last_Z, (nodesZ[i+1]-nodesZ[i]), 0.3))
	// 			{
	// 				i++;
	// 			}
	// 			else
	// 			{
	// 				Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
	// 				Last_Z = (nodesZ[i+1]-nodesZ[i]);

	// 				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[i+1], nodesY[i+1], nodesZ[i+1]+1.0);
	// 				Last_i = i+1;
	// 				i++;
	// 			}
	// 		}
	// 		else
	// 		{
	// 			if(Last_i != nodesSize-1)
	// 			{
	// 				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[nodesSize-1], nodesY[nodesSize-1], nodesZ[nodesSize-1]+1.0);
	// 			}
	// 			i++;
	// 		}
	// 	}

	// 	FCNPC_GoByMovePath(routeid, eNpcInfo[routeid][FCNPC_MovePathId], .speed = NpcMovementSpeed[ eNpcInfo[routeid][e_iType] ]);
	// 	FCNPC_ApplyAnimation(routeid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	// 	return 1;
	// }

	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// //----------------- FCNPC_OnFinishMovePathPoint -----------------
	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// public FCNPC_OnFinishMovePathPoint(npcid, pathid, pointid)
	// {
	// 	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID)
	// 	{
	// 		FCNPC_DestroyMovePath(pathid);
	// 		FCNPC_StartUpdateTimer(npcid);
	// 		return 1;
	// 	}

	// 	new chaseId = eNpcInfo[npcid][e_iChasedId];
	// 	FCNPC_OnChasePlayerByPath(npcid, chaseId, pathid);

	// 	// If FCNPC got close enough to player - use native chase and destroy the path.
	// 	new Float:distanceToPlayer = GetDistanceBetweenPlayers(npcid, chaseId);
	// 	if(distanceToPlayer <= RANGE_NPC_CHASE)
	// 	{
	// 		FCNPC_ChasePlayer(npcid, chaseId);
	// 		return 0;
	// 	}

	// 	// If NPC is chasing player by path but player
	// 	// has moved more than 15.0 meters from last node in path
	// 	// destroy old path for NPC and create a new one to move by.
	// 	new Float:x, Float:y, Float:z, 
	// 		lastNode = FCNPC_GetNumberMovePoint(pathid) - 1;

	// 	FCNPC_GetMovePoint(pathid, lastNode, x, y, z);

	// 	if(GetPlayerDistanceFromPoint(chaseId, x, y, z) > 10.0)
	// 	{
	// 		SCMF(chaseId, -1, "You tried to run away. Creating new path...");
	// 		FCNPC_DestroyMovePath(pathid);
	// 		FCNPC_ChasePlayerByPath(npcid, chaseId);
	// 	}
	// 	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	// 	return 0;
	// }

	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// //------------------- FCNPC_OnFinishMovePath --------------------
	// //=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
	// public FCNPC_OnFinishMovePath(npcid, pathid)
	// {
	// 	FCNPC_DestroyMovePath(pathid);
	// 	FCNPC_StartUpdateTimer(npcid);	
	// 	return 0;
	// }

//#endregion