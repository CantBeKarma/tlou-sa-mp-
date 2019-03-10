// Reload animations for player.
function PreloadAnimations(playerid)
{
	new animInfo[32];
    for(new i = 0; i != sizeof(AnimLibraries); i++)
    {
        if(IsPlayerNPC(playerid))
        {
			format(animInfo, sizeof(animInfo), "%s:null", AnimLibraries[i]);
			FCNPC_SetAnimationByName(playerid, animInfo, 4.1, 0, 0, 0, 0, 0);
        }
        else
        {
            ApplyAnimation(playerid, AnimLibraries[i], "null", 4.1, 0, 0, 0, 0, 0, 1);
        }
    }
}

// Apply player animation with OnPlayerAnimFinished callback.
ApplyCallbackAnim(playerid, animlib[], animname[])
{
	if(IsPlayerPlayingCallbackAnim(playerid))
	{
		return;
	}

	ApplyAnimation(playerid, animlib, animname, 4.1, 0, 1, 1, 0, 0);

	wait_ms(150);
	Bit_Let(ePlayerFlag[e_bAnimPlaying], playerid);
	ePlayerInfo[playerid][e_iAnimIdx] = GetPlayerAnimationIndex(playerid);
}

// Clear animations using other anim.
ClearAnims(playerid)
{
	return ApplyAnimation(playerid, "CARRY", "CRRY_PRTIAL", 4.1, 0, 1, 1, 0, 0, 1);
}

// Does a reload animation for player on the specified weapon type.
PlayPlayerReloadAnim(playerid, weaponId)
{
    switch(weaponId)
    {
        case 
            WEAPON_9MM,
            WEAPON_REVOLVER,
            WEAPON_SHORTY:
            {
                ApplyAnimation(playerid, "PYTHON", \
                    IsPlayerCrouched(playerid) ? "PYTHON_CROUCHRELOAD" : "PYTHON_RELOAD", \
                        4.1, 0, 1, 1, 0, 0);
            }

        case
            WEAPON_SHOTGUN:
            {
                ApplyAnimation(playerid, "BUDDY", \
                    IsPlayerCrouched(playerid) ? "BUDDY_CROUCHRELOAD" : "BUDDY_RELOAD", \
                        4.1, 0, 1, 1, 0, 0);
            }

        case
            WEAPON_RIFLE,
            WEAPON_ASSAULT:
            {
                ApplyAnimation(playerid, "RIFLE", \
                    IsPlayerCrouched(playerid) ? "RIFLE_CROUCHLOAD" : "RIFLE_LOAD", \
                        4.1, 0, 1, 1, 0, 0);
            }
    }
}

// Plays damage animations on player.
bool:PlayPlayerDamageAnimations(damagedId, bodypart = -1, issuerId = INVALID_PLAYER_ID, issuerWeaponId = -1, Float:distanceBetweenPlayers = 0.0)
{
    if(IsPlayerUnableToPerformAction(damagedId)
    || (GetConnectedTime(damagedId) - ePlayerInfo[damagedId][e_iTickCounter_damageAnim]) < TICK_RATE_DAM_ANIM)
    {
        return false;
    }

    ePlayerInfo[damagedId][e_iTickCounter_damageAnim] = GetConnectedTime(damagedId);

    if(IsPlayerNPC(issuerId))
    {
        ApplyAnimation(damagedId, "PED", \
            (random(2) == 0) ? ("HIT_BACK") : ("HIT_BEHIND"), \
                4.1, 0, 1, 1, 0, 0);

        return true;
    }

    switch(bodypart)
    {
        case BODY_PART_LEFT_ARM:  ApplyAnimation(damagedId, "PED", DamageAnimations[random(3)], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_ARM: ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 3], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_LEFT_LEG:  ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 6], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_RIGHT_LEG: ApplyAnimation(damagedId, "PED", DamageAnimations[random(3) + 9], 4.1, 0, 1, 1, 0, 0);
        case BODY_PART_TORSO:
        {
            if(issuerWeaponId == WEAPON_SHOTGUN && distanceBetweenPlayers <= SHOTGUN_TORSO_SHOT_DISTANCE)
            {
                FlyBackwardsWhenShot(damagedId);
            }
            else
            {
                ApplyAnimation(damagedId, "PED", DamageAnimations[random(4) + 12], 4.1, 0, 1, 1, 0, 0);
            }
        }
    }
	return true;
}

