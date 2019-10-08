#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------------- OnPlayerUpdate ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(IsPlayerPlayingCallbackAnim(playerid))
	{
		if(GetPlayerAnimationIndex(playerid) != ePlayerInfo[playerid][e_iAnimIdx])
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
			if(GetPlayerItemAmount(playerid, GetPlayerEquippedItemId(playerid)) <= 0)
			{
				SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
			}
			else
			{
				Bit_Vet(ePlayerFlag[e_bShootingCustomWeapon], playerid);
			}
		}

		// When player shived NPC/Player (Remove Shiv model from hand).
		case KNIFE_KILL_KNIFE_PLAYER:
		{
			HideItem(playerid);
			SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
		}

		default:
		{
			SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
		}
	}
	return 1;
}