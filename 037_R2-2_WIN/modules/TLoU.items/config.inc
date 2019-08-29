// Maximal values.
#define     MAX_ITEMS_DATA          (27)
#define     MAX_MELEE_ITEMS         (4)
#define     MAX_UPGRADE_ITEMS       (18)

// Other.
#define     NO_OWNER_UID            (-1)
#define     ITEM_LABEL_DRAW_DIST    (2.5)
#define     HEAL_ITEM_RECOVER_HP    (5.0)
#define     DAMAGE_VALUE_FISTS      (8.0)

// Types of the items available.
#define     ITEM_TYPE_MELEE         (0)
#define     ITEM_TYPE_WEAPON        (1)
#define     ITEM_TYPE_HEAL          (2)
#define     ITEM_TYPE_INGREDIENT    (3)
#define     ITEM_TYPE_PROJECTILE    (4)
#define     ITEM_TYPE_THROWABLE     (5)
#define     ITEM_TYPE_COLLECTIBLE   (6)
#define     ITEM_TYPE_AMMO          (7)
#define     ITEM_TYPE_MISC          (8)

//#region Item models.
    // -> Craftable
    #define     ITEM_MODEL_SHIV         (15071)
    #define     ITEM_MODEL_MEDKIT       (15072)
    #define     ITEM_MODEL_BOMB         (15111)
    #define     ITEM_MODEL_MOLOTOV      (15112)
    #define     ITEM_MODEL_SMKBMB       (15113)

    // -> Ingredients
    #define     ITEM_MODEL_BLADE        (15073)
    #define     ITEM_MODEL_BINDING      (15074)
    #define     ITEM_MODEL_RAG          (15075)
    #define     ITEM_MODEL_ALCOHOL      (15076)
    #define     ITEM_MODEL_EXPLOSIVE    (15077)
    #define     ITEM_MODEL_SUGAR        (15078)

    // -> Weapons.
    #define     ITEM_MODEL_PIPE         (336)
    #define     ITEM_MODEL_SCSR_PIPE    (337)
    #define     ITEM_MODEL_HATCHET      (321)
    #define     ITEM_MODEL_MACHETE      (323)

    #define     ITEM_MODEL_9MM          (347)
    #define     ITEM_MODEL_REVOLVER     (348)
    #define     ITEM_MODEL_SHORTY       (350)

    #define     ITEM_MODEL_SHOTGUN      (349)
    #define     ITEM_MODEL_RIFLE        (357)
    #define     ITEM_MODEL_ASSAULT      (356)
    #define     ITEM_MODEL_BOW          (15109)

    #define     ITEM_MODEL_BRICK        (15101)
    #define     ITEM_MODEL_BOTTLE       (1509)

    #define     ITEM_MODEL_ARROW        (15110)

    // -> Collectibles
    #define     ITEM_MODEL_PAPER        (2953)
    #define     ITEM_MODEL_NOTES        (2059)
//#endregion

// Weapon upgrade types.
#define     UPGRADE_TYPE_CLIPSIZE       (0)

enum ITEM_IDX
{
    ITEM_IDX_BLADE,
    ITEM_IDX_BINDING,
    ITEM_IDX_RAG,
    ITEM_IDX_ALCOHOL,
    ITEM_IDX_EXPLOSIVE,
    ITEM_IDX_SUGAR,

    ITEM_IDX_PIPE,
    ITEM_IDX_SCSR_PIPE,
    ITEM_IDX_HATCHET,
    ITEM_IDX_MACHETE,

    ITEM_IDX_9MM,
    ITEM_IDX_REVOLVER,
    ITEM_IDX_SHORTY,

    ITEM_IDX_SHOTGUN,
    ITEM_IDX_RIFLE,
    ITEM_IDX_ASSAULT_RIFLE,
    ITEM_IDX_BOW,
    ITEM_IDX_ARROW,

    ITEM_IDX_SHIV,
    ITEM_IDX_MEDKIT,
    ITEM_IDX_MOLOTOV,
    ITEM_IDX_NAIL_BOMB,
    ITEM_IDX_SMOKE_BOMB,

    ITEM_IDX_BRICK,
    ITEM_IDX_BOTTLE
};

stock const eItemsSprites[MAX_ITEMS_DATA][25] =
{
    "TLoU:ingr_blade_full",
    "TLoU:ingr_binding_full",
    "TLoU:ingr_rag_full",
    "TLoU:ingr_alcohol_full",
    "TLoU:ingr_explosive_full",
    "TLoU:ingr_sugar_full",

    "TLoU:weap_pipe",
    "TLoU:weap_scsr_pipe",
    "TLoU:weap_hatchet",
    "TLoU:weap_machete",

    "TLoU:weap_9mm",
    "TLoU:weap_rev",
    "TLoU:weap_shorty",

    "TLoU:weap_shot",
    "TLoU:weap_huntrif",
    "TLoU:weap_assault",
    "TLoU:weap_bow",
    "",

    "TLoU:weap_shiv",
    "TLoU:weap_medkit",
    "TLoU:weap_molotov",
    "TLoU:weap_nailbomb",
    "TLoU:weap_smkbmb",

    "TLoU:weap_brick",
    "TLoU:weap_bottle",

    "",
    ""
};