// Plays death animation for player (based on the current position e.g.: in vehicle or on foot).
PlayDeathAnimation(playerid, bodypart, weaponid)
{
	if(IsPlayerInAnyVehicle(playerid))
	{
		new 
			vehicleid = GetPlayerVehicleID(playerid),
			modelid = GetVehicleModel(vehicleid);

		if(IsVehicleModelBike(modelid))
		{
			new Float:x, Float:y, Float:z;
			GetVehiclePos(vehicleid, x, y, z);
			RemovePlayerFromVehicle(playerid);
			SetPlayerPos(playerid, x, y, z);
			ApplyAnimation(playerid, "PED", "BIKE_FALL_OFF", 4.1, 0, 1, 1, 1, 0, 1);
		}
		else
		{
			ApplyAnimation(playerid, "PED", "CAR_DEAD_LHS", 4.1, 0, 1, 1, 1, 0, 1);
		}
		return;
	}

	if(weaponid == WEAPON_SHIV)
	{
		ApplyCallbackAnim(playerid, "KNIFE", "KILL_KNIFE_PED_DAMAGE");
		return;
	}

	if(weaponid == WEAPON_MOLOTOV || weaponid == WEAPON_COLLISION)
	{
		ApplyAnimation(playerid, "PED", "KO_SHOT_FRONT", 4.1, 0, 1, 1, 1, 0, 1);
		return;
	}

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

// Applies player dead animation when dead body attacked.
PlayPlayerDeadAnimIfDead(playerid)
{
    if(!IsPlayerDead(playerid))
    {
        return;
    }

    switch(GetPlayerAnimationIndex(playerid))
    {
        case 
            BASEBALL_BAT_HIT_3, 
            PED_KO_SHOT_FACE,
            KNIFE_KILL_KNIFE_PED_DIE:
            {
                ApplyAnimation(playerid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0, 1);
            }

        default: 
        {
            ApplyAnimation(playerid, "PED", "FLOOR_HIT_F", 4.1, 0, 1, 1, 1, 0, 1);
        }
    }
}


// Actions in which player can't do some stuff (e.g. opening backpack, equipping last weapon and other).
IsPlayerUnableToPerformAction(playerid)
{
    return 
        IsPlayerUsingAnim(playerid, ANIM_COLLECTION_CLIMB)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_SWIM)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_JUMP)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_FALL)
		|| IsPlayerDead(playerid)
		|| IsPlayerBurning(playerid)
		|| IsPlayerStunned(playerid)
    ;
}

// Check if player is using certain collection of animations (based on index of an animation).
IsPlayerUsingAnim(playerid, animid)
{
	new playerAnimIdx = GetPlayerAnimationIndex(playerid);
	switch(animid)
	{
		case ANIM_COLLECTION_BUNNYHOP:
		{
			switch(playerAnimIdx)
			{
				case 
					PED_JUMP_LAUNCH,
					PED_JUMP_LAUNCH_R,
					PED_JUMP_LAND,
					PED_FALL_LAND,
					PED_RUN_CIVI:
					{
						return true;
					}
			}
		}

		case ANIM_COLLECTION_JUMP:
		{
			switch(playerAnimIdx)
			{
				case 
					PED_JUMP_LAUNCH,
					PED_JUMP_LAUNCH_R,
					PED_JUMP_GLIDE,
					PED_JUMP_LAND,
					PED_FALL_LAND:
					{
						return true;
					}
			}
		}

		case ANIM_COLLECTION_CLIMB:
		{
			switch(playerAnimIdx)
			{
				case 
					PED_CLIMB_IDLE,
					PED_CLIMB_JUMP2FALL,
					PED_CLIMB_PULL,
					PED_CLIMB_JUMP,
					PED_CLIMB_JUMP_B,
					SHOTGUN_SHOTGUN_FIRE:
					{
						return true;
					}
			}
		}

		case ANIM_COLLECTION_AIM:
		{
			switch(playerAnimIdx)
			{
				case 
					PED_GUN_STAND,
					PED_GUNMOVE_BWD, PED_GUNMOVE_FWD,
					PED_GUNMOVE_L, PED_GUNMOVE_R,
					PED_GUNCROUCHBWD, PED_GUNCROUCHFWD,
					COLT45_COLT45_FIRE, COLT45_COLT45_FIRE_2HANDS,
					COLT45_2GUNS_CROUCHFIRE, COLT45_COLT45_CROUCHFIRE,
					PYTHON_PYTHON_CROUCHFIRE, RIFLE_RIFLE_CROUCHFIRE,
					SHOTGUN_SHOTGUN_CROUCHFIRE, SHOTGUN_SHOTGUN_FIRE,
					SILENCED_SILENCECROUCHFIRE,
					TEC_TEC_CROUCHFIRE, UZI_UZI_CROUCHFIRE,
					BUDDY_BUDDY_CROUCHFIRE:
					{
						return true;
					}
			}
		}

		case ANIM_COLLECTION_SWIM_UNDERWATER:
		{
			switch(playerAnimIdx)
			{
				case 
					SWIM_SWIM_DIVE_UNDER, 
					SWIM_SWIM_GLIDE,
					SWIM_SWIM_UNDER:
					{
						return true;
					}
			}
		}

	    case ANIM_COLLECTION_SWIM:
	    {
			new animLib[32], animName[32];
			GetAnimationName(GetPlayerAnimationIndex(playerid), animLib, sizeof(animLib), animName, sizeof(animName));

	        if(!strcmp(animLib, "SWIM", true))
			{
				return true;
			}
		}

		case ANIM_COLLECTION_FALL:
		{
			switch(playerAnimIdx)
			{
				case 
					PED_FALL_COLLAPSE,
					PED_FALL_FALL,
					PED_FALL_GLIDE, 
					PED_FALL_LAND:
					{
						return true;
					}
			}
		}
	}
	return ANIM_COLLECTION_UNKNOWN;
}