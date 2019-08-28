// Check if HUD is visible to the player right now.
IsHudVisibleForPlayer(playerid)return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

// HUD : Showing.
ShowHud(playerid)
{
    PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

    HudTd_UpdateCraftingWrench(playerid);
    HudTd_UpdateShivs(playerid);
    HudTd_UpdateMelee(playerid);

    TdShowForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Outline]);
    TdShowForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Underneath]);
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status]);

    // Weapon HUD showing is being done inside 'SetPlayerWeapon' !
    RestartHideHudTimer(playerid);
}

// HUD : Hiding
timer HideHud[TIMER_HIDE_HUD](playerid)
{
    if(!IsHudVisibleForPlayer(playerid))
    {
        return false;
    }

    // Weapon HUD hiding is being done inside 'OnPlayerWeaponChange' !
    HideItem(playerid);
    
    if(IsPlayerHudFlashing(playerid))
    {
        return false;
    }

    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

    TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Wrench]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee]);

    TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Outline]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Underneath]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status]);
    return true;
}

// HUD : Checks player's armed weapon and displays it.
HudTd_UpdateWeapon(playerid, itemId)
{
    if(itemId == -1)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon]);
        return true;
    }

    for(new i = 0; i != MAX_HUD_WEAPONS; i++)
    {
        if(itemId != _:eHudMenu_Items[i][e_iHud_itemId])
        {
            continue;
        }

        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_sHud_WeapIcon]);
        PlayerTextDrawTextSize(playerid,  ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_fHud_WeapIconTextSize_X], eHudMenu_Items[i][e_fHud_WeapIconTextSize_Y]);
        PlayerTextDrawSetPos(playerid,    ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_fHud_WeapIconPos_X], eHudMenu_Items[i][e_fHud_WeapIconPos_Y]);
        PlayerTextDrawSetPos(playerid,    ePlayerTextDrawInfo[playerid][e_Hud_Ammo],   eHudMenu_Items[i][e_fHud_WeapAmmoPos_X], eHudMenu_Items[i][e_fHud_WeapAmmoPos_Y]);

        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon]);
        break;
    }
    return true;
}

// HUD : Checking if there's anything to craft and displays wrench icon if so.
HudTd_UpdateCraftingWrench(playerid)
{
    if(Iter_Count(ItemsToCraft<playerid>) <= 0)
    {
        CraftTd_CheckWhatsToCraft(playerid);
    }

    if(Iter_Count(ItemsToCraft<playerid>) <= 0)
        TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Wrench]);
    else 
        TdShowForPlayer(playerid, eTextDrawInfo[e_Hud_Wrench]);
}

// HUD : Checking if player has any shivs - if so, displays shiv icon and amount.
HudTd_UpdateShivs(playerid)
{
    if(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV) <= 0) TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
    else
    {
        TdShowForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
        
        new String:itemAmountStr = STRING_NULL;
        itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV));

        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], itemAmountStr);
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount]);
    }
}

