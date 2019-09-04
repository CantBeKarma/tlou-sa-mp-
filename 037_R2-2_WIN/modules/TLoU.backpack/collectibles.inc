//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------- OnPlayerCheckCollectibles ---------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerCheckCollectibles(playerid, bool:readCollectible)
{
    // If not reading collectible, show select box on the right side
    // of the selection. TextDraws are filled inside DB_LoadPlayerCollectibles().
    if(!readCollectible)
    {
        PlayerTextDrawShow(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox]);
        Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);
        return 0;
    }

    // If reading specific collectible, read its data from DB and callback OnPlayerReadCollectble().
    new collectibleUid = Iter_Index(CollectiblesOwned<playerid>, Coll_ViewingGetSelection(playerid));
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
    
    new oldSelection = Coll_ViewingGetSelection(playerid);
    if(updown == KEY_UP)
    {
        if(Coll_ViewingGetSelection(playerid) == 0)
        {
            return 1;
        }

        Coll_ViewingGetSelection(playerid) -= 1;
        playerCollSelectBoxY -= 24.0;
    }
    else if(updown == KEY_DOWN)
    {
        if(Coll_ViewingGetSelection(playerid) == 8)
        {
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

    Audio_PlayEx(playerid, AUDIO_COLL_UPDWN);

    Backpack_MoveCollSelectBox(playerid, playerCollSelectBoxX, playerCollSelectBoxY);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerReadCollectible ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerReadCollectible(playerid, row_title[], row_message[])
{
    SCMF(playerid, -1, "CollectibleData: title: %s | message: %s", row_title, row_message);
    return 0;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------ OnPlayerStopReadingCollectible -------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnPlayerStopReadingCollectible(playerid)
{
    return 0;
}

// Check if player has any collectible at all on the list.
HasPlayerGotAnyCollectible(playerid)
{
    return IsPlayerTextDrawVisible(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][0]);
}

// Display collectibles titles (artifacts & firefly pendants).
CollTd_ShowTitles(playerid)
{
    if(!IsPlayerUsingBackpack(playerid) && PlayerBackpackCategory(playerid) != BACKPACK_CATEGORY_COLLECTIBLES)
    {
        return false;
    }

    new selectedCategory = Coll_GetSelection(playerid);
    DB_LoadPlayerCollectibles(playerid, selectedCategory);

    return true;
}