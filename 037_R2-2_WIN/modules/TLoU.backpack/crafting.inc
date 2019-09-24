#include	<YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerStartedCrafting ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStartedCrafting(playerid, craftItemId)
{
    Craft_CraftItem(playerid, craftItemId, 0);
    Audio_PlayEx(playerid, eBackpack_CraftItemsData[craftItemId][e_iCraftSfx]);
    ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_LOOP", 4.0, 0, 1, 1, 1, 0, 1);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerCanceledCrafting ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCanceledCrafting(playerid)
{
    Audio_StopEx(playerid);
    stop ePlayerBpInfo[playerid][e_iTimer_craftingOrUpgrading];
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ OnPlayerCraftedItem ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCraftedItem(playerid, craftItemId)
{
    new itemId = 
        (craftItemId == CRAFT_ITEM_MELEE) 
        ? (GetPlayerMeleeItemId(playerid)) 
        : (_:eBackpack_CraftItemsData[craftItemId][e_iCraftItemId]);

    if(craftItemId != CRAFT_ITEM_MELEE)
    {
        GivePlayerItem(playerid, itemId);
        SetPlayerItemAmount(playerid, itemId, GetPlayerItemAmount(playerid, itemId) + 1);
    }
    else
    {
        new playerMeleeCnd = GetPlayerItemAmount(playerid, itemId);

        if(itemId == _:ITEM_IDX_PIPE)
        {
            RemovePlayerItem(playerid, itemId);

            itemId = _:ITEM_IDX_SCSR_PIPE;
            GivePlayerItem(playerid, itemId);
        }

        SetPlayerItemAmount(playerid, itemId, playerMeleeCnd, GetMeleeItemMaxCND(itemId, .extraCnd = true));
    }

    // Reduct ingredients.
    static reductIngrAmount = 100;

    new 
        firstRequiredIngr = _:eBackpack_CraftItemsData[craftItemId][e_iCraftFirstReqIngr],
        secondRequiredIngr = _:eBackpack_CraftItemsData[craftItemId][e_iCraftSecondReqIngr];

    SetPlayerItemAmount(playerid, firstRequiredIngr, GetPlayerItemAmount(playerid, firstRequiredIngr) - reductIngrAmount);
    SetPlayerItemAmount(playerid, secondRequiredIngr, GetPlayerItemAmount(playerid, secondRequiredIngr) - reductIngrAmount);

    // Set newly crafted item as last item used.
    if(IsItemType(itemId, ITEM_TYPE_PROJECTILE))
    {
        ePlayerInfo[playerid][e_iLastEquippedItemId] = itemId;
    }

    Audio_StopEx(playerid);
    Audio_PlayEx(playerid, AUDIO_CRAFT_FINISH);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerHoldingKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnPlayerHoldingKey(playerid, key)
{
    if(key & KEY_FIRE)
    {
        if(IsPlayerCrafting(playerid))
        {
            return 1;
        }

        // Crafting from the backpack menu (crafting category).
        if(IsPlayerUsingBackpack(playerid))
        {
            if(PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING && Iter_Contains(ItemsToCraft<playerid>, Craft_GetSelection(playerid)))
            {
                Bit_Let(ePlayerFlag[e_bIsCrafting], playerid);
                OnPlayerStartedCrafting(playerid, Craft_GetSelection(playerid));
            }
        }

        // Crafting from the weapon menu.
        else if(IsPlayerUsingWeaponMenu(playerid))
        {
            new iter = -1;
            switch(WeapMenu_GetSelectionV(playerid))
            {
                case WEAPON_MENU_V_BOMB:        iter = CRAFT_ITEM_BOMB;
                case WEAPON_MENU_V_MEDKIT:      iter = CRAFT_ITEM_MEDKIT;
                case WEAPON_MENU_V_MOLOTOV:     iter = CRAFT_ITEM_MOLOTOV;
                case WEAPON_MENU_V_SMOKE_BOMB:  iter = CRAFT_ITEM_SMOKE_BOMB;
            }

            if(iter == -1
            || !Iter_Contains(ItemsToCraft<playerid>, iter))
            {
                return 1;
            }

            new 
                oldselection = Craft_GetSelection(playerid),
                newSelection = Craft_GetSelection(playerid) = iter;

            SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_NONE);
            PlayerBackpackCategory(playerid) = BACKPACK_CATEGORY_CRAFTING;
            SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_USING_BACKPACK);
            CraftTd_HighlightSelection(playerid, newSelection, oldselection);
            Backpack_MoveSelectBox(playerid, eBackpack_CraftItemsData[newSelection][e_fSelBoxX], eBackpack_CraftItemsData[newSelection][e_fSelBoxY]);
        }
    }
	return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------- OnPlayerReleaseKey ------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-= 
hook OnPlayerReleaseKey(playerid, key, holding_time)
{
    if(key & KEY_FIRE)
    {
        if(IsPlayerCrafting(playerid))
        {
            Bit_Vet(ePlayerFlag[e_bIsCrafting], playerid);
            OnPlayerCanceledCrafting(playerid);
        }
    }
    return 1;
}

// Crafting : Craft item (recursive).
timer Craft_CraftItem[1000](playerid, craftItemId, craft_time)
{
    if(IsPlayerCrafting(playerid))
    {
        if(craft_time > 0) OnPlayerCraftedItem(playerid, craftItemId);
        else
        {
            new craftLvl = Skill_GetPlayerLevel(playerid, SKILL_ITEM_CRAFT_SPEED);
            switch(craftItemId)
            {
                case 
                    CRAFT_ITEM_MELEE,
                    CRAFT_ITEM_SHIV:
                    {
                        craft_time = 
                          (craftLvl == 0) ? (CRAFT_SPEED_MELEE_SHIV_LVL0) 
                        : (craftLvl == 1) ? (CRAFT_SPEED_MELEE_SHIV_LVL1) 
                        : (CRAFT_SPEED_MELEE_SHIV_LVL2);
                    }
                
                case 
                    CRAFT_ITEM_MEDKIT, 
                    CRAFT_ITEM_MOLOTOV, 
                    CRAFT_ITEM_SMOKE_BOMB:
                    {
                        craft_time = 
                          (craftLvl == 0) ? (CRAFT_SPEED_MED_MOL_SMKBMB_LVL0) 
                        : (craftLvl == 1) ? (CRAFT_SPEED_MED_MOL_SMKBMB_LVL1) 
                        : (CRAFT_SPEED_MED_MOL_SMKBMB_LVL2);
                    }
                     
                case CRAFT_ITEM_BOMB:
                {
                    craft_time = 
                      (craftLvl == 0) ? (CRAFT_SPEED_NAIL_BOMB_LVL0) 
                    : (craftLvl == 1) ? (CRAFT_SPEED_NAIL_BOMB_LVL1) 
                    : (CRAFT_SPEED_NAIL_BOMB_LVL2);
                }
            }

            ePlayerBpInfo[playerid][e_iTimer_craftingOrUpgrading] = defer Craft_CraftItem[craft_time](playerid, craftItemId, craft_time);
        }
    }
}

// Crafting : Show item's amount that player own.
CraftTd_UpdateItemAmount(playerid, itemId = -1)
{
    new String:itemAmountStr = STRING_NULL;
    if(itemId == -1)
    {
        for(new tdIdx = 0; tdIdx <= 4; tdIdx++)
        {
            switch(tdIdx)
            {
                case 0: itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV));
                case 1: itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_MEDKIT));
                case 2: itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_MOLOTOV));
                case 3: itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_NAIL_BOMB));
                case 4: itemAmountStr = str_val(GetPlayerItemAmount(playerid, _:ITEM_IDX_SMOKE_BOMB));
            }
            
            PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx], itemAmountStr);
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx]);
        }
    }
    else
    {
        new tdIdx = GetBackpackItemAmountTdIdx(itemId);
        itemAmountStr = str_val(GetPlayerItemAmount(playerid, itemId));

        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx], itemAmountStr);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx]);
    }
}

