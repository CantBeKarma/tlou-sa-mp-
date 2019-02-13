LoadGlobalTextDraws()
{
	eTextDrawInfo[e_Punish] = TextDrawCreate(35.000000, 280.000000, "_");
	TextDrawLetterSize		(eTextDrawInfo[e_Punish], 0.300000, 1.399999);
	TextDrawAlignment		(eTextDrawInfo[e_Punish], 1);
	TextDrawColor			(eTextDrawInfo[e_Punish], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Punish], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Punish], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Punish], 51);
	TextDrawFont			(eTextDrawInfo[e_Punish], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Punish], 1);

	eTextDrawInfo[e_MainMenu_Prompt] = TextDrawCreate(70.999992, 301.155670, "Press ~k~~VEHICLE_ENTER_EXIT~ key");
	TextDrawLetterSize		(eTextDrawInfo[e_MainMenu_Prompt], 0.348664, 1.919407);
	TextDrawAlignment		(eTextDrawInfo[e_MainMenu_Prompt], 1);
	TextDrawColor			(eTextDrawInfo[e_MainMenu_Prompt], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_MainMenu_Prompt], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_MainMenu_Prompt], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_MainMenu_Prompt], 51);
	TextDrawFont			(eTextDrawInfo[e_MainMenu_Prompt], 2);
	TextDrawSetProportional	(eTextDrawInfo[e_MainMenu_Prompt], 1);

	GlobalTextDraw_Crosshair();
	GlobalTextDraw_HUD();
	GlobalTextDraw_BackpackMenu();
	GlobalTextDraw_Backpack_Craft();
	GlobalTextDraw_WeaponMenu();

	// Left/Right side-bar : Selection box for the backpack.
	for(new playerid = 0; playerid != MAX_PLAYERS; playerid++)
	{
		eTextDrawInfo[e_Bp_SelectBox][playerid] = TextDrawCreate(309.666656, 55.674072, "TLoU:selectbox");
		TextDrawLetterSize	(eTextDrawInfo[e_Bp_SelectBox][playerid], 0.000000, 0.000000);
		TextDrawTextSize	(eTextDrawInfo[e_Bp_SelectBox][playerid], 45.0, 56.0);
		TextDrawAlignment	(eTextDrawInfo[e_Bp_SelectBox][playerid], 1);
		TextDrawColor		(eTextDrawInfo[e_Bp_SelectBox][playerid], -1);
		TextDrawSetShadow	(eTextDrawInfo[e_Bp_SelectBox][playerid], 0);
		TextDrawSetOutline	(eTextDrawInfo[e_Bp_SelectBox][playerid], 0);
		TextDrawFont		(eTextDrawInfo[e_Bp_SelectBox][playerid], 4);
	}

	GlobalTextDraw_Backpack_Skills();
	GlobalTextDraw_Backpack_Coll();
}

GlobalTextDraw_Backpack_Coll()
{
	// Right side-bar : Middle line in the collectibles menu
	eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine] = TextDrawCreate(482.333435, 4.562963, "I");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 0.118666, 48.610908);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_Coll_MiddleLine], 1);

//#region Right side-bar : "How to view" collectible item
	eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0] = TextDrawCreate(511.333343, 97.236961, "TLoU:mouseL");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 12.000020, 18.666685);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][0], 4);

	eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1] = TextDrawCreate(528.333068, 95.333396, "TO VIEW");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 0.250333, 1.960888);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 2);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_Coll_HowToView][1], 1);
//#endregion

//#region Right side-bar : List of items (collectibles/firefly pendants)
	new 
		Float:posY_collectible_Icon, Float:posY_collectible_Name,
		collectibleIcon[24], collectibleName[24];

	for(new i = 0; i != 2; i++)
	{
		switch(i)
		{
			case 0:posY_collectible_Icon = 131.7, posY_collectible_Name = 130.7, strcpy(collectibleIcon, "TLoU:collectible"), strcpy(collectibleName, "ARTIFACTS");
			case 1:posY_collectible_Icon = 161.7, posY_collectible_Name = 160.7, strcpy(collectibleIcon, "TLoU:firefly"), strcpy(collectibleName, "FIREFLY PENDANTS");
		}
		eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i] = TextDrawCreate(382.000000, posY_collectible_Icon, collectibleIcon);
		TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 0.000000, 0.000000);
		TextDrawTextSize		(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 14.333333, 13.688879);
		TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 1);
		TextDrawColor			(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], -1);
		TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 0);
		TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 0);
		TextDrawFont			(eTextDrawInfo[e_Bp_Rs_Coll_ItemIcon][i], 4);

		eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i] = TextDrawCreate(400.299987, posY_collectible_Name, collectibleName);
		TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 0.187665, 1.363554);
		TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 1);
		TextDrawColor			(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], -1);
		TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 0);
		TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 1);
		TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 51);
		TextDrawFont			(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 2);
		TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_Coll_ItemName][i], 1);
	}
