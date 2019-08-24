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
    // Find item index of the craft id.
    new itemId = -1;
    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        if(craftItemId == eItemsData[i][e_iItem_CraftItemId])
        {
            itemId = i;
            break;
        }
    }

    if(itemId == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], ERROR, "%s (%d) tried to craft (%d) an invalid item. (Couldn't find its index in the table!)", PlayerName(playerid), playerid, craftItemId);
        return 1;
    }

    if(craftItemId != CRAFT_ITEM_MELEE) SetPlayerItemAmount(playerid, itemId, GetPlayerItemAmount(playerid, itemId) + 1);
    else
    {
        if(GetPlayerMelee[playerid] == WEAPON_PIPE)
        {
            GetPlayerMelee[playerid] = WEAPON_SCISSOR_PIPE;
        }
        
        new 
            meleeCnd = GetPlayerItemAmount(playerid, WEAPON_SCISSOR_PIPE),
            meleeExtCnd = GetMeleeWeapon_Durability(GetPlayerMelee[playerid], true);

        SetPlayerItemAmount(playerid, itemId, meleeCnd, .meleeExtraCND = meleeExtCnd);
    }
    
    // Reduct ingredients.
    SetPlayerItemAmount(playerid, eItemsData[itemId][e_iItem_CraftIngrRequired1], Craft_IngrAmount(playerid, eItemsData[itemId][e_iItem_CraftIngrRequired1]) - 100);
    SetPlayerItemAmount(playerid, eItemsData[itemId][e_iItem_CraftIngrRequired2], Craft_IngrAmount(playerid, eItemsData[itemId][e_iItem_CraftIngrRequired2]) - 100);

    // Set newly crafted weapon as a last weapon used.
    if(craftItemId != CRAFT_ITEM_SHIV)
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
                case WEAPON_MENU_V_BOMB:        iter = WEAPON_SLOT_BOMB;
                case WEAPON_MENU_V_MEDKIT:      iter = WEAPON_SLOT_MEDKIT;
                case WEAPON_MENU_V_MOLOTOV:     iter = WEAPON_SLOT_MOLOTOV;
                case WEAPON_MENU_V_SMOKE_BOMB:  iter = WEAPON_SLOT_SMOKE_BOMB;
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
            new craftLvl = Skill_GetLevel(playerid, SKILL_ITEM_CRAFT_SPEED);
            switch(craftItemId)
            {
                case CRAFT_ITEM_MELEE,
                     CRAFT_ITEM_SHIV:       craft_time = (craftLvl == 0) ? (1650) : (craftLvl == 1) ? (1250) : (850);
                
                case CRAFT_ITEM_MEDKIT, 
                     CRAFT_ITEM_MOLOTOV, 
                     CRAFT_ITEM_SMOKE_BOMB: craft_time = (craftLvl == 0) ? (1400) : (craftLvl == 1) ? (1000) : (600);
                     
                case CRAFT_ITEM_BOMB:       craft_time = (craftLvl == 0) ? (1500) : (craftLvl == 1) ? (1100) : (700);                     
            }

            ePlayerBpInfo[playerid][e_iTimer_craftingOrUpgrading] = defer Craft_CraftItem[craft_time](playerid, craftItemId, craft_time);
        }
    }
}

// Crafting : Show item's amount that player own.
CraftTd_UpdateItemAmount(playerid, itemId = -1, calcAmount = -1)
{
    new strAmount[3];
    if(itemId == -1 && calcAmount == -1)
    {
        for(new tdIdx = 0, slotIdx = 1; tdIdx != 5; tdIdx++, slotIdx++)
        {
            valstr(strAmount, PlayerWeaponSlotAmmo[playerid, slotIdx]);
            PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx], strAmount);
        }
    }
    else
    {
        new tdIdx = itemId - 1; // tdIdx start from 0. Why this? because: (shiv)ItemId: 1 - 1 = 0, (medkit)ItemId: 2 - 1 = 1 (result is textdrawIndex).
        valstr(strAmount, calcAmount);
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][tdIdx], strAmount);
    }
}

