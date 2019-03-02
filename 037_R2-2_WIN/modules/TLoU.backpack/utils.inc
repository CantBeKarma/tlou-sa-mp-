// Clearing player's backpack data.
ResetEnum_ePlayerBpInfo(playerid)
{
    static sBlank[e_PLAYER_BACKPACK_DATA];
    ePlayerBpInfo[playerid] = sBlank;

    stop ePlayerBpInfo[playerid][e_iTimer_craftingOrUpgrading];

    print("ResetEnum_ePlayerBpInfo");
}

// Check if player is using backpack.
IsPlayerUsingBackpack(playerid)
{
    return IsTextDrawVisibleForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Bg]);
}

// Check if player is currently crafting something.
IsPlayerCrafting(playerid)
{
    return Bit_Get(ePlayerFlag[e_bIsCrafting], playerid);
}

// Change the position of selection box.
Backpack_MoveSelectBox(playerid, Float:x, Float:y)
{
    Backpack_ResizeSelectBox(playerid);
    TextDrawSetPos(eTextDrawInfo[e_Bp_SelectBox][playerid], x, y);
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_SelectBox][playerid]);
}

// Change size of the selection box for certain backpack category.
Backpack_ResizeSelectBox(playerid)
{
    if(IsPlayerUsingBackpack(playerid))
    {
        switch(PlayerBackpackCategory(playerid))
        {
            case BACKPACK_CATEGORY_CRAFTING:    TextDrawTextSize(eTextDrawInfo[e_Bp_SelectBox][playerid], 45.0, 56.0);
            case BACKPACK_CATEGORY_SKILLS:      TextDrawTextSize(eTextDrawInfo[e_Bp_SelectBox][playerid], 193.666671, 24.888868);
            case BACKPACK_CATEGORY_COLLECTIBLES:TextDrawTextSize(eTextDrawInfo[e_Bp_SelectBox][playerid], 108.333343, 29.037019);
        }
    }
    else if(IsPlayerUsingWeaponMenu(playerid))
    {
        TextDrawTextSize(eTextDrawInfo[e_Bp_SelectBox][playerid], 45.0, 56.0);
    }
}

// Perform anim. with camera backpack opening.
Perform_BackpackOpening(playerid)
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid))
    {
        return 1;
    }

    // If HUD background is flashing - disable it while backpack is opened.
    if(Bit_Get(ePlayerFlag[e_bIsHudBgFlashing], playerid))
    {
        Bit_Vet(ePlayerFlag[e_bIsHudBgFlashing], playerid);
    }

    HideHud(playerid);
    HidePlayerWeapon(playerid);
    AttachBackpackToPlayer(playerid, true);
    MoveCameraNextToPlayer(playerid, -0.9, 0.25, 500);
    ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 4.0, 0, 1, 1, 1, 0, 1);
    Audio_PlayEx(playerid, AUDIO_BP_OPEN);
    return 0;
}

// Perform anim. with camera backpack closing.
Perform_BackpackClosing(playerid)
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid)) return 1;
    
    // If HUD was flashing before using backpack - enable HUD flash again.
    if(eCharsInfo[playerid][e_fHealth] <= 30.0)
    {
        Bit_Let(ePlayerFlag[e_bIsHudBgFlashing], playerid);
        HudTd_Bg_FlashRed(playerid, 0, 0.0);
    }

    ShowHud(playerid);
    SetCameraBehindPlayer(playerid);
    AttachBackpackToPlayer(playerid);
    ApplyCallbackAnim(playerid, "BOMBER", "BOM_PLANT_2IDLE"); // ApplyCallbackAnim function because giving player's back his Last weapon used once animation finishes.
    Audio_PlayEx(playerid, AUDIO_BP_CLOSE);
    return 0;
}

// Change the colors of WRENCH and SUPPLEMENT icons in the navigation menu.
UpdateNavItemsColors(playerid)
{
    if(!IsPlayerUsingBackpack(playerid))
    {
        if(Iter_Count(ItemsToCraft<playerid>) <= 0)
            CraftTd_CheckWhatsToCraft(playerid);

        if(Iter_Count(ItemsToCraft<playerid>) > 0)
            ShowHud(playerid);
    }
    else
    {
        if(PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_CRAFTING)
        {
            if(Iter_Count(ItemsToCraft<playerid>) <= 0)
                CraftTd_CheckWhatsToCraft(playerid);
        }

        if(PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_SKILLS)
        {
            if(Iter_Count(SkillsToUpgrade<playerid>) <= 0)
                SkillsTd_CheckWhatsToUpgrade(playerid);
        }
      
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][0], (Iter_Count(ItemsToCraft<playerid>) <= 0) ? (-1) : (COLOR_GOLD));
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][1], (Iter_Count(SkillsToUpgrade<playerid>) <= 0) ? (-1) : (COLOR_GOLD));
    }
}

// Show player backpack based on the category (CRAFTING / SKILLS / COLLECTIBLE)
ShowBackpack(playerid, category)
{
    if(IsPlayerUnableToPerformAction(playerid))
    {
        return 1;
    }

    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Bg]);
    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_NavLeft]);
    for(new i = 0; i != 3; i++)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i]);
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToClose][i]);
    }
    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_NavRight]);

    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_CatLineTop]);
    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_CatLineBottom]);

    OnPlayerOpenBackpack(playerid, category);
    return 0;
}

