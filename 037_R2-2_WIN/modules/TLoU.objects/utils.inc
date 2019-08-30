// Reads all item data relation and returns its values to variables sent by reference.
ReadItemRelations(item_objectId, &itemUid, &itemId, &Float:itemPosX, &Float:itemPosY, &Float:itemPosZ, &itemVw, &itemIntId, \
    &itemAmount, &itemExtAmount, &itemCreated, &itemModel, &ITEM_TYPE:itemType, &itemWeaponId)
{
    Streamer_GetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

    itemUid = eObjData[e_iObj_Uid];
    itemId = eObjData[e_iObj_ItemId];
    itemPosX = eObjData[e_fObj_X];
    itemPosY = eObjData[e_fObj_Y];
    itemPosZ =  eObjData[e_fObj_Z];
    itemVw = eObjData[e_iObj_Vw];
    itemIntId = eObjData[e_iObj_Int];
    itemAmount = eObjData[e_iObj_ItemAmount];
    itemExtAmount = eObjData[e_iObj_MeleeUpgradedCnd];
    itemCreated = eObjData[e_bObj_Created];
    itemModel = eItemsData[itemId][e_iItem_ModelId];
    itemType = ITEM_TYPE:eItemsData[itemId][e_iItem_Type];
    itemWeaponId = (itemType == ITEM_TYPE:ITEM_TYPE_AMMO) ? GetWeaponIdForAmmoType(itemModel) : eItemsData[itemId][e_iItem_SA_WeaponId];
}

// Reads single item data relation and returns its value to variable sent by reference.
// May be used to return many relations/data.
ReadItemRelation(item_objectId, &itemUid = -1, &itemId = -1, &Float:itemPosX = 1.0, &Float:itemPosY = 1.0, &Float:itemPosZ = 1.0, &itemVw = -1, &itemIntId = -1, \
    &itemAmount = -1, &itemExtAmount = -1, &itemCreated = -1, &itemModel = -1, &ITEM_TYPE:itemType = ITEM_TYPE:-1, &itemWeaponId = -1)
{
    Streamer_GetArrayData(STREAMER_TYPE_OBJECT, item_objectId, E_STREAMER_EXTRA_ID, eObjData);

    if(itemUid != -1)
    {
        itemUid = eObjData[e_iObj_Uid];
    }

    if(itemId != -1)
    {
        itemId = eObjData[e_iObj_ItemId];
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

    if(itemCreated != -1)
    {
        itemCreated = eObjData[e_bObj_Created];
    }

    if(itemModel != -1)
    {
        itemModel = eItemsData[ eObjData[e_iObj_ItemId] ][e_iItem_ModelId];
    }

    if(itemType != ITEM_TYPE:-1)
    {
        itemType = ITEM_TYPE:eItemsData[ eObjData[e_iObj_ItemId] ][e_iItem_Type];
    }

    if(itemWeaponId != -1)
    {
        itemId = eObjData[e_iObj_ItemId];
        itemModel = eItemsData[itemId][e_iItem_ModelId];
        itemType = ITEM_TYPE:eItemsData[itemId][e_iItem_Type];
        itemWeaponId = (itemType == ITEM_TYPE:ITEM_TYPE_AMMO) ? GetWeaponIdForAmmoType(itemModel) : eItemsData[itemId][e_iItem_SA_WeaponId];
    }
}