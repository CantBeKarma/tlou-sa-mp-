// Apply animation when shot in close-range with shotgun.
FlyBackwardsWhenShot(damagedid)
{
    if(IsPlayerNPC(damagedid))
    {
        FCNPC_SetStatus(damagedid, STATUS_FALL);
        return 1;
    }

    SetPlayerStatus(damagedid, PLAYER_STATUS_FALL);

    if(!IsPlayerDead(damagedid))
    {
        defer DelayedTimer_AnimGetUp[1000](damagedid);
    }
    return 0;
}

// Calculates damage that player is going to cause to another player
// Optional param: bodypart.
Float:CalculateDamageAmount(damagerid, targetid, damagerItemId, &Float:distanceBetweenPlayers, bodypart = -1)
{
    if(IsItemType(damagerItemId, ITEM_TYPE_MELEE))
    {
        return PlayerWeaponBodyDamage[damagerid];
    }

    new Float:damageAmount = (bodypart == BODY_PART_HEAD) ? (PlayerWeaponHeadDamage[damagerid]) : (PlayerWeaponBodyDamage[damagerid]);

	distanceBetweenPlayers = GetDistanceBetweenPlayers(damagerid, targetid);
	if(distanceBetweenPlayers > REDUCE_DAMAGE_AMOUNT_DISTANCE && ITEM_IDX:damagerItemId != ITEM_IDX_RIFLE)
	{
		damageAmount = (damageAmount / 2);
	}
	return damageAmount;
}