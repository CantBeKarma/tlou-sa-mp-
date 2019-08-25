// Other defs.
#define     TICK_RATE_BP_OPENING    (1250)

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
#define     INGR_ITEM_BLADE         (0)
#define     INGR_ITEM_BINDING       (1)
#define     INGR_ITEM_RAG           (2)
#define     INGR_ITEM_ALCOHOL       (3)
#define     INGR_ITEM_EXPLOSIVE     (4)
#define     INGR_ITEM_SUGAR         (5)
#define     INGR_ITEM_MELEE         (6)

// MAX AMOUNTS OF ITEMS.
#define     MAX_AMOUNT_ITEMS        (3)

#define     MAX_AMOUNT_SHIV         (3)
#define     MAX_AMOUNT_MEDKIT       (3)
#define     MAX_AMOUNT_MOLOTOV      (3)
#define     MAX_AMOUNT_BOMB         (3)
#define     MAX_AMOUNT_SMOKE_BOMB   (3)

// MAX AMOUNTS OF COLLECTIBLES.
#define     MAX_ARTIFACTS           (100)

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

// Backpacks data : CRAFT
new Iterator:ItemsToCraft<MAX_PLAYERS, MAX_CRAFT_ITEMS>;

enum e_BACKPACK_CRAFT_DATA
{
    Float:e_fSelBoxX,
    Float:e_fSelBoxY,

    e_sCurrentItemIcon[32],
    e_sCurrentItemName[32],
    e_sCurrentItemDescr[32],
    e_iCraftSfx
};
stock const eBackpack_CraftItemsData[MAX_CRAFT_ITEMS][e_BACKPACK_CRAFT_DATA] =
{
    {309.666656,  55.674072, "TLoU:weap_scsr_2x4_icon", "MELEE WEAPON",     "Improves your melee weapon",    AUDIO_CRAFT_MELEE},
    {309.666656, 101.674072, "TLoU:weap_shiv",          "SHIV",             "Crafts a shiv",                 AUDIO_CRAFT_SHIV},
    {309.666656, 147.674072, "TLoU:weap_medkit",        "HEALTH KIT",       "Crafts a health kit",           AUDIO_CRAFT_MEDKIT_MOLOTOV},
    {309.666656, 193.674072, "TLoU:weap_molotov",       "MOLOTOV COCKTAIL", "Crafts a molotov cocktail",     AUDIO_CRAFT_MEDKIT_MOLOTOV},
    {309.666656, 239.674072, "TLoU:weap_nailbomb",      "BOMB",             "Crafts a bomb",                 AUDIO_CRAFT_BOMB},
    {309.666656, 285.674072, "TLoU:weap_smkbmb",        "SMOKE BOMB",       "Crafts a smoke bomb",           AUDIO_CRAFT_SMKBMB}
};

// Backpacks data : SKILLS
new Iterator:SkillsToUpgrade<MAX_PLAYERS, MAX_SKILLS_ITEMS>;

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

// Backpacks data : COLLECTIBLES
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

//new Iterator:ArtifactsOwned<MAX_PLAYERS, MAX_ARTIFACTS>;

// Player backpack's current data.
enum e_PLAYER_BACKPACK_DATA
{
    e_iBackpackSelected_Craft,
    e_iBackpackSelected_Skill,

    e_iColl_Uid,
    e_iColl_ArtifTitle[32],
    e_iColl_ArtifInnerMsg[128],

    e_Coll_iArtifCount,
    e_iBackpackSelected_Coll,

    e_iBackpackSelected_Category,
    e_iTickCounter_backpackOpening,

    e_iBackpackKeysUpDown,
	e_iBackpackKeysLeftRight,
    e_iTickCounter_backpackKeys,
	Timer:e_iTimer_craftingOrUpgrading
};
new ePlayerBpInfo[MAX_PLAYERS][e_PLAYER_BACKPACK_DATA];