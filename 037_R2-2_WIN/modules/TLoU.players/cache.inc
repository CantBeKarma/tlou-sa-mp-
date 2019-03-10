Task:LoadCache(playerid)
{
    if(IsPlayerCacheLoaded(playerid))
    {
        return;
    }

    ShowPlayerBlackScreen(playerid);
    LoadPlayerTextDraws(playerid);
    PreloadAnimations(playerid);

    // Doing backpack categories showing to load backpack textdraws
    // so next time player opens backpack is gonna be way faster.
    //ShowBackpack(playerid, BACKPACK_CATEGORY_CRAFTING); 
    //wait_ms(100);

    // HideBackpack(playerid);
    // ShowBackpack(playerid, BACKPACK_CATEGORY_SKILLS);
    // wait_ms(300);

    // HideBackpack(playerid);
    // ShowBackpack(playerid, BACKPACK_CATEGORY_COLLECTIBLES);
    // wait_ms(300);

    // HideBackpack(playerid);
    // ShowBackpack(playerid, BACKPACK_CATEGORY_SKILLS);
    // wait_ms(300);

    // HideBackpack(playerid);
    // ShowBackpack(playerid, BACKPACK_CATEGORY_CRAFTING); 
    // wait_ms(300);

    // HideBackpack(playerid);
    // wait_ms(100);

    Bit_Let(ePlayerFlag[e_bCacheLoaded], playerid);
}