#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// Clearing weapon's menu data for player.
ResetEnum_ePlayerWeapMenu(playerid)
{
    static sBlank[e_PLAYER_WEAPMENU_DATA];
    ePlayerWeapMenu[playerid] = sBlank;

    print("ResetEnum_ePlayerWeapMenu");
}

// Check if is player using weapon menu.
IsPlayerUsingWeaponMenu(playerid)
{
    return IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);
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
    for(new i = 0; i != 3; i++)
    {
        if(WeapMenu_Rs_WeaponId(playerid, i) == -1) return i;
    }
    return -1;
}

//#region Sort weapons on the left side-bar of the weapon menu (long-guns).
    // WeapMenu_Ls_SortWeapons(playerid)
    // {
    //     new slot = 1;
    //     while(slot < 3)
    //     {
    //         if(WeapMenu_Ls_WeaponId(playerid, slot) == -1) // If no weapon found in slot..
    //         {
    //             for(new nextSlot = slot+1; nextSlot <= 3; nextSlot++)
    //             {
    //                 if(WeapMenu_Ls_WeaponId(playerid, nextSlot) != -1)
    //                 {
    //                     WeapMenu_Ls_WeaponId(playerid, nextSlot) = WeapMenu_Ls_WeaponId(playerid, slot);
    //                 }
    //             }
    //         }

    //         switch(slot)
    //         {
    //             case 1: slot = 0;
    //             case 0: slot = 2;
    //             case 2: slot = 3;
    //         }
    //     }


    // 	// for(new i = 0; i != 4 i++)
    // 	// {
    // 	// 	if(WeapMenu_Ls_WeaponId(playerid, i) == -1) // If no weapon found in slot..
    // 	// 	{
    // 	// 		for(new nextWeaponInSlot = i+1; nextWeaponInSlot != 4; nextWeaponInSlot++)
    // 	// 		{
    // 	// 			if(WeapMenu_Ls_WeaponId(playerid, nextWeaponInSlot) != -1) // If weapon found in the next slot...
    // 	// 			{
    // 	// 				WeapMenu_Ls_WeaponId(playerid, nextWeaponInSlot) = WeapMenu_Ls_WeaponId(playerid, i);
    // 	// 			}
    // 	// 		}
    // 	// 	}
    // 	// }
    // }
//#endregion

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
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);

    if(selectionH == WEAPON_MENU_H_LEFT0 || selectionH == WEAPON_MENU_H_LEFT1)
    {
        new icon[18], weaponId;

        for(new i = 0; i != 4; i++)
        {
            weaponId = WeapMenu_Ls_WeaponId(playerid, i);
            if(weaponId == -1) continue;

            switch(weaponId)
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
}
        
// Show player weapon menu.
ShowWeaponMenu(playerid)
{
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);

    OnPlayerOpenWeaponMenu(playerid);
}

// Hide player's weapon menu.
HideWeaponMenu(playerid)
{
    TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgH]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_SelectBox][playerid]);

    for(new tdIdx = 0; tdIdx != 9; tdIdx++)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx]);

        if(tdIdx < 2)
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][tdIdx]);

        if(tdIdx < 4)
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][tdIdx]);
    }

    OnPlayerCloseWeaponMenu(playerid);
}

// Display weapons in the weapon menu based on player's ammo and weapons that player have.
WeapMenuTd_DisplayWeapsAndAmmo(playerid)
{
    new 
        tmp[5],
        String:icon = STRING_NULL,
        weaponId = WEAPON_NAIL_BOMB,
        weaponAmmo = 0;
    
    for(new tdIdx = 0; tdIdx != 9; tdIdx++)
    {
        switch(tdIdx)
        {
            // Vertical (top -> bottom)
            case 0: weaponId = WEAPON_NAIL_BOMB;
            case 1: weaponId = WEAPON_MEDKIT;
            case 2: weaponId = GetPlayerThrowable[playerid];
            case 3: weaponId = WEAPON_MOLOTOV;
            case 4: weaponId = WEAPON_SMOKE_BOMB;

            // Horizontal (left -> right)
            case 5: weaponId = WeapMenu_Ls_WeaponId(playerid, 0);
            case 6: weaponId = WeapMenu_Ls_WeaponId(playerid, 1);
            case 7: weaponId = WeapMenu_Rs_WeaponId(playerid, 0);
            case 8: weaponId = WeapMenu_Rs_WeaponId(playerid, 1);
        }

        if(!HasPlayerWeapon(playerid, weaponId)) continue;

        switch(weaponId)
        {
            case WEAPON_NAIL_BOMB:  icon = str_new_static("TLoU:weap_nailbomb");
            case WEAPON_MEDKIT:     icon = str_new_static("TLoU:weap_medkit");
            case WEAPON_BRICK:      icon = str_new_static("TLoU:weap_brick");
            case WEAPON_BOTTLE:     icon = str_new_static("TLoU:weap_bottle");
            case WEAPON_MOLOTOV:    icon = str_new_static("TLoU:weap_molotov");
            case WEAPON_SMOKE_BOMB: icon = str_new_static("TLoU:weap_smkbmb");

            case WEAPON_SHOTGUN:    icon = str_new_static("TLoU:weap_shot");
            case WEAPON_RIFLE:      icon = str_new_static("TLoU:weap_huntrif");
            case WEAPON_BOW:        icon = str_new_static("TLoU:weap_bow");
            case WEAPON_ASSAULT:    icon = str_new_static("TLoU:weap_assault");

            case WEAPON_9MM:        icon = str_new_static("TLoU:weap_9mm");
            case WEAPON_REVOLVER:   icon = str_new_static("TLoU:weap_rev");
            case WEAPON_SHORTY:     icon = str_new_static("TLoU:weap_shorty");
        }
        
        weaponAmmo = GetPlayerWeaponAmmo(playerid, weaponId);
        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], icon);
        PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx], (weaponAmmo <= 0 ? COLOR_GRAY : COLOR_WHITE));
        PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], (weaponAmmo <= 0 ? COLOR_RED_TRANS : COLOR_WHITE));
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][tdIdx]);

        valstr(tmp, weaponAmmo);
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][tdIdx], tmp);
    }
}