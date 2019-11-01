forward Float:CompressRotation(Float:rotation);
forward Float:GetXYBehindPlayer(playerid, &Float:x, &Float:y, Float:distance);

forward PLAYER_STATUS:GetPlayerStatus(playerid);
forward bool:SetPlayerStatus(playerid, PLAYER_STATUS:status, statusIssuerId = INVALID_PLAYER_ID);
forward bool:ValidateStatusChange(playerid, PLAYER_STATUS:newStatusId, statusIssuerId);

forward Float:SkillsTd_GetStartBarColumnPosX(targetTextdrawBarIdx);

forward NPC_STATUS:FCNPC_GetStatus(npcid);
forward NPC_TYPE:FCNPC_GetType(npcid);
forward bool:FCNPC_ChasePlayer(npcid, playerid);
forward bool:FCNPC_ValidateChasingPlayer(npcid, playerid);
forward bool:FCNPC_SetStatus(npcid, NPC_STATUS:status, statusIssuerId = INVALID_PLAYER_ID);
forward bool:FCNPC_ValidateStatusChange(npcid, NPC_STATUS:newStatusId, statusIssuerId = INVALID_PLAYER_ID);
forward bool:FCNPC_PlayDamageAnimation(npcid, bodypart, issuerWeaponId, Float:distanceBetweenPlayers);
forward bool:FCNPC_CalculatePathToPlayer(npcid, playerid);

forward bool:SetPlayerShivCND(playerid, cnd);
forward bool:DegradePlayerShivCND(playerid);