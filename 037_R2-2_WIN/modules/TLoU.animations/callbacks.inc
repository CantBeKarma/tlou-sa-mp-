#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPlayerUpdate ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(IsPlayerPlayingCallbackAnim(playerid))
	{
		new currentAnimIdx = GetPlayerAnimationIndex(playerid);
		if(currentAnimIdx != ePlayerInfo[playerid][e_iAnimIdx])
		{
			OnPlayerAnimFinish(playerid, ePlayerInfo[playerid][e_iAnimIdx]);
			ePlayerInfo[playerid][e_iAnimIdx] = 0;
		}
	}
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerAnimFinish ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerAnimFinish(playerid, animIdx)
{
	// new animLib[32], animName[32];
	// GetAnimationName(ePlayerInfo[playerid][e_iAnimIdx], animLib, sizeof(animLib), animName, sizeof(animName));
    // SCMF(playerid, -1, "Animation Lib: %s | Name: %s | Idx: %d", animLib, animName, animIdx);

	switch(animIdx)
	{
		// Projectile has been thrown.
		case GRENADE_WEAPON_THROW:
		{
			if(GetPlayerWeaponAmmo(playerid, CurrentPlayerWeaponId[playerid]) > 0)
			{
				if(IsPlayerAimingWeapon(playerid))
				{
					ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);
				}
			}
			else
			{
				if(IsPlayerAimingWeapon(playerid))
				{
					SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
				}
			}
		}

		// Player is being shived.
		case KNIFE_KILL_KNIFE_PED_DAMAGE:
		{
			ApplyCallbackAnim(playerid, "KNIFE", "KILL_KNIFE_PED_DIE");
		}

		// When player shived NPC/Player (Remove Shiv model from hand).
		case KNIFE_KILL_KNIFE_PLAYER:
		{
			HidePlayerWeapon(playerid);
		}

		default:
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
		}
	}
	return 1;
}