//#region Melee HUD textdraws functions

    // HUD : Checks if player has any melee weapon. If so, displays appropriate melee icon.
    HudTd_UpdateMelee(playerid)
    {
        if(GetPlayerMelee[playerid] == NO_MELEE) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee]);
        else
        {
            new tmp_weapIcon[20];
            switch(GetPlayerMelee[playerid])
            {
                case WEAPON_PIPE:         strcpy(tmp_weapIcon, "TLoU:weap_pipe");
                case WEAPON_SCISSOR_PIPE: strcpy(tmp_weapIcon, "TLoU:weap_scsr_pipe");
                case WEAPON_HATCHET:      strcpy(tmp_weapIcon, "TLoU:weap_hatchet");
                case WEAPON_MACHETE:      strcpy(tmp_weapIcon, "TLoU:weap_machete");
            }
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], tmp_weapIcon);
        }
    }

    // HUD : Display melee's weapon bars.
    HudTd_DisplayMeleeWeaponBars(playerid, weaponId)
    {
        if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED) return 1;

        new 
            meleeBasicCnd = GetMeleeWeapon_Durability(weaponId, false),
            maxMeleeBars = meleeBasicCnd + GetMeleeWeapon_Durability(weaponId, true),

            playerMeleeCnd = GetPlayerItemAmount(playerid, weaponid),
            playerMeleeExtCnd = GetPlayerMeleeExtraCnd(playerid, weaponId);

        for(new tdIdx = 0; tdIdx != maxMeleeBars; tdIdx++)
        {
            switch(weaponId)
            {
                case WEAPON_PIPE:
                {
                    PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                }

                case WEAPON_SCISSOR_PIPE:
                {
                    if(tdIdx < meleeBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }
                    else if(tdIdx >= meleeBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded[playerid]) break;
                        
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], COLOR_WHITE);
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }
                }

                case WEAPON_HATCHET:
                {
                    if(tdIdx < meleeBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]);
                    }
                    else if(tdIdx >= meleeBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded[playerid]) break;

                        new upgradedTdIdx = (tdIdx - meleeBasicCnd);
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx], (upgradedTdIdx < playerMeleeExtCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    } 
                }

                case WEAPON_MACHETE:
                {
                    if(tdIdx < meleeBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]);
                    }
                    else if(tdIdx >= meleeBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded[playerid]) break;

                        new upgradedTdIdx = (tdIdx - meleeBasicCnd);
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx], (upgradedTdIdx < playerMeleeExtCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    }
                }
            }
        }
        return 0;
    }

    // HUD : Update certain bar of the melee weapon (reduct it from the condition of the melee).
    HudTd_UpdateMeleeWeaponCND(playerid, weaponId, tdIdx)
    {
        if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED)
        {
            return;
        }

        if(!IsPlayerMeleeUpgraded[playerid]) 
        {
            //SetPlayerWeaponAmmo(playerid, weaponId, tdIdx);
        }
        else
        {
            ePlayerItemData[playerid][weaponId][e_iItemMeleeExtraCND] = tdIdx;
        }

        switch(weaponId)
        {
            case 
                WEAPON_PIPE, 
                WEAPON_SCISSOR_PIPE:
                {
                    PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], COLOR_GRAY_TRANS, true);
                }

            case WEAPON_HATCHET:
            {
                PlayerTdSetColor(playerid, \
                    (IsPlayerMeleeUpgraded[playerid]) ? (ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]) : (ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]), \
                    COLOR_GRAY_TRANS, true);
            }
                
            case WEAPON_MACHETE:
            {
                PlayerTdSetColor(playerid, \
                    (IsPlayerMeleeUpgraded[playerid]) ? (ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]) : (ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]), \
                    COLOR_GRAY_TRANS, true);
            }
        }
    }

    // HUD : Hide melee conditions bars.
    HudTd_HideMeleeWeaponBars(playerid, itemId)
    {
        if(!IsItemMeleeWeapon(itemId))
        {
            return false;
        }
        
        new 
            maxBasicCnd = GetPlayerItemMaxAmount(playerid, itemId),
            maxExtraCnd = GetPlayerItemMaxAmount(playerid, itemId, true);

        for(new tdIdx = 0; tdIdx != maxExtraCnd; tdIdx++)
        {
            switch(itemId)
            {
                case 
                    WEAPON_PIPE,
                    WEAPON_SCISSOR_PIPE:
                    {
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }

                case WEAPON_HATCHET:
                {
                    if(tdIdx < maxBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]);
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded[playerid]) 
                            break;

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    } 
                }

                case WEAPON_MACHETE:
                {
                    if(tdIdx < maxBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]);
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded[playerid]) 
                            break;

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    }
                }
            }
        }
        return true;
    }

    // HUD : Hide melee upgraded condition bars.
    HudTd_HideMeleeUpgradedBars(playerid, weaponId)
    {
        if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED || !IsPlayerMeleeUpgraded[playerid])
            return;

        if(weaponId == WEAPON_SCISSOR_PIPE) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][9]);
        else if(weaponId == WEAPON_HATCHET || weaponId == WEAPON_MACHETE)
        {
            for(new tdIdx = 0; tdIdx != 3; tdIdx++)
            {
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]);
            }
        }
    }

//#endregion

