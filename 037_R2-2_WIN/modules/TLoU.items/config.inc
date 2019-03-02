// Other
#define     MAX_ITEMS_DATA          (27)
#define     NO_OWNER_UID            (-1)
#define     ITEM_LABEL_DRAW_DIST    (2.5)

// Types of the items available.
#define     ITEM_TYPE_MELEE         (0)
#define     ITEM_TYPE_WEAPON        (1)
#define     ITEM_TYPE_HEAL          (2)
#define     ITEM_TYPE_INGREDIENT    (3)
#define     ITEM_TYPE_PROJECTILE    (4)
#define     ITEM_TYPE_THROWABLE     (5)
#define     ITEM_TYPE_COLLECTIBLE   (6)
#define     ITEM_TYPE_AMMO          (7)

//#region Item models.
    // -> Craftable
    #define     ITEM_MODEL_SHIV         (15071)
    #define     ITEM_MODEL_MEDKIT       (15072)
    #define     ITEM_MODEL_BOMB         (342)
    #define     ITEM_MODEL_MOLOTOV      (344)
    #define     ITEM_MODEL_SMKBMB       (343)

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

enum e_ITEM_DATA
{
    e_sItem_Name[24],
    e_iItem_Type,
    e_iItem_ModelId,
    e_iItem_SlotId,
    e_iItem_AudioPickupId,
    e_iItem_SA_WeaponId,

    Float:e_iItem_BodyDmg,
    Float:e_iItem_HeadDmg,

