#include  <TLoU.textdraws\td_global>
#include  <TLoU.textdraws\td_player>
#include  <YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Loading global textdraws...");
    LoadGlobalTextDraws();
    return 1;
}