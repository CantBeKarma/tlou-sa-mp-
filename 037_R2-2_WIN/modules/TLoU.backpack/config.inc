// Other defs.
#define     TICK_RATE_BP_OPENING            (1250)
#define     TICK_RATE_BP_KEYS_COOLDOWN      (450)
#define     TICK_RATE_BP_HOLD_TO_OPEN       (400)
#define     TIMER_USE_BACKPACK		        (50)

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