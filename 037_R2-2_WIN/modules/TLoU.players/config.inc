// TICK RATES.
#define     TICK_RATE_CBUG      (250)

// TIMERS.
#define		TIMER_GETUP_DELAY		(800)
#define		TIMER_BUNNYHOP_DELAY	(1500)
#define		TIMER_LOOKAT_RATE		(300)
#define		TIMER_HOLDKEY_RATE		(100)

// Other.
#define		PlayerName(%0)			 		ePlayerInfo[%0][e_sName]
#define		GetConnectedTime(%0) 	 		NetStats_GetConnectedTime(%0)
#define		GetPlayerFocusedPlayerId(%0)	ePlayerInfo[%0][e_iLookingAtPlayerId]

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
		Text3D:e_iPlayerLabel,
		e_iLookingAtPlayerId,
		Text3D:e_iLookingAt_Label,
		Timer:e_iTimer_lookingAt,

		e_iTickCounter_crouchBug,
		e_iTickCounter_damageAnim,
		e_iTickCounter_holdingKey,
		Timer:e_iTimer_hideChat,

		e_iCurrentEquippedItemId,
		e_iLastEquippedItemId
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
		e_bCacheLoaded,
		e_bHoldingKey,
		e_bKeyPress,
		e_bIsCrafting,
		e_bIsUpgradingSkill,
		e_bIsCheckingColls,
		e_bIsViewingColl,
		e_bIsSwitchingWeapons,
		e_bLookingAtTimerRunning,
		e_bIsHudBgFlashing,
		e_bShootingCustomWeapon,
		e_bShotNailbomb
	}
	new BitArray:ePlayerFlag[e_PLAYER_FLAGS]<MAX_PLAYERS>;

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