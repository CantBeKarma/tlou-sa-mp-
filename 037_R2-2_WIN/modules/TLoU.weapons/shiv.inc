#include	<YSI\y_hooks>

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

	SPAO(playerid, ITEM_MODEL_SHIV);
	SetPlayerHealthEx(shivedId, 0.0, playerid, WEAPON_SHIV);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }

	if(PRESSED(KEY_FIRE))
	{
		if(!IsPlayerAllowedToShiv(playerid))
		{
			return 1;
		}

		new shivedId = GetPlayerFocusedPlayerId(playerid);

		if(IsPlayerNPC(shivedId))
		{
			FCNPC_OnGetShived(shivedId, playerid);
		}
		else
		{
			OnPlayerShivPlayer(playerid, shivedId);
		}
	}
	return 1;
}