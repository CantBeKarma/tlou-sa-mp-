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
    Audio_PlayEx(playerid, AUDIO_COLL_BACK);
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
    DisplayCollectibleContents(playerid, Coll_GetSelection(playerid), row_title, row_message);

    Audio_StopEx(playerid);
    Audio_PlayEx(playerid, AUDIO_COLL_VIEW);
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
    Audio_PlayEx(playerid, AUDIO_COLL_BACK);
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