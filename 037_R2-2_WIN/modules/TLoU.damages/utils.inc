// Apply animation when shot in close-range with shotgun.
FlyBackwardsWhenShot(damagedid)
{
    if(IsPlayerNPC(damagedid))
    {
        if(FCNPC_GetType(damagedid) == NPC_TYPE:TYPE_BLOATER)
        {
            return 1;
        }

        FCNPC_SetAnimCallback(damagedid, BASEBALL_BAT_HIT_3, .freeze = true);
        return 1;
    }

    ApplyAnimation(damagedid, "BASEBALL", "BAT_HIT_3", 4.0, 0, 1, 1, 1, 0, 1);

    if(!IsPlayerDead(damagedid))
    {
        defer DelayedTimer_AnimGetUp[1000](damagedid);
    } 
    return 0;
}

// Calculates damage that player is going to cause to another player
// Optional param: bodypart.
Float:CalculateDamageAmount(damagerid, targetid, damagerWeaponId, &Float:distanceBetweenPlayers, bodypart = -1)
{
    if(IsMeleeWeapon(damagerWeaponId))
    {
        return PlayerWeaponBodyDamage[damagerid];
    }

    new Float:damageAmount = (bodypart == BODY_PART_HEAD) ? (PlayerWeaponHeadDamage[damagerid]) : (PlayerWeaponBodyDamage[damagerid]);

	distanceBetweenPlayers = GetDistanceBetweenPlayers(damagerid, targetid);
	if(distanceBetweenPlayers > REDUCE_DAMAGE_AMOUNT_DISTANCE && damagerWeaponId != WEAPON_RIFLE)
	{
		damageAmount = (damageAmount / 2);
	}
	return damageAmount;
}

// Reduces condition of player's melee weapon by one.
DegradePlayerMeleeWeapon(playerid, weaponid)
{
	// Durability logic (reduce upgraded durability or basic one?)
	new newMeleeDurability = (IsPlayerMeleeUpgraded[playerid]) ? (GetPlayerMeleeExtCnd[playerid]-1) : (GetPlayerMeleeCnd[playerid]-1);
	HudTd_UpdateMeleeWeaponCND(playerid, weaponid, newMeleeDurability);

	if(newMeleeDurability > 0)
    {
        return;
    }

    // Removes melee weapon if broke.
    if(!IsPlayerMeleeUpgraded[playerid])
    {
        RemovePlayerWeapon(playerid, weaponid);
        return;
    }

    // If melee is upgraded - remove its upgraded feature and hide bars of it.
    IsPlayerMeleeUpgraded[playerid] = false;
    HudTd_HideMeleeUpgradedBars(playerid, weaponid);

    if(weaponid == WEAPON_SCISSOR_PIPE)
    {
        GetPlayerMelee[playerid] = WEAPON_PIPE;
        SetPlayerWeapon(playerid, GetPlayerMelee[playerid], GetPlayerMeleeCnd[playerid]);
    }
}