forward OnPlayerBunnyHop(playerid);
forward OnPlayerCrouchBug(playerid);

forward OnPlayerFirstSpawn(playerid);
forward OnPlayerRespawn(playerid);
forward OnPlayerDie(playerid, issuerid, itemId, bodypart);

forward OnPlayerStartedHealing(playerid);
forward OnPlayerIsHealing(playerid, Float:oldHealth, Float:destHealth);
forward OnPlayerCanceledHealing(playerid, Float:oldHealth);
forward OnPlayerHealed(playerid, Float:oldHealth, Float:newHealth);
forward OnPlayerHealthChange(playerid, Float:oldHealth, Float:newHealth);

forward OnPlayerTargetPlayer(playerid, newTargetId, oldTargetId);

forward OnPlayerLookAtPlayer(playerid, lookingAt_playerId); 
forward OnPlayerLookAtNPC(playerid, lookingAt_NpcId);

forward OnPlayerGetStunned(playerid);
forward OnPlayerShakeOffStun(playerid);

forward OnPlayerGetHitByThrowable(playerid, issuerId, itemId);
forward [16]GetPlayerIpAddress(playerid);