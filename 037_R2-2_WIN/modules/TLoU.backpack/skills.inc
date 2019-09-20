#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartedUpgradingSkill -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartedUpgradingSkill(playerid, skillid)
{
    new 
        barIdx = 0,
        Float:barColumn = 0.0,
        newSkillLvl = Skill_GetPlayerLevel(playerid, skillid)+1;

    // Check which progress bar should be updated.
    switch(skillid)
    {
        case 0: barIdx = Skill_GetPlayerLevel(playerid, skillid);
        case 1: barIdx = Skill_GetPlayerLevel(playerid, skillid) + 2;
        case 2: barIdx = Skill_GetPlayerLevel(playerid, skillid) + 5;
        case 3: barIdx = Skill_GetPlayerLevel(playerid, skillid) + 8;
        case 4: barIdx = Skill_GetPlayerLevel(playerid, skillid) + 11;
        case 5: barIdx = Skill_GetPlayerLevel(playerid, skillid) + 13;
    }

    // Check bar column of the bar index (important to know where the progress bar should start from!).
    switch(barIdx)
    {
        case 0, 2, 5, 8, 11, 13: barColumn = 407.4; // First column
        case 1, 3, 6, 9, 12, 14: barColumn = 419.4; // Second column
        case 4, 7, 10:           barColumn = 431.4; // Third column
    }

    Audio_PlayEx(playerid, AUDIO_SKILL_UPGRADE);
    Skill_UpgradeSkill(playerid, skillid, newSkillLvl, barColumn, barIdx, 0.0);
    ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_LOOP", 4.0, 0, 1, 1, 1, 0, 1);   
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerCanceledUpgradingSkill ------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCanceledUpgradingSkill(playerid)
{
    Audio_StopEx(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerUpgradedSkill -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerUpgradedSkill(playerid, skillid)
{
    new newLevel = Skill_GetPlayerLevel(playerid, skillid) + 1;
    Skill_SetPlayerLevel(playerid, skillid, newLevel);

    Audio_StopEx(playerid);
    Audio_PlayEx(playerid, AUDIO_CRAFT_FINISH);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerKeyStateChange -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid)) 
    {
        return 1;
    }
    
    if(PRESSED(KEY_FIRE))
    {
        if(IsPlayerUsingBackpack(playerid))
        {
            if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS 
            && Iter_Contains(SkillsToUpgrade<playerid>, Skill_GetSelection(playerid))
            && !IsPlayerUpgradingSkill(playerid))
            {
                Bit_Let(ePlayerFlag[e_bIsUpgradingSkill], playerid);
                OnPlayerStartedUpgradingSkill(playerid, Skill_GetSelection(playerid));
            }
        }
    }
    return 1;
}

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

    SkillsTd_CheckWhatsToUpgrade(playerid);
    SkillTd_UpdateViewingItem(playerid);
    UpdateNavItemsColors(playerid);
}

// Change skill's progress bar value.
function Skill_UpgradeSkill(playerid, skillid, newSkillLvl, Float:barColumn, barIdx, Float:bar_start_value)
{
    if(!IsPlayerUpgradingSkill(playerid))
    {
        return false;
    }

    if(IsPlayerHoldingKey(playerid, KEY_FIRE))
    {
        if(Skill_GetPlayerLevel(playerid, skillid) < newSkillLvl)
        {
            // If first value is higher than 100.0
            if(floatcmp(bar_start_value, 100.0) == 1)
            {
                Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
                OnPlayerUpgradedSkill(playerid, skillid);
            }
            else
            {
                bar_start_value += 7.4;
                PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx], pb_percent(barColumn, 6, 100.0, bar_start_value), 0.0);
                SetTimerEx("Skill_UpgradeSkill", 120, false, "dddfdf", playerid, skillid, newSkillLvl, barColumn, barIdx, bar_start_value);
            }
        }
    }
    else
    {
        PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx], pb_percent(barColumn, 6, 100.0, 0.0), 0.0);
        Bit_Vet(ePlayerFlag[e_bIsUpgradingSkill], playerid);
        OnPlayerCanceledUpgradingSkill(playerid);
    }
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx]);
    return true;
}

// Display player total-supps.
SkillsTd_UpdateSuppsAmount(playerid)
{
    new String:suppsAmountStr = str_val(GetPlayerSupplementsAmount(playerid));
    PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], suppsAmountStr);

    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount]);
    }
}

// Change description of the currently viewing skill item.
SkillTd_UpdateViewingItem(playerid)
{
    if(!IsPlayerUsingBackpack(playerid) || PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_SKILLS)
    {
        return false;
    }

    new selection = Skill_GetSelection(playerid);
    if(selection == SKILL_ITEM_SHIV_MASTER && Skill_GetPlayerLevel(playerid, SKILL_ITEM_SHIV_MASTER) > 0)
    {
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], "Full-durability shivs don't break~n~when used to escape a Clicker grapple");
    }
    else
    {
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], eBackpack_SkillItemsData[selection][e_iSkill_Description]);
    }
    
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
        barIdx = -1, 
        Float:barColumn = -1;
        
    for(new skillId = 0; skillId != MAX_SKILLS_ITEMS; skillId++)
    {
        if(Skill_GetPlayerLevel(playerid, skillId) <= 0)
            continue;

        switch(skillId)
        {
            case 0: barIdx = 0;
            case 1: barIdx = 2;
            case 2: barIdx = 5;
            case 3: barIdx = 8;
            case 4: barIdx = 11;
            case 5: barIdx = 13;
        }

        for(new i = 1; i <= Skill_GetPlayerLevel(playerid, skillId); i++)
        {
            if(i > eBackpack_SkillItemsData[skillId][e_iSkill_MaxLvl])
                break;

            switch(barIdx)
            {
                case 0, 2, 5, 8, 11, 13: barColumn = 407.4; // First column
                case 1, 3, 6, 9, 12, 14: barColumn = 419.4; // Second column
                case 4, 7, 10:           barColumn = 431.4; // Third column
            }

            PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][barIdx], pb_percent(barColumn, 6, 100.0, 100.0), 0.0);

            barIdx += 1;
        }
    }
}