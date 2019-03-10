// TICK RATES.
#define     TICK_RATE_CBUG      (250)
#define     TICK_RATE_DAM_ANIM  (1800)

// TIMERS.
#define		TIMER_HIDE_HUD			(5000)
#define		TIMER_HIDECHAT_DELAY	(5000)
#define		TIMER_GETUP_DELAY		(800)
#define		TIMER_BUNNYHOP_DELAY	(1500)
#define		TIMER_LOOKAT_RATE		(300)

// MAX definitions.
#define		MAX_HUD_WEAPONS				(16)

// Other.
#define		PlayerName(%0)			 		ePlayerInfo[%0][e_sName]
#define		GetConnectedTime(%0) 	 		NetStats_GetConnectedTime(%0)
#define		GetPlayerFocusedObjectId(%0)	ePlayerInfo[%0][e_iLookingAtObjId]
#define		GetPlayerFocusedPlayerId(%0)	ePlayerInfo[%0][e_iLookingAtPlayerId]

#if !defined pb_percent
	#define 	pb_percent(%1,%2,%3,%4)		((%1 - 3.0) + ((((%1 + 3.0 + %2 - 0.8) - %1) / %3) * %4))
#endif

//#region Custom chat data variables.

	enum CP_ALIGNMENT
	{
		CP_ALIGN_UNKNOWN,
		CP_ALIGN_LEFT,
		CP_ALIGN_CENTERED,
		CP_ALIGN_RIGHT
	}

	enum CP_POSITION
	{
		CP_POS_TOP_LEFT,
		CP_POS_TOP_RIGHT,
		CP_POS_BOTTOM_LEFT
	}

	new PlayerCustomChatTextString[MAX_PLAYERS][MAX_CHAT_LINES][128];
	new Iterator:PlayerChatMessages<MAX_PLAYERS, MAX_CHAT_LINES>;

	stock const RestrictedTextCharactersList[] =
	{
		"~", 
		"|", 
		"%"
	};

//#endregion

//#region Player enum info & bit flags

	enum e_PLAYER_DATA
	{
		e_iGuid,
		e_sName[MAX_PLAYER_NAME],
		Timer:e_iTimer_HideHud,
		Timer:e_iTimer_Burning,

		PLAYER_STATUS:e_iCurrentStatus,

		e_iAnimIdx,
		Timer:e_iTimer_holdingKey,

		e_iTargetedPlayer,

		e_iLookingAtObjId,
		e_iLookingAtPlayerId,
		Text3D:e_iLookingAt_Label,
		Timer:e_iTimer_lookingAt,

		e_iTickCounter_crouchBug,
		e_iTickCounter_damageAnim,
		e_iTickCounter_holdingKey,
		Timer:e_iTimer_hideChat,
	}
	new ePlayerInfo[MAX_PLAYERS][e_PLAYER_DATA];

	// (<<= 1)
	// this is an optional operator which controls how each symbol is enumerated, <<= 1 is left shift by 1 bit, which will just rotate the bits to the left, 
	// so if the first element is 0001 then the next will be 0010, followed by 0100 etc
	enum e_PLAYER_FLAGS
	{
		e_bLogged,
		e_bDoingCrouchBug,
		e_bDoingBunnyHop,
		e_bAiming,
		e_bAnimPlaying,
		e_bCacheLoaded,
		e_bHoldingKey,
		e_bKeyPress,
		e_bIsCrafting,
		e_bIsUpgradingSkill,
		e_bIsSwitchingWeapons,
		e_bLookingAtTimerRunning,
		e_bIsHudBgFlashing
	}
	new BitArray:ePlayerFlag[e_PLAYER_FLAGS]<MAX_PLAYERS>;

//#endregion

