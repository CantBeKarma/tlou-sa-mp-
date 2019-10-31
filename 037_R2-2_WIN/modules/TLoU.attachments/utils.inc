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
			if(!IsItemType(i, ITEM_TYPE_WEAPON) && !IsItemType(i, ITEM_TYPE_MELEE)
			|| !HasPlayerGotItem(playerid, i))
			{
				continue;
			}

			SPAO(playerid, GetItemModel(i));
		}
	}
	else
	{
		if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE)
		|| !HasPlayerGotItem(playerid, itemId))
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
RemovePlayerAttachedObj(playerid, slotIndex)
{
    if(IsPlayerAttachedObjectSlotUsed(playerid, slotIndex) && slotIndex != -1)
	{
        RemovePlayerAttachedObject(playerid, slotIndex);
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

// Attach models to player (extraInfo is used if there are more than one option for a specific model).
SPAO(playerid, modelId, bool:extraInfo = false)
{
	if(modelId == -1)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: Tried to attach invalid model ID (-1)");
		return false;
	}
		
	new attachmentIndex = FindAttachDataInfo(playerid, modelId, extraInfo);
	if(attachmentIndex == INVALID_ATTACH_SLOT)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: 1. Retrievied invalid attachmentIndex (-1). Skin model: %d | Attach model: %d | Extra info: %d", GetPlayerSkinEx(playerid), modelId, extraInfo);
		return false;
	}

	new slotIndex = eAttachmentsData[attachmentIndex][e_iSlotIndex];
	if(slotIndex == ATTACH_SLOT_DYNAMIC)
	{
		slotIndex = GetDynamicSlotIndexForModel(playerid, modelId);

		if(IsModelFromLsRsWeapMenu(modelId))
		{
			new bool:requireNewAttachDataInfo = (slotIndex == ATTACH_SLOT_SHORT_GUN_2 || slotIndex == ATTACH_SLOT_LONG_GUN_2);
			if(requireNewAttachDataInfo != extraInfo)
			{
				attachmentIndex = FindAttachDataInfo(playerid, modelId, requireNewAttachDataInfo);

				if(attachmentIndex == INVALID_ATTACH_SLOT)
				{
					Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "SPAO: 2. Retrievied invalid attachmentIndex (-1). Skin model: %d | Attach model: %d | Extra info: %d", GetPlayerSkinEx(playerid), modelId, requireNewAttachDataInfo);
					return false;
				}
			}
		}
	}

	if(slotIndex == INVALID_ATTACH_SLOT)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "SPAO: slotIndex is invalid (-1). No free attachment slot is available for player!");
		return false;
	}

	SetPlayerAttachedObject(playerid, slotIndex, modelId, eAttachmentsData[attachmentIndex][e_iBoneId], \
		eAttachmentsData[attachmentIndex][e_fOffsetX], eAttachmentsData[attachmentIndex][e_fOffsetY], eAttachmentsData[attachmentIndex][e_fOffsetZ], \
		eAttachmentsData[attachmentIndex][e_fRotX],    eAttachmentsData[attachmentIndex][e_fRotY],    eAttachmentsData[attachmentIndex][e_fRotZ], \
		eAttachmentsData[attachmentIndex][e_fScaleX],  eAttachmentsData[attachmentIndex][e_fScaleY],  eAttachmentsData[attachmentIndex][e_fScaleZ]);

	return true;
}

// Finds attachment data information in the table and returns its values by reference.
FindAttachDataInfo(playerid, modelId, bool:extraInfo)
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
			return i;
		}
	}
	return INVALID_ATTACH_SLOT;
}

// Get dynamic slot index for model.
//
// Example:
// Left/Right sides of the weapon menu item models are calculated based on currently equipped items (static slot indexes)
// while other items like Molotov, Shiv, Medkit are assigned to indexes 6-9 dynamically
//
// Return values:
// Returning slotIndex for dynamically assigned model id.
GetDynamicSlotIndexForModel(playerid, modelId)
{
	new slotIndex = INVALID_ATTACH_SLOT;

	if(IsModelFromLsRsWeapMenu(modelId))
	{
		slotIndex = FindAttachSlotIdxForLsRsModel(playerid, modelId);
	}
	else
	{
		RemoveDynamicAttachedObj(playerid);
		slotIndex = GetFreeAttachmentSlot(playerid);
		eCharsInfo[playerid][e_iAttachSlot_Dynamic] = slotIndex;
	}
	return slotIndex;
}

// Finds associated attach slot index for Left/Right side weapon menu item models.
//
// Example:
// If first item on right side is 9mm then it will return SHORT_GUN_1 slot and accordingly it will
// attach that model to the player either to thigh or back pocket.
//
// Return values:
// Returning slotIndex for dynamically assigned model id.
FindAttachSlotIdxForLsRsModel(playerid, modelId)
{
	new slotIndex = INVALID_ATTACH_SLOT;

	for(new i = 0; i < 2; i++)
	{
		new slotItemModelId = -1;

		if(modelId == ITEM_MODEL_9MM
		|| modelId == ITEM_MODEL_REVOLVER
		|| modelId == ITEM_MODEL_SHORTY)
		{
			slotItemModelId = GetItemModel(WeapMenu_Rs_ItemId(playerid, i));
		}
		else
		{
			slotItemModelId = GetItemModel(WeapMenu_Ls_ItemId(playerid, i));
		}
		
		if(slotItemModelId != modelId)
		{
			continue;
		}

		if(modelId == ITEM_MODEL_9MM
		|| modelId == ITEM_MODEL_REVOLVER
		|| modelId == ITEM_MODEL_SHORTY)
		{
			slotIndex = (i == 0) ? (ATTACH_SLOT_SHORT_GUN_1) : (ATTACH_SLOT_SHORT_GUN_2);
		}
		else
		{
			slotIndex = (i == 0) ? (ATTACH_SLOT_LONG_GUN_1) : (ATTACH_SLOT_LONG_GUN_2);
		}

		return slotIndex;
	}
	return INVALID_ATTACH_SLOT;
}

// Detach weapon models from player's body.
DetachWeaponFromPlayersBody(playerid, itemId)
{
	if(!IsItemType(itemId, ITEM_TYPE_BULLET_WEAPON) // Don't detach BOW because it's attached custom weapon
	&& !IsItemType(itemId, ITEM_TYPE_MELEE))
	{
		return false;
	}

    new 
		slotIndex = INVALID_ATTACH_SLOT,
		itemModel = GetItemModel(itemId);

	for(new i = 0; i != MAX_ATTACHMENTS_DATA; i++)
	{
		if(itemModel != eAttachmentsData[i][e_iModelId])
		{
			continue;
		}

		slotIndex = eAttachmentsData[i][e_iSlotIndex];
		break;
	}

	if(IsModelFromLsRsWeapMenu(itemModel))
	{
		slotIndex = FindAttachSlotIdxForLsRsModel(playerid, itemModel);
	}

	if(slotIndex == INVALID_ATTACH_SLOT)
	{
		Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "DetachWeaponFromPlayersBody: Retrievied invalid slotId (-1) for item model: %d", GetItemModel(itemId));
		return false;
	}

    RemovePlayerAttachedObj(playerid, slotIndex);
	return true;
}