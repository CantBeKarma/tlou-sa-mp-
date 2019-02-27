#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!Bit_Get(ePlayerFlag[e_bLogged], playerid) || !IsPlayerControllable(playerid)) 
        return 1;
    
    if(PRESSED(KEY_NO))
    {
        // Backpack opening/closing.
        if(!IsPlayerCrafting(playerid) && (GetConnectedTime(playerid) - ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening]) > TICK_RATE_BP_OPENING)
        {
            if(!IsPlayerUsingBackpack(playerid) && !IsPlayerUsingWeaponMenu(playerid) 
                && GetPlayerSpeed(playerid) <= 0.0 && !IsPlayerLookingAtItem(playerid))
            {
                ShowBackpack(playerid, PlayerBackpackCategory(playerid));
            }
            else if(IsPlayerUsingBackpack(playerid))
            {
                HideBackpack(playerid);
            }
        }
    }

    if(PRESSED(KEY_AIM))
    {
        // Backpack closing.
        HideBackpack(playerid);
    }

    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerOpenBackpack ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerOpenBackpack(playerid, category)
{
    PlayerBackpackCategory(playerid) = category;
    ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening] = GetConnectedTime(playerid);
    
    Disable_OnPlayerLookAt(playerid);
    Perform_BackpackOpening(playerid);
    ShowBackpackCategory(playerid, category);
    Key_BackpackMenu(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerCloseBackpack ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCloseBackpack(playerid)
{
    ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening] = GetConnectedTime(playerid);
    
    Enable_OnPlayerLookAt(playerid);
    Perform_BackpackClosing(playerid);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerUseBackpack -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerUseBackpack(playerid, category, keys)
{
    switch(category)
    {
        case BACKPACK_CATEGORY_CRAFTING:
        {
            new oldSelection = Craft_GetSelection(playerid);
            if(keys == KEY_UP)
            {
                if(Craft_GetSelection(playerid) == CRAFT_ITEM_MELEE) 
                    return;
                
                Craft_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Craft_GetSelection(playerid) == CRAFT_ITEM_SMOKE_BOMB) 
                    return;
                
                Craft_GetSelection(playerid) += 1;
            }

            Audio_PlayEx(playerid, RandomEx(AUDIO_CRAFT_UPDWN1, AUDIO_CRAFT_UPDWN5));
            
            new newSelection = Craft_GetSelection(playerid);
            CraftTd_UpdateViewingItem(playerid, newSelection);
            CraftTd_HighlightSelection(playerid, newSelection, oldSelection);
            Backpack_MoveSelectBox(playerid, eBackpack_CraftItemsData[newSelection][e_fSelBoxX], eBackpack_CraftItemsData[newSelection][e_fSelBoxY]);
        }

        case BACKPACK_CATEGORY_SKILLS:
        {
            if(keys == KEY_UP)
            {
                if(Skill_GetSelection(playerid) == SKILL_ITEM_MAXHP)
                    return;
                
                Skill_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Skill_GetSelection(playerid) == SKILL_ITEM_SHIV_MASTER)
                    return;

                Skill_GetSelection(playerid) += 1;
            }

            Audio_PlayEx(playerid, AUDIO_SKILL_UPDWN);

            new selection = Skill_GetSelection(playerid);
            SkillTd_UpdateViewingItem(playerid, selection);
            Backpack_MoveSelectBox(playerid, eBackpack_SkillItemsData[selection][e_fSelBoxX], eBackpack_SkillItemsData[selection][e_fSelBoxY]);
        }

        case BACKPACK_CATEGORY_COLLECTIBLES:
        {
            if(keys == KEY_UP)
            {
                if(Coll_GetSelection(playerid) == COLL_ITEM_ARTIFACTS)
                    return;
                
                Coll_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Coll_GetSelection(playerid) == COLL_ITEM_FIREFLY_PEND) 
                    return;
                
                Coll_GetSelection(playerid) += 1;
            }

            Audio_PlayEx(playerid, AUDIO_SKILL_UPDWN);

            new selection = Coll_GetSelection(playerid);
            Backpack_MoveSelectBox(playerid, eBackpack_CollItemsData[selection][e_fSelBoxX], eBackpack_CollItemsData[selection][e_fSelBoxY]);
        }
    }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerSwitchBpCtgry -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerSwitchBpCtgry(playerid, leftRight)
{
    new oldCategory = PlayerBackpackCategory(playerid);
    if(leftRight == KEY_LEFT)
    {
        if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING) 
            return;

        PlayerBackpackCategory(playerid) -= 1;

        Audio_PlayEx(playerid, AUDIO_NAVI_LEFT);
    }
    else if(leftRight == KEY_RIGHT)
    {
        if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_COLLECTIBLES)
            return;

        PlayerBackpackCategory(playerid) += 1;

        Audio_PlayEx(playerid, AUDIO_NAVI_RIGHT);
    }

    HideBackpackCategory(playerid, oldCategory);

    new newCategory = PlayerBackpackCategory(playerid);
    ShowBackpackCategory(playerid, newCategory);    
}