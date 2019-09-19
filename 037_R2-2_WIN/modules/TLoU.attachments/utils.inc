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
    for(new i = 0; i != MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
        if(!IsPlayerAttachedObjectSlotUsed(playerid, i))
			return i;
    }
    return -1;
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
		return;	
	}
		
	new 
		slotIndex = -1, 
		attachmentIndex = -1;

	for(new i = 0; i != MAX_ATTACHMENTS_DATA; i++)
	{
		if(eAttachmentsData[i][e_iModelId] != modelId || eAttachmentsData[i][e_bExtraInfo] != extraInfo)
			continue;

		if(eAttachmentsData[i][e_iSkinId] == GetPlayerSkinEx(playerid) || eAttachmentsData[i][e_iSkinId] == -1)
		{
			attachmentIndex = i;

			slotIndex = GetFreeAttachmentSlot(playerid);
			break;
		}
	}

//#region Slot assignment based on model id

	if(slotIndex == -1 || attachmentIndex == -1)
		return;

	switch(modelId)
	{
		case 
			OBJECT_MODEL_JOELBP,
			OBJECT_MODEL_ELLIEBP,
			OBJECT_MODEL_TESSBP,
			OBJECT_MODEL_BILLBP:
			{
				if(eCharsInfo[playerid][e_iAttachSlot_Backpack] != -1)
					slotIndex = eCharsInfo[playerid][e_iAttachSlot_Backpack];
				else
					eCharsInfo[playerid][e_iAttachSlot_Backpack] = slotIndex;		
			}

		case 
			ITEM_MODEL_PIPE,
			ITEM_MODEL_SCSR_PIPE,
			ITEM_MODEL_HATCHET,
			ITEM_MODEL_MACHETE:
			{
				if(eCharsInfo[playerid][e_iAttachSlot_Melee] != -1)
					slotIndex = eCharsInfo[playerid][e_iAttachSlot_Melee];
				else
					eCharsInfo[playerid][e_iAttachSlot_Melee] = slotIndex;
			}

		case ITEM_MODEL_9MM, ITEM_MODEL_SHORTY:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_9mm] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_9mm];
			else
				eCharsInfo[playerid][e_iAttachSlot_9mm] = slotIndex;
		}

		case ITEM_MODEL_REVOLVER:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_Revolver] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_Revolver];
			else
				eCharsInfo[playerid][e_iAttachSlot_Revolver] = slotIndex;

		}

		case ITEM_MODEL_SHOTGUN, ITEM_MODEL_ASSAULT:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_LongGunOne] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_LongGunOne];
			else
				eCharsInfo[playerid][e_iAttachSlot_LongGunOne] = slotIndex;

		}

		case ITEM_MODEL_RIFLE, ITEM_MODEL_BOW:
		{
			if(eCharsInfo[playerid][e_iAttachSlot_LongGunTwo] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_LongGunTwo];
			else
				eCharsInfo[playerid][e_iAttachSlot_LongGunTwo] = slotIndex;
		}

		default: 
		{
			if(eCharsInfo[playerid][e_iAttachSlot_Dynamic] != -1)
				slotIndex = eCharsInfo[playerid][e_iAttachSlot_Dynamic];
			else
				eCharsInfo[playerid][e_iAttachSlot_Dynamic] = slotIndex;
		}
	}
	
//#endregion

	SetPlayerAttachedObject(playerid, slotIndex, eAttachmentsData[attachmentIndex][e_iModelId], eAttachmentsData[attachmentIndex][e_iBoneId], \
		eAttachmentsData[attachmentIndex][e_fOffsetX], eAttachmentsData[attachmentIndex][e_fOffsetY], eAttachmentsData[attachmentIndex][e_fOffsetZ], \
		eAttachmentsData[attachmentIndex][e_fRotX],    eAttachmentsData[attachmentIndex][e_fRotY],    eAttachmentsData[attachmentIndex][e_fRotZ], \
		eAttachmentsData[attachmentIndex][e_fScaleX],  eAttachmentsData[attachmentIndex][e_fScaleY],  eAttachmentsData[attachmentIndex][e_fScaleZ]);
}

// Detach weapon models from player's body.
DetachWeaponFromPlayersBody(playerid, itemId)
{
	if(!IsItemType(itemId, ITEM_TYPE_BULLET_WEAPON) && !IsItemType(itemId, ITEM_TYPE_MELEE))
	{
		return false;
	}

    // Depends on the weapon Id - if new weapon is equipped then remove its attached object from player's body.
    new slotId = -1;
    switch(ITEM_IDX:itemId)
    {
        case 
			ITEM_IDX_9MM, 
			ITEM_IDX_SHORTY:        
			{
				slotId = eCharsInfo[playerid][e_iAttachSlot_9mm];
			}

        case ITEM_IDX_REVOLVER:
		{
			slotId = eCharsInfo[playerid][e_iAttachSlot_Revolver];
		}
        
        case 
            ITEM_IDX_SHOTGUN,
            ITEM_IDX_ASSAULT_RIFLE:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_LongGunOne];
            }
            
        case 
            ITEM_IDX_RIFLE,
            ITEM_IDX_BOW:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_LongGunTwo];
            }

        case 
            ITEM_IDX_PIPE,
            ITEM_IDX_SCSR_PIPE,
            ITEM_IDX_HATCHET,
            ITEM_IDX_MACHETE:
            {
                slotId = eCharsInfo[playerid][e_iAttachSlot_Melee];
            }
        
        default:
        {
            // No need for seeking of dynamic object (e.g. projectiles, throwables, shiv)
            // since they are not attached to player's body at all.
            // so in fact, they are not going to be removed so we stop the code here from going any further.

            // slotId = eCharsInfo[playerid][e_iAttachSlot_Dynamic];
            return true;
        }
    }

    RemovePlayerAttachedObj(playerid, slotId);
	return true;
}