// Crafting : change the viewing item (description of it, icon and name) + check if player can craft it. If so, show appropriate message.
CraftTd_UpdateViewingItem(playerid, selection)
{
    if(selection != CRAFT_ITEM_MELEE) PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], eBackpack_CraftItemsData[selection][e_sCurrentItemIcon]);
    else
    {
        new playerMeleeItemId = GetPlayerMeleeItemId(playerid);
        new String:meleeIconStr = str_new_static("TLoU:weap_pipe");
        if(playerMeleeItemId != -1)
        {
            meleeIconStr = str_new_static(eItemsSprites[playerMeleeItemId]);
        }

        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][0], meleeIconStr);
        PlayerTextDrawSetStr_s(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], meleeIconStr);

        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][0]);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0]);
    }

    PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], eBackpack_CraftItemsData[selection][e_sCurrentItemName]);
    PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], eBackpack_CraftItemsData[selection][e_sCurrentItemDescr]);

    if(Iter_Contains(ItemsToCraft<playerid>, selection))
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][0]);
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][1]);
    }
    else
    {
        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][0]);
        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_HowToCraft][1]);
    }
}

// Crafting : Highlight selection of current craft item player has selected + Show if he's got enough ingredients to craft it.
CraftTd_HighlightSelection(playerid, newSelection, oldSelection = -1)
{
    if(oldSelection != -1)
    {
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][oldSelection], COLOR_GRAY);

        switch(oldSelection)
        {
            case CRAFT_ITEM_MELEE,  CRAFT_ITEM_SHIV:
            {
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][0]);
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][1]);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][0], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][1], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][6], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][0], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][3], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][18], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], COLOR_GRAY);                
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][18], COLOR_GRAY);
            }

            case CRAFT_ITEM_MEDKIT, CRAFT_ITEM_MOLOTOV:
            {
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][2]);
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][3]);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][2], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][3], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][6], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][9], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], COLOR_GRAY);
            }

            case CRAFT_ITEM_BOMB:
            {
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][0]);
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][4]);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][0], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][4], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][0], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][12], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], COLOR_GRAY);
            }

            case CRAFT_ITEM_SMOKE_BOMB:
            {
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][4]);
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][5]);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][4], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][5], COLOR_GRAY);

                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][12], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][15], COLOR_GRAY);
            
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], COLOR_GRAY);
                PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], COLOR_GRAY);
            }
        }
    }
    PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][newSelection], COLOR_WHITE);
    
    static requiredMinAmount = 100;

    new 
        firstRequiredIngr = _:eBackpack_CraftItemsData[newSelection][e_iCraftFirstReqIngr],
        secondRequiredIngr = _:eBackpack_CraftItemsData[newSelection][e_iCraftSecondReqIngr];

    CraftTd_HighlightIngr(playerid, firstRequiredIngr, GetPlayerItemAmount(playerid, firstRequiredIngr) >= requiredMinAmount);
    CraftTd_HighlightIngr(playerid, secondRequiredIngr, GetPlayerItemAmount(playerid, secondRequiredIngr) >= requiredMinAmount);

    if(newSelection == CRAFT_ITEM_MELEE)
    {
        CraftTd_HighlightIngr(playerid, INGR_ITEM_MELEE, (GetPlayerMeleeItemId(playerid) != -1 && !IsPlayerMeleeUpgraded(playerid)));
    }
}