//#endregion
}

GlobalTextDraw_BackpackMenu()
{
	// Left side-bar
	eTextDrawInfo[e_Bp_Ls_Bg] = TextDrawCreate(298.333099, -0.829629, "TLoU:menu_backpackL");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Ls_Bg], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Ls_Bg], 55.000003, 451.733398);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Ls_Bg], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Ls_Bg], COLOR_BG_BLACK_TRANS);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Ls_Bg], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Ls_Bg], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Ls_Bg], 4);

	// Right side-bar
	eTextDrawInfo[e_Bp_Rs_Bg] = TextDrawCreate(152.333358, -0.414824, "TLoU:menu_backpackR");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_Bg], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_Bg], 455.000244, 450.074401);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_Bg], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Rs_Bg], COLOR_BG_BLACK_TRANS);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_Bg], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_Bg], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Rs_Bg], 4);

//#region Right side-bar : Navigate in backpack
	eTextDrawInfo[e_Bp_Rs_NavLeft] = TextDrawCreate(379.000030, 42.311119, "LEFT");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_NavLeft], 0.359665, 1.363554);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_NavLeft], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_NavLeft], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_NavLeft], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_NavLeft], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_NavLeft], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_NavLeft], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_NavLeft], 1);

	eTextDrawInfo[e_Bp_Rs_NavRight] = TextDrawCreate(533.666625, 42.481498, "RIGHT");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_NavRight], 0.359665, 1.363554);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_NavRight], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_NavRight], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_NavRight], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_NavRight], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_NavRight], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_NavRight], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_NavRight], 1);
//#endregion

//#region Right side-bar : backpack category underscore TOP && BOTTOM
	eTextDrawInfo[e_Bp_Rs_CatLineTop] = TextDrawCreate(377.333343, 64.296295, "-");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_CatLineTop], 13.843002, 0.380443);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_CatLineTop], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_CatLineTop], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_CatLineTop], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_CatLineTop], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_CatLineTop], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_CatLineTop], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_CatLineTop], 1);

	eTextDrawInfo[e_Bp_Rs_CatLineBottom] = TextDrawCreate(377.333343, 85.207427, "-");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 13.843002, 0.380443);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_CatLineBottom], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_CatLineBottom], 1);
//#endregion

//#region Right side-bar : Message on how to close the backpack.
	eTextDrawInfo[e_Bp_Rs_HowToClose][0] = TextDrawCreate(377.333343, 383.459350, "-");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 13.843002, 0.380443);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_HowToClose][0], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_HowToClose][0], 1);

	eTextDrawInfo[e_Bp_Rs_HowToClose][1] = TextDrawCreate(434.000000, 388.681396, "TLoU:mouseR");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 12.000020, 18.666685);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Rs_HowToClose][1], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Rs_HowToClose][1], 4);

	eTextDrawInfo[e_Bp_Rs_HowToClose][2] = TextDrawCreate(452.666503, 386.363006, "TO CLOSE");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 0.283333, 2.189037);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_HowToClose][2], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 2);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_HowToClose][2], 1);
//#endregion
}

GlobalTextDraw_Backpack_Craft()
{
//#region Right side-bar : How to craft
	eTextDrawInfo[e_Bp_Rs_HowToCraft][0] = TextDrawCreate(439.000000, 123.614791, "HOLD     TO CREATE");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 0.283333, 2.189037);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 2);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_HowToCraft][0], 1);

	eTextDrawInfo[e_Bp_Rs_HowToCraft][1] = TextDrawCreate(475.000061, 126.688743, "TLoU:mouseL");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 12.000020, 18.666685);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Rs_HowToCraft][1], 4);
