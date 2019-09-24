// Used to calculate progress bar percentage length
#if !defined pb_percent
	#define 	pb_percent(%1,%2,%3,%4)		((%1 - 3.0) + ((((%1 + 3.0 + %2 - 0.8) - %1) / %3) * %4))
#endif

// GlobalTds (current count: 112 | limit: 2048)
enum e_GLOBAL_TD_DATA
{
	Text:e_Punish,
	
	Text:e_Aim[4],

	Text:e_MainMenu_Prompt,
	
	Text:e_Hud_Wrench,
	Text:e_Hud_Shiv,
	Text:e_Hud_Hp_Outline,
	Text:e_Hud_Hp_Underneath,

	Text:e_weapMenu_BgH,
	Text:e_weapMenu_BgV,

	Text:e_Bp_SelectBox[MAX_PLAYERS],

	Text:e_Bp_Ls_Bg,
	Text:e_Bp_Rs_Bg,
	Text:e_Bp_Rs_NavLeft,
	Text:e_Bp_Rs_NavRight,
	Text:e_Bp_Rs_CatLineTop,
	Text:e_Bp_Rs_CatLineBottom,
	Text:e_Bp_Rs_IngrStripe[6],
	Text:e_Bp_Rs_HowToCraft[2],
	Text:e_Bp_Rs_LineAboveIngr,
	Text:e_Bp_Rs_HowToClose[3],

	Text:e_Bp_Rs_Skills[6],
	Text:e_Bp_Rs_Skills_ProgBars[15],
	Text:e_Bp_Rs_HowToUpgrade[2],

	Text:e_Bp_Rs_Coll_MiddleLine,
	Text:e_Bp_Rs_Coll_HowToView[2],
	Text:e_Bp_Rs_Coll_ItemIcon[2],
	Text:e_Bp_Rs_Coll_ItemName[2],
	
	Text:e_Bp_Rs_Coll_PaperNote,
	Text:e_Bp_Rs_Coll_FireflyPend,
	Text:e_Bp_Rs_Coll_MessageText[MAX_PLAYERS]
};
new eTextDrawInfo[e_GLOBAL_TD_DATA];

// PlayerTds (current count: 251 | limit: 256)
enum e_PLAYER_TD_DATA
{
	PlayerText:e_BlackScreen,
	PlayerText:e_LoadingProgress,
	PlayerText:e_Logo,
	PlayerText:e_ChatLines[MAX_CHAT_LINES],
	PlayerText:e_DeathProcedure[2],

	PlayerText:e_weapMenu_howToSwap[2],
	PlayerText:e_weapMenu_swapWeapIcons[2],
	PlayerText:e_weapMenu_Icons[9],
	PlayerText:e_weapMenu_Amounts[9],
	PlayerText:e_weapMenu_wrench[4],

	PlayerText:e_Bp_Ls_Wrench[6],
	PlayerText:e_Bp_Ls_CraftItems[6],
	PlayerText:e_Bp_Ls_CraftItemsAmount[5],

	PlayerText:e_Bp_Rs_NavSelectBox,
	PlayerText:e_Bp_Rs_NavItem[3],
	PlayerText:e_Bp_Rs_CatName,
	PlayerText:e_Bp_Rs_CurrentItem[3],
	PlayerText:e_Bp_Rs_IngrIcon[19],
	PlayerText:e_Bp_Rs_IngrBox[19],
	PlayerText:e_Bp_Rs_IngrName[7],

	PlayerText:e_Bp_Rs_Skills_SuppsAmount,
	PlayerText:e_Bp_Rs_Skills_SuppsIcon,
	PlayerText:e_Bp_Rs_Skills_Names[6],
	PlayerText:e_Bp_Rs_Skills_SuppsIcons[6],
	PlayerText:e_Bp_Rs_Skills_SuppsRequire[6],
	PlayerText:e_Bp_Rs_Skills_Description,
	PlayerText:e_Bp_Rs_SkillProg_Status[15],

	PlayerText:e_Bp_Rs_Coll_SelectBox,
	PlayerText:e_Bp_Rs_Coll_Titles[9],

	PlayerText:e_Hud_Bg,
	PlayerText:e_Hud_Melee,
	PlayerText:e_Hud_Shiv_Amount,
	PlayerText:e_Hud_Weapon,
	PlayerText:e_Hud_Ammo,
	PlayerText:e_Hud_Bullet_9mm[14],
	PlayerText:e_Hud_Bullet_Revolver[6],
	PlayerText:e_Hud_Bullet_Shorty[6],
	PlayerText:e_Hud_Bullet_Shotgun[8],
	PlayerText:e_Hud_Bullet_Rifle[3],
	PlayerText:e_Hud_Bullet_Assault[30],
	PlayerText:e_Hud_Bars_Pipe[10],
	PlayerText:e_Hud_Bars_Machete[3],
	PlayerText:e_Hud_Bars_Hatchet[5],
	PlayerText:e_Hud_Bars_MeleeUpgraded[3],
	PlayerText:e_Hud_Hp_Status
};
new ePlayerTextDrawInfo[MAX_PLAYERS][e_PLAYER_TD_DATA];