// Crafting : Related to CraftTd_HighlightSelection
CraftTd_HighlightIngr(playerid, ingredientId, bool:positive = true)
{
    new textdraw_idx = (ingredientId == INGR_ITEM_BLADE ? ingredientId : ingredientId * 3);

    if(positive)
    {
        if(ingredientId != INGR_ITEM_MELEE) TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][ingredientId]);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][ingredientId], COLOR_WHITE);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][textdraw_idx], COLOR_WHITE);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][textdraw_idx], COLOR_WHITE);
    }
    else
    {
        if(ingredientId != INGR_ITEM_MELEE) TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][ingredientId]);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][ingredientId], COLOR_RED_TRANS);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][textdraw_idx], COLOR_RED_TRANS);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][textdraw_idx], COLOR_GRAY);
    }
}

// Crafting : During backpack opening - check what's there possibly to craft for player.
CraftTd_CheckWhatsToCraft(playerid)
{
    static requiredMinAmount = 100;

    for(new i = 0; i != MAX_CRAFT_ITEMS; i++)
    {
        new itemId = _:eBackpack_CraftItemsData[i][e_iCraftItemId],
            firstRequiredIngr = _:eBackpack_CraftItemsData[i][e_iCraftFirstReqIngr],
            secondRequiredIngr = _:eBackpack_CraftItemsData[i][e_iCraftSecondReqIngr];

        if(i == 0 && IsPlayerMeleeUpgraded(playerid)
        || i == 0 && GetPlayerMeleeItemId(playerid) == -1
        || i > 0 && (GetPlayerItemAmount(playerid, itemId) >= GetPlayerItemMaxAmount(playerid, itemId))
        || (GetPlayerItemAmount(playerid, firstRequiredIngr) < requiredMinAmount)
        || (GetPlayerItemAmount(playerid, secondRequiredIngr) < requiredMinAmount))
        {
            if(Iter_Contains(ItemsToCraft<playerid>, i))
            {
                Iter_Remove(ItemsToCraft<playerid>, i);
                CraftTd_UpdateCraftItemWrench(playerid, i);
            }
            continue;
        }

        if(!Iter_Contains(ItemsToCraft<playerid>, i))
        {
            Iter_Add(ItemsToCraft<playerid>, i);
        }
        CraftTd_UpdateCraftItemWrench(playerid, i);
    }
}

