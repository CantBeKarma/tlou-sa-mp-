forward PLAYER_STATUS:GetPlayerStatus(playerid);
forward bool:SetPlayerStatus(playerid, PLAYER_STATUS:status, statusIssuerId = INVALID_PLAYER_ID);
forward bool:ValidateStatusChange(playerid, PLAYER_STATUS:newStatusId, statusIssuerId);

forward Float:SkillsTd_GetStartBarColumnPosX(targetTextdrawBarIdx);

forward NPC_STATUS:FCNPC_GetStatus(npcid);
forward NPC_TYPE:FCNPC_GetType(npcid);
forward bool:FCNPC_ValidateStatusChange(npcid, NPC_STATUS:newStatusId, statusIssuerId = INVALID_PLAYER_ID);

