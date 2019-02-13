/* 
        Error & Return type 

    COMMAND_ZERO_RET      = 0 , // The command returned 0. 
    COMMAND_OK            = 1 , // Called corectly. 
    COMMAND_UNDEFINED     = 2 , // Command doesn't exist. 
    COMMAND_DENIED        = 3 , // Can't use the command. 
    COMMAND_HIDDEN        = 4 , // Can't use the command don't let them know it exists. 
    COMMAND_NO_PLAYER     = 6 , // Used by a player who shouldn't exist. 
    COMMAND_DISABLED      = 7 , // All commands are disabled for this player. 
    COMMAND_BAD_PREFIX    = 8 , // Used "/" instead of "#", or something similar. 
    COMMAND_INVALID_INPUT = 10, // Didn't type "/something". 
*/ 

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------- OnPlayerCommandReceived ----------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public e_COMMAND_ERRORS:OnPlayerCommandReceived(playerid, cmdtext[], e_COMMAND_ERRORS:success) 
{ 
    switch (success) 
    { 
        case COMMAND_UNDEFINED:
            Log(eLogs[e_Log_Index][LOG_INDEX_CMD], INFO, "%s [%d] tried to call unknown command: \"%s\"", PlayerName(playerid), playerid, cmdtext);
        
        case COMMAND_OK:
            Log(eLogs[e_Log_Index][LOG_INDEX_CMD], INFO, "%s [%d] successfully called command: \"%s\"", PlayerName(playerid), playerid, cmdtext);
    }
    return COMMAND_OK; 
}  