// Attach : attach backpack to player's back (happens during respawn).
AttachBackpackToPlayer(playerid, bool:isInFrontOfPlayer = false)
{
	switch(GetPlayerSkinEx(playerid))
	{
		case SKIN_JOEL:
			SPAO(playerid, OBJECT_MODEL_JOELBP, isInFrontOfPlayer);

		case SKIN_ELLIE, SKIN_ELLIE2:
			SPAO(playerid, OBJECT_MODEL_ELLIEBP, isInFrontOfPlayer);

		case SKIN_TESS:
			SPAO(playerid, OBJECT_MODEL_TESSBP, isInFrontOfPlayer);

		default:
			SPAO(playerid, OBJECT_MODEL_BILLBP, isInFrontOfPlayer);
	}
}

// Attach : attach weapons to player (happens during respawn).
AttachItemsToPlayer(playerid, itemId = -1)
{
	if(itemId == -1)
	{
		for(new i = 0; i != MAX_ITEMS_DATA; i++)
		{
			if(!IsItemType(i, ITEM_TYPE_WEAPON) && !IsItemType(i, ITEM_TYPE_MELEE))
			{
				continue;
			}

			if(!HasPlayerGotItem(playerid, i))
			{
				continue;
			}

			SPAO(playerid, GetItemModel(i));
		}
	}
	else
	{
		if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
		{
			return false;
		}

		if(!HasPlayerGotItem(playerid, itemId))
		{
			return false;
		}

		SPAO(playerid, GetItemModel(itemId));
	}

	return true;
}

// Gets a free available attachment slot.
GetFreeAttachmentSlot(playerid)
{
    for(new i = MIN_ATTACH_SLOT_START; i < MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
        if(!IsPlayerAttachedObjectSlotUsed(playerid, i))
		{
			return i;
		}
    }
    return INVALID_ATTACH_SLOT;
}

// Check if attached object slot is used - if so, remove attached object.
RemovePlayerAttachedObj(playerid, slotId)
{
    if(IsPlayerAttachedObjectSlotUsed(playerid, slotId) && slotId != -1)
	{
        RemovePlayerAttachedObject(playerid, slotId);
	}
}

// Attach models to player (extraInfo is used if there are more than one option for a specific model).
SPAO(playerid, modelId, bool:extraInfo = false)
{
	if(modelId == -1)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: Tried to attach invalid model ID (-1)");
		return false;
	}
		
	new 
		slotIndex = -1, 
		attachmentIndex = -1;

	FindAttachDataInfo(playerid, modelId, extraInfo, slotIndex, attachmentIndex);

	if(attachmentIndex == -1)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: Retrievied invalid attachmentIndex (-1) for skin model: %d - attach model: %d", GetPlayerSkinEx(playerid), modelId);
		return false;
	}

	if(slotIndex == -1)
	{
		RemoveDynamicAttachedObj(playerid);
		FindFreeDynamicAttachSlotIdx(playerid, slotIndex);

		if(slotIndex == -1)
		{
			Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SPAO: slotIndex is invalid (-1). No free attachment slot is available for player!");
			return false;
		}
	}

	SetPlayerAttachedObject(playerid, slotIndex, eAttachmentsData[attachmentIndex][e_iModelId], eAttachmentsData[attachmentIndex][e_iBoneId], \
		eAttachmentsData[attachmentIndex][e_fOffsetX], eAttachmentsData[attachmentIndex][e_fOffsetY], eAttachmentsData[attachmentIndex][e_fOffsetZ], \
		eAttachmentsData[attachmentIndex][e_fRotX],    eAttachmentsData[attachmentIndex][e_fRotY],    eAttachmentsData[attachmentIndex][e_fRotZ], \
		eAttachmentsData[attachmentIndex][e_fScaleX],  eAttachmentsData[attachmentIndex][e_fScaleY],  eAttachmentsData[attachmentIndex][e_fScaleZ]);

	return true;
}

// Finds attachment data information in the table and returns its values by reference.
FindAttachDataInfo(playerid, modelId, bool:extraInfo, &slotIndex, &attachmentIndex)
{
	for(new i = 0; i != MAX_ATTACHMENTS_DATA; i++)
	{
		if(eAttachmentsData[i][e_iModelId] != modelId 
		|| eAttachmentsData[i][e_bExtraInfo] != extraInfo)
		{
			continue;
		}

		if(eAttachmentsData[i][e_iSkinId] == GetPlayerSkinEx(playerid) 
		|| eAttachmentsData[i][e_iSkinId] == -1)
		{
			attachmentIndex = i;

			slotIndex = eAttachmentsData[i][e_iSlotId];
			break;
		}
	}
}

// Removes dynamically attached object to the player if used
RemoveDynamicAttachedObj(playerid)
{
	new playersDynamicSlotId = eCharsInfo[playerid][e_iAttachSlot_Dynamic];
	if(IsPlayerAttachedObjectSlotUsed(playerid, playersDynamicSlotId))
	{
		RemovePlayerAttachedObj(playerid, playersDynamicSlotId);
	}
}

// Finds free dynamic attachment slot for player (between 6-9 slots).
FindFreeDynamicAttachSlotIdx(playerid, &slotIndex)
{
	slotIndex = GetFreeAttachmentSlot(playerid);
	if(slotIndex == -1)
	{
		return false;
	}

	eCharsInfo[playerid][e_iAttachSlot_Dynamic] = slotIndex;
	return true;
}

// Detach weapon models from player's body.
DetachWeaponFromPlayersBody(playerid, itemId)
{
	if(!IsItemType(itemId, ITEM_TYPE_BULLET_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
	{
		return false;
	}

    new slotId = -1;

	for(new i = 0; i != MAX_ATTACHMENTS_DATA; i++)
	{
		if(GetItemModel(itemId) != eAttachmentsData[i][e_iModelId])
		{
			continue;
		}

		slotId = eAttachmentsData[i][e_iSlotId];
		break;
	}

	if(slotId == -1)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: Retrievied invalid slotId (-1) for item model: %d", GetItemModel(itemId));
		return false;
	}

    RemovePlayerAttachedObj(playerid, slotId);
	return true;
}