// Crafting : Updates wrenches icons in backpack menu (if necessary) & weapon menu with weapon icons if necessary to show.
CraftTd_UpdateCraftItemWrench(playerid, craftItemId)
{
    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
    {
        if(Iter_Contains(ItemsToCraft<playerid>, craftItemId))
        {
            PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][craftItemId]);
        }
        else
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][craftItemId]);
        }
    }
    else if(IsPlayerUsingWeaponMenu(playerid))
    {
        new 
            wrenchTdIdx = -1,
            weapIconTdIdx = -1;

        switch(craftItemId)
        {
            case CRAFT_ITEM_BOMB:       wrenchTdIdx = 0, weapIconTdIdx = 0;
            case CRAFT_ITEM_MEDKIT:     wrenchTdIdx = 1, weapIconTdIdx = 1;
            case CRAFT_ITEM_MOLOTOV:    wrenchTdIdx = 2, weapIconTdIdx = 3;
            case CRAFT_ITEM_SMOKE_BOMB: wrenchTdIdx = 3, weapIconTdIdx = 4;
        }

        if(wrenchTdIdx == -1 || weapIconTdIdx == -1)
        {
            return;
        }

        if(Iter_Contains(ItemsToCraft<playerid>, craftItemId))
        {
            PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][wrenchTdIdx]);
            PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][weapIconTdIdx]);
        }
        else
        {
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][wrenchTdIdx]);
            PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][weapIconTdIdx]);
        }
    }
}

// Crafting : Find ingredient sprite (texture) for specified amount (used internally in CraftTd_DisplayIngredients())
CraftTd_FindIngrSpriteIndex(ingr, amount)
{
    for(new i = 0; i != sizeof(eIngrSprites); i++)
    {
        if(ingr == _:eIngrSprites[i][e_iIngrId]
        && amount == eIngrSprites[i][e_iIngrValue])
        {
            return i;
        }
    }
    return -1;
}

// Crafting : Checking player's ingriedient(s) and displaying them in the backpack menu.
CraftTd_DisplayIngredients(playerid, ingredientId = -1)
{
    if(ingredientId == -1)
    {
        for(ingredientId = INGR_ITEM_BLADE; ingredientId <= INGR_ITEM_SUGAR; ingredientId++)
        {
            new Float:amount = (GetPlayerItemAmount(playerid, ingredientId) / 100.0);
            new fullAmounts = floatround(amount, floatround_floor);
            new remaining = floatround((amount - fullAmounts) * 100);

            new 
                tdIdxStart = (ingredientId == INGR_ITEM_BLADE ? 0 : ingredientId * 3),
                tdIdxEnd = tdIdxStart + 2,
                ingrSpriteIdx = -1;

            for(new tdIdx = tdIdxStart; tdIdx <= tdIdxEnd; tdIdx++)
            {
                if(fullAmounts > 0)
                {
                    ingrSpriteIdx = CraftTd_FindIngrSpriteIndex(ingredientId, 100);
                    fullAmounts -= 1;
                }
                else
                {
                    ingrSpriteIdx = CraftTd_FindIngrSpriteIndex(ingredientId, remaining);
                    remaining = 0;
                }

                if(isnull(eIngrSprites[ingrSpriteIdx][e_sIngrSprite]))
                {
                    PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx]);
                }
                else
                {
                    PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx], eIngrSprites[ingrSpriteIdx][e_sIngrSprite]);
                    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx]);
                }
            }
        }

        if(GetPlayerMeleeItemId(playerid) != -1 && !IsPlayerMeleeUpgraded(playerid))
        {
            PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][18]);
        }
    }
    else
    {
        new Float:amount = (GetPlayerItemAmount(playerid, ingredientId) / 100.0);
        new fullAmounts = floatround(amount, floatround_floor);
        new remaining = floatround((amount - fullAmounts) * 100);

        new 
            tdIdxStart = (ingredientId == INGR_ITEM_BLADE ? 0 : ingredientId * 3),
            tdIdxEnd = tdIdxStart + 2,
            ingrSpriteIdx = -1;

        for(new tdIdx = tdIdxStart; tdIdx <= tdIdxEnd; tdIdx++)
        {
            if(fullAmounts > 0)
            {
                ingrSpriteIdx = CraftTd_FindIngrSpriteIndex(ingredientId, 100);
                fullAmounts -= 1;
            }
            else
            {
                ingrSpriteIdx = CraftTd_FindIngrSpriteIndex(ingredientId, remaining);
                remaining = 0;
            }

            if(isnull(eIngrSprites[ingrSpriteIdx][e_sIngrSprite]))
            {
                PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx]);
            }
            else
            {
                PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx], eIngrSprites[ingrSpriteIdx][e_sIngrSprite]);
                PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIdx]);
            }
        }
    }
}