// Hide player's backpack menu.
HideBackpack(playerid, forceClose = false)
{
    if(!IsPlayerUsingBackpack(playerid))
        return;
        
    if(!forceClose)
    {
        if(IsPlayerCrafting(playerid) || IsPlayerUpgradingSkill(playerid)
        || ((GetConnectedTime(playerid) - ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening]) < TICK_RATE_BP_OPENING))
        {
            return;
        }
    }
    else
    {
        if(IsPlayerCrafting(playerid))
        {
            Bit_Vet(ePlayerFlag[e_bIsCrafting], playerid);
            Audio_StopEx(playerid);
            stop ePlayerBpInfo[playerid][e_iTimer_craftingOrUpgrading];
        }
        
        if(IsPlayerUpgradingSkill(playerid))
        {
            Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
            Audio_StopEx(playerid);
        }
    }

    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Bg]);

    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_SelectBox][playerid]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_NavLeft]);
    for(new i = 0; i != 3; i++)
    {
        PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i]);
        TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToClose][i]);
    }
    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_NavRight]);

    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_CatLineTop]);
    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName]);
    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_CatLineBottom]);

    HideBackpackCategory(playerid, PlayerBackpackCategory(playerid));
    OnPlayerCloseBackpack(playerid);
}

// Display backpack's category.
ShowBackpackCategory(playerid, category)
{
    UpdateNavItemsColors(playerid);
    switch(category)
    {
        case BACKPACK_CATEGORY_CRAFTING:
        {
            new selectedCraft = Craft_GetSelection(playerid);
            CraftTd_CheckWhatsToCraft(playerid);
            CraftTd_UpdateViewingItem(playerid, selectedCraft);
            CraftTd_HighlightSelection(playerid, selectedCraft);
            CraftTd_UpdateIngr(playerid);
            CraftTd_UpdateItemAmount(playerid);

            TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Ls_Bg]);
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], "CRAFTING");
            PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 428.0, 31.111112);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox]);

            TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_LineAboveIngr]);
            for(new i = 0; i != 19; i++)
            {
                PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i]);
                if(i < 7)
                {
                    PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i]);
                }

                if(i < 6)
                {
                    PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i]);
                }
            }
            Backpack_MoveSelectBox(playerid, eBackpack_CraftItemsData[selectedCraft][e_fSelBoxX], eBackpack_CraftItemsData[selectedCraft][e_fSelBoxY]);
        }

        case BACKPACK_CATEGORY_SKILLS:
        {
            new selectedSkill = Skill_GetSelection(playerid);
            SkillsTd_CheckSuppsAmount(playerid);
            SkillsTd_CheckWhatsToUpgrade(playerid);
            SkillTd_UpdateViewingItem(playerid, selectedSkill);

            PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon]);
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], "SKILLS");
            PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 459.666748, 31.111112);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox]);

            for(new i = 0; i != 15; i++)
            {
                TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i]);
                PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i]);
                if(i < 6)
                {
                    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Skills][i]);
                }
            }
            Backpack_MoveSelectBox(playerid, eBackpack_SkillItemsData[selectedSkill][e_fSelBoxX], eBackpack_SkillItemsData[selectedSkill][e_fSelBoxY]);
        }

        case BACKPACK_CATEGORY_COLLECTIBLES:
        {
            new selectedColl = Coll_GetSelection(playerid);
//            Display_ArtifactsList(playerid);
            
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], "COLLECTIBLES");
            PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 490.333557, 30.696300);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox]);

            TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine]);

            for(new i = 0; i != 2; i++)
            {
                TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][i]);
                TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i]);
                TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i]);
            }
            Backpack_MoveSelectBox(playerid, eBackpack_CollItemsData[selectedColl][e_fSelBoxX], eBackpack_CollItemsData[selectedColl][e_fSelBoxY]);
        }
    }
}

// Hide backpack category (not hiding backpack!)
HideBackpackCategory(playerid, category)
{
    switch(category)
    {
        case BACKPACK_CATEGORY_CRAFTING:
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Ls_Bg]);
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_LineAboveIngr]);

            for(new i = 0; i != 19; i++)
            {
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i]);
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i]);
                if(i < 7)
                {
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i]);
                }

                if(i < 6)
                {
                    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][i]);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i]);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i]);
                }

                if(i < 5)
                {
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i]);
                }

                if(i < 3)
                {
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][i]);
                }
            }

            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][0]);
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][1]);
        }

        case BACKPACK_CATEGORY_SKILLS:
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount]);

            for(new i = 0; i != 15; i++)
            {
                TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i]);
                PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i]);
                if(i < 6)
                {
                    TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Skills][i]);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i]);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i]);
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i]);
                }
            }
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description]);

            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0]);
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1]);
        }

        case BACKPACK_CATEGORY_COLLECTIBLES:
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine]);

            for(new i = 0; i != 2; i++)
            {
                TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][i]);
                TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i]);
                TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i]);
            }
        }
    }
}