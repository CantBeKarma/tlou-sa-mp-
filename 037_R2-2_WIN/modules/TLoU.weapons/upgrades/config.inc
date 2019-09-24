// Weapon upgrade types.
#define     UPGRADE_TYPE_CLIPSIZE       (0)

// Maximal values.
#define     MAX_MELEE_ITEMS         (4)
#define     MAX_UPGRADE_ITEMS       (18)

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