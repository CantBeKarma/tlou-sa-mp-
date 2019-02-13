#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerShivPlayer -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShivPlayer(playerid, shivedId)
{
	new newShivAmount = GetPlayerShivs[playerid] - 1;
	SetPlayerWeaponAmmo(playerid, WEAPON_SHIV, newShivAmount);
	
	if(newShivAmount <= 0)
	{
		RemovePlayerWeapon(playerid, WEAPON_SHIV);
	}

	HidePlayerWeapon(playerid);
	ApplyCallbackAnim(playerid, "KNIFE", "KILL_KNIFE_PLAYER");
	ApplyCallbackAnim(shivedId, "KNIFE", "KILL_KNIFE_PED_DAMAGE");

	OnPlayerDeathEx(shivedId, playerid, WEAPON_SHIV, BODY_PART_TORSO);
	SPAO(playerid, ITEM_MODEL_SHIV);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
	if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;

	if(PRESSED(KEY_FIRE))
	{
		if(IsPlayerAllowedToShiv(playerid))
		{
			new shivedId = GetPlayerFocusedPlayerId(playerid);

			if(IsPlayerNPC(shivedId))
				FCNPC_OnGetShived(shivedId, playerid);
			else
				OnPlayerShivPlayer(playerid, shivedId);
		}
	}
	return 1;
}

// Check if player is allowed to shiv (see's a SHIV label)
IsPlayerAllowedToShiv(playerid)
{
	return IsValidDynamic3DTextLabel(ePlayerInfo[playerid][e_iLookingAt_Label]) && IsPlayerLookingAtPlayer(playerid);
}