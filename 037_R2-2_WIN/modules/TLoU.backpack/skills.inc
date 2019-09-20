#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartedUpgradingSkill -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartedUpgradingSkill(playerid, skillid)
{
    new 
        barIdx = 0,
        Float:barColumn = 0.0,
        newSkillLvl = Skill_GetLevel(playerid, skillid)+1;

    // Check which progress bar should be updated.
    switch(skillid)
    {
        case 0: barIdx = Skill_GetLevel(playerid, skillid);
        case 1: barIdx = Skill_GetLevel(playerid, skillid) + 2;
        case 2: barIdx = Skill_GetLevel(playerid, skillid) + 5;
        case 3: barIdx = Skill_GetLevel(playerid, skillid) + 8;
        case 4: barIdx = Skill_GetLevel(playerid, skillid) + 11;
        case 5: barIdx = Skill_GetLevel(playerid, skillid) + 13;
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
    new newLevel = Skill_GetLevel(playerid, skillid) + 1;
    Skill_SetLevel(playerid, skillid, newLevel);

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

// Sets player certain level for the specified skillId.
Skill_SetLevel(playerid, skillId, level)
{
    Skill_GetLevel(playerid, skillId) = level;
    Skill_SuppsAmount(playerid) -= eBackpack_SkillItemsData[skillId][e_iSupps_Required];
    
    if(skillId == SKILL_ITEM_SHIV_MASTER)
    {
        SkillTd_UpdateViewingItem(playerid, SKILL_ITEM_SHIV_MASTER);
    }

    SkillsTd_UpdateSuppsAmount(playerid);
    SkillsTd_CheckWhatsToUpgrade(playerid);
    UpdateNavItemsColors(playerid);
}

// Change skill's progress bar value.
function Skill_UpgradeSkill(playerid, skillid, newSkillLvl, Float:barColumn, barIdx, Float:bar_start_value)
{
    if(!IsPlayerUpgradingSkill(playerid))
        return;

    if(IsPlayerHoldingKey(playerid, KEY_FIRE))
    {
        if(Skill_GetLevel(playerid, skillid) < newSkillLvl)
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
}

// Display player total-supps.
SkillsTd_UpdateSuppsAmount(playerid)
{
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        new String:suppsAmountStr = str_val(Skill_SuppsAmount(playerid));
        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], suppsAmountStr);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount]);
    }
}

// Change description of the currently viewing skill item.
SkillTd_UpdateViewingItem(playerid, selection)
{
    if(selection == SKILL_ITEM_SHIV_MASTER && Skill_GetLevel(playerid, SKILL_ITEM_SHIV_MASTER) > 0)
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], "Full-durability shivs don't break~n~when used to escape a Clicker grapple");
    else
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], eBackpack_SkillItemsData[selection][e_iSkill_Description]);
    
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

// Check which skills are allowed to upgrade.
SkillsTd_CheckWhatsToUpgrade(playerid)
{
    for(new i = 0; i != MAX_SKILLS_ITEMS; i++)
    {
        if(Skill_SuppsAmount(playerid) >= eBackpack_SkillItemsData[i][e_iSupps_Required] && Skill_GetLevel(playerid, i) < eBackpack_SkillItemsData[i][e_iSkill_MaxLvl])
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
    }

    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        for(new i = 0; i != MAX_SKILLS_ITEMS; i++)
        {
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i]);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i]);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i]);
        }
    }
}

// Most likely one-time-use (during sign in ONLY) | fills progress bars of the skills that player has.
SkillTd_FillPlayerSkillBars(playerid)
{
    new 
        barIdx = -1, 
        Float:barColumn = -1;
        
    for(new skillId = 0; skillId != MAX_SKILLS_ITEMS; skillId++)
    {
        if(Skill_GetLevel(playerid, skillId) <= 0)
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

        for(new i = 1; i <= Skill_GetLevel(playerid, skillId); i++)
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