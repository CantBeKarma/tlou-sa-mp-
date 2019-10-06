forward FCNPC_OnAttackPlayer(npcid, chaseid);
forward FCNPC_OnGetArrowShot(npcid, damagerid);
forward FCNPC_OnGetShived(npcid, shiverId);
forward FCNPC_OnGetHitByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount);
forward FCNPC_OnGetShotByPlayer(npcid, issuerid, issuerWeaponId, Float:damageAmount, Float:distanceBetweenPlayers, bodypart);
forward FCNPC_OnGetHitByThrowable(npcId, issuerId, itemId);
forward FCNPC_OnGetExplodedByNailbomb(npcid, throwerid);
forward FCNPC_OnTriggerNailbomb(npcid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z);
forward FCNPC_OnBurning(npcid);