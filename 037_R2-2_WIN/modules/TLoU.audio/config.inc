#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// AUDIO PLUGIN CONFIG
#define		AUDIO_PLUGIN_PORT		(7777)
#define     AUDIO_PLUGIN_PACK       "tlou_audio_pack"

#define     NO_AUDIO                (-1)

#define     MAX_3D_AUDIOS           (10)

//#region LOCAL AUDIO FILES (INDEXES)
    #define     AUDIO_RELOAD_SHOTGUN        (52)
    #define     AUDIO_RELOAD_RIFLE          (53)
    #define     AUDIO_RELOAD_9MM            (54)
    #define     AUDIO_RELOAD_REVOLVER       (55)
    #define     AUDIO_RELOAD_ASSAULT        (56)

    #define     AUDIO_BANDAGE_USE           (57)
    #define     AUDIO_BANDAGE_FINISH        (58)
    
    #define     AUDIO_PICKUP_BLADE          (59)
    #define     AUDIO_PICKUP_RAG            (60)
    #define     AUDIO_PICKUP_ALCOHOL        (61)
    #define     AUDIO_PICKUP_EXPLOSIVE      (62)
    #define     AUDIO_PICKUP_SUGAR          (63)
    #define     AUDIO_PICKUP_MEDKIT         (64)

    #define     AUDIO_DEATH_SFX             (65)

    #define     AUDIO_CRAFT_FINISH          (66)

    #define     AUDIO_WEAP_MENU_RIGHT       (67)
    #define     AUDIO_WEAP_MENU_LEFT        (68)

    #define     AUDIO_BP_SWITCH_WEAPS       (69)

    #define     AUDIO_SKILL_UPGRADE         (70)
    #define     AUDIO_SKILL_UPDWN           (71)

    #define     AUDIO_BOTTLE_PICKUP         (78)
    #define     AUDIO_BOTTLE_BREAK          (72)

    #define     AUDIO_BRICK_PICKUP          (73)
    #define     AUDIO_BRICK_BREAK           (74)

    #define     AUDIO_MOLOTOV_EXPLODE       (75)

    #define     AUDIO_SMKBMB_EXPLODE        (76)

    #define     AUDIO_NAILBMB_IN            (77)
    #define     AUDIO_NAILBMB_LOOP          (78)
    #define     AUDIO_NAILBMB_EXPLODE       (79)

    #define     AUDIO_WEAP_NOAMMO           (80)

    #define     AUDIO_NAVI_LEFT             (81)
    #define     AUDIO_NAVI_RIGHT            (82)

    #define     AUDIO_BP_OPEN               (83)
    #define     AUDIO_BP_CLOSE              (84)

    #define     AUDIO_CRAFT_UPDWN1          (85)
    #define     AUDIO_CRAFT_UPDWN2          (86)
    #define     AUDIO_CRAFT_UPDWN3          (87)
    #define     AUDIO_CRAFT_UPDWN4          (88)
    #define     AUDIO_CRAFT_UPDWN5          (89)

    #define     AUDIO_CRAFT_MELEE           (90)
    #define     AUDIO_CRAFT_SHIV            (91)
    #define     AUDIO_CRAFT_MEDKIT_MOLOTOV  (92)
    #define     AUDIO_CRAFT_BOMB            (93)
    #define     AUDIO_CRAFT_SMKBMB          (94)

    #define     AUDIO_BOW_PULL              (97)
    #define     AUDIO_BOW_CANCEL            (98)
    #define     AUDIO_BOW_SHOT              (99)
//#endregion

//#region REMOTE AUDIO FILES (INDEXES)
    #define AUDIO_CLICKER_DEATH01           (100)
    #define AUDIO_CLICKER_DEATH02           (101)
    #define AUDIO_CLICKER_DEATH03           (102)
//#endregion

// 3D Audio distances.
#define     DIST3D_CLICKER_SHIVED          (15.0)
#define     DIST3D_THROWABLE_BREAK_RANGE   (20.0)
#define     DIST3D_EXPLODE_RANGE           (30.0)
#define     DIST3D_RELOAD_WEAPON_RANGE     (10.0)

enum e_PLAYER_AUDIO
{
    e_iAudioHandle
}
new ePlayerAudio[MAX_PLAYERS][e_PLAYER_AUDIO];

new Iterator:iAudioHandle3D<MAX_PLAYERS, MAX_3D_AUDIOS>;

// Moved into INI file.
// stock const SFX_ClickerShivDeaths[3][65] =
// {
//     { "http://localhost/TLoU/infected_noise/clicker/clicker_death01.mp3" },
//     { "http://localhost/TLoU/infected_noise/clicker/clicker_death02.mp3" },
//     { "http://localhost/TLoU/infected_noise/clicker/clicker_death03.mp3" }
// };

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Establishing TCP server on port: %d for Audio Plugin...", AUDIO_PLUGIN_PORT);
    Audio_CreateTCPServer(AUDIO_PLUGIN_PORT);

    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Preparing server-audio files from: \"%s\"", AUDIO_PLUGIN_PACK);
    Audio_SetPack(AUDIO_PLUGIN_PACK);
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeExit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    Audio_DestroyTCPServer();
    return 1;
}