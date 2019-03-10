#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStartAimingBow ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartAimingBow(playerid)
{	
	FreezePlayer(playerid, true, -1);
	Bow_PullString(playerid);
	ForceChangeHoldingKeyDef(playerid, KEY_AIM);
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

	new newBowArrows = GetPlayerArrows[playerid] - 1;
	SetPlayerWeaponAmmo(playerid, WEAPON_BOW, newBowArrows);
	
	if(newBowArrows > 0)
	{
		if(IsPlayerAimingWeapon(playerid))
			Bow_PullString(playerid);
	}
	else
	{
		if(IsPlayerAimingWeapon(playerid))
		{
			SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
			OnPlayerStopAimingWeapon(playerid);
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
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerHoldingKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerHoldingKey(playerid, key)
{
    if(key & KEY_AIM)
    {
		if(IsPlayerAimingWeapon(playerid) && CurrentPlayerWeaponId[playerid] == WEAPON_BOW)
            SetAngleBasedOnCam(playerid);
    }
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerKeyStateChange -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }

	if(PRESSED(KEY_FIRE))
	{
		if(IsPlayerAimingWeapon(playerid) && CurrentPlayerWeaponId[playerid] == WEAPON_BOW)
		{
			//new bowString_pullTime = GetConnectedTime(playerid) - ePlayerInfo[playerid][e_iTickCounter_holdingKey];
			//new Float:arrowDistance = (bowString_pullTime >= 3800) ? (MAX_ARROW_DISTANCE) : (MAX_ARROW_DISTANCE / 2);
			Arrow_FireArrow(playerid);
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
			continue;

		switch(random(2))
		{
			case 0: DestroyDynamicObject(ePlayerWeapon[i][e_iArrowObjectId]), SCMF(i, -1, "Your arrow have been destroyed");
			case 1:
			{

				SCMF(i, -1, "You can still pick it up!");

				new Float:x, Float:y, Float:z;
				GetDynamicObjectPos(ePlayerWeapon[i][e_iArrowObjectId], x, y, z);
				CA_FindZ_For2DCoord(x, y, z);
				DestroyDynamicObject(ePlayerWeapon[i][e_iArrowObjectId]);

				CreateOrUpdateItem(17, 1, .x = x, .y = y, .z = z);
			}
		}
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
				continue;
			
			new Float:x, Float:y, Float:z;
			GetDynamicObjectPos(ePlayerWeapon[playerid][e_iArrowObjectId], x, y, z);

			if(IsPlayerInRangeOfPoint(i, ARROW_RANGE_DAMAGE, x, y, z))
			{
				DestroyDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId]);
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
	ePlayerWeapon[playerid][e_iTickCounter_bowShooting] = GetConnectedTime(playerid);
	ApplyAnimation(playerid, "TLoU", "BOW_AIM_IN", 3.1, 0, 1, 1, 1, 1);
	Audio_PlayEx(playerid, AUDIO_BOW_PULL);

	SPAO(playerid, ITEM_MODEL_ARROW);
	SPAO(playerid, ITEM_MODEL_BOW, true);
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
		ClearAnims(playerid);
}

// Fire arrow.
Arrow_FireArrow(playerid, Float:distance = MAX_ARROW_DISTANCE)
{
	// Validation.
	if(GetPlayerArrows[playerid] <= 0 || IsValidDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId])
	|| (GetConnectedTime(playerid) - ePlayerWeapon[playerid][e_iTickCounter_bowShooting]) < TICK_RATE_ARROW_SHOOT)
	{
		return 1;
	}
	ePlayerWeapon[playerid][e_iTickCounter_bowShooting] = GetConnectedTime(playerid);

	// Physics.
	new Float:pCamX, Float:pCamY, Float:pCamZ;
	GetPlayerCameraPos(playerid, pCamX, pCamY, pCamZ);

	new Float:pCamVecX, Float:pCamVecY, Float:pCamVecZ;
	GetPlayerCameraFrontVector(playerid, pCamVecX, pCamVecY, pCamVecZ);

	new Float:pPosX, Float:pPosY, Float:pPosZ, Float:pAngle;
	GetPlayerPos(playerid, pPosX, pPosY, pPosZ);
	GetPlayerFacingAngle(playerid, pAngle);
	GetXYInFrontOfPlayer(playerid, pPosX, pPosY, 1.2);

	new Float:dest_arrowX = pCamX + floatmul(1.2, floatcos(pAngle, degrees)) + floatmul(pCamVecX, distance);
	new Float:dest_arrowY = pCamY + floatmul(1.7, floatsin(pAngle, degrees)) + floatmul(pCamVecY, distance);
	new Float:dest_arrowZ = pCamZ + 4.2 + floatmul(pCamVecZ, distance);

	ePlayerWeapon[playerid][e_iArrowObjectId] = CreateDynamicObject(ITEM_MODEL_ARROW, pPosX, pPosY, pPosZ + 0.5, 0.0, 0.0, pAngle - 79.0);
	MoveDynamicObject(ePlayerWeapon[playerid][e_iArrowObjectId], dest_arrowX, dest_arrowY, dest_arrowZ, MAX_ARROW_SPEED);

	foreach(new i : Player)
	{
		Streamer_UpdateEx(i, pPosX, pPosY, pPosZ, .type = STREAMER_TYPE_OBJECT);
	}

	OnPlayerShotArrow(playerid);
	return 0;
}