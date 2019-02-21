// Check if HUD is visible to the player right now.
IsHudVisibleForPlayer(playerid)
{
    return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);
}

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

    KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
    ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);
}

// HUD : Hiding
function HideHud(playerid)
{
    if(!IsHudVisibleForPlayer(playerid)) 
        return;
    
    if(!Bit_Get(ePlayerFlag[e_bIsHudBgFlashing], playerid))
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg]);

        TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Wrench]);
        TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee]);

        TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Outline]);
        TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Hp_Underneath]);
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status]);
    }

    // Weapon HUD hiding is being done inside 'HidePlayerWeapon' !
    HidePlayerWeapon(playerid, CurrentPlayerWeaponId[playerid]);
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
    if(GetPlayerWeaponAmmo(playerid, WEAPON_SHIV) <= 0) TdHideForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
    else
    {
        new tmp_weapAmmo[5];
        valstr(tmp_weapAmmo, GetPlayerWeaponAmmo(playerid, WEAPON_SHIV));

        TdShowForPlayer(playerid, eTextDrawInfo[e_Hud_Shiv]);
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], tmp_weapAmmo);
    }
}

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

// HUD : Checks player's armed weapon and displays it.
HudTd_UpdateWeapon(playerid, weaponId)
{
    if(weaponId == WEAPON_UNARMED) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon]);
    else
    {
        for(new i = 0; i != MAX_HUD_WEAPONS; i++)
        {
            if(weaponId == eHudMenu_Items[i][e_iHud_WeaponId])
            {
                PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_sHud_WeapIcon]);
                PlayerTextDrawTextSize(playerid,  ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_fHud_WeapIconTextSize_X], eHudMenu_Items[i][e_fHud_WeapIconTextSize_Y]);
                PlayerTextDrawSetPos(playerid,    ePlayerTextDrawInfo[playerid][e_Hud_Weapon], eHudMenu_Items[i][e_fHud_WeapIconPos_X], eHudMenu_Items[i][e_fHud_WeapIconPos_Y]);
                PlayerTextDrawSetPos(playerid,    ePlayerTextDrawInfo[playerid][e_Hud_Ammo],   eHudMenu_Items[i][e_fHud_WeapAmmoPos_X], eHudMenu_Items[i][e_fHud_WeapAmmoPos_Y]);

                PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon]);
                break;
            }
        }
    }
}

// HUD : Updates overall ammo display.
HudTd_UpdateWeaponAmmo(playerid, weaponId)
{
    if(IsMeleeWeapon(weaponId) || IsThrowableWeapon(weaponId)) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo]);
    else
    {
        new tmp_weapAmmo[5];
        valstr(tmp_weapAmmo, GetPlayerWeaponAmmo(playerid, weaponId));
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], tmp_weapAmmo);
    }
}

// HUD : Display melee's weapon bars.
HudTd_DisplayMeleeWeaponBars(playerid, weaponId)
{
    if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED)
        return;

    new 
        meleeBasicCnd = GetMeleeWeapon_Durability(weaponId, false),
        maxMeleeBars = meleeBasicCnd + GetMeleeWeapon_Durability(weaponId, true),

        playerMeleeCnd = GetPlayerMeleeCnd[playerid],
        playerMeleeExtCnd = GetPlayerMeleeExtCnd[playerid];

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
                    if(!IsPlayerMeleeUpgraded[playerid]) 
                        break;
                    
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
                    if(!IsPlayerMeleeUpgraded[playerid]) 
                        break;

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
                    if(!IsPlayerMeleeUpgraded[playerid]) 
                        break;

                    new upgradedTdIdx = (tdIdx - meleeBasicCnd);
                    PlayerTextDrawBoxColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx], (upgradedTdIdx < playerMeleeExtCnd) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS));
                    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                }
            }
        }
    }
}