enum e_ITEM_DATA
{
    e_sItem_Name[24],
    e_iItem_Type,
    e_iItem_ModelId,
    e_iItem_MaxClipSize,
    e_iItem_MaxAmount,
    e_iItem_AudioPickupId,
    e_iItem_SA_WeaponId,

    Float:e_iItem_BodyDmg,
    Float:e_iItem_HeadDmg
};
stock const eItemsData[MAX_ITEMS_DATA][e_ITEM_DATA] =
{
    {"Blade",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BLADE,       -1,     300,        AUDIO_PICKUP_BLADE,         -1,                     0.0,    0.0},
    {"Binding",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BINDING,     -1,     300,        AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0},
    {"Rag",                 ITEM_TYPE_INGREDIENT,   ITEM_MODEL_RAG,         -1,     300,        AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0},
    {"Alcohol",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_ALCOHOL,     -1,     300,        AUDIO_PICKUP_ALCOHOL,       -1,                     0.0,    0.0},
    {"Explosive",           ITEM_TYPE_INGREDIENT,   ITEM_MODEL_EXPLOSIVE,   -1,     300,        AUDIO_PICKUP_EXPLOSIVE,     -1,                     0.0,    0.0},
    {"Sugar",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_SUGAR,       -1,     300,        AUDIO_PICKUP_SUGAR,         -1,                     0.0,    0.0},

    {"Pipe",                ITEM_TYPE_MELEE,        ITEM_MODEL_PIPE,        -1,     -1,         -1,                         WEAPON_PIPE,            12.0,   12.0},
    {"Scissor pipe",        ITEM_TYPE_MELEE,        ITEM_MODEL_SCSR_PIPE,   -1,     -1,         -1,                         WEAPON_SCISSOR_PIPE,    20.0,   20.0},
    {"Hatchet",             ITEM_TYPE_MELEE,        ITEM_MODEL_HATCHET,     -1,     -1,         -1,                         WEAPON_HATCHET,         20.0,   20.0},
    {"Machete",             ITEM_TYPE_MELEE,        ITEM_MODEL_MACHETE,     -1,     -1,         -1,                         WEAPON_MACHETE,         20.0,   20.0},

    {"9mm pistol",          ITEM_TYPE_WEAPON,       ITEM_MODEL_9MM,         -1,     -1,         -1,                         WEAPON_9MM,             20.0,   60.0},
    {"Revolver",            ITEM_TYPE_WEAPON,       ITEM_MODEL_REVOLVER,     6,     18,         -1,                         WEAPON_REVOLVER,        35.0,   75.0},
    {"Shorty",              ITEM_TYPE_WEAPON,       ITEM_MODEL_SHORTY,      -1,     -1,         -1,                         WEAPON_SHORTY,          35.0,   35.0},

    {"Shotgun",             ITEM_TYPE_WEAPON,       ITEM_MODEL_SHOTGUN,     -1,     -1,         -1,                         WEAPON_SHOTGUN,         40.0,   80.0},
    {"Rifle",               ITEM_TYPE_WEAPON,       ITEM_MODEL_RIFLE,       -1,     -1,         -1,                         WEAPON_RIFLE,           70.0,   100.0},
    {"Assault rifle",       ITEM_TYPE_WEAPON,       ITEM_MODEL_ASSAULT,     30,     90,         -1,                         WEAPON_ASSAULT,         20.0,   60.0},
    {"Bow",                 ITEM_TYPE_WEAPON,       ITEM_MODEL_BOW,         -1,      9,         -1,                         WEAPON_BOW,             45.0,   35.0},
    {"Arrow",               ITEM_TYPE_AMMO,         ITEM_MODEL_ARROW,       -1,      0,         -1,                         -1,                     0.0,    0.0},

    {"Shiv",                ITEM_TYPE_MISC,         ITEM_MODEL_SHIV,        -1,      3,          -1,                         WEAPON_SHIV,            0.0,    0.0},
    {"Medkit",              ITEM_TYPE_HEAL,         ITEM_MODEL_MEDKIT,      -1,      3,          AUDIO_PICKUP_MEDKIT,        WEAPON_MEDKIT,          0.0,    0.0},
    {"Molotov Cocktail",    ITEM_TYPE_PROJECTILE,   ITEM_MODEL_MOLOTOV,     -1,      3,          -1,                         WEAPON_MOLOTOV,         5.0,    5.0},
    {"Nail bomb",           ITEM_TYPE_PROJECTILE,   ITEM_MODEL_BOMB,        -1,      3,          -1,                         WEAPON_NAIL_BOMB,       85.0,   85.0},
    {"Smoke Bomb",          ITEM_TYPE_PROJECTILE,   ITEM_MODEL_SMKBMB,      -1,      3,          -1,                         WEAPON_SMOKE_BOMB,      0.0,    0.0},

    {"Brick",               ITEM_TYPE_THROWABLE,    ITEM_MODEL_BRICK,       -1,      1,          AUDIO_BRICK_PICKUP,         WEAPON_BRICK,           10.0,   10.0},
    {"Bottle",              ITEM_TYPE_THROWABLE,    ITEM_MODEL_BOTTLE,      -1,      1,          AUDIO_BOTTLE_PICKUP,        WEAPON_BOTTLE,          10.0,   10.0},

    {"Note #1",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_PAPER,       -1,      0,          -1,                         -1,                     0.0,    0.0},
    {"Note #2",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_NOTES,       -1,      0,          -1,                         -1,                     0.0,    0.0}
};

