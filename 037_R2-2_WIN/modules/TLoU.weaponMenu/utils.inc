#include	<YSI\y_hooks>

// Clearing weapon's menu data for player.
ResetEnum_ePlayerWeapMenu(playerid)
{
    static sBlank[e_PLAYER_WEAPMENU_DATA];
    ePlayerWeapMenu[playerid] = sBlank;

    print("ResetEnum_ePlayerWeapMenu");
}

//#region Validation

    IsPlayerSwitchingWeapons(playerid)return Bit_Get(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
    
    IsAllowedToSwitchWeaps(playerid)
    {
        new selectionH = WeapMenu_GetSelectionH(playerid);
        if(selectionH == -1 || IsPlayerSwitchingWeapons(playerid))
        {
            return false;
        }

        switch(selectionH)
        {
            case WEAPON_MENU_H_LEFT0,  WEAPON_MENU_H_LEFT1:  return (WeapMenu_Ls_ItemId(playerid, 2) != -1);
            case WEAPON_MENU_H_RIGHT0, WEAPON_MENU_H_RIGHT1: return (WeapMenu_Rs_ItemId(playerid, 2) != -1);
        }

        return false;
    }

//#endregion

// Return textdraw index (in weapon menu) for specified item.
WeapMenu_GetTdIdxOfItemId(playerid, itemId)
{
    new result = -1;

    switch(itemId)
    {
        case ITEM_IDX_NAIL_BOMB:    result = 0;
        case ITEM_IDX_MEDKIT:       result = 1;

        case 
            ITEM_IDX_BRICK, 
            ITEM_IDX_BOTTLE:
            {
                result = 2;
            }

        case ITEM_IDX_MOLOTOV:      result = 3;
        case ITEM_IDX_SMOKE_BOMB:   result = 4;
        default:
        {
            for(new i = 0; i <= 1; i++)
            {
                if(WeapMenu_Ls_ItemId(playerid, i) != -1
                && WeapMenu_Ls_ItemId(playerid, i) == itemId)
                {
                    result = 5 + i; // 6 - 0 = 6, 6 - 1 = 5
                    break;
                }

                if(WeapMenu_Rs_ItemId(playerid, i) != -1
                && WeapMenu_Rs_ItemId(playerid, i) == itemId)
                {
                    result = 7 + i;
                    break;
                }
            }
        }
    }
    return result;
}

//#region SORTING | Horizontal weapon menu bar

    // Get free slot in which player's weapon is going to be placed on the LEFT SIDE of the weapon menu
    // Used internally in WeapMenu_AssignItemToFreeSlot() - DO NOT USE MANUALLY!
    WeapMenuTd_GetFreeSlot_H_Ls(playerid)
    {
        if(WeapMenu_Ls_ItemId(playerid, 1) == -1) return 1;
        else if(WeapMenu_Ls_ItemId(playerid, 0) == -1) return 0;
        else if(WeapMenu_Ls_ItemId(playerid, 2) == -1) return 2;
        else if(WeapMenu_Ls_ItemId(playerid, 3) == -1) return 3;
        else return -1;
    }

    // Get free slot in which player's weapon is going to be placed on the RIGHT SIDE of the weapon menu
    // Used internally in WeapMenu_AssignItemToFreeSlot() - DO NOT USE MANUALLY!
    WeapMenuTd_GetFreeSlot_H_Rs(playerid)
    {
        for(new i = 0; i < 3; i++)
        {
            if(WeapMenu_Rs_ItemId(playerid, i) == -1) 
                return i;
        }
        return -1;
    }

    // Assigns item to a first available slot in the weapon menu (horizontal bar).
    // Used internally in GiveItem() - DO NOT USE MANUALLY!
    WeapMenu_AssignItemToFreeSlot(playerid, itemId)
    {
        new freeSlotId = -1;
        switch(ITEM_IDX:itemId)
        {
            case 
                ITEM_IDX_SHOTGUN, ITEM_IDX_RIFLE, ITEM_IDX_BOW, ITEM_IDX_ASSAULT_RIFLE:
                {
                    freeSlotId = WeapMenuTd_GetFreeSlot_H_Ls(playerid);
                    WeapMenu_Ls_ItemId(playerid, freeSlotId) = itemId;
                }

            case 
                ITEM_IDX_9MM, ITEM_IDX_REVOLVER, ITEM_IDX_SHORTY:
                {
                    freeSlotId = WeapMenuTd_GetFreeSlot_H_Rs(playerid);
                    WeapMenu_Rs_ItemId(playerid, freeSlotId) = itemId;
                }
        }
        return freeSlotId;
    }

    // Removes player item and performs sorting of the weapons in the weapon menu (horizonal bar).
    // Used internally in RemoveItem() - DO NOT USE MANUALLY!
    WeapMenu_RemoveAndSortItems(playerid, itemId)
    {
        switch(ITEM_IDX:itemId)
        {
            case 
                ITEM_IDX_SHOTGUN, ITEM_IDX_RIFLE, ITEM_IDX_BOW, ITEM_IDX_ASSAULT_RIFLE:
                {
                    RemoveItemFromHorizontalBar(playerid, itemId, "left_side");
                    WeapMenu_Horizontal_SortWeapons(playerid, "left_side");
                }

            case 
                ITEM_IDX_9MM, ITEM_IDX_REVOLVER, ITEM_IDX_SHORTY:
                {
                    RemoveItemFromHorizontalBar(playerid, itemId, "right_side");
                    WeapMenu_Horizontal_SortWeapons(playerid, "right_side");
                }
        }
    }

    // Sorts weapons in the horizontal bar of the weapon menu.
    // Used internally in WeapMenu_RemoveAndSortItems() - DO NOT USE MANUALLY!
    WeapMenu_Horizontal_SortWeapons(playerid, leftOrRight[] = "")
    {
        new 
            currentSlot = -1,
            slotToCheck = 1;

        if(!strcmp(leftOrRight, "left_side"))
        {
            currentSlot = 1;
            
            while(currentSlot < 4)
            {
                slotToCheck =
                    (currentSlot == 1) ? (0)
                :   (currentSlot == 0) ? (2)
                :   (currentSlot == 2) ? (3)
                :   (-1);

                if(slotToCheck == -1)
                {
                    break;
                }

                if(WeapMenu_Ls_ItemId(playerid, currentSlot) == -1)
                {
                    if(WeapMenu_Ls_ItemId(playerid, slotToCheck) != -1)
                    {
                        WeapMenu_Ls_ItemId(playerid, currentSlot) = WeapMenu_Ls_ItemId(playerid, slotToCheck);
                        WeapMenu_Ls_ItemId(playerid, slotToCheck) = -1;
                    }
                }

                currentSlot = slotToCheck;
            }
        }
        else if(!strcmp(leftOrRight, "right_side"))
        {
            currentSlot = 0;

            while(currentSlot < 3)
            {
                slotToCheck = 
                    (currentSlot == 0) ? (1) 
                :   (currentSlot == 1) ? (2)
                :   (-1);

                if(slotToCheck == -1)
                {
                    break;
                }

                if(WeapMenu_Rs_ItemId(playerid, currentSlot) == -1)
                {
                    if(WeapMenu_Rs_ItemId(playerid, slotToCheck) != -1)
                    {
                        WeapMenu_Rs_ItemId(playerid, currentSlot) = WeapMenu_Rs_ItemId(playerid, slotToCheck);
                        WeapMenu_Rs_ItemId(playerid, slotToCheck) = -1;
                    }
                }

                currentSlot = slotToCheck;
            }
        }

        if(IsPlayerUsingWeaponMenu(playerid))
        {
            WeapMenuTd_DisplayWeapsAndAmmo(playerid);
        }
    }

    // Resets itemId assignment to variable (dynamically assigned in horizontal bar of the weapon menu).
    // Used internally in WeapMenu_RemoveAndSortItems() - DO NOT USE MANUALLY!
    RemoveItemFromHorizontalBar(playerid, itemId, leftOrRight[] = "")
    {
        if(!strcmp(leftOrRight, "left_side"))
        {
            for(new i = 0; i <= 3; i++)
            {
                if(itemId == WeapMenu_Ls_ItemId(playerid, i))
                {
                    WeapMenu_Ls_ItemId(playerid, i) = -1;
                    break;
                }
            }
        }
        else if(!strcmp(leftOrRight, "right_side"))
        {
            for(new i = 0; i <= 2; i++)
            {
                if(itemId == WeapMenu_Rs_ItemId(playerid, i))
                {
                    WeapMenu_Rs_ItemId(playerid, i) = -1;
                    break;
                }
            }
        }
    }

//#endregion

//#region Swap weapon icons & help text

    // Change the position of selection box.
    WeapMenu_MoveSelectBox(playerid, Float:x, Float:y)
    {
        Backpack_MoveSelectBox(playerid, x, y);
    }

    // Shows message "HOLD LMB TO SWAP".
    WeapMenuTd_ShowSwapTextHelp(playerid)
    {
        if(!IsAllowedToSwitchWeaps(playerid))
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
            return;
        }
        
        WeapMenu_MoveSwapTextHelp(playerid, WeapMenu_GetSelectionH(playerid));
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
    }

    // Changes the position of "HOLD LMB TO SWAP" textdraw in weapon menu (horizontal bar).
    // DO NOT USE ANYWHERE ! - Used internally in WeapMenuTd_ShowSwapTextHelp().
    WeapMenu_MoveSwapTextHelp(playerid, selectionH)
    {
        switch(selectionH)
        {
            case WEAPON_MENU_H_LEFT0:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 345.666656, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 368.999603, 281.244445);
            case WEAPON_MENU_H_LEFT1:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 389.666656, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 413.666351, 281.244445);
            case WEAPON_MENU_H_RIGHT0: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 468.666717, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 492.666625, 281.244445);
            case WEAPON_MENU_H_RIGHT1: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 519.333312, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 542.999694, 281.244445);
        }
    }

    // Shows swap weapon icons and hides "HOLD LMB TO SWAP" text help.
    WeapMenuTd_ShowSwapWeapIcons(playerid)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);

        WeapMenuTd_MoveSwapWeaponIcons(playerid);
        WeapMenuTd_UpdateSwapWeapIcons(playerid);
    }

    // Changes the position of swap weapons icons.
    // DO NOT USE ANYWHERE ! - Used internally in WeapMenuTd_ShowSwapWeapIcons().
    WeapMenuTd_MoveSwapWeaponIcons(playerid)
    {
        new selectionH = WeapMenu_GetSelectionH(playerid);

        if(selectionH == WEAPON_MENU_H_LEFT0 
        || selectionH == WEAPON_MENU_H_LEFT1)
        {
            PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 41.000015, 52.681484);
        }
        else
        {
            PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 35.333316, 40.651855);
        }

        switch(selectionH)
        {
            case WEAPON_MENU_H_LEFT0:
            {
                PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 335.999969, 185.892562);
                PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 335.999969, 265.892547);
            }

            case WEAPON_MENU_H_LEFT1:  
            {
                PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 384.666656, 180.500000);
                PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 384.666656, 260.500000);
            }

            case WEAPON_MENU_H_RIGHT0: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 473.333435, 185.892562);
            case WEAPON_MENU_H_RIGHT1: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 519.333435, 185.892562);
        }
    }

    // Updates swap weapon icons while swapping weapons.
    WeapMenuTd_UpdateSwapWeapIcons(playerid)
    {
        new selectionH = WeapMenu_GetSelectionH(playerid);
        new loopCount = (selectionH == WEAPON_MENU_H_RIGHT0 || selectionH == WEAPON_MENU_H_RIGHT1) ? (2) : (3);

        for(new i = 0; i <= loopCount; i++)
        {
            new itemId = -1;
            if(selectionH == WEAPON_MENU_H_LEFT0 || selectionH == WEAPON_MENU_H_LEFT1)
            {
                itemId = WeapMenu_Ls_ItemId(playerid, i);
            }
            else if(selectionH == WEAPON_MENU_H_RIGHT0 || selectionH == WEAPON_MENU_H_RIGHT1)
            {
                itemId = WeapMenu_Rs_ItemId(playerid, i);
            }
            
            if(itemId == -1)
            {
                break;
            }

            switch(i)
            {
                case 0, 1: WeapMenuTd_DisplayWeapsAndAmmo(playerid, itemId);
                case 2:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], eItemsSprites[itemId]);
                case 3:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], eItemsSprites[itemId]);
            }
        }
    }

