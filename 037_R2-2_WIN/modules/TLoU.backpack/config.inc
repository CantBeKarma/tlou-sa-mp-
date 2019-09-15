// Other defs.
#define     TICK_RATE_BP_OPENING            (1250)
#define		TIMER_USE_BACKPACK		        (50)
#define     TICK_RATE_CHANGE_COLL_TYPES     (800)

// CRAFT ITEMS IDs.
#define     CRAFT_ITEM_MELEE        (0)
#define     CRAFT_ITEM_SHIV         (1)
#define     CRAFT_ITEM_MEDKIT       (2)
#define     CRAFT_ITEM_MOLOTOV      (3)
#define     CRAFT_ITEM_BOMB         (4)
#define     CRAFT_ITEM_SMOKE_BOMB   (5)

// SKILL ITEM IDs.
#define     SKILL_ITEM_MAXHP        (0)
#define     SKILL_ITEM_LISTEN_DIST  (1)
#define     SKILL_ITEM_CRAFT_SPEED  (2)
#define     SKILL_ITEM_HEAL_SPEED   (3)
#define     SKILL_ITEM_WEAP_SWAY    (4)
#define     SKILL_ITEM_SHIV_MASTER  (5)

// COLLECTIBLES ITEM IDs.
#define     COLL_ITEM_ARTIFACTS     (0)
#define     COLL_ITEM_FIREFLY_PEND  (1)

// INGREDIENT IDs.
//#define     MAX_INGR_TYPES          (7)
#define     MAX_INGR_SPRITES        (30)
#define     INGR_ITEM_BLADE         (0)
#define     INGR_ITEM_BINDING       (1)
#define     INGR_ITEM_RAG           (2)
#define     INGR_ITEM_ALCOHOL       (3)
#define     INGR_ITEM_EXPLOSIVE     (4)
#define     INGR_ITEM_SUGAR         (5)
#define     INGR_ITEM_MELEE         (6)

// MAX AMOUNTS OF COLLECTIBLES.
#define     MAX_COLLECTIBLES_DATA       (3)
#define     MAX_PLAYER_COLLECTIBLES     (100)
#define     MAX_DISPLAY_COLLECTIBLES    (9)

// MAX AMOUNT OF SUPPS
#define     MAX_PLAYER_SUPPS_AMOUNT     (530)

// BACKPACK CATEGORIES.
#define		BACKPACK_CATEGORY_CRAFTING		(0)
#define		BACKPACK_CATEGORY_SKILLS		(1)
#define		BACKPACK_CATEGORY_COLLECTIBLES	(2)

// Items list in backpack.
#define     MAX_CRAFT_ITEMS         (6)
#define     MAX_SKILLS_ITEMS        (6)
#define     MAX_COLL_ITEMS          (2)

// Helpers.
#define     PlayerBackpackCategory(%0)      ePlayerBpInfo[%0][e_iBackpackSelected_Category]

#define     Craft_GetSelection(%0)          ePlayerBpInfo[%0][e_iBackpackSelected_Craft]

#define     Skill_GetSelection(%0)          ePlayerBpInfo[%0][e_iBackpackSelected_Skill]
#define     Skill_GetLevel(%0,%1)           eCharsInfo[%0][e_iSkillLevel][%1]
#define     Skill_SuppsAmount(%0)           eCharsInfo[%0][e_iSkills_SuppsAmount]

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

//#region Backpacks data : CRAFT

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

//#endregion

//#region Backpacks data : SKILLS

    enum e_BACKPACK_SKILL_DATA
    {
        Float:e_fSelBoxX,
        Float:e_fSelBoxY,

        e_iSkill_MaxLvl,
        e_iSupps_Required,
        e_iSkill_Description[64]
    };
    stock const eBackpack_SkillItemsData[MAX_SKILLS_ITEMS][e_BACKPACK_SKILL_DATA] =
    {
        {385.0,  95.911079, 2, 50, "Increases your maximum health"},
        {385.0, 133.659271, 3, 20, "Increases the radius of listen mode"},
        {385.0, 170.992584, 3, 20, "Reduces crafting time"},
        {385.0, 208.325881, 3, 20, "Use health kits faster"},
        {385.0, 246.488891, 2, 50, "Reduces weapon sway"},
        {385.0, 283.82,     2, 75, "Use a shiv to save yourself when~n~grabbed by a clicker"}
    };

    new Iterator:SkillsToUpgrade<MAX_PLAYERS, MAX_SKILLS_ITEMS>;

//#endregion

//#region Backpacks data : COLLECTIBLES

    // Collectibles backpack section information (X, Y positions of select box).
    enum e_BACKPACK_COLL_DATA
    {
        Float:e_fSelBoxX,
        Float:e_fSelBoxY
    };
    stock const eBackpack_CollItemsData[MAX_COLL_ITEMS][e_BACKPACK_COLL_DATA] =
    {
        {376.333312, 123.703620},
        {376.333312, 152.703613}
    };

    // Player's collectibles data information (Artifacts & Firefly Pendants).
    new Iterator:ArtifactsOwned<MAX_PLAYERS, MAX_PLAYER_COLLECTIBLES>;
    new Iterator:PendantsOwned<MAX_PLAYERS, MAX_PLAYER_COLLECTIBLES>;

    // List of available types of collectibles.
    enum e_COLLECTIBLE_TYPES
    {
        e_iCollectibleType,
        e_iCollectible_ModelId,
        e_iCollectible_AudioPickupId
    };
    stock const eCollectiblesData[MAX_COLLECTIBLES_DATA][e_COLLECTIBLE_TYPES] =
    {
        { ITEM_TYPE_ARTIFACT,       ITEM_MODEL_PAPER,       AUDIO_PICKUP_NOTE },
        { ITEM_TYPE_ARTIFACT,       ITEM_MODEL_NOTES,       AUDIO_PICKUP_NOTE },
        { ITEM_TYPE_FIREFLY_PEND,   ITEM_MODEL_PENDANT,     AUDIO_PICKUP_PENDANT }
    };

//#endregion

// Player backpack's current data.
enum e_PLAYER_BACKPACK_DATA
{
    e_iBackpackSelected_Craft,
    e_iBackpackSelected_Skill,
    e_iBackpackSelected_Coll,
    e_iBackpackSelected_ViewColl,

    e_iBackpackSelected_Category,
    e_iTickCounter_backpackOpening,
    e_iTickCounter_changeCollTypes,

    e_iBackpackKeysUpDown,
	e_iBackpackKeysLeftRight,
    e_iTickCounter_backpackKeys,
	Timer:e_iTimer_craftingOrUpgrading
};
new ePlayerBpInfo[MAX_PLAYERS][e_PLAYER_BACKPACK_DATA];