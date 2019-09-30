//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnPlayingAnim ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnPlayingAnim(npcid, animIdx)
{
    GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM PLAYING: NPCID (%d)", npcid);

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
    GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM FINISH: NPCID (%d)", npcid);

    switch(animIdx)
    {
        // Knifed by other player.
        case KNIFE_KILL_KNIFE_PED_DAMAGE:
        {
            FCNPC_SetAnimation(npcid, KNIFE_KILL_KNIFE_PED_DIE, 4.1, 0, 1, 1, 1, 0);
        }

        // Shot in stomach by shotgun (flew back on impact).
        case BASEBALL_BAT_HIT_3:
        {
            FCNPC_SetAnimCallback(npcid, PED_GETUP);
        }

        case PED_FLEE_LKAROUND_01:
        {
            if(FCNPC_IsDistracted(npcid))
            {
                FCNPC_GoToOnGroundCol(npcid, eNpcInfo[npcid][e_fDistractedX], eNpcInfo[npcid][e_fDistractedY], eNpcInfo[npcid][e_fDistractedZ], FCNPC_MOVE_RUN);
            }
        }

        default:
        {
            FCNPC_SetStatus(npcid, STATUS_IDLE);
        }
    }
    //printf("FCNPC_OnAnimFinish(%d, %d)", npcid, animIdx);
    return 1;
}