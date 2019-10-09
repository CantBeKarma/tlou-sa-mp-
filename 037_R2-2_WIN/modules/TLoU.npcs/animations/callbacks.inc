//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnPlayingAnim ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnPlayingAnim(npcid, animIdx)
{
    if(!FCNPC_HasCallbackAnimFinished(npcid, animIdx))
    {
        eNpcInfo[npcid][e_iPlayingAnim_Tick]++;
        //GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM PLAYING: NPCID (%d)", npcid);
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
    //GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM FINISH: NPCID (%d)", npcid);
    if(FCNPC_GetAnimationIndex(npcid) != animIdx)
    {
        return 1;
    }

    switch(animIdx)
    {
        // Flew backwards on impact by shotgun shot.
        case BASEBALL_BAT_HIT_3:
        {
            if(!FCNPC_IsDeadEx(npcid))
            {
                FCNPC_SetAnimation(npcid, PED_GETUP, 4.1, 0, 1, 1, 0, 0);
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
    //printf("FCNPC_OnAnimFinish(%d, %d)", npcid, animIdx);
    return 1;
}