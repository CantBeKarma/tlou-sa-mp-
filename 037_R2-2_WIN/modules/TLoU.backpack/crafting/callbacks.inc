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

        if(craftItemId == CRAFT_ITEM_SHIV)
        {
            SetPlayerShivCND(playerid, GetPlayerMaxShivCND(playerid));
        }
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