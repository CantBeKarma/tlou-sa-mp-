#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerShotArrow --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShotArrow(playerid)
{
	Audio_PlayEx(playerid, AUDIO_BOW_SHOT);
	Bit_Vet(ePlayerFlag[e_bShootingCustomWeapon], playerid);

	new newBowArrows = GetPlayerItemAmount(playerid, _:ITEM_IDX_BOW) - 1;
	SetPlayerItemAmount(playerid, _:ITEM_IDX_BOW, newBowArrows);

	if(newBowArrows > 0)
	{
		Key_AimingCustomWeapon(playerid);
	}
	else
	{
		if(IsPlayerAimingWeapon(playerid))
		{
			SetPlayerStatus(playerid, PLAYER_STATUS_NONE);
		}
	}
	return 1;
}