// Crafting : change the viewing item (description of it, icon and name) + check if player can craft it. If so, show appropriate message.
CraftTd_UpdateViewingItem(playerid, selection)
{
    if(selection != CRAFT_ITEM_MELEE) PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], eBackpack_CraftItemsData[selection][e_sCurrentItemIcon]);
    else
    {
        new meleeIconStr[20];
        switch(GetPlayerMelee[playerid])
        {
            case WEAPON_PIPE:         strcpy(meleeIconStr, "TLoU:weap_pipe");
            case WEAPON_SCISSOR_PIPE: strcpy(meleeIconStr, "TLoU:weap_scsr_pipe");
            case WEAPON_HATCHET:      strcpy(meleeIconStr, "TLoU:weap_hatchet");
            case WEAPON_MACHETE:      strcpy(meleeIconStr, "TLoU:weap_machete");
            default:                  strcpy(meleeIconStr, "TLoU:weap_pipe");
        }
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][0], meleeIconStr);
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], meleeIconStr);
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
    
    switch(newSelection)
    {
        case CRAFT_ITEM_MELEE, CRAFT_ITEM_SHIV:
        {
            CraftTd_HighlightIngr(playerid, INGR_ITEM_BLADE, (Craft_IngrAmount(playerid, INGR_ITEM_BLADE) < 100 ? false : true));
            CraftTd_HighlightIngr(playerid, INGR_ITEM_BINDING, (Craft_IngrAmount(playerid, INGR_ITEM_BINDING) < 100 ? false : true));
            
            if(newSelection == CRAFT_ITEM_MELEE)
                CraftTd_HighlightIngr(playerid, INGR_ITEM_MELEE, (GetPlayerMelee[playerid] != NO_MELEE && !IsPlayerMeleeUpgraded[playerid] ? true : false));
        }

        case CRAFT_ITEM_MEDKIT, CRAFT_ITEM_MOLOTOV:
        {
            CraftTd_HighlightIngr(playerid, INGR_ITEM_RAG, (Craft_IngrAmount(playerid, INGR_ITEM_RAG) < 100 ? false : true));
            CraftTd_HighlightIngr(playerid, INGR_ITEM_ALCOHOL, (Craft_IngrAmount(playerid, INGR_ITEM_ALCOHOL) < 100 ? false : true));
        }

        case CRAFT_ITEM_BOMB:
        {
            CraftTd_HighlightIngr(playerid, INGR_ITEM_BLADE, (Craft_IngrAmount(playerid, INGR_ITEM_BLADE) < 100 ? false : true));
            CraftTd_HighlightIngr(playerid, INGR_ITEM_EXPLOSIVE, (Craft_IngrAmount(playerid, INGR_ITEM_EXPLOSIVE) < 100 ? false : true));
        }

        case CRAFT_ITEM_SMOKE_BOMB:
        {
            CraftTd_HighlightIngr(playerid, INGR_ITEM_EXPLOSIVE, (Craft_IngrAmount(playerid, INGR_ITEM_EXPLOSIVE) < 100 ? false : true));
            CraftTd_HighlightIngr(playerid, INGR_ITEM_SUGAR, (Craft_IngrAmount(playerid, INGR_ITEM_SUGAR) < 100 ? false : true));
        }
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

    //#region BLADE + BINDING (Melee upgrade & Shiv)
        if(Craft_IngrAmount(playerid, INGR_ITEM_BLADE) >= requiredMinAmount && Craft_IngrAmount(playerid, INGR_ITEM_BINDING) >= requiredMinAmount)
        {
            if(GetPlayerMelee[playerid] != NO_MELEE && IsPlayerMeleeUpgraded[playerid]) 
                Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_MELEE);
            else 
                Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MELEE);

            if(GetPlayerItemAmount(playerid, _:ITEM_IDX_SHIV) < MAX_AMOUNT_SHIV) Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_SHIV);
            else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_SHIV);
        }
        else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MELEE), Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_SHIV);
    //#endregion

    //#region RAG + ALCOHOL (Medkit & Molotov cocktail)
        if(Craft_IngrAmount(playerid, INGR_ITEM_RAG) >= requiredMinAmount && Craft_IngrAmount(playerid, INGR_ITEM_ALCOHOL) >= requiredMinAmount)
        {
            if(GetPlayerItemAmount(playerid, _:ITEM_IDX_MEDKIT) < MAX_AMOUNT_MEDKIT) Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_MEDKIT);
            else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MEDKIT);

            if(GetPlayerItemAmount(playerid, _:ITEM_IDX_MOLOTOV) < MAX_AMOUNT_MOLOTOV) Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_MOLOTOV);
            else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MOLOTOV);
        }
        else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MEDKIT), Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_MOLOTOV);
    //#endregion

    //#region BLADE + EXPLOSIVE (Nailbomb)
        if(Craft_IngrAmount(playerid, INGR_ITEM_BLADE) >= requiredMinAmount && Craft_IngrAmount(playerid, INGR_ITEM_EXPLOSIVE) >= requiredMinAmount)
        {
            if(GetPlayerItemAmount(playerid, _:ITEM_IDX_NAIL_BOMB) < MAX_AMOUNT_BOMB) Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_BOMB);
            else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_BOMB);
        }
        else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_BOMB);
    //#endregion

    //#region EXPLOSIVE + SUGAR (Smoke bomb)
        if(Craft_IngrAmount(playerid, INGR_ITEM_EXPLOSIVE) >= requiredMinAmount && Craft_IngrAmount(playerid, INGR_ITEM_SUGAR) >= requiredMinAmount)
        {
            if(GetPlayerItemAmount(playerid, _:ITEM_IDX_SMOKE_BOMB) < MAX_AMOUNT_SMOKE_BOMB) Iter_Add(ItemsToCraft<playerid>, WEAPON_SLOT_SMOKE_BOMB);
            else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_SMOKE_BOMB);
        }
        else Iter_Remove(ItemsToCraft<playerid>, WEAPON_SLOT_SMOKE_BOMB);
    //#endregion

    //#region Displaying items possible to craft (backpack / weapon menu).
        // Show wrench icons next to the craft items (backpack).
        if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_CRAFTING)
        {
            // Check which iter is not in the list and hide related textdraw to it if it's visible.
            for(new i = 0; i != MAX_CRAFT_ITEMS; i++)
            {
                if(!Iter_Contains(ItemsToCraft<playerid>, i))
                    PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i]);
                else
                    PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i]);
            }
        }
        // Show wrench icons next to the craft items (weapon menu).
        else if(IsPlayerUsingWeaponMenu(playerid))
        {
            foreach(new i : ItemsToCraft<playerid>)
            {
                switch(i)
                {
                    // PlayerTdShow does "show icon if it's not visible and item is possible to craft".
                    case WEAPON_SLOT_BOMB:       PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][0]), PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][0]);
                    case WEAPON_SLOT_MEDKIT:     PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][1]), PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][1]);
                    case WEAPON_SLOT_MOLOTOV:    PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][2]), PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][3]);
                    case WEAPON_SLOT_SMOKE_BOMB: PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][3]), PlayerTdShow(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][4]);
                }
            }
        }
    //#endregion
}

