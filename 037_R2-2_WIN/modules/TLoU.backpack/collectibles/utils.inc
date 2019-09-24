// Change position/alignment and other properties of message text to display
// over collectible item (artifact/firefly pendant).
Coll_ManageMessageTextPosition(playerid)
{
    new selectedCategory = Coll_GetSelection(playerid);

    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        TextDrawSetPos(eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid], 402.666778, 152.237045);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        TextDrawSetPos(eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid], 454.333587, 228.562805);
    }
}

// Return free textdraws slot index (0-9) based on if textdraw is visible.
// Returns -1 if no free slot found.
GetFreeTitleTextDrawSlot(playerid)
{
    for(new i = 0; i < MAX_DISPLAY_COLLECTIBLES; i++)
    {
        if(!IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i]))
        {
            return i;
        }
    }
    return -1;
}

// Check if player has any collectible at all on the list.
// Checks if player has first textdraw title shown.
HasPlayerGotAnyCollectible(playerid)
{
    return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][0]);
}

// Display collectibles titles (artifacts & firefly pendants).
// Happens once when selecting category/opening backpack.
CollTd_ShowTitles(playerid)
{
    if(!IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_COLLECTIBLES)
    {
        return false;
    }

    Coll_ClearTitlesTextDraws(playerid);

    new 
        count = 0,
        selectedCategory = Coll_GetSelection(playerid);

    ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] = 0;
    Coll_ViewingGetSelection(playerid) = 0;
    PlayerTextDrawSetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 483.333068, 124.348159);
    
    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        foreach(new artifact : ArtifactsOwned<playerid>)
        {
            if(count == MAX_DISPLAY_COLLECTIBLES)
            {
                break;
            }
            
            DB_LoadCollectibleData(playerid, selectedCategory, artifact);
            count += 1;
        }
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        foreach(new pendant : PendantsOwned<playerid>)
        {
            if(count == MAX_DISPLAY_COLLECTIBLES)
            {
                break;
            }

            DB_LoadCollectibleData(playerid, selectedCategory, pendant);
            count += 1;
        }
    }

    // If no collectibles were loaded, hide textdraw
    // "HOW TO VIEW" if visible (for example changed sections).
    if(count == 0)
    {
        TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0]);
        TdHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1]);
    }
    return true;
}

// Clears titles textdraws in collectibles.
// Used internally in CollTd_ShowTiltes() - DO NOT USE MANUALLY !
Coll_ClearTitlesTextDraws(playerid)
{
    for(new i = 0; i < MAX_DISPLAY_COLLECTIBLES; i++)
    {
        if(!IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i]))
        {
            break;
        }

        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], "");
        PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i]);
    }
}

// Moves each collectible title down by one (user's scrolling UPWARDS).
MoveEachCollectibleDownByOne(playerid)
{
    new selectedCategory = Coll_GetSelection(playerid);
    new uid = -1;

    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        uid = Iter_Index(ArtifactsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        uid = Iter_Index(PendantsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }

    if(uid == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "MoveEachCollectibleDownByOne: %s (guid: %d) attempt to read/write at index: -1 ", PlayerName(playerid), GetPlayerCharUid(playerid));
        return false;
    }

    MoveEachTitleTextDrawDownByOne(playerid);
    DB_LoadCollectibleData(playerid, selectedCategory, uid);
    return true;
}

// Moves each collectible title textdraw down by one slot.
// Used internally in MoveEachCollectibleDownByOne() - DO NOT USE MANUALLY !
MoveEachTitleTextDrawDownByOne(playerid)
{
    new title[MAX_COLL_TIT_LEN];
    for(new i = (MAX_DISPLAY_COLLECTIBLES-1); i > 0; i--)
    {
        new lineAboveIdx = (i - 1);
        PlayerTextDrawGetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][lineAboveIdx], title, sizeof(title));

        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], title);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i]);
    }
}

// Moves each collectible title down by one (user's scrolling DOWNARDS).
MoveEachCollectibleUpByOne(playerid)
{
    new selectedCategory = Coll_GetSelection(playerid);
    new uid = -1;

    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        uid = Iter_Index(ArtifactsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        uid = Iter_Index(PendantsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }

    if(uid == -1)
    {
        Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], WARNING, "MoveEachCollectibleUpByOne: %s (guid: %d) attempt to read/write at index: -1 ", PlayerName(playerid), GetPlayerCharUid(playerid));
        return false;
    }

    MoveEachTitleTextDrawUpByOne(playerid);
    DB_LoadCollectibleData(playerid, selectedCategory, uid);
    return true;
}

// Moves each collectible title textdraw up by one slot.
// Used internally in MoveEachCollectibleUpByOne() - DO NOT USE MANUALLY !
MoveEachTitleTextDrawUpByOne(playerid)
{
    new title[MAX_COLL_TIT_LEN];
    for(new i = 0; i < MAX_DISPLAY_COLLECTIBLES; i++)
    {
        new lineBelowIdx = (i + 1);
        if(lineBelowIdx >= MAX_DISPLAY_COLLECTIBLES
        || !IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][lineBelowIdx]))
        {
            break;
        }

        PlayerTextDrawGetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][lineBelowIdx], title, sizeof(title));

        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], title);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i]);
    }
}

// Displays collectible textdraw (based on its type: artifact/pendant)
// And does a new line every 35 characters (usually applies to artifacts).
DisplayCollectibleContents(playerid, collectibleType, title[], message[])
{
    #pragma unused title
    if(collectibleType == COLL_ITEM_ARTIFACTS)
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_PaperNote]);
    }
    else if(collectibleType == COLL_ITEM_FIREFLY_PEND)
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_FireflyPend]);
    }

    new formattedMessage[512];
    strcat(formattedMessage, message, sizeof(formattedMessage));

    new count = 0;
    for(new i = 0; i < strlen(message); i++)
    {
        count += 1;
        if(count == 35)
        {
            strins(formattedMessage, "-~n~", i, MAX_COLL_MSG_LEN);
            count = 0;
        }
    }
    
    TextDrawSetString(eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid], formattedMessage);
    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid]);
}