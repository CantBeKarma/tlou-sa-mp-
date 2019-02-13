// Reset all characters data in enumerator (called in OnPlayerDisconnect)
ResetEnum_eCharsInfo(playerid)
{
    static sBlank[e_CHARS_DATA];
    eCharsInfo[playerid] = sBlank;

    print("ResetEnum_eCharsInfo");
}