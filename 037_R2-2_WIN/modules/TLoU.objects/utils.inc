// Reads single item data relation and returns its value to variable sent by reference.
// May be used to return many relations/data.
ReadItemRelation(item_objectId, &itemUid = -1, &itemId = -1, &itemTypeId = -1, &Float:itemPosX = 1.0, &Float:itemPosY = 1.0, &Float:itemPosZ = 1.0, &itemVw = -1, &itemIntId = -1, \
    &itemAmount = -1, &itemExtAmount = -1, &itemAreaId = -1, &itemCreated = -1)
{
    if(!IsValidDynamicObject(item_objectId))
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], WARNING, "ReadItemRelation: Tried to read invalid object ID data");
        return 1;
    }

    Streamer_GetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

    if(itemUid != -1)
    {
        itemUid = eObjData[e_iObj_Uid];
    }

    if(itemId != -1)
    {
        itemId = eObjData[e_iObj_ItemId];
    }

    if(itemTypeId != -1)
    {
        itemTypeId = eObjData[e_iObj_TypeId];
    }

    if(itemPosX != 1.0)
    {
        itemPosX = eObjData[e_fObj_X];
    }

    if(itemPosY != 1.0)
    {
        itemPosY = eObjData[e_fObj_Y];
    }

    if(itemPosZ != 1.0)
    {
        itemPosZ = eObjData[e_fObj_Z];
    }

    if(itemVw != -1)
    {
        itemVw = eObjData[e_iObj_Vw];
    }

    if(itemIntId != -1)
    {
        itemIntId = eObjData[e_iObj_Int];
    }

    if(itemAmount != -1)
    {
        itemAmount = eObjData[e_iObj_ItemAmount];
    }

    if(itemExtAmount != -1)
    {
        itemExtAmount = eObjData[e_iObj_MeleeUpgradedCnd];
    }

    if(itemAreaId != -1)
    {
        itemAreaId = eObjData[e_iObj_SpriteAreaId];
    }

    if(itemCreated != -1)
    {
        itemCreated = eObjData[e_bObj_Created];
    }

    return 0;
}