//#endregion

//#region Showing & Hiding weapon menu

    // Show player weapon menu.
    bool:ShowWeaponMenu(playerid)
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);
        return true;
    }

    // Hide player's weapon menu.
    bool:HideWeaponMenu(playerid, hidePart = HIDE_WEAPMENU_PART_ALL)
    {
        if(!IsPlayerUsingWeaponMenu(playerid))
        {
            return false;
        }

        if(hidePart == HIDE_WEAPMENU_PART_ALL || hidePart == HIDE_WEAPMENU_PART_VERT)
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);

            for(new i = 0; i <= 4; i++)
            {
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i]);
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i]);

                if(i <= 3)
                {
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i]);
                }
            }
        }

        if(hidePart == HIDE_WEAPMENU_PART_ALL || hidePart == HIDE_WEAPMENU_PART_HORI)
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);

            for(new i = 5; i <= 8; i++)
            {
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i]);
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i]);
            }
        }

        if(hidePart == HIDE_WEAPMENU_PART_ALL)
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_SelectBox][playerid]);

            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);

            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][0]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][1]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][2]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][3]);
        }
        return true;
    }

    // Display weapons in the weapon menu based on player's ammo and weapons that player have.
    WeapMenuTd_DisplayWeapsAndAmmo(playerid, itemId = -1)
    {
        new 
            itemAmount,
            String:itemAmountStr = STRING_NULL;

        if(itemId == -1)
        {
            for(itemId = 0; itemId != MAX_ITEMS_DATA; itemId++)
            {
                if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_THROWABLE)
                && !IsItemType(itemId, ITEM_TYPE_PROJECTILE) && !IsItemType(itemId, ITEM_TYPE_MEDKIT))
                {
                    continue;
                }

                if(!HasPlayerGotItem(playerid, itemId))
                {
                    continue;
                }

                new tdIdx = WeapMenu_GetTdIdxOfItemId(playerid, itemId);
                if(tdIdx == -1)
                {
                    continue;
                }

                itemAmount = GetPlayerItemAmount(playerid, itemId);
                PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], eItemsSprites[itemId]);
                PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], (itemAmount <= 0 ? COLOR_GRAY : COLOR_WHITE));
                PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);

                itemAmountStr = str_val(itemAmount);
                PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], itemAmountStr);
                PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], (itemAmount <= 0 ? COLOR_RED_TRANS : COLOR_WHITE));
                PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx]);
            }
        }
        else
        {
            if(!IsItemType(itemId, ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE_THROWABLE)
            && !IsItemType(itemId, ITEM_TYPE_PROJECTILE) && !IsItemType(itemId, ITEM_TYPE_MEDKIT))
            {
                return false;
            }

            if(!HasPlayerGotItem(playerid, itemId))
            {
                return false;
            }

            new tdIdx = WeapMenu_GetTdIdxOfItemId(playerid, itemId);
            if(tdIdx == -1)
            {
                return false;
            }

            itemAmount = GetPlayerItemAmount(playerid, itemId);
            PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], eItemsSprites[itemId]);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], (itemAmount <= 0 ? COLOR_GRAY : COLOR_WHITE));
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);

            itemAmountStr = str_val(itemAmount);
            PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], itemAmountStr);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], (itemAmount <= 0 ? COLOR_RED_TRANS : COLOR_WHITE));
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx]);
        }
        return true;
    }

//#endregion