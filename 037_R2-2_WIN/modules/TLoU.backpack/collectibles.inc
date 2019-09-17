//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerCheckCollectibles ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCheckCollectibles(playerid, bool:readCollectible)
{
    // If not reading collectible, show select box on the right side of the collectibles list
    // so player can pick a certain collectible item.
    if(!readCollectible)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox]);
        Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);
        return 0;
    }

    // If reading specific collectible, read its data from DB and callback OnPlayerReadCollectble().
    new
        collectibleUid = -1,
        selectedCategory = Coll_GetSelection(playerid);

    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        collectibleUid = Iter_Index(ArtifactsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        collectibleUid = Iter_Index(PendantsOwned<playerid>, ePlayerBpInfo[playerid][e_iBackpack_CollTrackList]);
    }

    if(collectibleUid == -1)
    {
        return 1;
    }
    
    DB_ReadCollectibleData(playerid, collectibleUid);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------- OnPlayerStopCheckingCollectible -----------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopCheckingCollectible(playerid)
{
    PlayerTextDrawHide(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox]);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------- OnPlayerViewingCollectibles --------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerViewingCollectibles(playerid, updown)
{
    new Float:playerCollSelectBoxX, Float:playerCollSelectBoxY;
    PlayerTextDrawGetPos(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], playerCollSelectBoxX, playerCollSelectBoxY);
    
    new selectedCategory = Coll_GetSelection(playerid);
    new oldSelection = Coll_ViewingGetSelection(playerid);
    if(updown == KEY_UP)
    {
        if(ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] == 0)
        {
            return 1;
        }

        ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] -= 1;
        Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);
        if(Coll_ViewingGetSelection(playerid) == 0)
        {
            MoveEachCollectibleDownByOne(playerid);
            return 1;
        }

        Coll_ViewingGetSelection(playerid) -= 1;
        playerCollSelectBoxY -= 24.0;
    }
    else if(updown == KEY_DOWN)
    {
        if(selectedCategory == COLL_ITEM_ARTIFACTS && ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] == (Iter_Count(ArtifactsOwned<playerid>)-1)
        || selectedCategory == COLL_ITEM_FIREFLY_PEND && ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] == (Iter_Count(PendantsOwned<playerid>)-1))
        {
            return 1;
        }

        ePlayerBpInfo[playerid][e_iBackpack_CollTrackList] += 1;
        Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);
        if(Coll_ViewingGetSelection(playerid) == (MAX_DISPLAY_COLLECTIBLES-1))
        {
            MoveEachCollectibleUpByOne(playerid);
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

    Backpack_MoveCollSelectBox(playerid, playerCollSelectBoxX, playerCollSelectBoxY);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerReadCollectible ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerReadCollectible(playerid, row_title[], row_message[])
{
    HideBackpackCategory(playerid, BACKPACK_CATEGORY_COLLECTIBLES);
    
    new selectedCategory = Coll_GetSelection(playerid);
    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_PaperNote]);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_FireflyPend]);
    }

    new count = 0;
    for(new i = 0; i < strlen(row_message); i++)
    {
        count += 1;
        if(count == 35)
        {
            strins(row_message, "-~n~", i, MAX_COLL_MSG_LEN);
            count = 0;
        }
    }
    TextDrawSetString(eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid], row_message);

    TextDrawShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid]);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------ OnPlayerStopReadingCollectible -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopReadingCollectible(playerid)
{
    new selectedCategory = Coll_GetSelection(playerid);
    if(selectedCategory == COLL_ITEM_ARTIFACTS)
    {
        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_PaperNote]);
    }
    else if(selectedCategory == COLL_ITEM_FIREFLY_PEND)
    {
        TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_FireflyPend]);
    }

    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0]);
    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1]);

    TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid]);
    ShowBackpackCategory(playerid, BACKPACK_CATEGORY_COLLECTIBLES, false);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerCollectibleLoad -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCollectibleLoad(playerid, title[])
{
    new titleTdIdx = GetFreeTitleTextDrawSlot(playerid);

    if(titleTdIdx == -1)
    {
        // Checks which point of the list player is on (TOP or BOTTOM)
        // and puts correct new title on textdraw slot.
        titleTdIdx = (Coll_ViewingGetSelection(playerid) == 0) ? (0) : (MAX_DISPLAY_COLLECTIBLES-1);
        PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][titleTdIdx], title);
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][titleTdIdx]);
        return 1;
    }

    PlayerTextDrawSetString(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][titleTdIdx], title);
    PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][titleTdIdx]);

    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0]);
    TdShowForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1]);
    return 1;
}

//#region Utils

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

//#endregion