// Item upgrade's data.
enum e_ITEM_UPGRADES_DATA
{
    ITEM_IDX:e_iItemId,
    e_iItem_UpgradeLvl,
    e_iItem_UpgradeType,

    e_iItem_ClipSize,
    e_iItem_MaxAmount,

    e_iItem_RequiredParts,
    e_iItem_RequiredToolsLvl
};
stock const eItemsUpgrades[MAX_UPGRADE_ITEMS][e_ITEM_UPGRADES_DATA] =
{
    {ITEM_IDX_9MM, 0, UPGRADE_TYPE_CLIPSIZE,  6, 20,  0, 0},
    {ITEM_IDX_9MM, 1, UPGRADE_TYPE_CLIPSIZE,  8, 22, 15, 0},
    {ITEM_IDX_9MM, 2, UPGRADE_TYPE_CLIPSIZE, 10, 24, 20, 1},
    {ITEM_IDX_9MM, 3, UPGRADE_TYPE_CLIPSIZE, 12, 26, 30, 2},
    {ITEM_IDX_9MM, 4, UPGRADE_TYPE_CLIPSIZE, 14, 28, 40, 3},

    {ITEM_IDX_SHORTY, 0, UPGRADE_TYPE_CLIPSIZE, 2, 20, 20, 2},
    {ITEM_IDX_SHORTY, 1, UPGRADE_TYPE_CLIPSIZE, 3, 22, 30, 4},
    {ITEM_IDX_SHORTY, 2, UPGRADE_TYPE_CLIPSIZE, 4, 24, 40, 4},
    {ITEM_IDX_SHORTY, 3, UPGRADE_TYPE_CLIPSIZE, 5, 26, 50, 5},
    {ITEM_IDX_SHORTY, 3, UPGRADE_TYPE_CLIPSIZE, 6, 28, 50, 5},

    {ITEM_IDX_SHOTGUN, 0, UPGRADE_TYPE_CLIPSIZE, 4, 14,  0, 0},
    {ITEM_IDX_SHOTGUN, 1, UPGRADE_TYPE_CLIPSIZE, 5, 15, 15, 2},
    {ITEM_IDX_SHOTGUN, 2, UPGRADE_TYPE_CLIPSIZE, 6, 16, 30, 3},
    {ITEM_IDX_SHOTGUN, 3, UPGRADE_TYPE_CLIPSIZE, 7, 17, 40, 4},
    {ITEM_IDX_SHOTGUN, 4, UPGRADE_TYPE_CLIPSIZE, 8, 18, 50, 5},

    {ITEM_IDX_RIFLE, 0, UPGRADE_TYPE_CLIPSIZE, 1, 6,  0, 0},
    {ITEM_IDX_RIFLE, 1, UPGRADE_TYPE_CLIPSIZE, 2, 7, 20, 1},
    {ITEM_IDX_RIFLE, 2, UPGRADE_TYPE_CLIPSIZE, 3, 9, 40, 4}
};

// Melee weapons conditions (CND).
enum e_ITEM_MELEE_CND
{
    ITEM_IDX:e_iItemId,
    e_iItemBasicCND,
    e_iItemExtraCND
};
stock const eItemsMeleeCND[MAX_MELEE_ITEMS][e_ITEM_MELEE_CND] =
{
    {ITEM_IDX_PIPE,        9, 0},
    {ITEM_IDX_SCSR_PIPE,   9, 1},
    {ITEM_IDX_HATCHET,     5, 3},
    {ITEM_IDX_MACHETE,     3, 3}
};

// Player item's data.
enum e_PLAYER_ITEM_DATA
{
    e_iItemId,
    e_iItemAmount,
    e_iItemClipAmount,
    e_iItemMeleeExtraCND,
    e_iItemUpgradeLvl_ClipSize
};
new ePlayerItemData[MAX_PLAYERS][MAX_ITEMS_DATA][e_PLAYER_ITEM_DATA];