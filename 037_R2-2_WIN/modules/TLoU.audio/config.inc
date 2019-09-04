#include	<YSI\y_hooks>

//#define     USE_VPS

// AUDIO PLUGIN CONFIG
#if defined USE_VPS
    #define		AUDIO_PLUGIN_PORT		(2560)
#else
    #define     AUDIO_PLUGIN_PORT       (7777)
#endif

#define     AUDIO_PLUGIN_PACK       "tlou_audio_pack"

#define     NO_AUDIO                (-1)

#define     MAX_3D_AUDIOS           (10)
#define     MAX_RELOAD_SFX_AUDIOS   (6)
#define     MAX_PROJECTILES_SFX     (5)

//#region LOCAL AUDIO FILES (INDEXES)
    #define     AUDIO_PICKUP_PENDANT        (47)
    #define     AUDIO_PICKUP_NOTE           (48)

    #define     AUDIO_COLL_UPDWN            (49)
    #define     AUDIO_COLL_VIEW             (50)
    
    #define     AUDIO_RELOAD_SHORTY         (51)
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

    #define     AUDIO_NAILBMB_TRIGGER       (77)
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

enum e_RELOAD_AUDIO_SFX
{
    e_iWeaponId,
    e_iAudioId
}
stock const SFX_WeaponReloads[MAX_RELOAD_SFX_AUDIOS][e_RELOAD_AUDIO_SFX] =
{
    {WEAPON_9MM,            AUDIO_RELOAD_9MM},
    {WEAPON_REVOLVER,       AUDIO_RELOAD_REVOLVER},
    {WEAPON_SHORTY,         AUDIO_RELOAD_SHORTY},
    {WEAPON_SHOTGUN,        AUDIO_RELOAD_SHOTGUN},
    {WEAPON_RIFLE,          AUDIO_RELOAD_RIFLE},
    {WEAPON_ASSAULT,        AUDIO_RELOAD_ASSAULT}
};

enum e_SERVER_PROJECTILES_SFX
{
    e_iProjectileWeaponId,
    e_iProjectileDestroySFX,
    Float:e_fProjectileDistanceSFX
};
stock const SFX_ProjectilesDestroy[MAX_PROJECTILES_SFX][e_SERVER_PROJECTILES_SFX] =
{
    {WEAPON_BOTTLE,         AUDIO_BOTTLE_BREAK,     DIST3D_THROWABLE_BREAK_RANGE},
    {WEAPON_BRICK,          AUDIO_BRICK_BREAK,      DIST3D_THROWABLE_BREAK_RANGE},
    {WEAPON_MOLOTOV,        AUDIO_MOLOTOV_EXPLODE,  DIST3D_EXPLODE_RANGE},
    {WEAPON_NAIL_BOMB,      AUDIO_NAILBMB_EXPLODE,  DIST3D_EXPLODE_RANGE},
    {WEAPON_SMOKE_BOMB,     AUDIO_SMKBMB_EXPLODE,   DIST3D_EXPLODE_RANGE}
};

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