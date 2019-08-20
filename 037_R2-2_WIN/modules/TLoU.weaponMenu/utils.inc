#include	<YSI\y_hooks>

// Clearing weapon's menu data for player.
ResetEnum_ePlayerWeapMenu(playerid)
{
    static sBlank[e_PLAYER_WEAPMENU_DATA];
    ePlayerWeapMenu[playerid] = sBlank;

    print("ResetEnum_ePlayerWeapMenu");
}

// Check if player is switching weapons.
IsPlayerSwitchingWeapons(playerid)
{
    return Bit_Get(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
}

// Check if player has any weapons to switch in the weapon menu.
IsAllowedToSwitchWeaps(playerid)
{
    return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
}

// Get free slot in which player's weapon is going to be placed on the LEFT SIDE of the weapon menu
WeapMenuTd_GetFreeSlot_H_Ls(playerid)
{
    if(WeapMenu_Ls_WeaponId(playerid, 1) == -1) return 1;
    else if(WeapMenu_Ls_WeaponId(playerid, 0) == -1) return 0;
    else if(WeapMenu_Ls_WeaponId(playerid, 2) == -1) return 2;
    else if(WeapMenu_Ls_WeaponId(playerid, 3) == -1) return 3;
    else return -1;
}

// Get free slot in which player's weapon is going to be placed on the RIGHT SIDE of the weapon menu
WeapMenuTd_GetFreeSlot_H_Rs(playerid)
{
    for(new i = 0; i < 3; i++)
    {
        if(WeapMenu_Rs_WeaponId(playerid, i) == -1) 
            return i;
    }
    return -1;
}

// Sort weapons on the left side-bar of the weapon menu (long-guns).
WeapMenu_Ls_SortWeapons(playerid)
{
    new currentSlot = 1;

    while(currentSlot < 3)
    {
        new slotToCheck =
            (currentSlot == 1) ? (0)
        :   (currentSlot == 0) ? (2)
        :   (currentSlot == 2) ? (3)
        :   (-1);

        if(slotToCheck == -1)
            break;

        if(WeapMenu_Ls_WeaponId(playerid, currentSlot) == -1)
        {
            if(WeapMenu_Ls_WeaponId(playerid, slotToCheck) != -1)
            {
                WeapMenu_Ls_WeaponId(playerid, currentSlot) = WeapMenu_Ls_WeaponId(playerid, slotToCheck);
                WeapMenu_Ls_WeaponId(playerid, slotToCheck) = -1;
            }
        }

        currentSlot = slotToCheck;
    }

    if(IsPlayerUsingWeaponMenu(playerid))
    {
        WeapMenuTd_DisplayWeapsAndAmmo(playerid);
    }
}

// Sort weapons on the right side-bar of the wepaon menu (short-guns).
Weapon_Rs_SortWeapons(playerid)
{
    new currentSlot = 0;

    while(currentSlot < 3)
    {
        new slotToCheck = 
            (currentSlot == 0) ? (1) 
        :   (currentSlot == 1) ? (2)
        :   (-1);

        if(slotToCheck == -1)
            break;

        if(WeapMenu_Rs_WeaponId(playerid, currentSlot) == -1)
        {
            if(WeapMenu_Rs_WeaponId(playerid, slotToCheck) != -1)
            {
                WeapMenu_Rs_WeaponId(playerid, currentSlot) = WeapMenu_Rs_WeaponId(playerid, slotToCheck);
                WeapMenu_Rs_WeaponId(playerid, slotToCheck) = -1;
            }
        }

        currentSlot = slotToCheck;
    }

    if(IsPlayerUsingWeaponMenu(playerid))
    {
        WeapMenuTd_DisplayWeapsAndAmmo(playerid);
    }
}

// Change the position of selection box
WeapMenu_MoveSelectBox(playerid, Float:x, Float:y)
{
    Backpack_MoveSelectBox(playerid, x, y);
}

// Change the position of "How to Swap" text
WeapMenu_MoveSwapTextHelp(playerid, selectId)
{
    switch(selectId)
    {
        case WEAPON_MENU_H_LEFT0:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 345.666656, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 368.999603, 281.244445);
        case WEAPON_MENU_H_LEFT1:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 389.666656, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 413.666351, 281.244445);
        //case WEAPON_MENU_H_RIGHT0: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 468.666717, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 492.666625, 281.244445);
        //case WEAPON_MENU_H_RIGHT1: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 519.333312, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 542.999694, 281.244445);
    }
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
}

// WeapMenu : Show "how to swap" text in horizontal bar.
WeapMenuTd_ShowSwapTextHelp(playerid, selectionH)
{
    if(selectionH == WEAPON_MENU_H_LEFT0 || selectionH == WEAPON_MENU_H_LEFT1)
    {
        if(WeapMenu_Ls_WeaponId(playerid, 2) != -1) WeapMenu_MoveSwapTextHelp(playerid, selectionH);
        else
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
        }
    }

    if(selectionH == WEAPON_MENU_H_RIGHT0 || selectionH == WEAPON_MENU_H_RIGHT1)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
    }

    // case WEAPON_MENU_H_RIGHT0, WEAPON_MENU_H_RIGHT1:
    // {
    //     if(WeapMenu_Rs_WeaponId(playerid, 2) != -1) WeapMenu_MoveSwapTextHelp(playerid, selectionH);
    //     else
    //     {
    //         PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
    //         PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
    //     }
    // }
}

