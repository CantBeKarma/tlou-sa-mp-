// Weapon upgrade types.
#define     UPGRADE_TYPE_CLIPSIZE       (0)

// Maximal values.
#define     MAX_WEAPONS_UPGRADES_DATA   (23)
#define     MAX_WEAPONS_MELEE_DATA      (4)

// Weapon upgrades data.
enum e_WEAPON_UPGRADES_DATA
{
    e_iWeaponId,
    e_iWeapon_UpgradeLvl,
    e_iWeapon_UpgradeType,

    e_iWeapon_ClipSize,
    e_iWeapon_MaxAmmo,

    e_iWeapon_RequiredParts,
    e_iWeapon_RequiredToolsLvl
};
stock const eWeaponsUpgrades[MAX_WEAPONS_UPGRADES_DATA][e_WEAPON_UPGRADES_DATA] =
{
    {WEAPON_9MM, 0, UPGRADE_TYPE_CLIPSIZE,  6, 20,  0, 0},
    {WEAPON_9MM, 1, UPGRADE_TYPE_CLIPSIZE,  8, 22, 15, 0},
    {WEAPON_9MM, 2, UPGRADE_TYPE_CLIPSIZE, 10, 24, 20, 1},
    {WEAPON_9MM, 3, UPGRADE_TYPE_CLIPSIZE, 12, 26, 30, 2},
    {WEAPON_9MM, 4, UPGRADE_TYPE_CLIPSIZE, 14, 28, 40, 3},

    {WEAPON_SHOTGUN, 0, UPGRADE_TYPE_CLIPSIZE, 4, 14,  0, 0},
    {WEAPON_SHOTGUN, 1, UPGRADE_TYPE_CLIPSIZE, 5, 15, 15, 2},
    {WEAPON_SHOTGUN, 2, UPGRADE_TYPE_CLIPSIZE, 6, 16, 30, 3},
    {WEAPON_SHOTGUN, 3, UPGRADE_TYPE_CLIPSIZE, 7, 17, 40, 4},
    {WEAPON_SHOTGUN, 4, UPGRADE_TYPE_CLIPSIZE, 8, 18, 50, 5},

    {WEAPON_RIFLE, 0, UPGRADE_TYPE_CLIPSIZE, 1, 6,  0, 0},
    {WEAPON_RIFLE, 1, UPGRADE_TYPE_CLIPSIZE, 2, 7, 20, 1},
    {WEAPON_RIFLE, 2, UPGRADE_TYPE_CLIPSIZE, 3, 9, 40, 4},

    // Just MAX AMMO is used for those. Player can't upgrade clipsize of the weapons below.
    {WEAPON_REVOLVER,   0, UPGRADE_TYPE_CLIPSIZE,  6, 18, 0, 0},
    {WEAPON_ASSAULT,    0, UPGRADE_TYPE_CLIPSIZE, 30, 90, 0, 0},
    {WEAPON_BOW,        0, UPGRADE_TYPE_CLIPSIZE,  1,  9, 0, 0},
    {WEAPON_SHIV,       0, UPGRADE_TYPE_CLIPSIZE,  1,  3, 0, 0},
    {WEAPON_NAIL_BOMB,  0, UPGRADE_TYPE_CLIPSIZE,  1,  3, 0, 0},
    {WEAPON_MEDKIT,     0, UPGRADE_TYPE_CLIPSIZE,  1,  3, 0, 0},
    {WEAPON_MOLOTOV,    0, UPGRADE_TYPE_CLIPSIZE,  1,  3, 0, 0},
    {WEAPON_SMOKE_BOMB, 0, UPGRADE_TYPE_CLIPSIZE,  1,  3, 0, 0},
    {WEAPON_BRICK,      0, UPGRADE_TYPE_CLIPSIZE,  1,  1, 0, 0},
    {WEAPON_BOTTLE,     0, UPGRADE_TYPE_CLIPSIZE,  1,  1, 0, 0}
};

enum e_WEAPON_MELEE_DATA
{
    e_iWeaponId,
    e_iWeaponDurability,
    e_iWeaponExtDurability
};
stock const eMeleeWeaponsData[MAX_WEAPONS_MELEE_DATA][e_WEAPON_MELEE_DATA] =
{
    {WEAPON_PIPE,           9, 0},
    {WEAPON_SCISSOR_PIPE,   9, 1},
    {WEAPON_HATCHET,        5, 3},
    {WEAPON_MACHETE,        3, 3}
};