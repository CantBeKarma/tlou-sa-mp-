// MAXIMAL VALUES.
#define     MAX_CRAFT_ITEMS         (6)

// CRAFT ITEMS IDs.
#define     CRAFT_ITEM_MELEE        (0)
#define     CRAFT_ITEM_SHIV         (1)
#define     CRAFT_ITEM_MEDKIT       (2)
#define     CRAFT_ITEM_MOLOTOV      (3)
#define     CRAFT_ITEM_BOMB         (4)
#define     CRAFT_ITEM_SMOKE_BOMB   (5)

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

new Iterator:ItemsToCraft<MAX_PLAYERS, MAX_CRAFT_ITEMS>;