// Change the position of "Swap weapon icons"
WeapMenu_MoveSwapWeaponIcons(playerid, selectId)
{
    switch(selectId)
    {
        case WEAPON_MENU_H_LEFT0:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 335.999969, 185.892562), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 335.999969, 265.892547);
        case WEAPON_MENU_H_LEFT1:  PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 384.666656, 180.500000), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 384.666656, 260.500000);
        //case WEAPON_MENU_H_RIGHT0: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 468.666717, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 492.666625, 281.244445);
        //case WEAPON_MENU_H_RIGHT1: PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 519.333312, 278.755615), PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 542.999694, 281.244445);
    }
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0]);
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1]);
}

// WeapMenu : Show "Swap weapon icons"
WeapMenuTd_ShowSwapWeapIcons(playerid, selectionH)
{
    if(selectionH != WEAPON_MENU_H_LEFT0 && selectionH != WEAPON_MENU_H_LEFT1)
        return;

    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);

    new icon[18];

    for(new i = 0; i != 4; i++)
    {
        if(WeapMenu_Ls_WeaponId(playerid, i) == -1) 
            continue;

        switch(WeapMenu_Ls_WeaponId(playerid, i))
        {
            case WEAPON_SHOTGUN:    strcpy(icon, "TLoU:weap_shot");
            case WEAPON_RIFLE:      strcpy(icon, "TLoU:weap_huntrif");
            case WEAPON_BOW:        strcpy(icon, "TLoU:weap_bow");
            case WEAPON_ASSAULT:    strcpy(icon, "TLoU:weap_assault");
        }

        switch(i)
        {
            case 0, 1: WeapMenuTd_DisplayWeapsAndAmmo(playerid);
            case 2:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], icon);
            case 3:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], icon);
        }
    }
}
        
// Show player weapon menu.
bool:ShowWeaponMenu(playerid)
{
    if(IsPlayerUnableToPerformAction(playerid) || IsPlayerAimingWeapon(playerid))
    {
        return false;
    }

    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);
    return true;
}

// Hide player's weapon menu.
bool:HideWeaponMenu(playerid)
{
    if(!IsPlayerUsingWeaponMenu(playerid))
    {
        return false;
    }
    
    TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_SelectBox][playerid]);

    for(new tdIdx = 0; tdIdx != 9; tdIdx++)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx]);

        if(tdIdx < 2)
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][tdIdx]);
        }

        if(tdIdx < 4)
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][tdIdx]);
        }
    }
    return true;
}

// Display weapons in the weapon menu based on player's ammo and weapons that player have.
WeapMenuTd_DisplayWeapsAndAmmo(playerid)
{
    new 
        itemAmount = -1,
        String:itemAmountStr = STRING_NULL,
        String:icon = STRING_NULL,
        ITEM_IDX:itemId = ITEM_IDX:ITEM_IDX_NAIL_BOMB;
    
    for(new tdIdx = 0; tdIdx != 9; tdIdx++)
    {
        switch(tdIdx)
        {
            // Vertical (top -> bottom)
            case 0: itemId = ITEM_IDX:ITEM_IDX_NAIL_BOMB;
            case 1: itemId = ITEM_IDX:ITEM_IDX_MEDKIT;
            case 2: itemId = ITEM_IDX:ITEM_IDX_BRICK; // GetPlayerThrowable[playerid] - that needs to return itemId
            case 3: itemId = ITEM_IDX:ITEM_IDX_MOLOTOV;
            case 4: itemId = ITEM_IDX:ITEM_IDX_SMOKE_BOMB;

            // Horizontal (left -> right)
            case 5: itemId = ITEM_IDX:WeapMenu_Ls_WeaponId(playerid, 0);
            case 6: itemId = ITEM_IDX:WeapMenu_Ls_WeaponId(playerid, 1);
            case 7: itemId = ITEM_IDX:WeapMenu_Rs_WeaponId(playerid, 0);
            case 8: itemId = ITEM_IDX:WeapMenu_Rs_WeaponId(playerid, 1);
        }

        if(!HasPlayerGotItem(playerid, _:itemId))
        {
            continue;
        }

        icon = str_new_static(eItemsSprites[_:itemId]);
        itemAmount = GetPlayerItemAmount(playerid, _:itemId);

        SCMF(playerid, -1, "Displaying icon: %s with amount: %d", _:icon, itemAmount);

        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], icon);
        PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], (itemAmount <= 0 ? COLOR_GRAY : COLOR_WHITE));
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);

        itemAmountStr = str_val(itemAmount);
        SCMF(playerid, -1, "Amount in string: %s", _:itemAmountStr);
        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], itemAmountStr);
        PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], (itemAmount <= 0 ? COLOR_RED_TRANS : COLOR_WHITE));
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx]);
    }
}