//#endregion

	// Right side-bar : Line above ingredients / Line below `How to craft`
	eTextDrawInfo[e_Bp_Rs_LineAboveIngr] = TextDrawCreate(378.000122, 146.844482, "-");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 13.843002, 0.359703);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 1);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_LineAboveIngr], 1);

//#region Right side-bar : Ingredient stripes
	for(new i = 0, Float:defY_ingr_stripe = 174.214828; i != 6; i++, defY_ingr_stripe += 31.7)
	{
		eTextDrawInfo[e_Bp_Rs_IngrStripe][i] = TextDrawCreate(398.333435, defY_ingr_stripe, "-");
		TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 4.823338, 0.367998);
		TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 1);
		TextDrawColor			(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], -1);
		TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 0);
		TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 1);
		TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 51);
		TextDrawFont			(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 1);
		TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_IngrStripe][i], 1);
	}
//#endregion
}

GlobalTextDraw_Backpack_Skills()
{
//#region Right side-bar : skills icons
	new 
		Float:posY_skill,
		symbol_skill[24];

	for(new i = 0; i != 6; i++)
	{
		switch(i)
		{
			case 0:posY_skill = 96.651763, strcpy(symbol_skill, "TLoU:skill_maxhp");
			case 1:posY_skill = 134.155487, strcpy(symbol_skill, "TLoU:skill_listen");
			case 2:posY_skill = 171.659210, strcpy(symbol_skill, "TLoU:skill_craft");
			case 3:posY_skill = 209.162933, strcpy(symbol_skill, "TLoU:skill_heal");
			case 4:posY_skill = 246.666656, strcpy(symbol_skill, "TLoU:skill_sway");
			case 5:posY_skill = 284.170379, strcpy(symbol_skill, "TLoU:skill_shiv");
		}
		eTextDrawInfo[e_Bp_Rs_Skills][i] = TextDrawCreate(380.000061, posY_skill, symbol_skill);
		TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_Skills][i], 0.000000, 0.000000);
		TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_Skills][i], 21.000005, 24.474058);
		TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_Skills][i], 1);
		TextDrawColor		(eTextDrawInfo[e_Bp_Rs_Skills][i], -1);
		TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_Skills][i], 0);
		TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_Skills][i], 0);
		TextDrawFont		(eTextDrawInfo[e_Bp_Rs_Skills][i], 4);
	}
//#endregion

//#region Right side-bar : skills bars
	new 
		Float:posX_skillBar,
		Float:posY_skillsBar;

	for(new i = 0; i != 15; i++)
	{
		switch(i)
		{
			// SKILL : Max HP
			case 0:posX_skillBar = 405.333312, posY_skillsBar = 113.085037;
			case 1:posX_skillBar = 417.333312, posY_skillsBar = 113.085037;

			// SKILL : Listen
			case 2:posX_skillBar = 405.333312, posY_skillsBar = 150.588714;
			case 3:posX_skillBar = 417.333312, posY_skillsBar = 150.588714;
			case 4:posX_skillBar = 429.333312, posY_skillsBar = 150.588714;

			// SKILL : Craft
			case 5:posX_skillBar = 405.333312, posY_skillsBar = 188.092437;
			case 6:posX_skillBar = 417.333312, posY_skillsBar = 188.092437;
			case 7:posX_skillBar = 429.333312, posY_skillsBar = 188.092437;

			// SKILL : Heal
			case 8: posX_skillBar = 405.333312, posY_skillsBar = 225.596115;
			case 9: posX_skillBar = 417.333312, posY_skillsBar = 225.596115;
			case 10:posX_skillBar = 429.333312, posY_skillsBar = 225.596115;

			// SKILL : Weapon Sway
			case 11:posX_skillBar = 405.333312, posY_skillsBar = 263.099792;
			case 12:posX_skillBar = 417.333312, posY_skillsBar = 263.099792;

			// SKILL : Shiv
			case 13:posX_skillBar = 405.333312, posY_skillsBar = 300.603454;
			case 14:posX_skillBar = 417.333312, posY_skillsBar = 300.603454;
		}
		eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i] = TextDrawCreate(posX_skillBar, posY_skillsBar, "TLoU:progress_bar");
		TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 0.000000, 0.000000);
		TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 9.000012, 4.148138);
		TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 1);
		TextDrawColor		(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], -1);
		TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 0);
		TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 0);
		TextDrawFont		(eTextDrawInfo[e_Bp_Rs_Skills_ProgBars][i], 4);
	}
