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
	if(!Bit_Get(ePlayerFlag[e_bAnimPlaying], playerid))
	{
		ApplyAnimation(playerid, animlib, animname, 4.1, 0, 1, 1, 0, 0);
		//defer DelayTimer_GetAnimIndex(playerid);

		wait_ms(150);
		Bit_Let(ePlayerFlag[e_bAnimPlaying], playerid);
		ePlayerInfo[playerid][e_iAnimIdx] = GetPlayerAnimationIndex(playerid);
	}
}

// Get player animation index.
// timer DelayTimer_GetAnimIndex[150](playerid)
// {
// 	Bit_Let(ePlayerFlag[e_bAnimPlaying], playerid);
//     ePlayerInfo[playerid][e_iAnimIdx] = GetPlayerAnimationIndex(playerid);
//     return ePlayerInfo[playerid][e_iAnimIdx];
// }

// Clear animations using other anim.
ClearAnims(playerid)
{
	return ApplyAnimation(playerid, "CARRY", "CRRY_PRTIAL", 4.1, 0, 1, 1, 0, 0, 1);
}

// Does a reload animation for player on the specified weapon type.
PerformReloadAnimation(playerid, weaponId)
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

// Actions in which player can't do some stuff (e.g. opening backpack, equipping last weapon and other).
IsPlayerPlayingActions(playerid)
{
    return 
        IsPlayerUsingAnim(playerid, ANIM_COLLECTION_CLIMB)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_SWIM)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_JUMP)
        || IsPlayerUsingAnim(playerid, ANIM_COLLECTION_FALL)
		|| IsPlayerDead(playerid)
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