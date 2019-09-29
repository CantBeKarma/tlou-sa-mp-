//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- FCNPC_OnPlayingAnim ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public FCNPC_OnPlayingAnim(npcid, animIdx)
{
    GameTextForAllf(500, 3, "~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ ~n~ANIM PLAYING: NPCID (%d)", npcid);

    if(FCNPC_CheckIfAnimationFinished(npcid, animIdx))
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
                SCMF_ALL(-1, "I was distracted. Finished animation, so time to roam to the specific location");
                FCNPC_SetStatus(npcid, STATUS_ROAM);
            }
        }

        default:
        {
            FCNPC_SetStatus(npcid, NPC_STATUS:STATUS_IDLE);
        }
    }
    //printf("FCNPC_OnAnimFinish(%d, %d)", npcid, animIdx);
    return 1;
}