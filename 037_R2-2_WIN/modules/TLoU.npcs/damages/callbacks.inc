//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnTakeDamage ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnTakeDamage(npcid, issuerid, Float:amount, weaponid, bodypart)
{
    if(FCNPC_GetStatus(npcid) == NPC_STATUS:STATUS_DISABLED)
    {
        return 0;
    }
    
    if(FCNPC_IsDeadEx(npcid))
    {
        FCNPC_PlayDeadAnimIfDead(npcid);
        return 0;
    }

    new playerItemId = GetPlayerEquippedItemId(issuerid);
    new issuerWeaponId = GetItemNativeWeaponId(playerItemId);
    new Float:distanceBetweenPlayers = 0.0;
    new Float:damageAmount = CalculateDamageAmount(issuerid, npcid, playerItemId, distanceBetweenPlayers, bodypart);

    if(IsItemType(playerItemId, ITEM_TYPE_MELEE))
    {
        FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, damageAmount);
    }
    else if(IsItemType(playerItemId, ITEM_TYPE_BULLET_WEAPON))
    {
        FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, damageAmount, distanceBetweenPlayers, bodypart);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnGetHitByPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount)
{
    // if(FCNPC_IsPlayingAnim(npcid))
    // {
    FCNPC_ClearAnims(npcid);
    //}
    FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - damageAmount, issuerid, issuerWeaponId);

    if(issuerWeaponId == WEAPON_UNARMED)
    {
        return 0;
    }
    
    // Doesn't hide weapon if we are hitting someone.
    RestartHideHudTimer(issuerid);
    DegradePlayerMeleeWeapon(issuerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ FCNPC_OnGetShotByPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount, Float:distanceBetweenPlayers, bodypart)
{
    SCMF(issuerid, -1, "You shot FCNPC (%d) with weapon (%d). Damage done: %f to bodypart: %d on distance: %f", npcid, issuerWeaponId, damageAmount, bodypart, distanceBetweenPlayers);

    ShowPlayerHitMarker(issuerid, bodypart);

    if(bodypart == BODY_PART_HEAD)
    {
        if(FCNPC_GetType(npcid) == TYPE_RUNNER
        || FCNPC_GetType(npcid) == TYPE_STALKER)
        {
            FCNPC_SetHealthEx(npcid, 0.0, issuerid, issuerWeaponId, bodypart);
            return 0;
        }
    }

    FCNPC_SetHealthEx(npcid, (FCNPC_GetHealthEx(npcid) - damageAmount), issuerid, issuerWeaponId, bodypart);
    FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId, distanceBetweenPlayers);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnGetShived ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetShived(npcid, shiverId)
{
    // if(FCNPC_ValidateStatusChange(npcid, STATUS_PLAYING_ANIM) == false)
    // {
    //     SCMF(shiverId, COLOR_RED, "Unable to shiv NPC!");
    //     return 1;
    // }

    EquipItem(shiverId, _:ITEM_IDX_SHIV);
    ApplyCallbackAnim(shiverId, "KNIFE", "KILL_KNIFE_PLAYER");
    FCNPC_SetHealthEx(npcid, 0.0, shiverId, .weaponid = WEAPON_SHIV);

    new Float:x, Float:y, Float:z;
    GetPlayerPos(shiverId, x, y, z);
    Audio3D_Play(shiverId, RandomEx(AUDIO_CLICKER_DEATH01, AUDIO_CLICKER_DEATH03), x, y, z, DIST3D_CLICKER_SHIVED, .downmix = true);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnGetArrowShot --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetArrowShot(npcid, damagerid)
{
    ShowPlayerHitMarker(damagerid, BODY_PART_TORSO);
    FCNPC_PlayDamageAnimation(npcid, BODY_PART_TORSO);
    FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - PlayerWeaponBodyDamage[damagerid], damagerid, WEAPON_BOW);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ FCNPC_OnGetHitByThrowable ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetHitByThrowable(npcId, issuerId, itemId)
{
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnAttackPlayer --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnAttackPlayer(npcid, chaseid)
{
	if(GetDistanceBetweenPlayers(npcid, chaseid) > RANGE_NPC_ATTACK
	|| IsPlayerDead(chaseid))
	{
		FCNPC_SetStatus(npcid, STATUS_IDLE);
		return 1;
	}

	SetPlayerToFacePlayer(npcid, chaseid);
	FCNPC_SetAnimation(npcid, NpcAttackAnims[random(sizeof(NpcAttackAnims))], 4.1, 1, 0, 0, 0, 0);

	OnPlayerGetAttackedByNpc(chaseid, npcid);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnBurning ------------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnBurning(npcid)
{
	FCNPC_SetAnimation(npcid, PED_FLOOR_HIT_F, 4.1, 1, 1, 1, 0, 0);
	FCNPC_SetHealthEx(npcid, FCNPC_GetHealthEx(npcid) - FIRE_DAMAGE_AMOUNT, .weaponid = WEAPON_MOLOTOV);
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- FCNPC_OnTriggerNailbomb -------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnTriggerNailbomb(npcid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z)
{
	ApplyAnimation(npcid, "PED", "EV_STEP", 4.1, 0, 1, 1, 0, 0, 0);
	Audio3D_Play(throwerid, AUDIO_NAILBMB_TRIGGER, x, y, z, 10.0);
	defer OnNailbombExplode(npcid, throwerid, nailBmbIdx, x, y, z);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- FCNPC_OnGetExplodedByNailbomb -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnGetExplodedByNailbomb(npcid, throwerid)
{
	return 0;
}