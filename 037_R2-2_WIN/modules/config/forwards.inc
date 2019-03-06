forward OnPlayerCrouchBug(playerid);
forward OnPlayerBunnyHop(playerid);
forward OnPlayerAnimFinish(playerid, animIdx);
forward OnPlayerFirstSpawn(playerid);
forward OnPlayerRespawn(playerid);
forward OnPlayerPrepareToSign(playerid);
forward OnPlayerDeathEx(playerid, issuerid, weaponid, bodypart);
forward OnPlayerWeaponChange(playerid, newWeaponId, oldWeaponId);
forward OnPlayerTargetPlayer(playerid, newTargetId, oldTargetId);
forward OnPlayerLookAtPlayer(playerid, lookingAt_playerId); 
forward OnPlayerLookAtNPC(playerid, lookingAt_NpcId);
forward OnPlayerShivPlayer(playerid, shivedId);

forward OnPlayerDamageHimself(playerid, Float:damageAmount);
forward OnPlayerHitPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount);
forward OnPlayerShootPlayer(damagedId, issuerId, issuerWeaponId, Float:damageAmount, Float:distance, bodypart);
forward OnPlayerGetAttackedByNpc(playerid, npcid);

forward OnPlayerHoldingKey(playerid, key);
forward OnPlayerReleaseKey(playerid, key, holding_time);

forward OnPlayerStartedHealing(playerid);
forward OnPlayerIsHealing(playerid, Float:oldHealth, Float:destHealth);
forward OnPlayerCanceledHealing(playerid, Float:oldHealth);
forward OnPlayerHealed(playerid, Float:oldHealth, Float:newHealth);
forward OnPlayerHealthChange(playerid, Float:oldHealth, Float:newHealth);

forward OnPlayerStartBurning(playerid, fireCreatorId);
forward OnPlayerStopBurning(playerid);

forward OnPlayerOpenBackpack(playerid, category);
forward OnPlayerCloseBackpack(playerid);
forward OnPlayerUseBackpack(playerid, category, keys);
forward OnPlayerSwitchBpCtgry(playerid, leftRight);
forward OnPlayerStartedCrafting(playerid, craftItemId);
forward OnPlayerCanceledCrafting(playerid);
forward OnPlayerCraftedItem(playerid, craftItemId);

forward OnPlayerStartedUpgradingSkill(playerid, skillid);
forward OnPlayerCanceledUpgradingSkill(playerid);
forward OnPlayerUpgradedSkill(playerid, skillid);

forward OnPlayerOpenWeaponMenu(playerid);
forward OnPlayerCloseWeaponMenu(playerid);
forward OnPlayerUD_WeaponMenu(playerid, keys);
forward OnPlayerLR_WeaponMenu(playerid, keys);
forward OnPlayerUD_SwapWeaps(playerid, keys);
forward OnPlayerStartSwappingWeaps(playerid);
forward OnPlayerFinishSwappingWeaps(playerid);

forward OnPlayerTryAimingWeapon(playerid, weaponId);
forward OnPlayerAimsWeapon(playerid, weaponId);
forward OnPlayerStopAimingWeapon(playerid);
forward OnPlayerStartAimingBulletWeapon(playerid);
forward OnPlayerStopAimingBulletWeapon(playerid);
forward OnPlayerReloadWeapon(playerid, weaponId, oldClipAmmo, newClipAmmo);
forward OnPlayerWeaponShoot(playerid, weaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ);

forward OnPlayerStartAimingBow(playerid);
forward OnPlayerStopAimingBow(playerid);
forward OnPlayerShotArrow(playerid);
forward OnPlayerGetArrowShot(playerid, damagerid);

forward OnPlayerStartAimingProjectile(playerid, weaponId);
forward OnPlayerStopAimingProjectile(playerid);
forward OnPlayerThrowProjectile(playerid, weaponId);
forward OnPlayerDropNailbomb(playerid);
forward OnPlayerTriggerNailbomb(triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z);
forward OnPlayerGetStunned(playerid);

forward OnMolotovExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);
forward OnSmokebombExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);
forward OnThrowableDestroy(projid, throwerid, hitid, weaponid, Float:x, Float:y, Float:z);
forward OnNailbombInitiate(nailBmbIdx, throwerid, Float:x, Float:y, Float:z);

forward FCNPC_OnIdle(npcid);
forward FCNPC_OnRoam(npcid, Float:target_x, Float:target_y, Float:target_z);
forward FCNPC_OnChasePlayer(npcid, chaseid);
forward FCNPC_OnChasePlayerByPath(npcid, chaseid, pathid);
forward FCNPC_OnAttackPlayer(npcid, chaseid);
forward FCNPC_OnAnimFinish(npcid, animIdx);
forward FCNPC_OnPlayingAnim(npcid, animIdx);
forward FCNPC_OnDeathEx(npcid, killerid, weaponid, bodypart);
forward FCNPC_OnStartBurning(npcid);
forward FCNPC_OnBurning(npcid);
forward FCNPC_OnGetArrowShot(npcid, damagerid);
forward FCNPC_OnGetShived(npcid, shiverId);
forward FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount);
forward FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount, Float:distanceBetweenPlayers, bodypart);
forward FCNPC_OnGetDistracted(npcid, Float:x, Float:y, Float:z);
forward FCNPC_OnTriggerNailbomb(npcid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z);
forward FCNPC_OnGetStunned(npcid);
forward FCNPC_OnShakeOffStun(npcid);
forward FCNPC_OnEnterDynArea(npcid, areaid);
forward FCNPC_OnLeaveDynArea(npcid, areaid);

forward OnPlayerLookAtItem(playerid, item_objectId);
forward OnPlayerPickUpItem(playerid, item_objectId);

forward OnItemCreate(itemId, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId);
forward OnItemDestroy(uid_item);
forward OnItemUpdate(updateItemUid, newItemId, newItemAmount, newMeleeExtCnd, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, updaterId);