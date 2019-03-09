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
        PlayPlayerDeadAnimIfDead(playerid);
        return 1;
    }

    if(playerid != FACTION_NONE && GetPlayerTeam(playerid) == GetPlayerTeam(issuerid)) 
    {
        return 1;
    }
    
    HideBackpack(playerid);

    if(issuerid == INVALID_PLAYER_ID)
    {
        OnPlayerDamageHimself(playerid, amount);
        return 1;
    }

    new issuerWeaponId = CurrentPlayerWeaponId[issuerid];
    new Float:distanceBetweenPlayers = 0.0;
    new Float:damageAmount = CalculateDamageAmount(issuerid, playerid, issuerWeaponId, distanceBetweenPlayers, bodypart);

    if(IsMeleeWeapon(issuerWeaponId))
    {
        OnPlayerHitPlayer(playerid, issuerid, issuerWeaponId, damageAmount);
    }
    else if(IsBulletWeapon(issuerWeaponId))
    {
        OnPlayerShootPlayer(playerid, issuerid, issuerWeaponId, damageAmount, distanceBetweenPlayers, bodypart);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerDamageHimself ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerDamageHimself(playerid, Float:damageAmount)
{
    SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - damageAmount);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnPlayerHitPlayer -----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerHitPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount)
{
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount, issuerId, issuerWeaponId, BODY_PART_TORSO);
    PlayPlayerDamageAnimations(damagedId, BODY_PART_TORSO, issuerId, issuerWeaponId);

    if(issuerWeaponId == WEAPON_UNARMED)
    {
        return 0;
    }
    
    // Doesn't hide weapon if we are hitting someone.
    RestartHideHudTimer(issuerId);

    DegradePlayerMeleeWeapon(issuerId, issuerWeaponId);    
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerShootPlayer ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShootPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount, Float:distance, bodypart)
{
    SCMF(damagedId, -1, "Got shot by another player (%d), with weapon (%d). Damage done: %f to bodypart: %d", issuerId, issuerWeaponId, damageAmount, bodypart);
    SCMF(issuerId, -1, "You shot another player (%d), with weapon (%d). Damage done: %f to bodypart: %d", damagedId, issuerWeaponId, damageAmount, bodypart);
    
    ShowPlayerHitMarker(issuerId, bodypart);
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount, issuerId, issuerWeaponId, bodypart);
    PlayPlayerDamageAnimations(damagedId, bodypart, issuerId, issuerWeaponId, distance);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerGetAttackedByNpc --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetAttackedByNpc(playerid, npcid)
{
	new bool:success = PlayPlayerDamageAnimations(playerid, .issuerId = npcid);

    if(!success)
    {
        return 1;
    }

	SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - NpcDamageValues[_:FCNPC_GetType(npcid)]);
	return 0;
}