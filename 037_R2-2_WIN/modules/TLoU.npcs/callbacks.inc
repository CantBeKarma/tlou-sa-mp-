//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- FCNPC_OnCreate ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnCreate(npcid)
{
	new 
		skinid = 0,
		npcType = eNpcInfo[npcid][e_iType] = NPC_TYPE_CLICKER;//random(4);

	switch(npcType)
	{
		case NPC_TYPE_RUNNER:	skinid = NpcSkins[random(7)];
		case NPC_TYPE_STALKER:	skinid = NpcSkins[random(6) + 7];
		case NPC_TYPE_CLICKER:	skinid = NpcSkins[13];
		case NPC_TYPE_BLOATER:  skinid = NpcSkins[14];
	}

	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[CREATE]: NPC ID: %d | TYPE: %d", npcid, npcType);

	if(skinid != 0)
		FCNPC_Spawn(npcid, skinid, 2510.5881, -1675.1179, 13.5457);
	else
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "[CREATE]: NPC ID: %d | TYPE: %d | Couldn't find skin model for the NPC!", npcid, npcType);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- FCNPC_OnDestroy --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnDestroy(npcid)
{
	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[DESTROY]: NPC ID: %d", npcid);

    ResetEnum_eNpcInfo(npcid);
	ResetEnum_eNpcFlag(npcid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnSpawn ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnSpawn(npcid)
{
	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[SPAWN]: NPC ID: %d", npcid);

	PreloadAnimations(npcid);
	FCNPC_PrepareForSpawn(npcid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- FCNPC_OnRespawn -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnRespawn(npcid)
{
	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[RESPAWN]: NPC ID: %d", npcid);

	FCNPC_PrepareForSpawn(npcid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnDeath ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnDeathEx(npcid, killerid, weaponid, bodypart)
{
	FCNPC_StopUpdateTimer(npcid);
	FCNPC_SetHealthEx(npcid, 0.0);

	if(weaponid != WEAPON_MOLOTOV)
	{
		switch(bodypart)
		{
			case BODY_PART_HEAD: FCNPC_SetAnimation(npcid, PED_KO_SHOT_FACE, 4.1, 0, 1, 1, 1, 0);
			default:
			{
				if(FCNPC_GetAnimationIndex(npcid) != BASEBALL_BAT_HIT_3 
				&& FCNPC_GetAnimationIndex(npcid) != KNIFE_KILL_KNIFE_PED_DAMAGE)
				{
					FCNPC_SetAnimation(npcid, PED_KO_SHOT_STOM, 4.1, 0, 1, 1, 1, 0);
				}
			}
		}
	}

	defer FCNPC_RespawnDelay(npcid);

	Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "[DEATH]: NPC ID: %d | KILLER ID: %d | WEAPON ID: %d | BODYPART: %d", npcid, killerid, weaponid, bodypart);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- FCNPC_OnInfectedUpdate -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
timer FCNPC_OnInfectedUpdate[TIMER_FCNPC_UPDATE](npcid)
{
	if(eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_DISTRACTED)
	{
		return 1;
	}

	foreach(new chaseid : Player)
	{
		if(!Bit_Get(ePlayerFlag[e_bLogged], chaseid) || IsPlayerDead(chaseid)
		|| IsPlayerCrouched(chaseid) && eNpcInfo[npcid][e_iCurrentStatus] != NPC_STATUS_CHASING)
		{
			continue;
		}

		// If NPC is in the IDLE state and player is close enough to chase him
		// but also player is above or below NPC on certain distance
		// NPC ignores that player.
		new Float:distanceToPlayer = GetDistanceBetweenPlayers(npcid, chaseid);
		if(eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_IDLE)
		{
			if(distanceToPlayer <= RANGE_NPC_CHASE || (distanceToPlayer <= RANGE_NPC_CHASE_BY_PATH && distanceToPlayer > RANGE_NPC_CHASE))
			{
				new Float:chasePosZ;
				GetPlayerPos(chaseid, chasePosZ, chasePosZ, chasePosZ);

				new Float:npcPosZ;
				FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

				if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
				{
					continue;
				}
			}
		}

		if(distanceToPlayer <= RANGE_NPC_ATTACK)
		{
			FCNPC_AttackPlayer(npcid, chaseid);
		}
		else if(distanceToPlayer <= RANGE_NPC_CHASE)
		{
			FCNPC_ChasePlayer(npcid, chaseid);
		}
		else if(distanceToPlayer <= RANGE_NPC_CHASE_BY_PATH && distanceToPlayer > RANGE_NPC_CHASE)
		{
			FCNPC_ChasePlayerByPath(npcid, chaseid);
		}
		else
		{
			FCNPC_OnIdle(npcid);
		}
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnIdle -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnIdle(npcid)
{
	if(eNpcInfo[npcid][e_iCurrentStatus] != NPC_STATUS_IDLE)
	{
		eNpcInfo[npcid][e_iCurrentStatus] = NPC_STATUS_IDLE;
	}

	if(eNpcInfo[npcid][e_iChasedId] != INVALID_PLAYER_ID)
	{
		eNpcInfo[npcid][e_iChasedId] = INVALID_PLAYER_ID;
	}
	
	if(FCNPC_IsMoving(npcid))
	{
		FCNPC_Stop(npcid);
		FCNPC_ClearAnims(npcid);
	}

	if(!Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
	{
		FCNPC_SetAnimCallback(npcid, NpcIdleAnims[random(sizeof(NpcIdleAnims))]);
	}

	//FCNPC_SetAnimationByName(npcid, "TLoU:INF_WALK", 4.1, 1, 1, 1, 1, 1);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnChasePlayer ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnChasePlayer(npcid, chaseid)
{
	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~NATIVE CHASE", 1000, 5);

	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

	new Float:npcPosZ;
	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
	{
		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ FCNPC_OnChasePlayerByPath ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
public FCNPC_OnChasePlayerByPath(npcid, chaseid, pathid)
{
	GameTextForPlayer(chaseid, "~n~~n~~n~~n~~n~PATH CHASE", 1000, 5);

	new Float:chasePosX, Float:chasePosY, Float:chasePosZ;
	GetPlayerPos(chaseid, chasePosX, chasePosY, chasePosZ);

	new Float:npcPosZ;
	FCNPC_GetPosition(npcid, npcPosZ, npcPosZ, npcPosZ);

	if(!CompareZ(chasePosZ, npcPosZ, RANGE_Z_DIFF_CHASE) || IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
	{
		SCMF_ALL(-1, "FCNPC_OnChasePlayer: Higher or lower than a player is! Distracting...");
		GetXYBehindPlayer(chaseid, chasePosX, chasePosY, -5.0);
		CA_FindZ_For2DCoord(chasePosX, chasePosY, chasePosZ);

		FCNPC_Distract(npcid, chasePosX, chasePosY, chasePosZ, true);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnAttackPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnAttackPlayer(npcid, chaseid, Float:chaseId_oldHealth, Float:chaseId_newHealth)
{
	// If player run away from NPC...
	if(GetDistanceBetweenPlayers(npcid, chaseid) > RANGE_NPC_ATTACK)
	{
		FCNPC_StartUpdateTimer(npcid);
		return 1;
	}

	//SetPlayerHealthEx(chaseid, chaseId_newHealth);

	// Anims.
	if(IsPlayerUsingAnim(chaseid, ANIM_COLLECTION_CLIMB))
		return 1;

	if(IsBulletWeapon(CurrentPlayerWeaponId[chaseid]))
	{
		switch(random(2))
		{
			case 0: ApplyCallbackAnim(chaseid, "PED", "HIT_BACK");
			case 1: ApplyCallbackAnim(chaseid, "PED", "HIT_BEHIND");
		}
	}
	else
	{
		if(random(2) == 1)
		{
			switch(random(2))
			{
				case 0: ApplyCallbackAnim(chaseid, "PED", "HIT_BACK");
				case 1: ApplyCallbackAnim(chaseid, "PED", "HIT_BEHIND");
			}
		}
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnGetDistracted ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetDistracted(npcid, Float:distractPointX, Float:distractPointY, Float:distractPointZ)
{
	eNpcInfo[npcid][e_fDistractedX] = distractPointX;
	eNpcInfo[npcid][e_fDistractedY] = distractPointY;
	eNpcInfo[npcid][e_fDistractedZ] = distractPointZ;
	FCNPC_SetAnimCallback(npcid, PED_FLEE_LKAROUND_01);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnTakeDamage ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnTakeDamage(npcid, damagerid, weaponid, bodypart, Float:health_loss)
{
	if(FCNPC_IsDeadEx(npcid))
	{
		switch(FCNPC_GetAnimationIndex(npcid))
		{
			case 
				BASEBALL_BAT_HIT_3, 
				PED_KO_SHOT_FACE, 
				KNIFE_KILL_KNIFE_PED_DIE:
				{
					FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0);
				}

			default: FCNPC_ApplyAnimation(npcid, "PED", "FLOOR_HIT_F", 4.1, 0, 1, 1, 1, 0);
		}
		return 0;
	}
	
	new 
        Float:damageAmount = 0.0,
		damagerWeaponId = CurrentPlayerWeaponId[damagerid];

    if(IsMeleeWeapon(damagerWeaponId))
    {
        damageAmount = PlayerWeaponBodyDamage[damagerid];
        FCNPC_OnGetHitByPlayer(npcid, damagerid, damagerWeaponId, damageAmount);
    }
	else if(IsBulletWeapon(damagerWeaponId))
	{
		damageAmount = (bodypart == BODY_PART_HEAD) ? (PlayerWeaponHeadDamage[damagerid]) : (PlayerWeaponBodyDamage[damagerid]);

		new	Float:distance = GetDistanceBetweenPlayers(damagerid, npcid);
		if(distance > REDUCE_DAMAGE_AMOUNT_DISTANCE && damagerWeaponId != WEAPON_RIFLE)
			damageAmount = (damageAmount / 2);

		FCNPC_OnGetShotByPlayer(npcid, damagerid, damagerWeaponId, damageAmount, distance, bodypart);
	}

	if(FCNPC_IsDeadEx(npcid))
	{
		FCNPC_OnDeathEx(npcid, damagerid, weaponid, bodypart);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnGetHitByPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetHitByPlayer(npcid, damagerId, damagerWeaponId, Float:damageAmount)
{
	if(damagerWeaponId != WEAPON_UNARMED)
	{
		// Doesn't hide weapon if we are hitting someone.
		KillTimer(ePlayerInfo[damagerId][e_iTimer_HideHud]);
		ePlayerInfo[damagerId][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", damagerId);

		// Durability logic (reduce upgraded durability or basic one?)
		new newMeleeDurability = (IsPlayerMeleeUpgraded[damagerId]) ? (GetPlayerMeleeExtCnd[damagerId] - 1) : (GetPlayerMeleeCnd[damagerId] - 1);
		HudTd_WeakenMeleeWeapon(damagerId, damagerWeaponId, newMeleeDurability);

		// Removes melee weapon if broke.
		if(newMeleeDurability <= 0)
		{
			if(!IsPlayerMeleeUpgraded[damagerId]) RemovePlayerWeapon(damagerId, damagerWeaponId);
			else
			{
				HudTd_HideMeleeUpgradedBars(damagerId, damagerWeaponId);
				IsPlayerMeleeUpgraded[damagerId] = false;

				if(damagerWeaponId == WEAPON_SCISSOR_PIPE)
				{
					GetPlayerMelee[damagerId] = WEAPON_PIPE;
					SetPlayerWeapon(damagerId, GetPlayerMelee[damagerId], GetPlayerMeleeCnd[damagerId]);
				}
			}
		}
	}

    // Reduced damaged npc health.
	FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - damageAmount);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ FCNPC_OnGetShotByPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetShotByPlayer(npcid, damagerId, damagerWeaponId, Float:damageAmount, Float:distance, bodypart)
{
	SCMF(damagerId, -1, "You shot FCNPC (%d) with weapon (%d). Damage done: %f to bodypart: %d on distance: %f", npcid, damagerWeaponId, damageAmount, bodypart, distance);
	FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - damageAmount);
	ShowPlayerHitMarker(damagerId, bodypart);

	if((GetTickCount() - eNpcInfo[npcid][e_iTickCounter_damageAnim]) >= TICK_RATE_DAM_ANIM)
	{
		FCNPC_ResetAnimation(npcid);
		switch(bodypart)
		{
			case BODY_PART_LEFT_ARM:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3)], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_RIGHT_ARM: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 3], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_LEFT_LEG:  FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 6], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_RIGHT_LEG: FCNPC_SetAnimation(npcid, NpcDamageAnims[random(3) + 9], 4.1, 0, 1, 1, 0, 0);
			case BODY_PART_TORSO:
			{
				if(damagerWeaponId == WEAPON_SHOTGUN && distance <= SHOTGUN_TORSO_SHOT_DISTANCE) FlyBackwardsWhenShot(npcid);
				else FCNPC_SetAnimation(npcid, NpcDamageAnims[random(4) + 12], 4.1, 0, 1, 1, 0, 0);
			}

			case BODY_PART_HEAD:
			{
				if(eNpcInfo[npcid][e_iType] == NPC_TYPE_RUNNER || eNpcInfo[npcid][e_iType] == NPC_TYPE_STALKER) 
					FCNPC_SetHealthEx(npcid, 0.0);
			}
		}
	}
	eNpcInfo[npcid][e_iTickCounter_damageAnim] = GetTickCount();
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnGetShived ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetShived(npcid, shiverId)
{
	FCNPC_OnDeathEx(npcid, shiverId, WEAPON_SHIV, BODY_PART_TORSO);

	new Float:x, Float:y, Float:z;
	GetPlayerPos(shiverId, x, y, z);

	// Clicker's death sound effect.
	//new audioIdx = Audio3D_Stream(shiverId, SFX_ClickerShivDeaths[random(sizeof(SFX_ClickerShivDeaths))], x, y, z, DIST3D_CLICKER_SHIVED, .downmix = true);
	Audio3D_Play(shiverId, RandomEx(AUDIO_CLICKER_DEATH01, AUDIO_CLICKER_DEATH03), x, y, z, DIST3D_CLICKER_SHIVED, .downmix = true);

	HidePlayerWeapon(shiverId);
	FCNPC_SetAnimCallback(npcid, KNIFE_KILL_KNIFE_PED_DAMAGE);
	ApplyCallbackAnim(shiverId, "KNIFE", "KILL_KNIFE_PLAYER");
	SPAO(shiverId, ITEM_MODEL_SHIV);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnGetArrowShot --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetArrowShot(npcid, damagerid)
{
	ShowPlayerHitMarker(damagerid, BODY_PART_TORSO);
	FCNPC_ApplyAnimation(npcid, "PED", "HIT_GUN_BUTT", 4.1, 0, 1, 1, 0, 350);
	FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - PlayerWeaponBodyDamage[damagerid]);

	if(FCNPC_IsDeadEx(npcid))
	{
		FCNPC_OnDeathEx(npcid, damagerid, WEAPON_BOW, BODY_PART_TORSO);
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnBurn -------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnBurn(npcid)
{
	if(eNpcInfo[npcid][e_iType] == NPC_TYPE_BLOATER) { }
	else
	{
		FCNPC_SetAnimation(npcid, PED_KO_SHOT_FRONT, 4.1, 0, 1, 1, 1, 0);
		FCNPC_OnDeathEx(npcid, INVALID_PLAYER_ID, WEAPON_MOLOTOV, BODY_PART_TORSO);
	}
	SPAO(npcid, OBJECT_MODEL_BURNING);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnAnimFinish ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnAnimFinish(npcid, animIdx)
{
	switch(animIdx)
	{
		case KNIFE_KILL_KNIFE_PED_DAMAGE: FCNPC_SetAnimation(npcid, KNIFE_KILL_KNIFE_PED_DIE, 4.1, 0, 1, 1, 1, 0);
		//case PED_GAS_CWR: FCNPC_SetAnimCallback(npcid, PED_IDLE_TIRED);
		
		case 
			PED_GETUP://, 
			//PED_IDLE_TIRED:
			{
				FCNPC_StartUpdateTimer(npcid);
			}

		case PED_FLEE_LKAROUND_01:
		{
			if(eNpcInfo[npcid][e_iCurrentStatus] == NPC_STATUS_DISTRACTED)
			{
				FCNPC_GoTo(npcid, eNpcInfo[npcid][e_fDistractedX], eNpcInfo[npcid][e_fDistractedY], eNpcInfo[npcid][e_fDistractedZ]+1.0);
				FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
			}
		}
	}
	//printf("FCNPC_OnAnimFinish(%d, %d)", npcid, animIdx);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnReachDestination ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnReachDestination(npcid)
{
	switch(eNpcInfo[npcid][e_iCurrentStatus])
	{
		case NPC_STATUS_DISTRACTED:
		{
			FCNPC_ClearAnims(npcid);
			FCNPC_StartUpdateTimer(npcid);
		}
	}
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------------ FCNPC_OnUpdate -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnUpdate(npcid)
{
	// Checks if NPCs animation has finished.
	if(Bit_Get(eNpcFlag[e_bAnimPlaying], npcid))
	{
		eNpcInfo[npcid][e_iAnimTick] += 1;
		if(FCNPC_GetAnimationIndex(npcid) != eNpcInfo[npcid][e_iAnimIdx] || eNpcInfo[npcid][e_iAnimTick] >= 0)
		{
			Bit_Vet(eNpcFlag[e_bAnimPlaying], npcid);
			FCNPC_OnAnimFinish(npcid, eNpcInfo[npcid][e_iAnimIdx]);
		}
	}

	// Detects if NPC got into fire area.
	foreach(new i : ServerFires)
	{
		if(IsPlayerInRangeOfPoint(npcid, FIRE_RANGE + 1.5, eFireData[i][e_fFirePos_X], eFireData[i][e_fFirePos_Y], eFireData[i][e_fFirePos_Z]))
		{
			FCNPC_SetOnFire(npcid);
		}
	}

	// Detects if NPC got shot by player's arrow.
	foreach(new playerid : Player)
	{
		if(IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]) && IsDynamicObjectMoving(ePlayerWeapon[playerid][e_iArrowObjectId]))
		{
			new Float:x, Float:y, Float:z;
			GetDynamicObjectPos(ePlayerWeapon[playerid][e_iArrowObjectId], x, y, z);

			if(IsPlayerInRangeOfPoint(npcid, ARROW_RANGE_DAMAGE, x, y, z))
			{
				if(!FCNPC_IsDeadEx(npcid))
				{
					DestroyDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]);
					FCNPC_OnGetArrowShot(npcid, playerid);
					break;
				}
			}
		}
	}
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPathCalculated ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPathCalculated(routeid, success, Float:nodesX[], Float:nodesY[], Float:nodesZ[], nodesSize)
{
	if(!success)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't calculate path for NPCID: %d (tried to create %d nodes)", routeid, nodesSize);
		return 1;
	}

	eNpcInfo[routeid][FCNPC_MovePathId] = FCNPC_CreateMovePath();
	if(eNpcInfo[routeid][FCNPC_MovePathId] == FCNPC_INVALID_MOVEPATH_ID)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_NPC], ERROR, "Couldn't create Move Path for NPCID: %d", routeid);
		return 1;
	}

	FCNPC_StopUpdateTimer(routeid);
	eNpcInfo[routeid][e_iCurrentStatus] = NPC_STATUS_CHASING_BY_PATH;

	new 
		i,
		Last_i = -1;

	new 
		Float:Last_A = -1,
		Float:Last_Z = -1;

	while(i < nodesSize)
	{
		if(i == 0)
		{
			Last_i = i;
			Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
			Last_Z = (nodesZ[i+1]-nodesZ[i]);
			i++;
		}
		else if(i < nodesSize-1)
		{
			if(Last_A == floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i])) && CompareZ(Last_Z, (nodesZ[i+1]-nodesZ[i]), 0.3))
			{
				i++;
			}
			else
			{
				Last_A = floatabs(270.0 - atan2(nodesX[i+1]-nodesX[i], nodesY[i+1]-nodesY[i]));
				Last_Z = (nodesZ[i+1]-nodesZ[i]);

				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[i+1], nodesY[i+1], nodesZ[i+1]+1.0);
				Last_i = i+1;
				i++;
			}
		}
		else
		{
			if(Last_i != nodesSize-1)
			{
				FCNPC_AddPointToPath(eNpcInfo[routeid][FCNPC_MovePathId], nodesX[nodesSize-1], nodesY[nodesSize-1], nodesZ[nodesSize-1]+1.0);
			}
			i++;
		}
	}

	FCNPC_GoByMovePath(routeid, eNpcInfo[routeid][FCNPC_MovePathId], .speed = NpcMovementSpeed[ eNpcInfo[routeid][e_iType] ]);
	FCNPC_ApplyAnimation(routeid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- FCNPC_OnFinishMovePathPoint -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnFinishMovePathPoint(npcid, pathid, pointid)
{
	if(eNpcInfo[npcid][e_iChasedId] == INVALID_PLAYER_ID)
	{
		FCNPC_DestroyMovePath(pathid);
		FCNPC_StartUpdateTimer(npcid);
		return 1;
	}

	new chaseId = eNpcInfo[npcid][e_iChasedId];
	FCNPC_OnChasePlayerByPath(npcid, chaseId, pathid);

	// If FCNPC got close enough to player - use native chase and destroy the path.
	new Float:distanceToPlayer = GetDistanceBetweenPlayers(npcid, chaseId);
	if(distanceToPlayer <= RANGE_NPC_CHASE)
	{
		FCNPC_ChasePlayer(npcid, chaseId);
		return 0;
	}

	// If NPC is chasing player by path but player
	// has moved more than 15.0 meters from last node in path
	// destroy old path for NPC and create a new one to move by.
	new Float:x, Float:y, Float:z, 
		lastNode = FCNPC_GetNumberMovePoint(pathid) - 1;

	FCNPC_GetMovePoint(pathid, lastNode, x, y, z);

	if(GetPlayerDistanceFromPoint(chaseId, x, y, z) > 10.0)
	{
		SCMF(chaseId, -1, "You tried to run away. Creating new path...");
		FCNPC_DestroyMovePath(pathid);
		FCNPC_ChasePlayerByPath(npcid, chaseId);
	}
	FCNPC_ApplyAnimation(npcid, "TLoU", "INF_RUN", 4.1, 1, 1, 1, 0, 0);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnFinishMovePath --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnFinishMovePath(npcid, pathid)
{
	FCNPC_DestroyMovePath(pathid);
	FCNPC_StartUpdateTimer(npcid);	
	return 0;
}