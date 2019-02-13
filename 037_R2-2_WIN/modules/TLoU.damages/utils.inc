// Apply animation when shot in close-range with shotgun.
FlyBackwardsWhenShot(damagedid)
{
    if(IsPlayerNPC(damagedid))
    {
        if(eNpcInfo[damagedid][e_iType] == NPC_TYPE_BLOATER) 
            return 1;

        FCNPC_StopUpdateTimer(damagedid);
        FCNPC_SetAnimation(damagedid, BASEBALL_BAT_HIT_3, 4.1, 0, 1, 1, 1, 0);

        if(!FCNPC_IsDeadEx(damagedid))
            defer DelayedTimer_AnimGetUp[1000](damagedid);
    }
    else 
    {
        ApplyAnimation(damagedid, "BASEBALL", "BAT_HIT_3", 4.0, 0, 1, 1, 1, 0, 1);

        if(!IsPlayerDead(damagedid))
            defer DelayedTimer_AnimGetUp[1000](damagedid);
    }
    return 0;
}