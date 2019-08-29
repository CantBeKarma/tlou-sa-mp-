//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerOpenWeaponMenu ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerOpenWeaponMenu(playerid)
{
    WeapMenu_GetSelectionV(playerid) = WeapMenu_GetSelectionH(playerid) = -1;
    
    FreezePlayer(playerid, true, -1);

    WeapMenuTd_DisplayWeapsAndAmmo(playerid);
    CraftTd_CheckWhatsToCraft(playerid);
    Key_WeaponMenu(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerCloseWeaponMenu ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCloseWeaponMenu(playerid)
{
    WeapMenu_GetSelectionV(playerid) = WeapMenu_GetSelectionH(playerid) = -1;

    UnfreezePlayer(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerUD_WeaponMenu -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerUD_WeaponMenu(playerid, keys)
{
    new oldSelection = WeapMenu_GetSelectionV(playerid);
    if(keys == KEY_UP)
    {
        if(oldSelection == -1) WeapMenu_GetSelectionV(playerid) = WEAPON_MENU_V_MEDKIT;
        else if(oldSelection == WEAPON_MENU_V_BOMB) return 1;
        else WeapMenu_GetSelectionV(playerid) -= 1;

        Audio_PlayEx(playerid, AUDIO_WEAP_MENU_RIGHT);
    }
    else if(keys == KEY_DOWN)
    {
        if(oldSelection == -1) WeapMenu_GetSelectionV(playerid) = WEAPON_MENU_V_BRICK;
        else if(oldSelection == WEAPON_MENU_V_SMOKE_BOMB) return 1;
        else WeapMenu_GetSelectionV(playerid) += 1;

        Audio_PlayEx(playerid, AUDIO_WEAP_MENU_LEFT);
    }

    new newSelection = WeapMenu_GetSelectionV(playerid);
    if(!IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][newSelection]))
    {
        WeapMenu_GetSelectionV(playerid) = oldSelection;
        return 1;
    }

    if(WeapMenu_GetSelectionH(playerid) != -1)
    {
        WeapMenu_GetSelectionH(playerid) = -1;
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1]);
    }

    WeapMenu_MoveSelectBox(playerid, eWeapMenu_Items[newSelection][e_fSelBoxX], eWeapMenu_Items[newSelection][e_fSelBoxY]);

    switch(newSelection)
    {
        case WEAPON_MENU_V_BOMB:        EquipItem(playerid, _:ITEM_IDX_NAIL_BOMB);
        case WEAPON_MENU_V_MEDKIT:      EquipItem(playerid, _:ITEM_IDX_MEDKIT);
        case WEAPON_MENU_V_BRICK:       EquipItem(playerid, GetPlayerThrowableItemId(playerid));
        case WEAPON_MENU_V_MOLOTOV:     EquipItem(playerid, _:ITEM_IDX_MOLOTOV);
        case WEAPON_MENU_V_SMOKE_BOMB:  EquipItem(playerid, _:ITEM_IDX_SMOKE_BOMB);
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerLR_WeaponMenu -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerLR_WeaponMenu(playerid, keys)
{
    new oldSelection = WeapMenu_GetSelectionH(playerid);
    if(keys == KEY_LEFT)
    {
        if(oldSelection == -1) WeapMenu_GetSelectionH(playerid) = WEAPON_MENU_H_LEFT1;
        else if(oldSelection == WEAPON_MENU_H_LEFT0) return 1;
        else WeapMenu_GetSelectionH(playerid) -= 1;

        Audio_PlayEx(playerid, AUDIO_WEAP_MENU_LEFT);
    }
    else if(keys == KEY_RIGHT)
    {
        if(oldSelection == -1) WeapMenu_GetSelectionH(playerid) = WEAPON_MENU_H_RIGHT0;
        else if(oldSelection == WEAPON_MENU_H_RIGHT1) return 1;
        else WeapMenu_GetSelectionH(playerid) += 1;

        Audio_PlayEx(playerid, AUDIO_WEAP_MENU_RIGHT);
    }

    new newSelection = WeapMenu_GetSelectionH(playerid);
    if(!IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][newSelection+5]))
    {
        WeapMenu_GetSelectionH(playerid) = oldSelection;
        return 1;
    }

    if(WeapMenu_GetSelectionV(playerid) != -1)
    {
        WeapMenu_GetSelectionV(playerid) = -1;
    }

    WeapMenu_MoveSelectBox(playerid, eWeapMenu_Items[newSelection+5][e_fSelBoxX], eWeapMenu_Items[newSelection+5][e_fSelBoxY]);
    WeapMenuTd_ShowSwapTextHelp(playerid);

    switch(newSelection)
    {
        case WEAPON_MENU_H_LEFT0:  EquipItem(playerid, WeapMenu_Ls_ItemId(playerid, 0));
        case WEAPON_MENU_H_LEFT1:  EquipItem(playerid, WeapMenu_Ls_ItemId(playerid, 1));
        case WEAPON_MENU_H_RIGHT0: EquipItem(playerid, WeapMenu_Rs_ItemId(playerid, 0));
        case WEAPON_MENU_H_RIGHT1: EquipItem(playerid, WeapMenu_Rs_ItemId(playerid, 1));
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerStartSwappingWeaps --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartSwappingWeaps(playerid)
{
    WeapMenuTd_ShowSwapWeapIcons(playerid);
    HideWeaponMenu(playerid, HIDE_WEAPMENU_PART_VERT);
    Perform_BackpackOpening(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------- OnPlayerFinishSwappingWeaps --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerFinishSwappingWeaps(playerid)
{
    for(new i = 0; i != 2; i++)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][i]);
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][i]);
    }

    // Restore vertical bar of weapons menu and their icons.
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_weapMenu_BgV]);
    WeapMenuTd_DisplayWeapsAndAmmo(playerid);
    CraftTd_CheckWhatsToCraft(playerid);

    CLF(#Perform_BackpackClosing, "d", playerid);

    ePlayerInfo[playerid][e_iLastEquippedItemId] =
        (WeapMenu_GetSelectionH(playerid) == WEAPON_MENU_H_LEFT0 || WeapMenu_GetSelectionH(playerid) == WEAPON_MENU_H_LEFT1) 
        ? WeapMenu_Ls_ItemId(playerid, selectionH)
        : WeapMenu_Rs_ItemId(playerid, selectionH-2);

    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerUD_SwapWeaps ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerUD_SwapWeaps(playerid, keys)
{
    new 
        selectionH = WeapMenu_GetSelectionH(playerid),
        previousItemId = -1;
        
    if(keys == KEY_UP)
    {
        if(selectionH == WEAPON_MENU_H_LEFT0 || selectionH == WEAPON_MENU_H_LEFT1)
        {
            previousItemId = WeapMenu_Ls_ItemId(playerid, selectionH);

            new slotIdx = (WeapMenu_Ls_ItemId(playerid, 3) != -1) ? (3) : (2);
            WeapMenu_Ls_ItemId(playerid, selectionH) = WeapMenu_Ls_ItemId(playerid, slotIdx);

            if(slotIdx == 3) WeapMenu_Ls_ItemId(playerid, 3) = WeapMenu_Ls_ItemId(playerid, 2);
            WeapMenu_Ls_ItemId(playerid, 2) = previousItemId;
        }
        else
        {
            new selectedSlotId = (selectionH == WEAPON_MENU_H_RIGHT0) ? (0) : (1);
            previousItemId = WeapMenu_Rs_ItemId(playerid, selectedSlotId);
            WeapMenu_Rs_ItemId(playerid, selectedSlotId) = WeapMenu_Rs_ItemId(playerid, 2);
            WeapMenu_Rs_ItemId(playerid, 2) = previousItemId;
        }
    }
    else if(keys == KEY_DOWN)
    {
        if(selectionH == WEAPON_MENU_H_LEFT0 || selectionH == WEAPON_MENU_H_LEFT1)
        {
            previousItemId = WeapMenu_Ls_ItemId(playerid, selectionH);
            WeapMenu_Ls_ItemId(playerid, selectionH) = WeapMenu_Ls_ItemId(playerid, 2);

            if(WeapMenu_Ls_ItemId(playerid, 3) == -1) WeapMenu_Ls_ItemId(playerid, 2) = previousItemId;
            else 
            {
                WeapMenu_Ls_ItemId(playerid, 2) = WeapMenu_Ls_ItemId(playerid, 3);
                WeapMenu_Ls_ItemId(playerid, 3) = previousItemId;
            }
        }
        else
        {
            new selectedSlotId = (selectionH == WEAPON_MENU_H_RIGHT0) ? (0) : (1);
            previousItemId = WeapMenu_Rs_ItemId(playerid, selectedSlotId);
            WeapMenu_Rs_ItemId(playerid, selectedSlotId) = WeapMenu_Rs_ItemId(playerid, 2);
            WeapMenu_Rs_ItemId(playerid, 2) = previousItemId;
        }
    }

    Audio_PlayEx(playerid, AUDIO_BP_SWITCH_WEAPS);
    WeapMenuTd_UpdateSwapWeapIcons(playerid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerKeyStateChange ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }
    
    if(newkeys & KEY_YES)
    {
        if(!IsPlayerUsingWeaponMenu(playerid))
        {
            if(GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_2IDLE 
            && GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_IN
            && GetPlayerAnimationIndex(playerid) != BOMBER_BOM_PLANT_LOOP)
            {
                SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_USING_WEAPMENU);
            }
        }
        else 
        {
            if(!IsPlayerSwitchingWeapons(playerid))
            {
                SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
            }
        }
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerHoldingKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerHoldingKey(playerid, key)
{
    if(key & KEY_FIRE)
    {
        if(IsPlayerUsingWeaponMenu(playerid) && IsAllowedToSwitchWeaps(playerid))
        {
            Bit_Let(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
            OnPlayerStartSwappingWeaps(playerid);
        }
    }
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerReleaseKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
hook OnPlayerReleaseKey(playerid, key, holding_time)
{
    if(key & KEY_FIRE)
    {
        if(IsPlayerSwitchingWeapons(playerid))
        {
            Bit_Vet(ePlayerFlag[e_bIsSwitchingWeapons], playerid);
            OnPlayerFinishSwappingWeaps(playerid);
        }
    }
    return 1;
}