// Check if HUD is visible to the player right now.
IsHudVisibleForPlayer(playerid)return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

//#region Overall HUD functions (Showing/Hiding)

    // HUD : Showing.
    ShowHud(playerid)
    {
        PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

        HudTd_UpdateCraftingWrench(playerid);
        HudTd_UpdateShivs(playerid);
        HudTd_UpdateMelee(playerid);

        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Outline]);
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Underneath]);
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

//#endregion

//#region Melee HUD textdraws functions

    // HUD : Checks if player has any melee weapon. If so, displays appropriate melee icon.
    HudTd_UpdateMelee(playerid)
    {
        new playerMeleeItemId = GetPlayerMeleeItemId(playerid);
        if(playerMeleeItemId == -1) 
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee]);
        }
        else
        {
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], eItemsSprites[playerMeleeItemId]);
        }
    }

    // HUD : Display melee's weapon bars.
    HudTd_DisplayMeleeWeaponBars(playerid, itemId)
    {
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MELEE))
        {
            return false;
        }

        new 
            maxBasicCnd = GetPlayerItemMaxAmount(playerid, itemId),
            maxMeleeBars = maxBasicCnd + GetPlayerItemMaxAmount(playerid, itemId, true);

        new 
            playerMeleeCnd = GetPlayerItemAmount(playerid, itemId),
            playerMeleeExtCnd = GetPlayerMeleeExtraCnd(playerid, itemId);

        for(new tdIdx = 0; tdIdx != maxMeleeBars; tdIdx++)
        {
            switch(ITEM_IDX:itemId)
            {
                case ITEM_IDX_PIPE:
                {
                    PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                }

                case ITEM_IDX_SCSR_PIPE:
                {
                    if(tdIdx < maxBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded(playerid))
                        {
                            break;
                        }
                        
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], COLOR_WHITE);
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }
                }

                case ITEM_IDX_HATCHET:
                {
                    if(tdIdx < maxBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]);
                    }
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded(playerid))
                        {
                            break;
                        }

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx], (upgradedTdIdx < playerMeleeExtCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    } 
                }

                case ITEM_IDX_MACHETE:
                {
                    if(tdIdx < maxBasicCnd)
                    {
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx], (tdIdx < playerMeleeCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]);
                    }
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded(playerid))
                        {
                            break;
                        }

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx], (upgradedTdIdx < playerMeleeExtCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    }
                }
            }
        }
        return true;
    }

    // HUD : Update certain bar of the melee weapon (reduct it from the condition of the melee).
    HudTd_UpdateMeleeWeaponCND(playerid, itemId, tdIdx)
    {
        if(!IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            return false;
        }

        switch(ITEM_IDX:itemId)
        {
            case 
                ITEM_IDX_PIPE, 
                ITEM_IDX_SCSR_PIPE:
                {
                    PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx], COLOR_GRAY_TRANS, true);
                }

            case ITEM_IDX_HATCHET:
            {
                PlayerTdSetColor(playerid, \
                    (IsPlayerMeleeUpgraded(playerid)) ? (ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]) : (ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]), \
                    COLOR_GRAY_TRANS, true);
            }
                
            case ITEM_IDX_MACHETE:
            {
                PlayerTdSetColor(playerid, \
                    (IsPlayerMeleeUpgraded(playerid)) ? (ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]) : (ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]), \
                    COLOR_GRAY_TRANS, true);
            }
        }
        return true;
    }

    // HUD : Hide melee conditions bars.
    HudTd_HideMeleeWeaponBars(playerid, itemId)
    {
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MELEE))
        {
            return false;
        }
        
        new 
            maxBasicCnd = GetPlayerItemMaxAmount(playerid, itemId),
            maxMeleeBars = maxBasicCnd + GetPlayerItemMaxAmount(playerid, itemId, true);

        for(new tdIdx = 0; tdIdx != maxMeleeBars; tdIdx++)
        {
            switch(ITEM_IDX:itemId)
            {
                case 
                    ITEM_IDX_PIPE,
                    ITEM_IDX_SCSR_PIPE:
                    {
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                    }

                case ITEM_IDX_HATCHET:
                {
                    if(tdIdx < maxBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]);
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded(playerid))
                        {
                            break;
                        }

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    } 
                }

                case ITEM_IDX_MACHETE:
                {
                    if(tdIdx < maxBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]);
                    else if(tdIdx >= maxBasicCnd)
                    {
                        if(!IsPlayerMeleeUpgraded(playerid))
                        {
                            break;
                        }

                        new upgradedTdIdx = (tdIdx - maxBasicCnd);
                        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                    }
                }
            }
        }
        return true;
    }

    // HUD : Hide melee upgraded condition bars.
    HudTd_HideMeleeUpgradedBars(playerid, itemId)
    {
        if(!IsItemType(itemId, ITEM_TYPE_MELEE))
        {
            return false;
        }

        if(itemId == _:ITEM_IDX_SCSR_PIPE) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][9]);
        else if(itemId == _:ITEM_IDX_HATCHET || itemId == _:ITEM_IDX_MACHETE)
        {
            for(new tdIdx = 0; tdIdx <= 2; tdIdx++)
            {
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][tdIdx]);
            }
        }
        return true;
    }

//#endregion

//#region Bullet weapons HUD textdraws functions

    // HUD : Updates overall ammo display.
    HudTd_UpdateWeaponAmmo(playerid, itemId)
    {
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_WEAPON) && !IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_PROJECTILE) && !IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_MEDKIT))
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
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON))
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
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON))
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
        if(!IsItemType(itemId, ITEM_TYPE:ITEM_TYPE_BULLET_WEAPON))
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

//#region Hud flashing (red-to-black and red-dark red)

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
            SetTimerEx("HudTd_Bg_RedToBlack", TIMER_HUD_FLASH_RATE, false, "df", playerid, amount += 0.1);
        }
    }

    // HUD : Switch colors from RED <-> DARK RED (if wounded) of the hud background.
    function HudTd_Bg_FlashRed(playerid, colorId, Float:amount)
    {
        if(!IsPlayerHudFlashing(playerid))
        {
            return;
        }

        if(GetPlayerHealthEx(playerid) <= MIN_HEALTH_FLASH_HUD)
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
                SetTimerEx("HudTd_Bg_FlashRed", TIMER_HUD_FLASH_RATE, false, "ddf", playerid, colorId, amount += 0.1);
            }
            else
            {
                SetTimerEx("HudTd_Bg_FlashRed", TIMER_HUD_FLASH_RATE, false, "ddf", playerid, (colorId == 0) ? (1) : (0), 0.0);
            }
            return;
        }

        Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
        HudTd_Bg_RedToBlack(playerid, 0.0);

        RestartHideHudTimer(playerid);
    }

//#endregion