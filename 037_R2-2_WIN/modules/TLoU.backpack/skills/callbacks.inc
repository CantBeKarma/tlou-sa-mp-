#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStartedUpgradingSkill -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartedUpgradingSkill(playerid, skillId)
{
    new 
        textdrawBarIdx = SkillsTd_GetStartBarIdx(playerid, skillId, .basedOnPlayerSkill = true),
        Float:textdrawBarColumnStartPosX = SkillsTd_GetStartBarColumnPosX(textdrawBarIdx),
        newSkillLvl = Skill_GetPlayerLevel(playerid, skillId)+1;

    static Float:barStartValue = 0.0;

    Audio_PlayEx(playerid, AUDIO_SKILL_UPGRADE);
    Skill_UpgradeSkill(playerid, skillId, newSkillLvl, textdrawBarColumnStartPosX, textdrawBarIdx, barStartValue);
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
public OnPlayerUpgradedSkill(playerid, skillId)
{
    new newLevel = Skill_GetPlayerLevel(playerid, skillId) + 1;
    Skill_SetPlayerLevel(playerid, skillId, newLevel);

    if(skillId == SKILL_ITEM_SHIV_MASTER)
    {
        SetPlayerShivCND(playerid, GetPlayerMaxShivCND(playerid));
    }

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