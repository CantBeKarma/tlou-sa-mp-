// MAXIMAL AMOUNTS.
#define     MAX_PLAYER_SUPPS_AMOUNT     (530)
#define     MAX_SKILLS_ITEMS            (6)

// SKILL ITEM IDs.
#define     SKILL_ITEM_MAXHP            (0)
#define     SKILL_ITEM_LISTEN_DIST      (1)
#define     SKILL_ITEM_CRAFT_SPEED      (2)
#define     SKILL_ITEM_HEAL_SPEED       (3)
#define     SKILL_ITEM_WEAP_SWAY        (4)
#define     SKILL_ITEM_SHIV_MASTER      (5)

// Skills boosts (speed values in ms, weapon sway is amount of drunk effect value)
#define     SKILL_MAXHP_LVL0                    (100.0)
#define     SKILL_MAXHP_LVL1                    (120.0)
#define     SKILL_MAXHP_LVL2                    (140.0)

#define		SKILL_HEAL_SPEED_LVL0		        (500)
#define		SKILL_HEAL_SPEED_LVL1		        (300)
#define		SKILL_HEAL_SPEED_LVL2		        (100)

#define     CRAFT_SPEED_MELEE_SHIV_LVL0         (1650)
#define     CRAFT_SPEED_MELEE_SHIV_LVL1         (1250)
#define     CRAFT_SPEED_MELEE_SHIV_LVL2         (850)

#define     CRAFT_SPEED_MED_MOL_SMKBMB_LVL0     (1400)
#define     CRAFT_SPEED_MED_MOL_SMKBMB_LVL1     (1000)
#define     CRAFT_SPEED_MED_MOL_SMKBMB_LVL2     (600)

#define     CRAFT_SPEED_NAIL_BOMB_LVL0          (1500)
#define     CRAFT_SPEED_NAIL_BOMB_LVL1          (1100)
#define     CRAFT_SPEED_NAIL_BOMB_LVL2          (700)

#define		SKILL_WEAP_SWAY_LVL0	            (5000)
#define		SKILL_WEAP_SWAY_LVL1	            (2500)
#define		SKILL_WEAP_SWAY_LVL2	            (0)

#define     SKILL_SHIV_MASTER_LVL0              (1)
#define     SKILL_SHIV_MASTER_LVL1              (2)
#define     SKILL_SHIV_MASTER_LVL2              (3)

// Player's possible skills to upgrade data information.
new Iterator:SkillsToUpgrade<MAX_PLAYERS, MAX_SKILLS_ITEMS>;

// Backpacks data : SKILLS
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