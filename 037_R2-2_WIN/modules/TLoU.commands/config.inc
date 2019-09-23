
enum e_PLAYER_CMD_DATA
{
    e_sCreateNoteTitle[MAX_COLL_TIT_LEN],
    e_sCreateNoteInnerMsg[MAX_COLL_MSG_LEN],
    e_iCreateNotePageCount
};
new ePlayerCmdData[MAX_PLAYERS][e_PLAYER_CMD_DATA];