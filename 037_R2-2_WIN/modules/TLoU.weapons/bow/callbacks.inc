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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnDynamicObjectMoved -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnDynamicObjectMoved(objectid)
{
	foreach(new i : Player)
	{
		if(!IsValidDynamicObject(ePlayerWeapon[i][e_iArrowObjectId]))
		{
			continue;
		}
		
		if(RandomEx(0, 2) == 1)
		{
			new Float:x, Float:y, Float:z;

			GetDynamicObjectPos(ePlayerWeapon[i][e_iArrowObjectId], x, y, z);
			CA_FindZ_For2DCoord(x, y, z);
			CreateOrUpdateItem(_:ITEM_IDX_ARROW, 1, .x = x, .y = y, .z = z);
		}

		DestroyPlayerArrow(i);
	}
	return 1;
}

// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
// --------------------- OnPlayerUpdate --------------------
// =-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerUpdate(playerid)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 0;
    }

	if(IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]) && IsDynamicObjectMoving(ePlayerWeapon[playerid][e_iArrowObjectId]))
	{
		foreach(new i : Player)
	    {
			if(i == playerid)
			{
				continue;
			}
			
			new Float:x, Float:y, Float:z;
			GetDynamicObjectPos(ePlayerWeapon[playerid][e_iArrowObjectId], x, y, z);

			if(IsPlayerInRangeOfPoint(i, ARROW_RANGE_DAMAGE, x, y, z))
			{
				DestroyPlayerArrow(playerid);
				OnPlayerGetArrowShot(i, playerid);
				break;
			}
		}
	}
	return 1;
}