    e_iItem_CraftItemId,
    e_iItem_CraftIngrRequired1,
    e_iItem_CraftIngrRequired2
};
stock const eItemsData[MAX_ITEMS_DATA][e_ITEM_DATA] =
{
    {"Blade",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BLADE,       -1,                         AUDIO_PICKUP_BLADE,         -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Binding",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BINDING,     -1,                         AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Rag",                 ITEM_TYPE_INGREDIENT,   ITEM_MODEL_RAG,         -1,                         AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Alcohol",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_ALCOHOL,     -1,                         AUDIO_PICKUP_ALCOHOL,       -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Explosive",           ITEM_TYPE_INGREDIENT,   ITEM_MODEL_EXPLOSIVE,   -1,                         AUDIO_PICKUP_EXPLOSIVE,     -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Sugar",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_SUGAR,       -1,                         AUDIO_PICKUP_SUGAR,         -1,                     0.0,    0.0,        -1,                     -1,                     -1},

    {"Pipe",                ITEM_TYPE_MELEE,        ITEM_MODEL_PIPE,        WEAPON_SLOT_MELEE,          -1,                         WEAPON_PIPE,            12.0,   12.0,       CRAFT_ITEM_MELEE,       -1,                     -1},
    {"Scissor pipe",        ITEM_TYPE_MELEE,        ITEM_MODEL_SCSR_PIPE,   WEAPON_SLOT_MELEE,          -1,                         WEAPON_SCISSOR_PIPE,    20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Hatchet",             ITEM_TYPE_MELEE,        ITEM_MODEL_HATCHET,     WEAPON_SLOT_MELEE,          -1,                         WEAPON_HATCHET,         20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Machete",             ITEM_TYPE_MELEE,        ITEM_MODEL_MACHETE,     WEAPON_SLOT_MELEE,          -1,                         WEAPON_MACHETE,         20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},

    {"9mm pistol",          ITEM_TYPE_WEAPON,       ITEM_MODEL_9MM,         WEAPON_SLOT_9MM,            -1,                         WEAPON_9MM,             20.0,   60.0,       -1,                     -1,                     -1},
    {"Revolver",            ITEM_TYPE_WEAPON,       ITEM_MODEL_REVOLVER,    WEAPON_SLOT_REVOLVER,       -1,                         WEAPON_REVOLVER,        35.0,   75.0,       -1,                     -1,                     -1},
    {"Shorty",              ITEM_TYPE_WEAPON,       ITEM_MODEL_SHORTY,      WEAPON_SLOT_SHORTY,         -1,                         WEAPON_SHORTY,          35.0,   35.0,       -1,                     -1,                     -1},

    {"Shotgun",             ITEM_TYPE_WEAPON,       ITEM_MODEL_SHOTGUN,     WEAPON_SLOT_SHOTGUN,        -1,                         WEAPON_SHOTGUN,         40.0,   80.0,       -1,                     -1,                     -1},
    {"Rifle",               ITEM_TYPE_WEAPON,       ITEM_MODEL_RIFLE,       WEAPON_SLOT_RIFLE,          -1,                         WEAPON_RIFLE,           70.0,   100.0,      -1,                     -1,                     -1},
    {"Assault rifle",       ITEM_TYPE_WEAPON,       ITEM_MODEL_ASSAULT,     WEAPON_SLOT_ASSAULT,        -1,                         WEAPON_ASSAULT,         20.0,   60.0,       -1,                     -1,                     -1},
    {"Bow",                 ITEM_TYPE_WEAPON,       ITEM_MODEL_BOW,         WEAPON_SLOT_BOW,            -1,                         WEAPON_BOW,             45.0,   35.0,       -1,                     -1,                     -1},
    {"Arrow",               ITEM_TYPE_AMMO,         ITEM_MODEL_ARROW,       -1,                         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1},

    {"Shiv",                ITEM_TYPE_WEAPON,       ITEM_MODEL_SHIV,        WEAPON_SLOT_SHIV,           -1,                         WEAPON_SHIV,            0.0,    0.0,        CRAFT_ITEM_SHIV,        INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Medkit",              ITEM_TYPE_HEAL,         ITEM_MODEL_MEDKIT,      WEAPON_SLOT_MEDKIT,         AUDIO_PICKUP_MEDKIT,        WEAPON_MEDKIT,          0.0,    0.0,        CRAFT_ITEM_MEDKIT,      INGR_ITEM_RAG,          INGR_ITEM_ALCOHOL},
    {"Molotov Cocktail",    ITEM_TYPE_WEAPON,       ITEM_MODEL_MOLOTOV,     WEAPON_SLOT_MOLOTOV,        -1,                         WEAPON_MOLOTOV,         5.0,    5.0,        CRAFT_ITEM_MOLOTOV,     INGR_ITEM_RAG,          INGR_ITEM_ALCOHOL},
    {"Nail bomb",           ITEM_TYPE_WEAPON,       ITEM_MODEL_BOMB,        WEAPON_SLOT_BOMB,           -1,                         WEAPON_NAIL_BOMB,       85.0,   85.0,       CRAFT_ITEM_BOMB,        INGR_ITEM_BLADE,        INGR_ITEM_EXPLOSIVE},
    {"Smoke Bomb",          ITEM_TYPE_WEAPON,       ITEM_MODEL_SMKBMB,      WEAPON_SLOT_SMOKE_BOMB,     -1,                         WEAPON_SMOKE_BOMB,      0.0,    0.0,        CRAFT_ITEM_SMOKE_BOMB,  INGR_ITEM_EXPLOSIVE,    INGR_ITEM_SUGAR},

    {"Brick",               ITEM_TYPE_THROWABLE,    ITEM_MODEL_BRICK,       WEAPON_SLOT_BRICK_BOTTLE,   AUDIO_BRICK_PICKUP,         WEAPON_BRICK,           10.0,   10.0,       -1,                     -1,                     -1},
    {"Bottle",              ITEM_TYPE_THROWABLE,    ITEM_MODEL_BOTTLE,      WEAPON_SLOT_BRICK_BOTTLE,   AUDIO_BOTTLE_PICKUP,        WEAPON_BOTTLE,          10.0,   10.0,       -1,                     -1,                     -1},

    {"Note #1",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_PAPER,       -1,                         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Note #2",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_NOTES,       -1,                         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1}
};