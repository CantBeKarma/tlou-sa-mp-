#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerBunnyHop -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerBunnyHop(playerid)
{
	TogglePlayerAction(playerid, PLAYER_ACTION_JUMP, false);
	defer DelayedTimer_EnableJumping(playerid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)	
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;

	if(HOLDING(KEY_SPRINT) && PRESSED(KEY_JUMP) || PRESSED(KEY_JUMP))
	{
		if(!Bit_Get(ePlayerFlag[e_bDoingBunnyHop], playerid) 
		&& IsPlayerUsingAnim(playerid, ANIM_COLLECTION_BUNNYHOP))
		{
			Bit_Let(ePlayerFlag[e_bDoingBunnyHop], playerid);
			OnPlayerBunnyHop(playerid);
		}
	}
	return 1;
}