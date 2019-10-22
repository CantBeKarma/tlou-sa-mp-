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
    
    SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE, issuerid);

    if(issuerid == INVALID_PLAYER_ID)
    {
        OnPlayerDamageHimself(playerid, amount);
        return 1;
    }

    new issuerItemId = GetPlayerEquippedItemId(issuerid);
    new Float:distanceBetweenPlayers = 0.0;
    new Float:damageAmount = CalculateDamageAmount(issuerid, playerid, issuerItemId, distanceBetweenPlayers, bodypart);

    if(IsItemType(issuerItemId, ITEM_TYPE_MELEE))
    {
        OnPlayerHitPlayer(playerid, issuerid, issuerItemId, damageAmount);
    }
    else if(IsItemType(issuerItemId, ITEM_TYPE_BULLET_WEAPON))
    {
        OnPlayerShootPlayer(playerid, issuerid, issuerItemId, damageAmount, distanceBetweenPlayers, bodypart);
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
public OnPlayerHitPlayer(damagedId, issuerId, issuerItemId, Float:damageAmount)
{
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount, issuerId, issuerItemId, BODY_PART_TORSO);
    PlayPlayerDamageAnimations(damagedId, BODY_PART_TORSO, issuerId, issuerItemId);

    // Doesn't hide weapon if we are hitting someone.
    RestartHideHudTimer(issuerId);
    DegradePlayerMeleeWeapon(issuerId);    
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnPlayerShootPlayer ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerShootPlayer(damagedId, issuerId, issuerItemId, Float:damageAmount, Float:distance, bodypart)
{
    SCMF(damagedId, -1, "Got shot by another player (%d), with item (%d). Damage done: %f to bodypart: %d", issuerId, issuerItemId, damageAmount, bodypart);
    SCMF(issuerId, -1, "You shot another player (%d), with item (%d). Damage done: %f to bodypart: %d", damagedId, issuerItemId, damageAmount, bodypart);
    
    ShowPlayerHitMarker(issuerId, bodypart);
    SetPlayerHealthEx(damagedId, GetPlayerHealthEx(damagedId) - damageAmount, issuerId, issuerItemId, bodypart);
    PlayPlayerDamageAnimations(damagedId, bodypart, issuerId, issuerItemId, distance);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerGetAttackedByNPC --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerGetAttackedByNPC(playerid, npcid)
{
	new bool:success = PlayPlayerDamageAnimations(playerid, .issuerId = npcid);

    if(!success)
    {
        return 1;
    }

	SetPlayerHealthEx(playerid, GetPlayerHealthEx(playerid) - NpcDamageValues[_:FCNPC_GetType(npcid)]);
	return 0;
}