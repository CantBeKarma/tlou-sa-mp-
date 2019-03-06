//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//----------------- Audio_OnClientConnect -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public Audio_OnClientConnect(playerid)
{
    if(IsPalyerLogged(playerid))
    {
        return 1;
    }

    Audio_PlayStreamed(playerid, "http://localhost/TLoU/ambient_noise/TLoU_MainMenu.mp3");
    ConnectionIntroProcedure(playerid);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//------------------ Audio_OnTransferFile -----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public Audio_OnTransferFile(playerid, file[], current, total, result)
{
    new strProgress[16];
    format(strProgress, sizeof(strProgress), "Loading: %0.1f%", (float(current) / float(total)) * 100);
    PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], strProgress);

    if(current == total)
    {
        PlayerTdSetString(playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], "Done!");
        Delay_PlayerTdHide(playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 3);
    }
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- Audio_OnStop ----------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public Audio_OnStop(playerid, handleid)
{
    if(Iter_Contains(iAudioHandle3D<playerid>, handleid))
    {
        Iter_Remove(iAudioHandle3D<playerid>, handleid);
    }
    return 1;
}