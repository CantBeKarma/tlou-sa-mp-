// MAXIMAL VALUES.
#define     MAX_CRAFT_ITEMS         (6)
#define     MAX_INGR_SPRITES        (30)

// CRAFT ITEMS IDs.
#define     CRAFT_ITEM_MELEE        (0)
#define     CRAFT_ITEM_SHIV         (1)
#define     CRAFT_ITEM_MEDKIT       (2)
#define     CRAFT_ITEM_MOLOTOV      (3)
#define     CRAFT_ITEM_BOMB         (4)
#define     CRAFT_ITEM_SMOKE_BOMB   (5)

// INGREDIENT IDs.
#define     INGR_ITEM_BLADE         (0)
#define     INGR_ITEM_BINDING       (1)
#define     INGR_ITEM_RAG           (2)
#define     INGR_ITEM_ALCOHOL       (3)
#define     INGR_ITEM_EXPLOSIVE     (4)
#define     INGR_ITEM_SUGAR         (5)
#define     INGR_ITEM_MELEE         (6)

// Player's possible items to craft data information.
new Iterator:ItemsToCraft<MAX_PLAYERS, MAX_CRAFT_ITEMS>;

// Backpacks data : CRAFT
enum e_BACKPACK_CRAFT_DATA
{
    ITEM_IDX:e_iCraftItemId,
    Float:e_fSelBoxX,
    Float:e_fSelBoxY,

    e_sCurrentItemIcon[32],
    e_sCurrentItemName[32],
    e_sCurrentItemDescr[32],
    ITEM_IDX:e_iCraftFirstReqIngr,
    ITEM_IDX:e_iCraftSecondReqIngr,
    e_iCraftSfx
};
stock const eBackpack_CraftItemsData[MAX_CRAFT_ITEMS][e_BACKPACK_CRAFT_DATA] =
{
    {ITEM_IDX_PIPE,         309.666656,  55.674072,     "TLoU:weap_scsr_2x4_icon", "MELEE WEAPON",     "Improves your melee weapon",    ITEM_IDX_BLADE,     ITEM_IDX_BINDING,   AUDIO_CRAFT_MELEE},
    {ITEM_IDX_SHIV,         309.666656, 101.674072,     "TLoU:weap_shiv",          "SHIV",             "Crafts a shiv",                 ITEM_IDX_BLADE,     ITEM_IDX_BINDING,   AUDIO_CRAFT_SHIV},
    {ITEM_IDX_MEDKIT,       309.666656, 147.674072,     "TLoU:weap_medkit",        "HEALTH KIT",       "Crafts a health kit",           ITEM_IDX_RAG,       ITEM_IDX_ALCOHOL,   AUDIO_CRAFT_MEDKIT_MOLOTOV},
    {ITEM_IDX_MOLOTOV,      309.666656, 193.674072,     "TLoU:weap_molotov",       "MOLOTOV COCKTAIL", "Crafts a molotov cocktail",     ITEM_IDX_RAG,       ITEM_IDX_ALCOHOL,   AUDIO_CRAFT_MEDKIT_MOLOTOV},
    {ITEM_IDX_NAIL_BOMB,    309.666656, 239.674072,     "TLoU:weap_nailbomb",      "BOMB",             "Crafts a bomb",                 ITEM_IDX_BLADE,     ITEM_IDX_EXPLOSIVE, AUDIO_CRAFT_BOMB},
    {ITEM_IDX_SMOKE_BOMB,   309.666656, 285.674072,     "TLoU:weap_smkbmb",        "SMOKE BOMB",       "Crafts a smoke bomb",           ITEM_IDX_EXPLOSIVE, ITEM_IDX_SUGAR,     AUDIO_CRAFT_SMKBMB}
};

// Sprites and values of the ingredients in the backpack (for CraftTd_DisplayIngredients() func).
enum e_INGR_SPRITES
{
    ITEM_IDX:e_iIngrId,
    e_iIngrValue,
    e_sIngrSprite[32]
};
stock const eIngrSprites[MAX_INGR_SPRITES][e_INGR_SPRITES] =
{
    {ITEM_IDX_BLADE,         0,     ""},
    {ITEM_IDX_BLADE,        25,     "TLoU:ingr_blade_quater"},
    {ITEM_IDX_BLADE,        50,     "TLoU:ingr_blade_half"},
    {ITEM_IDX_BLADE,        75,     "TLoU:ingr_blade_3quaters"},
    {ITEM_IDX_BLADE,        100,    "TLoU:ingr_blade_full"},

    {ITEM_IDX_BINDING,       0,     ""},
    {ITEM_IDX_BINDING,      25,     "TLoU:ingr_binding_quater"},
    {ITEM_IDX_BINDING,      50,     "TLoU:ingr_binding_half"},
    {ITEM_IDX_BINDING,      75,     "TLoU:ingr_binding_3quaters"},
    {ITEM_IDX_BINDING,     100,     "TLoU:ingr_binding_full"},

    {ITEM_IDX_RAG,           0,     ""},
    {ITEM_IDX_RAG,          25,     "TLoU:ingr_rag_quater"},
    {ITEM_IDX_RAG,          50,     "TLoU:ingr_rag_half"},
    {ITEM_IDX_RAG,          75,     "TLoU:ingr_rag_3quaters"},
    {ITEM_IDX_RAG,         100,     "TLoU:ingr_rag_full"},

    {ITEM_IDX_ALCOHOL,       0,     ""},
    {ITEM_IDX_ALCOHOL,      25,     "TLoU:ingr_alcohol_quater"},
    {ITEM_IDX_ALCOHOL,      50,     "TLoU:ingr_alcohol_half"},
    {ITEM_IDX_ALCOHOL,      75,     "TLoU:ingr_alcohol_3quaters"},
    {ITEM_IDX_ALCOHOL,     100,     "TLoU:ingr_alcohol_full"},

    {ITEM_IDX_EXPLOSIVE,     0,     ""},
    {ITEM_IDX_EXPLOSIVE,    25,     "TLoU:ingr_explosive_quater"},
    {ITEM_IDX_EXPLOSIVE,    50,     "TLoU:ingr_explosive_half"},
    {ITEM_IDX_EXPLOSIVE,    75,     "TLoU:ingr_explosive_3quaters"},
    {ITEM_IDX_EXPLOSIVE,   100,     "TLoU:ingr_explosive_full"},

    {ITEM_IDX_SUGAR,         0,     ""},
    {ITEM_IDX_SUGAR,        25,     "TLoU:ingr_sugar_quater"},
    {ITEM_IDX_SUGAR,        50,     "TLoU:ingr_sugar_half"},
    {ITEM_IDX_SUGAR,        75,     "TLoU:ingr_sugar_3quaters"},
    {ITEM_IDX_SUGAR,       100,     "TLoU:ingr_sugar_full"}
};