// HUD : Update certain bar of the melee weapon (reduct it from the condition of the melee).
HudTd_WeakenMeleeWeapon(playerid, weaponId, tdIdx)
{
    if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED)
        return;

    if(!IsPlayerMeleeUpgraded[playerid]) 
        SetPlayerWeaponAmmo(playerid, weaponId, tdIdx);
    else
        GetPlayerMeleeExtCnd[playerid] = tdIdx;

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
HudTd_HideMeleeWeaponBars(playerid, weaponId)
{
    if(!IsMeleeWeapon(weaponId) || weaponId == WEAPON_UNARMED) 
        return;
    
    new 
        meleeBasicCnd = GetMeleeWeapon_Durability(weaponId, false),
        maxMeleeBars = meleeBasicCnd + GetMeleeWeapon_Durability(weaponId, true);

    for(new tdIdx = 0; tdIdx != maxMeleeBars; tdIdx++)
    {
        switch(weaponId)
        {
            case 
                WEAPON_PIPE,
                WEAPON_SCISSOR_PIPE:
                {
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][tdIdx]);
                }

            case WEAPON_HATCHET:
            {
                if(tdIdx < meleeBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][tdIdx]);
                else if(tdIdx >= meleeBasicCnd)
                {
                    if(!IsPlayerMeleeUpgraded[playerid]) 
                        break;

                    new upgradedTdIdx = (tdIdx - meleeBasicCnd);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                } 
            }

            case WEAPON_MACHETE:
            {
                if(tdIdx < meleeBasicCnd) PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][tdIdx]);
                else if(tdIdx >= meleeBasicCnd)
                {
                    if(!IsPlayerMeleeUpgraded[playerid]) 
                        break;

                    new upgradedTdIdx = (tdIdx - meleeBasicCnd);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][upgradedTdIdx]);
                }
            }
        }
    }
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

// HUD : Update current ammo display.
HudTd_DisplayWeaponCurrentAmmo(playerid, weaponId)
{
    if(!IsBulletWeapon(weaponId))
        return;

    new 
        maxWeaponClipSize = GetPlayerWeaponLvl_MaxClipSize(playerid, weaponId),
        currentClipAmmo = CurrentPlayerWeaponClipSize[playerid];

    if(maxWeaponClipSize == 0)
        return;

    for(new tdIdx = 0; tdIdx != maxWeaponClipSize; tdIdx++)
    {
        switch(weaponId)
        {
            case WEAPON_9MM:      PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx],      (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx]);
            case WEAPON_REVOLVER: PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx], (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx]);
            case WEAPON_SHOTGUN:  PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx],  (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx]);
            case WEAPON_RIFLE:    PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx],    (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx]);
            case WEAPON_ASSAULT:  PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx],  (tdIdx < currentClipAmmo) ? (COLOR_WHITE) : (COLOR_GRAY_TRANS)), PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx]);
        }
    }
}

// HUD : Update certain bullet color.
HudTd_ReduceWeaponCurrentAmmo(playerid, weaponId, tdIdx)
{
    if(!IsBulletWeapon(weaponId))
        return;

    switch(weaponId)
    {
        case WEAPON_9MM:            PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx],      COLOR_GRAY_TRANS);
        case WEAPON_REVOLVER:       PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx], COLOR_GRAY_TRANS);
        case WEAPON_SHOTGUN:        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx],  COLOR_GRAY_TRANS);
        case WEAPON_RIFLE:          PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx],    COLOR_GRAY_TRANS);
        case WEAPON_ASSAULT:        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx],  COLOR_GRAY_TRANS);
    }
}

// HUD : Hide weapon's bullets.
HudTd_HideWeaponCurrentAmmo(playerid, weaponId)
{
    if(!IsBulletWeapon(weaponId))
        return;
    
    new maxWeaponClipSize = GetPlayerWeaponLvl_MaxClipSize(playerid, weaponId);

    if(maxWeaponClipSize == 0)
        return;
        
    for(new tdIdx = 0; tdIdx != maxWeaponClipSize; tdIdx++)
    {
        switch(weaponId)
        {
            case WEAPON_9MM:        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][tdIdx]);
            case WEAPON_REVOLVER:   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][tdIdx]);
            case WEAPON_SHOTGUN:    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][tdIdx]);
            case WEAPON_RIFLE:      PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][tdIdx]);
            case WEAPON_ASSAULT:    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][tdIdx]);
        }
    }
}

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
    if(Bit_Get(ePlayerFlag[e_bIsHudBgFlashing], playerid))
    {
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
                SetTimerEx("HudTd_Bg_FlashRed", 100, false, "ddf", playerid, colorId, amount += 0.1);
            else 
                SetTimerEx("HudTd_Bg_FlashRed", 100, false, "ddf", playerid, (colorId == 0) ? (1) : (0), 0.0);
        }
        else
        {
            Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
            HudTd_Bg_RedToBlack(playerid, 0.0);

            KillTimer(ePlayerInfo[playerid][e_iTimer_HideHud]);
            ePlayerInfo[playerid][e_iTimer_HideHud] = SetTimerEx("HideHud", TIMER_HIDE_HUD, false, "d", playerid);
        }
    }
}