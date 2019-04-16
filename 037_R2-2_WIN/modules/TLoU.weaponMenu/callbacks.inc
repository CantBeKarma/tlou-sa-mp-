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
        case WEAPON_MENU_V_BOMB:        SetPlayerWeapon(playerid, WEAPON_NAIL_BOMB,             GetPlayerWeaponAmmo(playerid, WEAPON_NAIL_BOMB));
        case WEAPON_MENU_V_MEDKIT:      SetPlayerWeapon(playerid, WEAPON_MEDKIT,                GetPlayerWeaponAmmo(playerid, WEAPON_MEDKIT));
        case WEAPON_MENU_V_BRICK:       SetPlayerWeapon(playerid, GetPlayerThrowable[playerid], GetPlayerWeaponAmmo(playerid, GetPlayerThrowable[playerid]));
        case WEAPON_MENU_V_MOLOTOV:     SetPlayerWeapon(playerid, WEAPON_MOLOTOV,               GetPlayerWeaponAmmo(playerid, WEAPON_MOLOTOV));
        case WEAPON_MENU_V_SMOKE_BOMB:  SetPlayerWeapon(playerid, WEAPON_SMOKE_BOMB,            GetPlayerWeaponAmmo(playerid, WEAPON_SMOKE_BOMB));
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
    WeapMenuTd_ShowSwapTextHelp(playerid, newSelection);

    switch(newSelection)
    {
        case WEAPON_MENU_H_LEFT0:  SetPlayerWeapon(playerid, WeapMenu_Ls_WeaponId(playerid, 0), GetPlayerWeaponAmmo(playerid, WeapMenu_Ls_WeaponId(playerid, 0)));
        case WEAPON_MENU_H_LEFT1:  SetPlayerWeapon(playerid, WeapMenu_Ls_WeaponId(playerid, 1), GetPlayerWeaponAmmo(playerid, WeapMenu_Ls_WeaponId(playerid, 1)));
        case WEAPON_MENU_H_RIGHT0: SetPlayerWeapon(playerid, WeapMenu_Rs_WeaponId(playerid, 0), GetPlayerWeaponAmmo(playerid, WeapMenu_Rs_WeaponId(playerid, 0)));
        case WEAPON_MENU_H_RIGHT1: SetPlayerWeapon(playerid, WeapMenu_Rs_WeaponId(playerid, 1), GetPlayerWeaponAmmo(playerid, WeapMenu_Rs_WeaponId(playerid, 1)));
    }
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerStartSwappingWeaps --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartSwappingWeaps(playerid)
{
    new selectionH = WeapMenu_GetSelectionH(playerid);
    WeapMenu_MoveSwapWeaponIcons(playerid, selectionH);
    WeapMenuTd_ShowSwapWeapIcons(playerid, selectionH);

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
    
    CLF(#Perform_BackpackClosing, "d", playerid);
    LastPlayerWeaponId[playerid] = WeapMenu_Ls_WeaponId(playerid, WeapMenu_GetSelectionH(playerid));
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerUD_SwapWeaps ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerUD_SwapWeaps(playerid, keys)
{
    new 
        selectionH = WeapMenu_GetSelectionH(playerid),
        oldWeapon = WeapMenu_Ls_WeaponId(playerid, selectionH);
        
    if(keys == KEY_UP)
    {
        new slotIdx = (WeapMenu_Ls_WeaponId(playerid, 3) != -1) ? (3) : (2);
        WeapMenu_Ls_WeaponId(playerid, selectionH) = WeapMenu_Ls_WeaponId(playerid, slotIdx);

        if(slotIdx == 3) WeapMenu_Ls_WeaponId(playerid, 3) = WeapMenu_Ls_WeaponId(playerid, 2);
        WeapMenu_Ls_WeaponId(playerid, 2) = oldWeapon;
    }
    else if(keys == KEY_DOWN)
    {
        WeapMenu_Ls_WeaponId(playerid, selectionH) = WeapMenu_Ls_WeaponId(playerid, 2);

        if(WeapMenu_Ls_WeaponId(playerid, 3) == -1) WeapMenu_Ls_WeaponId(playerid, 2) = oldWeapon;
        else 
        {
            WeapMenu_Ls_WeaponId(playerid, 2) = WeapMenu_Ls_WeaponId(playerid, 3);
            WeapMenu_Ls_WeaponId(playerid, 3) = oldWeapon;
        }
    }
    Audio_PlayEx(playerid, AUDIO_BP_SWITCH_WEAPS);

    WeapMenuTd_ShowSwapWeapIcons(playerid, selectionH);
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
        if(IsPlayerUsingWeaponMenu(playerid) && IsAllowedToSwitchWeaps(playerid) && !IsPlayerSwitchingWeapons(playerid))
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