//#endregion

//#region Right side-bar : How to upgrade
	eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0] = TextDrawCreate(452.000000, 363.548065, "TLoU:mouseL");
	TextDrawLetterSize	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 12.000020, 18.666685);
	TextDrawAlignment	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 1);
	TextDrawColor		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 0);
	TextDrawFont		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][0], 4);

	eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1] = TextDrawCreate(416.333343, 360.474090, "HOLD     TO UPGRADE");
	TextDrawLetterSize		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 0.283333, 2.189037);
	TextDrawAlignment		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 1);
	TextDrawColor			(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], -1);
	TextDrawSetShadow		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 1);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 51);
	TextDrawFont			(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 2);
	TextDrawSetProportional	(eTextDrawInfo[e_Bp_Rs_HowToUpgrade][1], 1);
//#endregion
}

GlobalTextDraw_Crosshair()
{
	eTextDrawInfo[e_Aim][0] = TextDrawCreate(331.999938, 172.148178, "TLoU:aim_standard");
	TextDrawLetterSize	(eTextDrawInfo[e_Aim][0], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Aim][0], 14.000000, 14.518508);
	TextDrawAlignment	(eTextDrawInfo[e_Aim][0], 1);
	TextDrawColor		(eTextDrawInfo[e_Aim][0], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Aim][0], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Aim][0], 0);
	TextDrawFont		(eTextDrawInfo[e_Aim][0], 4);

	eTextDrawInfo[e_Aim][1] = TextDrawCreate(317.666442, 154.896331, "TLoU:aim_shotgun");
	TextDrawLetterSize	(eTextDrawInfo[e_Aim][1], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Aim][1], 42.999988, 50.192565);
	TextDrawAlignment	(eTextDrawInfo[e_Aim][1], 1);
	TextDrawColor		(eTextDrawInfo[e_Aim][1], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Aim][1], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Aim][1], 0);
	TextDrawFont		(eTextDrawInfo[e_Aim][1], 4);

	eTextDrawInfo[e_Aim][2] = TextDrawCreate(326.999847, 167.755615, "TLoU:aim_head");
	TextDrawLetterSize	(eTextDrawInfo[e_Aim][2], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Aim][2], 24.000000, 24.474058);
	TextDrawAlignment	(eTextDrawInfo[e_Aim][2], 1);
	TextDrawColor		(eTextDrawInfo[e_Aim][2], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Aim][2], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Aim][2], 0);
	TextDrawFont		(eTextDrawInfo[e_Aim][2], 4);

	eTextDrawInfo[e_Aim][3] = TextDrawCreate(329.666503, 169.829696, "TLoU:aim_hit");
	TextDrawLetterSize	(eTextDrawInfo[e_Aim][3], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Aim][3], 19.333326, 19.333326);
	TextDrawAlignment	(eTextDrawInfo[e_Aim][3], 1);
	TextDrawColor		(eTextDrawInfo[e_Aim][3], -1);
	TextDrawSetShadow	(eTextDrawInfo[e_Aim][3], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Aim][3], 0);
	TextDrawFont		(eTextDrawInfo[e_Aim][3], 4);
}