// Crafting : Ugly way of checking what ingredients player has and showing its icon on the list (but it works).
CraftTd_UpdateIngr(playerid, ingredientId = -1)
{
    if(ingredientId == -1 || ingredientId == INGR_ITEM_BLADE)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_BLADE))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2], "TLoU:ingr_blade_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2], "TLoU:ingr_blade_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2], "TLoU:ingr_blade_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][0], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][1], "TLoU:ingr_blade_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][2], "TLoU:ingr_blade_full");
        }
    }

    if(ingredientId == -1 || ingredientId == INGR_ITEM_BINDING)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_BINDING))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5], "TLoU:ingr_binding_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5], "TLoU:ingr_binding_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5], "TLoU:ingr_binding_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][3], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][4], "TLoU:ingr_binding_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][5], "TLoU:ingr_binding_full");
        }
    }

    if(ingredientId == -1 || ingredientId == INGR_ITEM_RAG)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_RAG))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8], "TLoU:ingr_rag_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8], "TLoU:ingr_rag_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8], "TLoU:ingr_rag_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][6], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][7], "TLoU:ingr_rag_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][8], "TLoU:ingr_rag_full");
        }
    }

    if(ingredientId == -1 || ingredientId == INGR_ITEM_ALCOHOL)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_ALCOHOL))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11], "TLoU:ingr_alcohol_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11], "TLoU:ingr_alcohol_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11], "TLoU:ingr_alcohol_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][9], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][10], "TLoU:ingr_alcohol_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][11], "TLoU:ingr_alcohol_full");
        }
    }

    if(ingredientId == -1 || ingredientId == INGR_ITEM_EXPLOSIVE)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_EXPLOSIVE))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14], "TLoU:ingr_explosive_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14], "TLoU:ingr_explosive_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14], "TLoU:ingr_explosive_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][12], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][13], "TLoU:ingr_explosive_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][14], "TLoU:ingr_explosive_full");
        }
    }

    if(ingredientId == -1 || ingredientId == INGR_ITEM_SUGAR)
    {
        switch(Craft_IngrAmount(playerid, INGR_ITEM_SUGAR))
        {
            case 0:  PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 25: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 50: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 75: PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 100:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16]), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);

            case 125:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_quater"),   PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 150:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_half"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 175:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_3quaters"), PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);
            case 200:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_full"),     PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17]);

            case 225:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17], "TLoU:ingr_sugar_quater");
            case 250:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17], "TLoU:ingr_sugar_half");
            case 275:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17], "TLoU:ingr_sugar_3quaters");
            case 300:PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][15], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][16], "TLoU:ingr_sugar_full"), PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][17], "TLoU:ingr_sugar_full");
        }
    }
}