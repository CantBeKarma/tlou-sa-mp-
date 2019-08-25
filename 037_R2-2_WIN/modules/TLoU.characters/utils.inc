// Reset all characters data in enumerator (called in OnPlayerDisconnect)
ResetEnum_eCharsInfo(playerid)
{
    static sBlank[e_CHARS_DATA];
    eCharsInfo[playerid] = sBlank;

    print("ResetEnum_eCharsInfo");
}

// Returns character Uid that player is playing as.
GetPlayerCharUid(playerid)
{
    return eCharsInfo[playerid][e_iUid];
}

// Set player's supplements amount.
SetPlayerSupplements(playerid, amount)
{
    eCharsInfo[playerid][e_iSkills_SuppsAmount] = ((amount < 0) ? (0) : (amount > MAX_PLAYER_SUPPS_AMOUNT) ? (MAX_PLAYER_SUPPS_AMOUNT) : (amount));

    if(IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) == BACKPACK_CATEGORY_SKILLS)
    {
        SkillsTd_CheckSuppsAmount(playerid);
        SkillsTd_CheckWhatsToUpgrade(playerid);
        SkillTd_UpdateViewingItem(playerid, Skill_GetSelection(playerid));
    }
    UpdateNavItemsColors(playerid);
}