//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerGiveDamage ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGiveDamage(playerid, damagedid, Float:amount, weaponid, bodypart)
{
    // Don't let unknown players damage.
    if(damagedid == INVALID_PLAYER_ID)
    {
        return 0;
    }

    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerTakeDamage ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerTakeDamage(playerid, issuerid, Float:amount, weaponid, bodypart)
{
    if(IsPlayerDead(playerid))
    {
        if(issuerid == INVALID_PLAYER_ID) 
            return 1;
            
        switch(GetPlayerAnimationIndex(playerid))
        {
            case BASEBALL_BAT_HIT_3, PED_KO_SHOT_FACE: ApplyAnimation(playerid, "PED", "FLOOR_HIT", 4.1, 0, 1, 1, 1, 0, 1);
            default: ApplyAnimation(playerid, "PED", "FLOOR_HIT_F", 4.1, 0, 1, 1, 1, 0, 1);
        }
        return 1;
    }

    // Disallow team-mates damages (no friendly-fire).
    if(playerid != FACTION_NONE && GetPlayerTeam(playerid) == GetPlayerTeam(issuerid)) 
    {
        return 1;
    }

    // If player is using backpack.
    HideBackpack(playerid);

    if(issuerid == INVALID_PLAYER_ID)
    {
        OnPlayerDamageHimself(playerid, amount);
    }
    else
    {
        new 
            Float:damageAmount = 0.0,
            issuerWeaponId = CurrentPlayerWeaponId[issuerid];

        if(IsMeleeWeapon(issuerWeaponId))
        {
            damageAmount = PlayerWeaponBodyDamage[issuerid];
            OnPlayerHitPlayer(playerid, issuerid, issuerWeaponId, damageAmount);
        }
        else if(IsBulletWeapon(issuerWeaponId))
        {
            damageAmount = (bodypart == BODY_PART_HEAD) ? (PlayerWeaponHeadDamage[playerid]) : (PlayerWeaponBodyDamage[playerid]);

            new Float:distance = GetDistanceBetweenPlayers(issuerid, playerid);
            if(distance > REDUCE_DAMAGE_AMOUNT_DISTANCE && issuerid != WEAPON_RIFLE)
                damageAmount = (damageAmount / 2);

            OnPlayerShootPlayer(playerid, issuerid, issuerWeaponId, damageAmount, distance, bodypart);
        }
    }

    if(IsPlayerDead(playerid))
    {
        OnPlayerDeathEx(playerid, issuerid, weaponid, bodypart);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerDamageHimself ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDamageHimself(playerid, Float:damageAmount)
{
    SCMF(playerid, -1, "Damaged myself! DamageAmount: %f", damageAmount);
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - damageAmount);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerHitPlayer -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerHitPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount)
{
	if(issuerWeaponId == WEAPON_UNARMED)
	{
		// Doesn't hide weapon if we are hitting someone.
		KillTimer(ePlayerInfo[issuerId][e_iTimer_HideHud]);
		ePlayerInfo[issuerId][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", issuerId);

        DegradePlayerMeleeWeapon(issuerId, issuerWeaponId);
	}

    // Reduced damaged npc health.
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerShootPlayer -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShootPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount, Float:distance, bodypart)
{
    SCMF(damagedId, -1, "Got shot by another player (%d), with weapon (%d). Damage done: %f to bodypart: %d", issuerId, issuerWeaponId, damageAmount, bodypart);
    SCMF(issuerId, -1, "You shot another player (%d), with weapon (%d). Damage done: %f to bodypart: %d", damagedId, issuerWeaponId, damageAmount, bodypart);
    
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount);
    ShowPlayerHitMarker(issuerId, bodypart);
    PlayPlayerDamageAnimations(damagedId, bodypart, issuerId, issuerWeaponId, distance);
    return 1;
}