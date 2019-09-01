#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerKeyStateChange ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerKeyStateChange(playerid, newkeys, oldkeys)
{
    if(!IsPlayerLogged(playerid) || !IsPlayerControllable(playerid))
    {
        return 1;
    }
    
    if(PRESSED(KEY_NO))
    {
        if(!IsPlayerCrafting(playerid) && (GetConnectedTime(playerid) - ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening]) > TICK_RATE_BP_OPENING)
        {
            if(!IsPlayerUsingBackpack(playerid) && !IsPlayerLookingAtItem(playerid))
            {
                SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_USING_BACKPACK);
            }
            else if(IsPlayerUsingBackpack(playerid))
            {
                SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
            }
        }
    }

    if(PRESSED(KEY_AIM))
    {
        if(IsPlayerUsingBackpack(playerid) && !IsPlayerViewingCollectible(playerid) && !IsPlayerCheckingCollectibles(playerid)
        && (GetConnectedTime(playerid) - ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening]) > TICK_RATE_BP_OPENING)
        {
            SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
        }

        if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_COLLECTIBLES)
        {
            if(IsPlayerCheckingCollectibles(playerid))
            {
                Bit_Vet(ePlayerFlag[e_bIsCheckingColls], playerid);
                PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox]);
            }
            else if(IsPlayerViewingCollectible(playerid))
            {
                Bit_Vet(ePlayerFlag[e_bIsViewingColl], playerid);
            }
        }
    }

    if(PRESSED(KEY_FIRE))
    {
        if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_COLLECTIBLES)
        {
            if(HasPlayerGotCollectible(playerid))
            {
                if(!IsPlayerCheckingCollectibles(playerid))
                {
                    Bit_Let(ePlayerFlag[e_bIsCheckingColls], playerid);
                    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox]);
                    Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);
                }
                else
                {
                    Bit_Let(ePlayerFlag[e_bIsViewingColl], playerid);
                    DB_ReadArtifact(playerid, Iter_Index(CollectiblesOwned<playerid>, Coll_ViewingGetSelection(playerid)));
                }
            }
        }
    }
    return 1;
}


//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerOpenBackpack ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerOpenBackpack(playerid, category)
{
    ePlayerBpInfo[playerid][e_iTickCounter_backpackOpening] = GetConnectedTime(playerid);
    
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
    CLF(#Perform_BackpackClosing, "d", playerid);
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
                {
                    return;
                }
                
                Craft_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Craft_GetSelection(playerid) == CRAFT_ITEM_SMOKE_BOMB)
                {
                    return;
                }
                
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
                {
                    return;
                }
                
                Skill_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Skill_GetSelection(playerid) == SKILL_ITEM_SHIV_MASTER)
                {
                    return;
                }

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
                {
                    return;
                }
                
                Coll_GetSelection(playerid) -= 1;
            }
            else if(keys == KEY_DOWN)
            {
                if(Coll_GetSelection(playerid) == COLL_ITEM_FIREFLY_PEND)
                {
                    return;
                }
                
                Coll_GetSelection(playerid) += 1;
            }
            
            Audio_PlayEx(playerid, AUDIO_SKILL_UPDWN);

            new selection = Coll_GetSelection(playerid);
            Backpack_MoveSelectBox(playerid, eBackpack_CollItemsData[selection][e_fSelBoxX], eBackpack_CollItemsData[selection][e_fSelBoxY]);

            CollTd_ShowTitles(playerid);
        }
    }
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- OnPlayerSwitchBpCtgry -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerSwitchBpCtgry(playerid, leftRight)
{
    new 
        newCategory = -1,
        oldCategory = PlayerBackpackCategory(playerid);

    if(leftRight == KEY_LEFT)
    {
        if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING) 
        {
            return;
        }

        newCategory = PlayerBackpackCategory(playerid) -= 1;
        Audio_PlayEx(playerid, AUDIO_NAVI_LEFT);
    }
    else if(leftRight == KEY_RIGHT)
    {
        if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_COLLECTIBLES)
        {
            return;
        }

        newCategory = PlayerBackpackCategory(playerid) += 1;
        Audio_PlayEx(playerid, AUDIO_NAVI_RIGHT);
    }

    HideBackpackCategory(playerid, oldCategory);
    ShowBackpackCategory(playerid, newCategory);    
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerViewCollectibles ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
forward OnPlayerViewCollectibles(playerid, updown);
public OnPlayerViewCollectibles(playerid, updown)
{
    new Float:playerCollSelectBoxX, Float:playerCollSelectBoxY;
    PlayerTextDrawGetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], playerCollSelectBoxX, playerCollSelectBoxY);
    
    new oldSelection = Coll_ViewingGetSelection(playerid);
    if(updown == KEY_UP)
    {
        if(Coll_ViewingGetSelection(playerid) == 0)
        {
            return 1;
        }

        Coll_ViewingGetSelection(playerid) -= 1;
        playerCollSelectBoxY -= 24.0;
    }
    else if(updown == KEY_DOWN)
    {
        if(Coll_ViewingGetSelection(playerid) == 8)
        {
            return 1;
        }
        
        Coll_ViewingGetSelection(playerid) += 1;
        playerCollSelectBoxY += 24.0;
    }

    if(!IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][Coll_ViewingGetSelection(playerid)]))
    {
        Coll_ViewingGetSelection(playerid) = oldSelection;
        return 1;
    }

    Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);

    Backpack_MoveCollSelectBox(playerid, playerCollSelectBoxX, playerCollSelectBoxY);
    return 0;
}

HasPlayerGotCollectible(playerid)
{
    return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][0]);
}

CMD:mycoll(playerid, params[])
{
    SCMF(playerid, -1, "Coll_ViewingGetSelection(playerid) = %d", Coll_ViewingGetSelection(playerid));
    return 1;
}