//#region Player statuses

	enum PLAYER_STATUS
	{
		PLAYER_STATUS_NONE,
		PLAYER_STATUS_USING_BACKPACK,
		PLAYER_STATUS_USING_WEAPMENU,
		PLAYER_STATUS_BURNING,
		PLAYER_STATUS_STUNNED,
		PLAYER_STATUS_HEALING,
		PLAYER_STATUS_DEAD
	};

//#endregion

//#region Hud menu items (icons and their positions)

	enum e_HUDMENU_DATA
	{
		e_iHud_WeaponId,
		e_sHud_WeapIcon[24],
		Float:e_fHud_WeapIconTextSize_X,
		Float:e_fHud_WeapIconTextSize_Y,

		Float:e_fHud_WeapIconPos_X,
		Float:e_fHud_WeapIconPos_Y,

		Float:e_fHud_WeapAmmoPos_X,
		Float:e_fHud_WeapAmmoPos_Y
	};
	stock const eHudMenu_Items[MAX_HUD_WEAPONS][e_HUDMENU_DATA] =
	{
		{WEAPON_9MM, 	    	"TLoU:weap_9mm", 			 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_REVOLVER,  	 	"TLoU:weap_rev", 			 27.333326, 33.600013, 530.666748, 338.488983, 556.333557, 352.177368},
		{WEAPON_SHOTGUN,    	"TLoU:weap_shot_horiz", 	 50.333328, 50.607410, 507.333221, 324.385223, 559.333557, 352.177368},
		{WEAPON_RIFLE,	    	"TLoU:weap_huntrif_horiz", 	 48.666625, 52.266689, 512.666503, 332.266540, 556.333557, 357.155120},
		{WEAPON_ASSAULT,    	"TLoU:weap_assault_horiz", 	 53.666641, 66.785194, 506.666748, 316.503479, 557.000244, 346.784698},
		{WEAPON_BOW, 	    	"TLoU:weap_bow",			 36.666618, 53.096324, 525.999633, 328.118286, 557.000183, 355.080993},
		{WEAPON_NAIL_BOMB,  	"TLoU:weap_nailbomb", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_MEDKIT,			"TLoU:weap_medkit", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_BRICK, 	    	"TLoU:weap_brick", 		 	 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_BOTTLE,	    	"TLoU:weap_bottle", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_MOLOTOV,    	"TLoU:weap_molotov", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_SMOKE_BOMB, 	"TLoU:weap_smkbmb", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
		{WEAPON_PIPE, 		  	"TLoU:weap_pipe",	   	 	 36.666633, 44.800029, 527.999816, 331.436859, 0.0, 0.0},
		{WEAPON_SCISSOR_PIPE, 	"TLoU:weap_scsr_pipe",	 	 40.999946, 44.385219, 516.999755, 331.436798, 0.0, 0.0},
		{WEAPON_HATCHET,		"TLoU:weap_hatchet",		 33.999958, 48.533359, 528.333129, 326.044158, 0.0, 0.0},
		{WEAPON_MACHETE,		"TLoU:weap_machete",		 36.666633, 44.800029, 527.999816, 331.436859, 0.0, 0.0}
	};

//#endregion

stock const CombatTips[13][128] =
{
	{"Sprinting around runs the risk of being heard"},
	{"You're an easy target running directly at an enemy"},
	{"Human enemies change their behavior once they notice you~n~have a weapon"},
	{"Find Toolboxes to help you upgrade your weapons"},
	{"Punching a clicker is ineffective: try a melee weapon"},
	{"Learn essential survival techniques by finding Training~n~Manuals"},
	{"Supplements boost your survival skills"},
	{"Infected Runnes still have their sight. Be very cautious when~n~approaching them"},
	{"Bind a blade to your melee weapon to create a more brutal~n~attack"},
	{"Throw a brick or bottle at a Clicker to stun them"},
	{"Some infected have overgrown fungal players that act as~n~protective armour"},
	{"There are limited supplies. Make each bullet count."},
	{"Collect Parts to upgrade your weapons"}
};