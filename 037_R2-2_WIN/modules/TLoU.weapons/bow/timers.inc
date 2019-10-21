timer OnArrowObjectUpdate[TIMER_ARROW_UPDATE_RATE](shooterId, arrowObjectId)
{
    new Float:x, Float:y, Float:z;
    GetDynamicObjectPos(arrowObjectId, x, y, z);

    if(!IsDynamicObjectMoving(arrowObjectId))
    {
        DestroyDynamicObject(arrowObjectId);
        Random_CreateArrowItem(x, y, z);
        return 0;
    }

    new shotEntity = GetEntityInRangeOfArrow(x, y, z, shooterId);
    if(shotEntity == INVALID_PLAYER_ID)
    {
        defer OnArrowObjectUpdate(shooterId, arrowObjectId);
    }
    else
    {
        if(IsPlayerNPC(shotEntity))
        {
            FCNPC_OnGetArrowShot(shotEntity, shooterId);
        }
        else
        {
            OnPlayerGetArrowShot(shotEntity, shooterId);
        }
        
        DestroyDynamicObject(arrowObjectId);
    }
    return 0;
}