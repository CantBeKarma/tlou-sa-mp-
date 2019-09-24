// Gets skill maximal possible level.
Skill_GetMaxLevel(skillId)
{
    return eBackpack_SkillItemsData[skillId][e_iSkill_MaxLvl];
}

// Get current player's skill level.
Skill_GetPlayerLevel(playerid, skillId)
{
    return eCharsInfo[playerid][e_iSkillLevel][skillId];
}

// Sets player certain level for the specified skillId.
Skill_SetPlayerLevel(playerid, skillId, level)
{
    eCharsInfo[playerid][e_iSkillLevel][skillId] = (level > Skill_GetMaxLevel(skillId) ? Skill_GetMaxLevel(skillId) : level);
    SetPlayerSupplements(playerid, GetPlayerSupplementsAmount(playerid) - eBackpack_SkillItemsData[skillId][e_iSupps_Required]);
}

// Change skill's progress bar value.
function Skill_UpgradeSkill(playerid, skillid, newSkillLvl, Float:barColumn, barIdx, Float:barStartValue)
{
    if(!IsPlayerUpgradingSkill(playerid))
    {
        return false;
    }

    if(IsPlayerHoldingKey(playerid, KEY_FIRE))
    {
        if(Skill_GetPlayerLevel(playerid, skillid) < newSkillLvl)
        {
            if(floatcmp(barStartValue, 100.0) == 1)
            {
                Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
                OnPlayerUpgradedSkill(playerid, skillid);
            }
            else
            {
                barStartValue += 7.4;
                PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx], pb_percent(barColumn, 6, 100.0, barStartValue), 0.0);
                SetTimerEx("Skill_UpgradeSkill", 120, false, "dddfdf", playerid, skillid, newSkillLvl, barColumn, barIdx, barStartValue);
            }
        }
    }
    else
    {
        barStartValue = 0.0;
        PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx], pb_percent(barColumn, 6, 100.0, barStartValue), 0.0);
        Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
        OnPlayerCanceledUpgradingSkill(playerid);
    }
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx]);
    return true;
}

// Display player total amount of supplements.
SkillsTd_UpdateSuppsAmount(playerid)
{
    new String:suppsAmountStr = str_val(GetPlayerSupplementsAmount(playerid));
    PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], suppsAmountStr);

    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount]);
    }
}

// Change description of player's currently viewing skill item and show how to upgrade if possible.
SkillsTd_UpdateViewingItem(playerid)
{
    new selection = Skill_GetSelection(playerid);
    if(selection == SKILL_ITEM_SHIV_MASTER && Skill_GetPlayerLevel(playerid, SKILL_ITEM_SHIV_MASTER) > 0)
    {
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], "Full-durability shivs don't break~n~when used to escape a Clicker grapple");
    }
    else
    {
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], eBackpack_SkillItemsData[selection][e_iSkill_Description]);
    }
    
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description]);

        if(Iter_Contains(SkillsToUpgrade<playerid>, selection))
        {
            TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0]);
            TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1]);
        }
        else
        {
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0]);
            TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1]);
        }
    }
    return true;
}

// Check which skills are allowed to upgrade.
SkillsTd_CheckWhatsToUpgrade(playerid)
{
    for(new i = 0; i != MAX_SKILLS_ITEMS; i++)
    {
        if(GetPlayerSupplementsAmount(playerid) >= eBackpack_SkillItemsData[i][e_iSupps_Required] && Skill_GetPlayerLevel(playerid, i) < Skill_GetMaxLevel(i))
        {
            if(!Iter_Contains(SkillsToUpgrade<playerid>, i))
            {
                Iter_Add(SkillsToUpgrade<playerid>, i);
            }

            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], COLOR_GRAY);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], COLOR_GRAY);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], COLOR_GRAY);
        }
        else
        {
            if(Iter_Contains(SkillsToUpgrade<playerid>, i))
            {
                Iter_Remove(SkillsToUpgrade<playerid>, i);
            }

            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], COLOR_RED_TRANS);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], COLOR_RED_TRANS);
            PlayerTextDrawColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], COLOR_RED_TRANS);
        }

        if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
        {
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i]);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i]);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i]);
        }
    }
}

// Most likely one-time-use (during sign in ONLY) | fills progress bars of the skills that player has.
SkillsTd_FillPlayerSkillBars(playerid)
{
    new 
        textdrawBarIdx = -1, 
        Float:textdrawBarColumnStartPosX = -1;
        
    for(new skillId = 0; skillId != MAX_SKILLS_ITEMS; skillId++)
    {
        if(Skill_GetPlayerLevel(playerid, skillId) <= 0)
            continue;

        textdrawBarIdx = SkillsTd_GetStartBarIdx(playerid, skillId, .basedOnPlayerSkill = false);

        for(new i = 1; i <= Skill_GetPlayerLevel(playerid, skillId); i++)
        {
            if(i > eBackpack_SkillItemsData[skillId][e_iSkill_MaxLvl])
                break;

            textdrawBarColumnStartPosX = SkillsTd_GetStartBarColumnPosX(textdrawBarIdx);
            PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][textdrawBarIdx], pb_percent(textdrawBarColumnStartPosX, 6, 100.0, 100.0), 0.0);

            textdrawBarIdx += 1;
        }
    }
}

// This function returns internal TEXTDRAW index of which progress bar should start drawing from. 
// It MUST be used before SkillsTd_GetStartBarColumnPosX()
SkillsTd_GetStartBarIdx(playerid, skillId, bool:basedOnPlayerSkill)
{
    new textdrawBarIdx = -1;
    if(basedOnPlayerSkill == false)
    {
        switch(skillId)
        {
            case SKILL_ITEM_MAXHP:          textdrawBarIdx = 0;
            case SKILL_ITEM_LISTEN_DIST:    textdrawBarIdx = 2;
            case SKILL_ITEM_CRAFT_SPEED:    textdrawBarIdx = 5;
            case SKILL_ITEM_HEAL_SPEED:     textdrawBarIdx = 8;
            case SKILL_ITEM_WEAP_SWAY:      textdrawBarIdx = 11;
            case SKILL_ITEM_SHIV_MASTER:    textdrawBarIdx = 13;
        }
    }
    else
    {
        switch(skillId)
        {
            case SKILL_ITEM_MAXHP:          textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId);
            case SKILL_ITEM_LISTEN_DIST:    textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId) + 2;
            case SKILL_ITEM_CRAFT_SPEED:    textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId) + 5;
            case SKILL_ITEM_HEAL_SPEED:     textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId) + 8;
            case SKILL_ITEM_WEAP_SWAY:      textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId) + 11;
            case SKILL_ITEM_SHIV_MASTER:    textdrawBarIdx = Skill_GetPlayerLevel(playerid, skillId) + 13;
        }
    }
    return textdrawBarIdx;
}

// This function returns TEXTDRAW progress bar internal position X of which bar should start drawing from.
// It MUST be used AFTER retrieving information of function: SkillsTd_GetStartBarIdx()
Float:SkillsTd_GetStartBarColumnPosX(targetTextdrawBarIdx)
{
    new Float:textdrawBarColumnStartPosX = 0.0;

    switch(targetTextdrawBarIdx)
    {
        case 0, 2, 5, 8, 11, 13: textdrawBarColumnStartPosX = 407.4; // First column
        case 1, 3, 6, 9, 12, 14: textdrawBarColumnStartPosX = 419.4; // Second column
        case 4, 7, 10:           textdrawBarColumnStartPosX = 431.4; // Third column
    }
    return textdrawBarColumnStartPosX;
}