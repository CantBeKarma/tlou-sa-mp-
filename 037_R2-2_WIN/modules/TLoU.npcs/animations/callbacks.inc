//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnPlayingAnim ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnPlayingAnim(npcid, animIdx)
{
    if(!FCNPC_HasCallbackAnimFinished(npcid, animIdx))
    {
        eNpcInfo[npcid][e_iPlayingAnim_Tick]++;
    }
    else
    {
        FCNPC_OnAnimFinish(npcid, animIdx);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- FCNPC_OnAnimFinish ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnAnimFinish(npcid, animIdx)
{
    if(FCNPC_GetAnimationIndex(npcid) != animIdx)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_NPC], WARNING, "FCNPC_OnAnimFinish: NPC (ID: %d) was supposed to finish animation ID: %d (instead current NPC animation ID is: %d)", npcid, animIdx, FCNPC_GetAnimationIndex(npcid));
        return 1;
    }

    switch(animIdx)
    {
        // Flew backwards on impact by shotgun shot.
        case BASEBALL_BAT_HIT_3:
        {
            if(!FCNPC_IsDeadEx(npcid))
            {
                FCNPC_SetStatus(npcid, STATUS_DISABLED);
                FCNPC_SetAnimCallback(npcid, PED_GETUP);
            }
        }

        // IDLE animations.
        case 
            PED_DAM_ARML_FRMFT,
            PED_DAM_ARMR_FRMFT,
            FLOWERS_FLOWER_HIT,
            KISSING_GF_CARARGUE_02:
        {
            FCNPC_StopUpdateTimer(npcid);
            FCNPC_SetStatus(npcid, STATUS_IDLE);
        }

        default:
        {
            FCNPC_SetStatus(npcid, STATUS_IDLE);
        }
    }
    return 1;
}