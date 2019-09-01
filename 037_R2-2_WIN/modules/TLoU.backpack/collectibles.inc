// Display collectibles titles (artifacts & firefly pendants).
CollTd_ShowTitles(playerid)
{
    if(!IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_COLLECTIBLES)
    {
        SCMF(playerid, -1, "Not using BP or in wrong category");
        return false;
    }

    new selectedCategory = Coll_GetSelection(playerid);
    DB_LoadPlayerCollectibles(playerid, selectedCategory);

    return true;
}