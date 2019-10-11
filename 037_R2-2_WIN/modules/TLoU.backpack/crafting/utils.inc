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

        new 
            tdIngrStripeIdx1 = -1,
            tdIngrStripeIdx2 = -1;

        new
            tdIngrNameIdx1 = -1,
            tdIngrNameIdx2 = -1;

        new
            tdIngrBoxIdx1 = -1,
            tdIngrBoxIdx2 = -1;

        new 
            tdIngrIconIdx1 = -1,
            tdIngrIconIdx2 = -1;

        switch(oldSelection)
        {
            case CRAFT_ITEM_MELEE,  CRAFT_ITEM_SHIV:
            {
                tdIngrStripeIdx1 = tdIngrNameIdx1 = 0;
                tdIngrStripeIdx2 = tdIngrNameIdx2 = 1;

                tdIngrBoxIdx1 = tdIngrIconIdx1 = 0;
                tdIngrBoxIdx2 = tdIngrIconIdx2 = 3;
            }

            case CRAFT_ITEM_MEDKIT, CRAFT_ITEM_MOLOTOV:
            {
                tdIngrStripeIdx1 = tdIngrNameIdx1 = 2;
                tdIngrStripeIdx2 = tdIngrNameIdx2 = 3;

                tdIngrBoxIdx1 = tdIngrIconIdx1 = 6;
                tdIngrBoxIdx2 = tdIngrIconIdx2 = 9;
            }

            case CRAFT_ITEM_BOMB:
            {
                tdIngrStripeIdx1 = tdIngrNameIdx1 = 0;
                tdIngrStripeIdx2 = tdIngrNameIdx2 = 4;

                tdIngrBoxIdx1 = tdIngrIconIdx1 = 0;
                tdIngrBoxIdx2 = tdIngrIconIdx2 = 12;
            }

            case CRAFT_ITEM_SMOKE_BOMB:
            {
                tdIngrStripeIdx1 = tdIngrNameIdx1 = 4;
                tdIngrStripeIdx2 = tdIngrNameIdx2 = 5;

                tdIngrBoxIdx1 = tdIngrIconIdx1 = 12;
                tdIngrBoxIdx2 = tdIngrIconIdx2 = 15;
            }
        }

        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][tdIngrStripeIdx1]);
        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_IngrStripe][tdIngrStripeIdx2]);

        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][tdIngrNameIdx1], COLOR_GRAY);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][tdIngrNameIdx2], COLOR_GRAY);

        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][tdIngrBoxIdx1], COLOR_GRAY);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][tdIngrBoxIdx2], COLOR_GRAY);

        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIngrIconIdx1], COLOR_GRAY);
        PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][tdIngrIconIdx2], COLOR_GRAY);

        if(oldSelection == CRAFT_ITEM_MELEE)
        {
            PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][6], COLOR_GRAY);
            PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][18], COLOR_GRAY);
            PlayerTdSetColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][18], COLOR_GRAY);
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