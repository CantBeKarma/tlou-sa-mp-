#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStartAimingBow ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingBow(playerid)
{
	FreezePlayer(playerid, true, -1);
	Key_AimingCustomWeapon(playerid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStopAimingBow -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopAimingBow(playerid)
{
	UnfreezePlayer(playerid);
	Bow_ReleaseString(playerid);
	return 1;
}

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
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
		}
	}
	return 1;
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

// Pull player's bow string.
Bow_PullString(playerid)
{
	if(ePlayerWeapon[playerid][e_bAimingArrow])
	{
		return;
	}

	ApplyAnimation(playerid, "TLoU", "BOW_AIM_IN", 3.1, 0, 1, 1, 1, 1);
	Audio_PlayEx(playerid, AUDIO_BOW_PULL);

	SPAO(playerid, ITEM_MODEL_ARROW);
	SPAO(playerid, ITEM_MODEL_BOW, true);

	ePlayerWeapon[playerid][e_bAimingArrow] = true;
	ePlayerWeapon[playerid][e_iTickCounter_bowString] = GetConnectedTime(playerid);
}

// Release player's bow string.
Bow_ReleaseString(playerid)
{
	Audio_StopEx(playerid);
	Audio_PlayEx(playerid, AUDIO_BOW_CANCEL);
	
	SPAO(playerid, ITEM_MODEL_BOW, true);
	RemovePlayerAttachedObj(playerid, eCharsInfo[playerid][e_iAttachSlot_Dynamic]);

	// normal = -0.071999, 0.002999, 0.069999, -72.899917, -171.399917, 1.300000, 0.758000, 0.627999, 1.000000
	// crouched = -0.090999, -0.061999, -0.011000, -5.899932, -171.399917,  1.700000, 0.758000, 0.627999, 1.000000

	if(!IsPlayerCrouched(playerid))
	{
		ClearAnims(playerid);
	}

	ePlayerWeapon[playerid][e_bAimingArrow] = false;
	ePlayerWeapon[playerid][e_iTickCounter_bowString] = 0;
}

// Fire arrow.
Arrow_FireArrow(playerid)
{
	// Validation.
	if(GetPlayerItemAmount(playerid, _:ITEM_IDX_BOW) <= 0 || ePlayerWeapon[playerid][e_iArrowObjectId] != -1)
	{
		return 1;
	}

	// Physics.
	new Float:pCamX, Float:pCamY, Float:pCamZ;
	GetPlayerCameraPos(playerid, pCamX, pCamY, pCamZ);

	new Float:pCamVecX, Float:pCamVecY, Float:pCamVecZ;
	GetPlayerCameraFrontVector(playerid, pCamVecX, pCamVecY, pCamVecZ);

	new Float:pPosX, Float:pPosY, Float:pPosZ, Float:pAngle;
	GetPlayerPos(playerid, pPosX, pPosY, pPosZ);
	GetPlayerFacingAngle(playerid, pAngle);
	GetXYInFrontOfPlayer(playerid, pPosX, pPosY, 1.2);

	new stretch_time = (GetConnectedTime(playerid) - ePlayerWeapon[playerid][e_iTickCounter_bowString]);
	new Float:distance = (stretch_time >= MAX_ARROW_STRETCH_TIME) ? (MAX_ARROW_DISTANCE) : ((stretch_time / 1000.0) * 15);

	new Float:dest_arrowX = pCamX + floatmul((distance / 100) * 3, floatcos(pAngle, degrees)) + floatmul(pCamVecX, distance);
	new Float:dest_arrowY = pCamY + floatmul((distance / 100) * 4.2, floatsin(pAngle, degrees)) + floatmul(pCamVecY, distance);
	new Float:dest_arrowZ = pCamZ + ((distance / 100) * 10.5) + floatmul(pCamVecZ, distance);

	ePlayerWeapon[playerid][e_iArrowObjectId] = CreateDynamicObject(ITEM_MODEL_ARROW, pPosX, pPosY, pPosZ + 0.5, 0.0, 0.0, pAngle - 79.0);
	MoveDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId], dest_arrowX, dest_arrowY, dest_arrowZ, (distance * 1.5));

	foreach(new i : Player)
	{
		Streamer_UpdateEx(i, pPosX, pPosY, pPosZ, .type = STREAMER_TYPE_OBJECT);
	}

	ePlayerWeapon[playerid][e_bAimingArrow] = false;
	Bit_Let(ePlayerFlag[e_bShootingCustomWeapon], playerid);
	OnPlayerShotArrow(playerid);
	return 0;
}

// Destroy player arrow (and reset arrow variable).
DestroyPlayerArrow(playerid)
{
	if(IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]))
	{
		DestroyDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]);
	}
	
	ePlayerWeapon[playerid][e_iArrowObjectId] = -1;
}