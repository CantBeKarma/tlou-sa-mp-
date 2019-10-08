#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerShivPlayer ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShivPlayer(playerid, shivedId)
{    
    EquipItem(playerid, _:ITEM_IDX_SHIV);
	GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV) - 1;
	ApplyCallbackAnim(playerid, "KNIFE", "KILL_KNIFE_PLAYER");
	SetPlayerHealthEx(shivedId, 0.0, playerid, _:ITEM_IDX_SHIV);
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