GlobalTextDraw_HUD()
{
	eTextDrawInfo[e_Hud_Wrench] = TextDrawCreate(566.000183, 363.377868, "TLoU:wrench");
	TextDrawLetterSize	(eTextDrawInfo[e_Hud_Wrench], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Hud_Wrench], 19.333372, 16.592601);
	TextDrawAlignment	(eTextDrawInfo[e_Hud_Wrench], 1);
	TextDrawColor		(eTextDrawInfo[e_Hud_Wrench], COLOR_GRAY);
	TextDrawSetShadow	(eTextDrawInfo[e_Hud_Wrench], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Hud_Wrench], 0);
	TextDrawFont		(eTextDrawInfo[e_Hud_Wrench], 4);

	eTextDrawInfo[e_Hud_Shiv] = TextDrawCreate(557.333435, 314.014678, "TLoU:weap_shiv");
	TextDrawLetterSize	(eTextDrawInfo[e_Hud_Shiv], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_Hud_Shiv], 19.999977, 24.474082);
	TextDrawAlignment	(eTextDrawInfo[e_Hud_Shiv], 1);
	TextDrawColor		(eTextDrawInfo[e_Hud_Shiv], COLOR_GRAY);
	TextDrawSetShadow	(eTextDrawInfo[e_Hud_Shiv], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Hud_Shiv], 0);
	TextDrawFont		(eTextDrawInfo[e_Hud_Shiv], 4);

	eTextDrawInfo[e_Hud_Hp_Outline] = TextDrawCreate(567.332275, 376.548461, "hpbar_outline");
	TextDrawLetterSize	(eTextDrawInfo[e_Hud_Hp_Outline], 0.000000, 0.450405);
	TextDrawTextSize	(eTextDrawInfo[e_Hud_Hp_Outline], 524.665954, 0.000000);
	TextDrawAlignment	(eTextDrawInfo[e_Hud_Hp_Outline], 1);
	TextDrawColor		(eTextDrawInfo[e_Hud_Hp_Outline], COLOR_BLACK);
	TextDrawUseBox		(eTextDrawInfo[e_Hud_Hp_Outline], true);
	TextDrawBoxColor	(eTextDrawInfo[e_Hud_Hp_Outline], 255);
	TextDrawSetShadow	(eTextDrawInfo[e_Hud_Hp_Outline], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_Hud_Hp_Outline], 0);
	TextDrawFont		(eTextDrawInfo[e_Hud_Hp_Outline], 0);

	eTextDrawInfo[e_Hud_Hp_Underneath] = TextDrawCreate(565.999084, 377.792938, "hpbar_underneath");
	TextDrawLetterSize		(eTextDrawInfo[e_Hud_Hp_Underneath], 0.000000, 0.151231);
	TextDrawTextSize		(eTextDrawInfo[e_Hud_Hp_Underneath], 526.000122, 0.000000);
	TextDrawAlignment		(eTextDrawInfo[e_Hud_Hp_Underneath], 1);
	TextDrawColor			(eTextDrawInfo[e_Hud_Hp_Underneath], 0);
	TextDrawUseBox			(eTextDrawInfo[e_Hud_Hp_Underneath], true);
	TextDrawBoxColor		(eTextDrawInfo[e_Hud_Hp_Underneath], 976696575);
	TextDrawSetShadow		(eTextDrawInfo[e_Hud_Hp_Underneath], 0);
	TextDrawSetOutline		(eTextDrawInfo[e_Hud_Hp_Underneath], 0);
	TextDrawBackgroundColor	(eTextDrawInfo[e_Hud_Hp_Underneath], 1510739711);
	TextDrawFont			(eTextDrawInfo[e_Hud_Hp_Underneath], 0);
}

GlobalTextDraw_WeaponMenu()
{
//#region Weapon Menu : Background
	eTextDrawInfo[e_weapMenu_BgH] = TextDrawCreate(331.999908, 228.563064, "TLoU:menu_weapH");
	TextDrawLetterSize	(eTextDrawInfo[e_weapMenu_BgH], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_weapMenu_BgH], 231.666793, 49.363014);
	TextDrawAlignment	(eTextDrawInfo[e_weapMenu_BgH], 1);
	TextDrawColor		(eTextDrawInfo[e_weapMenu_BgH], COLOR_BG_BLACK_TRANS);
	TextDrawSetShadow	(eTextDrawInfo[e_weapMenu_BgH], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_weapMenu_BgH], 0);
	TextDrawFont		(eTextDrawInfo[e_weapMenu_BgH], 4);

	eTextDrawInfo[e_weapMenu_BgV] = TextDrawCreate(427.333374, 96.822250, "TLoU:menu_weapV");
	TextDrawLetterSize	(eTextDrawInfo[e_weapMenu_BgV], 0.000000, 0.000000);
	TextDrawTextSize	(eTextDrawInfo[e_weapMenu_BgV], 44.000114, 313.600189);
	TextDrawAlignment	(eTextDrawInfo[e_weapMenu_BgV], 1);
	TextDrawColor		(eTextDrawInfo[e_weapMenu_BgV], COLOR_BG_BLACK_TRANS);
	TextDrawSetShadow	(eTextDrawInfo[e_weapMenu_BgV], 0);
	TextDrawSetOutline	(eTextDrawInfo[e_weapMenu_BgV], 0);
	TextDrawFont		(eTextDrawInfo[e_weapMenu_BgV], 4);
//#endregion
}