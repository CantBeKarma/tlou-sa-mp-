#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPlayerUpdate ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(Bit_Get(ePlayerFlag[e_bAnimPlaying], playerid))
	{
		new currentAnimIdx = GetPlayerAnimationIndex(playerid);
		if(currentAnimIdx != ePlayerInfo[playerid][e_iAnimIdx])
		{
			Bit_Vet(ePlayerFlag[e_bAnimPlaying], playerid);
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
		// Backpack has been closed.
		case BOMBER_BOM_PLANT_2IDLE:
		{
			Enable_OnPlayerLookAt(playerid);

			new 
				lastWeaponId = LastPlayerWeaponId[playerid],
				lastWeaponAmmo = GetPlayerWeaponAmmo(playerid, lastWeaponId);

			if(lastWeaponId != WEAPON_UNARMED && lastWeaponAmmo > 0)
				SetPlayerWeapon(playerid, lastWeaponId, lastWeaponAmmo);
		}

		// Projectile has been thrown.
		case GRENADE_WEAPON_THROW:
		{
			if(GetPlayerWeaponAmmo(playerid, CurrentPlayerWeaponId[playerid]) > 0)
			{
				if(IsPlayerAimingWeapon(playerid))
					ApplyAnimation(playerid, "DILDO", "DILDO_IDLE", 4.1, 1, 1, 1, 0, 0);
			}
			else
			{
				if(IsPlayerAimingWeapon(playerid))
				{
					Bit_Vet(ePlayerFlag[e_bAiming], playerid);
					OnPlayerStopAimingWeapon(playerid);
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
			RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);
		}

		// Player got stunned by smoke bomb.
		case PED_GAS_CWR:
		{
			if(IsPlayerStunned(playerid))
				ApplyCallbackAnim(playerid, "PED", "IDLE_TIRED");
		}

		// Not stunned anymore.
		case PED_IDLE_TIRED:
		{
			if(IsPlayerStunned(playerid))
			{
				Enable_OnPlayerLookAt(playerid);
				Bit_Vet(ePlayerFlag[e_bIsStunned], playerid);
			}
		}
	}
	return 1;
}