//#region Bullet weapons HUD textdraws functions

    // HUD : Updates overall ammo display.
    HudTd_UpdateWeaponAmmo(playerid, itemId)
    {
        if(!IsItemWeapon(itemId) && !IsItemProjectile(itemId) && !IsItemMedkit(itemId))
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo]);
            return true;
        }

        new itemAmount = GetPlayerItemAmount(playerid, itemId);
        if(itemAmount == -1)
        {
            return false;
        }

        new String:itemAmountStr = str_val(itemAmount);
        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], itemAmountStr);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo]);
        return true;
    }

    // HUD : Update current ammo display.
    HudTd_DisplayWeaponCurrentAmmo(playerid, itemId)
    {
        if(!IsItemBulletWeapon(itemId))
        {
            return false;
        }

        new 
            currentClipAmmo = GetPlayerItemClipAmount(playerid, itemId),
            maxWeaponClipSize = GetPlayerItemMaxClipSize(playerid, itemId);

        if(maxWeaponClipSize == -1)
        {
            return false;
        }

        for(new tdIdx = 0; tdIdx != maxWeaponClipSize; tdIdx++)
        {
            switch(ITEM_IDX:itemId)
            {
                case ITEM_IDX_9MM:              PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx],      (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx]);
                case ITEM_IDX_REVOLVER:         PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx], (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx]);
                case ITEM_IDX_SHORTY:           PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][tdIdx],   (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][tdIdx]);
                case ITEM_IDX_SHOTGUN:          PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx],  (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx]);
                case ITEM_IDX_RIFLE:            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx],    (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx]);
                case ITEM_IDX_ASSAULT_RIFLE:    PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx],  (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx]);
            }
        }
        return true;
    }

    // HUD : Update certain bullet color.
    HudTd_ReduceWeaponCurrentAmmo(playerid, itemId, tdIdx)
    {
        if(!IsItemBulletWeapon(itemId))
        {
            return false;
        }

        switch(ITEM_IDX:itemId)
        {
            case ITEM_IDX_9MM:            PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx],      COLOR_GRAY_TRANS);
            case ITEM_IDX_REVOLVER:       PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx], COLOR_GRAY_TRANS);
            case ITEM_IDX_SHORTY:         PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][tdIdx],   COLOR_GRAY_TRANS);
            case ITEM_IDX_SHOTGUN:        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx],  COLOR_GRAY_TRANS);
            case ITEM_IDX_RIFLE:          PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx],    COLOR_GRAY_TRANS);
            case ITEM_IDX_ASSAULT_RIFLE:  PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx],  COLOR_GRAY_TRANS);
        }
        return true;
    }

    // HUD : Hide weapon's bullets.
    HudTd_HideWeaponCurrentAmmo(playerid, itemId)
    {
        if(!IsItemBulletWeapon(itemId))
        {
            return false;
        }

        new maxItemClipSize = GetPlayerItemMaxClipSize(playerid, itemId);
        if(maxItemClipSize == -1)
        {
            return false;
        }

        for(new tdIdx = 0; tdIdx != maxItemClipSize; tdIdx++)
        {
            switch(ITEM_IDX:itemId)
            {
                case ITEM_IDX_9MM:            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx]);
                case ITEM_IDX_REVOLVER:       PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx]);
                case ITEM_IDX_SHORTY:         PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][tdIdx]);
                case ITEM_IDX_SHOTGUN:        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx]);
                case ITEM_IDX_RIFLE:          PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx]);
                case ITEM_IDX_ASSAULT_RIFLE:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx]);
            }
        }
        return true;
    }

//#endregion

// HUD : Change hud background color from Red to Black (used when lost some health).
function HudTd_Bg_RedToBlack(playerid, Float:amount)
{
    new color = InterpolateColors(COLOR_DARK_RED, COLOR_BLACK_TRANS, amount);
    PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], color);

    if(!IsPlayerUsingBackpack(playerid))
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);
    }

    if(amount < 1.0)
    {
        SetTimerEx("HudTd_Bg_RedToBlack", 100, false, "df", playerid, amount += 0.1);
    }
}

// HUD : Switch colors from RED <-> DARK RED (if wounded) of the hud background.
function HudTd_Bg_FlashRed(playerid, colorId, Float:amount)
{
    if(!IsPlayerHudFlashing(playerid))
    {
        return;
    }

    if(GetPlayerHealthEx(playerid) <= 30.0)
    {
        new color;
        switch(colorId)
        {
            case 0:color = InterpolateColors(COLOR_RED, COLOR_DARK_RED, amount);
            case 1:color = InterpolateColors(COLOR_DARK_RED, COLOR_RED, amount);
        }
        PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], color);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

        if(amount < 1.0)
        {
            SetTimerEx("HudTd_Bg_FlashRed", 100, false, "ddf", playerid, colorId, amount += 0.1);
        }
        else
        {
            SetTimerEx("HudTd_Bg_FlashRed", 100, false, "ddf", playerid, (colorId == 0) ? (1) : (0), 0.0);
        }
        return;
    }

    Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
    HudTd_Bg_RedToBlack(playerid, 0.0);

    RestartHideHudTimer(playerid);
}