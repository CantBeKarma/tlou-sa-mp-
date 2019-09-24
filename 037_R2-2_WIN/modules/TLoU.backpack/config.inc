// Other defs.
#define     TICK_RATE_BP_OPENING            (1250)
#define     TICK_RATE_BP_KEYS_COOLDOWN      (450)
#define     TIMER_USE_BACKPACK		        (50)

// INGREDIENT IDs.
#define     MAX_INGR_SPRITES        (30)
#define     INGR_ITEM_BLADE         (0)
#define     INGR_ITEM_BINDING       (1)
#define     INGR_ITEM_RAG           (2)
#define     INGR_ITEM_ALCOHOL       (3)
#define     INGR_ITEM_EXPLOSIVE     (4)
#define     INGR_ITEM_SUGAR         (5)
#define     INGR_ITEM_MELEE         (6)

// BACKPACK CATEGORIES.
#define		BACKPACK_CATEGORY_CRAFTING		(0)
#define		BACKPACK_CATEGORY_SKILLS		(1)
#define		BACKPACK_CATEGORY_COLLECTIBLES	(2)

// Helpers.
#define     PlayerBackpackCategory(%0)      ePlayerBpInfo[%0][e_iBackpackSelected_Category]

#define     Craft_GetSelection(%0)          ePlayerBpInfo[%0][e_iBackpackSelected_Craft]
#define     Skill_GetSelection(%0)          ePlayerBpInfo[%0][e_iBackpackSelected_Skill]
#define     Coll_GetSelection(%0)           ePlayerBpInfo[%0][e_iBackpackSelected_Coll]
#define     Coll_ViewingGetSelection(%0)    ePlayerBpInfo[%0][e_iBackpackSelected_ViewColl]

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

// Player backpack's current data.
enum e_PLAYER_BACKPACK_DATA
{
    e_iBackpackSelected_Craft,
    e_iBackpackSelected_Skill,
    e_iBackpackSelected_Coll,
    e_iBackpackSelected_ViewColl,

    e_iBackpack_CollTrackList,

    e_iBackpackSelected_Category,
    e_iTickCounter_backpackOpening,

    e_iBackpackKeysUpDown,
	e_iBackpackKeysLeftRight,
    e_iTickCounter_backpackKeys,
	Timer:e_iTimer_craftingOrUpgrading
};
new ePlayerBpInfo[MAX_PLAYERS][e_PLAYER_BACKPACK_DATA];