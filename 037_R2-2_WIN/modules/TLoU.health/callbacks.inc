#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerStartedHealing ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartedHealing(playerid)
{
	stop ePlayerInfo[playerid][e_iTimer_HideHud];
	Audio_PlayEx(playerid, AUDIO_BANDAGE_USE, .loop = true);

	new Float:oldHealth = GetPlayerHealthEx(playerid);
	new Float:destHealth = oldHealth + 20.0;

	HealMyself(playerid, oldHealth, destHealth);
	ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_LOOP", 4.0, 1, 0, 0, 0, 0);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerIsHealing --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerIsHealing(playerid, Float:oldHealth, Float:destHealth)
{
	new Float:currentHealth = GetPlayerHealthEx(playerid);
	if(currentHealth >= 100.0 || currentHealth >= destHealth)
	{
		OnPlayerHealed(playerid, oldHealth, currentHealth);
	}
	else
	{
		new healSpeed = 
				(Skill_GetPlayerLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 0) ? (SKILL_HEAL_SPEED_LVL0) 
			: (Skill_GetPlayerLevel(playerid, SKILL_ITEM_HEAL_SPEED) == 1) ? (SKILL_HEAL_SPEED_LVL1) 
			: (SKILL_HEAL_SPEED_LVL2);

		SetPlayerHealthEx(playerid, currentHealth + RESTORE_HP_WITH_MEDKIT_AMOUNT);

		SetTimerEx("HealMyself", healSpeed, false, "dff", playerid, oldHealth, destHealth);
	}
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerCanceledHealing ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCanceledHealing(playerid, Float:oldHealth)
{
	SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
	ePlayerInfo[playerid][e_iTimer_HideHud] = defer HideHud(playerid);

	Audio_StopEx(playerid);
	SetPlayerHealthEx(playerid, oldHealth);
	ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0, 1);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerHealed ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerHealed(playerid, Float:oldHealth, Float:newHealth)
{
	SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
	ePlayerInfo[playerid][e_iTimer_HideHud] = defer HideHud(playerid);

	Audio_StopEx(playerid);
	Audio_PlayEx(playerid, AUDIO_BANDAGE_FINISH);

	new newMedkitAmount = GetPlayerItemAmount(playerid, _:ITEM_IDX_MEDKIT) - 1;
	SetPlayerItemAmount(playerid, _:ITEM_IDX_MEDKIT, newMedkitAmount);

	if(newMedkitAmount <= 0)
	{
		HideItem(playerid);
	}

	ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_2IDLE", 4.1, 0, 1, 1, 0, 0);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerHealthChange ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerHealthChange(playerid, Float:oldHealth, Float:newHealth)
{
	AdjustPlayerHealthBar(playerid);
	ShowHud(playerid);
	
	if(newHealth <= MIN_HEALTH_FLASH_HUD)
	{
		// Flash HUD background if health is lower than defined.
		if(!IsPlayerHudFlashing(playerid))
		{
			Bit_Let(ePlayerFlag[e_bIsHudBgFlashing], playerid);
			HudTd_Bg_FlashRed(playerid, 0, 0.0);
		}
	}
	else
	{
		if(newHealth < oldHealth)
		{
			// Fade HUD background from RED to BLACK.
			HudTd_Bg_RedToBlack(playerid, 0.0);
		}
	}
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
        if(IsPlayerIdle(playerid) && IsItemType(GetPlayerEquippedItemId(playerid), ITEM_TYPE_MEDKIT))
		{
			if(!IsPlayerHealing(playerid) && GetPlayerHealthEx(playerid) < 100.0)
			{
				SetPlayerStatus(playerid, PLAYER_STATUS_HEALING);
				OnPlayerStartedHealing(playerid);
			}
		}
	}
    return 1;
}