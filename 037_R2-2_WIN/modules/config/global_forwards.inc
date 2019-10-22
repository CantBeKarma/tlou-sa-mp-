forward Float:CompressRotation(Float:rotation);
forward Float:GetXYBehindPlayer(playerid, &Float:x, &Float:y, Float:distance);

forward PLAYER_STATUS:GetPlayerStatus(playerid);
forward bool:SetPlayerStatus(playerid, PLAYER_STATUS:status, statusIssuerId = INVALID_PLAYER_ID);
forward bool:ValidateStatusChange(playerid, PLAYER_STATUS:newStatusId, statusIssuerId);

forward Float:SkillsTd_GetStartBarColumnPosX(targetTextdrawBarIdx);

forward NPC_STATUS:FCNPC_GetStatus(npcid);
forward NPC_TYPE:FCNPC_GetType(npcid);
forward bool:FCNPC_ChasePlayer(npcid, playerid);
forward bool:FCNPC_GoToPlayerOnGroundCol(npcid, playerid, walkOrRun = 0, type = FCNPC_MOVE_TYPE_AUTO, mode = FCNPC_MOVE_MODE_AUTO, bool:set_angle = true, stopdelay = 250);
forward bool:FCNPC_SetStatus(npcid, NPC_STATUS:status, statusIssuerId = INVALID_PLAYER_ID);
forward bool:FCNPC_ValidateStatusChange(npcid, NPC_STATUS:newStatusId, statusIssuerId = INVALID_PLAYER_ID);

forward bool:SetPlayerShivCND(playerid, cnd);
forward bool:DegradePlayerShivCND(playerid);