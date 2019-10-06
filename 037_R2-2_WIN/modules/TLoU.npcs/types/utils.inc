// Get the type of an NPC.
NPC_TYPE:FCNPC_GetType(npcid)
{
    return eNpcInfo[npcid][e_iType];
}

// Set NPC to the specific type and change its skin automatically.
FCNPC_SetType(npcid, NPC_TYPE:type)
{
    if(FCNPC_GetType(npcid) == type)
    {
        return true;
    }

    if(FCNPC_SetSkinBasedOnType(npcid, type) == 0)
    {
        return false;
    }
    
    eNpcInfo[npcid][e_iType] = type;
    Log(eLogs[e_Log_Index][LOG_INDEX_NPC], INFO, "FCNPC_SetType: Successfully changed type of NPC (ID: %d) to %s", npcid, NpcTypeNames[_:type]);
    return true;
}

// Set NPC new random skin based on its type.
FCNPC_SetSkinBasedOnType(npcid, NPC_TYPE:type)
{
    new skinId = 0;

    switch(type)
    {
        case TYPE_RUNNER:	skinId = NpcSkinIds[0];
        case TYPE_STALKER:	skinId = NpcSkinIds[1];
        case TYPE_CLICKER:	skinId = NpcSkinIds[2];
        case TYPE_BLOATER:	skinId = NpcSkinIds[3];
    }

    if(skinId == 0)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_NPC], WARNING, "FCNPC_SetSkinBasedOnType: Unable to set skin for NPCID: %d (no skin found for type: %s)", npcid, NpcTypeNames[_:type]);
        return 0;
    }

    SetPlayerSkin(npcid, skinId);
    return skinId;
}