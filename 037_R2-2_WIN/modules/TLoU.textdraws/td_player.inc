CreatePlayerIntroTds(playerid)
{
	ePlayerTextDrawInfo[playerid][e_BlackScreen] = CreatePlayerTextDraw(playerid, 643.499877, -4.138914, "usebox");
	PlayerTextDrawLetterSize	(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 0.339333, 56.466365);
	PlayerTextDrawTextSize		(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], -2.666674, 1.000000);
	PlayerTextDrawAlignment		(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 1);
	PlayerTextDrawColor			(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 0);
	PlayerTextDrawUseBox		(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], true);
	PlayerTextDrawBoxColor		(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 255);
	PlayerTextDrawSetShadow		(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 0);
	PlayerTextDrawSetOutline	(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 0);
	PlayerTextDrawFont			(playerid, 	ePlayerTextDrawInfo[playerid][e_BlackScreen], 0);

	ePlayerTextDrawInfo[playerid][e_Logo] = CreatePlayerTextDraw(playerid, 248.000000, 44.385162, "TLoU:logo");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Logo], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], 149.000183, 332.681610);
	PlayerTextDrawAlignment		(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], 1);
	PlayerTextDrawColor			(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], -1);
	PlayerTextDrawSetShadow		(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], 0);
	PlayerTextDrawSetOutline	(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], 0);
	PlayerTextDrawFont			(playerid, 	ePlayerTextDrawInfo[playerid][e_Logo], 4);

	ePlayerTextDrawInfo[playerid][e_LoadingProgress] = CreatePlayerTextDraw(playerid, 633.000122, 427.259307, "Loading 100%");
	PlayerTextDrawLetterSize	 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 0.260666, 1.205925);
	PlayerTextDrawAlignment		 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 3);
	PlayerTextDrawColor			 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], -1);
	PlayerTextDrawSetShadow		 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 0);
	PlayerTextDrawSetOutline	 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 1);
	PlayerTextDrawBackgroundColor(playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 51);
	PlayerTextDrawFont			 (playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 1);
	PlayerTextDrawSetProportional(playerid, ePlayerTextDrawInfo[playerid][e_LoadingProgress], 1);
}

LoadPlayerTextDraws(playerid)
{
	Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "Loading player textdraws for: %s [%d]...", ePlayerInfo[playerid][e_sName], playerid);

	PlayerTextDraw_Chat(playerid);
	PlayerTextDraw_HUD(playerid);
	PlayerTextDraw_BackpackMenu(playerid);
	PlayerTextDraw_WeaponMenu(playerid);
	PlayerTextDraw_DeathProcedure(playerid);
}

PlayerTextDraw_DeathProcedure(playerid)
{
	ePlayerTextDrawInfo[playerid][e_DeathProcedure][0] = CreatePlayerTextDraw(playerid, 21.333421, 299.081542, "Some infected have overgrown fungal plates that act as~n~protective armor");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 0.320999, 1.599997);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 1);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][0], 1);

	ePlayerTextDrawInfo[playerid][e_DeathProcedure][1] = CreatePlayerTextDraw(playerid, 20.666746, 359.815063, "Press ~k~~PED_SPRINT~ to continue");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 0.188332, 0.932145);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 1);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_DeathProcedure][1], 1);
}

PlayerTextDraw_Chat(playerid)
{
	// 2.5 TOP + 10.0
	// 430 BOTTOM - 10.0
	new Float:chat_posY = 430.0;
	for(new i = 0; i != MAX_CHAT_LINES; i++)
	{
		PlayerCustomChatTextString[playerid][i][0] = EOS;

		ePlayerTextDrawInfo[playerid][e_ChatLines][i] = CreatePlayerTextDraw(playerid, 15.0, chat_posY, " ");
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 0.2, 1.0);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 1);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], -1);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 255);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 1);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_ChatLines][i], 1);
		
		chat_posY -= 10.0;
	}
	Iter_Clear(PlayerChatMessages<playerid>);
}

PlayerTextDraw_HUD(playerid)
{
//#region HUD
	ePlayerTextDrawInfo[playerid][e_Hud_Bg] = CreatePlayerTextDraw(playerid, 522.666931, 325.629455, "TLoU:hud_bg");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 53.666618, 64.711120);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], COLOR_BLACK_TRANS);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bg], 4);

	ePlayerTextDrawInfo[playerid][e_Hud_Melee] = CreatePlayerTextDraw(playerid, 549.000000, 381.629455, "TLoU:weap_pipe");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 25.333309, 26.962972);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], COLOR_GRAY);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Melee], 4);

	ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount] = CreatePlayerTextDraw(playerid, 578.666992, 324.554992, "NUMBER_OF_SHIVS");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 0.189658, 1.525328);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 2);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], COLOR_GRAY);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 2);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Shiv_Amount], 1);

	ePlayerTextDrawInfo[playerid][e_Hud_Weapon] = CreatePlayerTextDraw(playerid, 507.333221, 324.385223, "TLoU:weap_shot_horiz");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 0.000000, 0.000000);
	PlayerTextDrawTextSize			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 50.333328, 50.607410);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], COLOR_GRAY);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 0);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Weapon], 4);

	ePlayerTextDrawInfo[playerid][e_Hud_Ammo] = CreatePlayerTextDraw(playerid, 556.333557, 352.177368, "NUMBER_OF_AMMO");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 0.189658, 1.525328);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 2);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], COLOR_GRAY);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 2);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Ammo], 1);
//#endregion

//#region HUD : bullets -> 9mm pistol
	//for(new i = 0, Float:defX_bull_9mm = 536.667297; i != 14; i++, defX_bull_9mm += 2)
	for(new i = 13, Float:defX_bull_9mm = 536.667297; i >= 0; i--, defX_bull_9mm += 2)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i] = CreatePlayerTextDraw(playerid, defX_bull_9mm, 364.377899, "TLoU:bull_9mm");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 1.666707, 8.296298);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_9mm][i], 4);
	}
//#endregion

//#region HUD : bullets -> revolver
	//for(new i = 0, Float:defX_bull_rev = 550.0; i != 6; i++, defX_bull_rev += 2)
	for(new i = 5, Float:defX_bull_rev = 550.0; i >= 0; i--, defX_bull_rev += 2)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i] = CreatePlayerTextDraw(playerid, defX_bull_rev, 364.548156, "TLoU:bull_rev");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 1.666697, 9.540731);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Revolver][i], 4);
	}
//#endregion

//#region HUD : bullets -> shorty
	for(new i = 5, Float:defX_bull_shotgun = 530.666320; i >= 0; i--, defX_bull_shotgun += 4)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i] = CreatePlayerTextDraw(playerid, defX_bull_shotgun, 355.436950, "TLoU:bull_shotgun");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 2.666692, 15.348139);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shorty][i], 4);
	}
//#endregion

//#region HUD : bullets -> shotgun
	//for(new i = 0, Float:defX_bull_shotgun = 522.667236; i != 8; i++, defX_bull_shotgun += 4)
	for(new i = 7, Float:defX_bull_shotgun = 522.667236; i >= 0; i--, defX_bull_shotgun += 4)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i] = CreatePlayerTextDraw(playerid, defX_bull_shotgun, 352.763122, "TLoU:bull_shotgun");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 2.666707, 14.518527);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Shotgun][i], 4);
	}
//#endregion

//#region HUD : bullets -> hunting rifle
	//for(new i = 0, Float:defX_bull_rifle = 542.666442; i != 3; i++, defX_bull_rifle += 3)
	for(new i = 2, Float:defX_bull_rifle = 542.666442; i >= 0; i--, defX_bull_rifle += 3)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i] = CreatePlayerTextDraw(playerid, defX_bull_rifle, 360.0, "TLoU:bull_huntrif");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 2.666692, 10.785178);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Rifle][i], 4);
	}
//#endregion

//#region HUD : bullets -> assault rifle
	for(new i = 29, Float:defX_bull_assault = 521.328674; i >= 0; i--, defX_bull_assault += 1.333497)
	//for(new i = 0, Float:defX_bull_assault = 521.328674; i != 30; i++, defX_bull_assault += 1.333497)
	{
		ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i] = CreatePlayerTextDraw(playerid, defX_bull_assault, 362.385101, "TLoU:bull_assault");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 1.333361, 7.466660);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bullet_Assault][i], 4);
	}
//#endregion

//#region HUD : bars -> (scissor) pipe
	new 
		Float:meleeBar_posX,
		Float:meleeBar_posY,
		Float:meleeBar_textSizeX;

	for(new i = 0; i != 10; i++)
	{
		switch(i)
		{
			case 0:meleeBar_posX = 533.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 528.0;
			case 1:meleeBar_posX = 537.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 532.0;
			case 2:meleeBar_posX = 541.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 536.0;
			case 3:meleeBar_posX = 545.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 540.0;
			case 4:meleeBar_posX = 549.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 544.0;
			case 5:meleeBar_posX = 553.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 548.0;
			case 6:meleeBar_posX = 557.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 552.0;
			case 7:meleeBar_posX = 561.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 556.0;
			case 8:meleeBar_posX = 565.3, meleeBar_posY = 366.951751, meleeBar_textSizeX = 560.0;
			case 9:meleeBar_posX = 556.9, meleeBar_posY = 371.414825, meleeBar_textSizeX = 546.6;
		}

		ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i] = CreatePlayerTextDraw(playerid, meleeBar_posX, meleeBar_posY, "usebox");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 0.000000, -0.152878);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], meleeBar_textSizeX, 0.000000);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 0);
		PlayerTextDrawUseBox	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], true);
		PlayerTextDrawBoxColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Pipe][i], 0);
	}
//#endregion

//#region HUD : bars -> machete
	new 
		Float:macheteBar_posX,
		Float:macheteBar_textSizeX;

	for(new i = 0; i != 3; i++)
	{
		switch(i)
		{
			// Basic CND bars.
			case 0: macheteBar_posX = 541.599975, macheteBar_textSizeX = 531.333679;
			case 1: macheteBar_posX = 551.599975, macheteBar_textSizeX = 541.333679;
			case 2: macheteBar_posX = 561.599975, macheteBar_textSizeX = 551.333679;
		}

		ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i] = CreatePlayerTextDraw(playerid, macheteBar_posX, 366.707366, "usebox");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 0.000000, -0.086211);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], macheteBar_textSizeX, 0.000000);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 0);
		PlayerTextDrawUseBox	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], true);
		PlayerTextDrawBoxColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Machete][i], 0);
	}
//#endregion

//#region HUD : bars -> hatchet
	new 
		Float:hatchetBar_posX,
		Float:hatchetBar_textSizeX;

	for(new i = 0; i != 5; i++)
	{
		switch(i)
		{
			case 0: hatchetBar_posX = 538.299926, hatchetBar_textSizeX = 531.333374;
			case 1: hatchetBar_posX = 543.999755, hatchetBar_textSizeX = 537.033203;
			case 2: hatchetBar_posX = 549.699584, hatchetBar_textSizeX = 542.733032;
			case 3: hatchetBar_posX = 555.399414, hatchetBar_textSizeX = 548.432861;
			case 4: hatchetBar_posX = 561.099243, hatchetBar_textSizeX = 554.132690;
		}

		ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i] = CreatePlayerTextDraw(playerid, hatchetBar_posX, 366.951751, "usebox");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 0.000000, -0.086211);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], hatchetBar_textSizeX, 0.000000);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 0);
		PlayerTextDrawUseBox	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], true);
		PlayerTextDrawBoxColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_Hatchet][i], 0);
	}
//#endregion

//#region HUD : bars -> upgraded CND bars
	new 
		Float:upgradedBar_posX,
		Float:upgradedBar_textSizeX;

	for(new i = 0; i != 3; i++)
	{
		switch(i)
		{
			// Upgraded CND bars.
			case 0: upgradedBar_posX = 541.599975, upgradedBar_textSizeX = 531.333679;
			case 1: upgradedBar_posX = 551.599975, upgradedBar_textSizeX = 541.333679;
			case 2: upgradedBar_posX = 561.599975, upgradedBar_textSizeX = 551.333679;
		}

		ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i] = CreatePlayerTextDraw(playerid, upgradedBar_posX, 371.414825, "usebox");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 0.000000, -0.086211);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], upgradedBar_textSizeX, 0.000000);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 0);
		PlayerTextDrawUseBox	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], true);
		PlayerTextDrawBoxColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], COLOR_GRAY_TRANS);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Bars_MeleeUpgraded][i], 0);
	}
//#endregion

	// HUD : health bar status
	ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status] = CreatePlayerTextDraw(playerid, 528.6, 377.792938, "hpbar_status");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 0.000000, 0.151231);
	PlayerTextDrawTextSize			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], pb_percent(526.0, 38.3, 100.0, 0.0), 0.0);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 1550998015);
	PlayerTextDrawUseBox			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], true);
	PlayerTextDrawBoxColor			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 1550998015);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 0);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 1510739711);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], 0);
}

PlayerTextDraw_BackpackMenu(playerid)
{
	PlayerTextDraw_Backpack_Craft(playerid);
	PlayerTextDraw_Backpack_Skills(playerid);
	PlayerTextDraw_Backpack_Colls(playerid);

//#region Right side-bar : Navigation top menu (select box + wrench/supplement/collectible)
	ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox] = CreatePlayerTextDraw(playerid, 428.000000, 31.111112, "TLoU:selectbox");
	PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 0.000000, 0.000000);
	PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 28.000036, 34.844440);
	PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 1);
	PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], -1);
	PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 0);
	PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 0);
	PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavSelectBox], 4);

	new 
		Float:posX_navItem,
		Float:posY_navItem,
		Float:sizeX_navItem,
		Float:sizeY_navItem,
		symbol_navItem[24];

	for(new i = 0; i != 3; i++)
	{
		switch(i)
		{
			case 0:posX_navItem = 428.0, posY_navItem = 36.503700, sizeX_navItem = 28.000000, sizeY_navItem = 26.133335, strcpy(symbol_navItem, "TLoU:wrench");
			case 1:posX_navItem = 463.666534, posY_navItem = 39.822227, sizeX_navItem = 21.333351, sizeY_navItem = 21.985206, strcpy(symbol_navItem, "TLoU:supplement");
			case 2:posX_navItem = 495.333374, posY_navItem = 41.066658, sizeX_navItem = 19.333335, sizeY_navItem = 18.666662, strcpy(symbol_navItem, "TLoU:collectible");
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i] = CreatePlayerTextDraw(playerid, posX_navItem, posY_navItem, symbol_navItem);
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], sizeX_navItem, sizeY_navItem);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], -1);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_NavItem][i], 4);
	}
//#endregion
}

PlayerTextDraw_Backpack_Craft(playerid)
{
//#region Left side-bar : Item icons list and their Wrench icons.
	new 
		Float:posX_craftItem,
		Float:posY_craftItem,
		Float:sizeX_craftItem, 
		Float:sizeY_craftItem,
		Float:posY_wrench_backpack = 69.5,
		symbol_craftItem[24];
	
	for(new i = 0; i != 6; i++)
	{
		switch(i)
		{
			case 0:posX_craftItem = 316.999908, posY_craftItem = 62.637035, sizeX_craftItem = 28.666673, sizeY_craftItem = 35.259239, strcpy(symbol_craftItem, "TLoU:weap_scsr_2x4_icon");
			case 1:posX_craftItem = 316.999908, posY_craftItem = 109.681503, sizeX_craftItem = 28.666673, sizeY_craftItem = 35.259239, strcpy(symbol_craftItem, "TLoU:weap_shiv");
			case 2:posX_craftItem = 316.999908, posY_craftItem = 157.970413, sizeX_craftItem = 29.000011, sizeY_craftItem = 34.014793, strcpy(symbol_craftItem, "TLoU:weap_medkit");
			case 3:posX_craftItem = 319.666717, posY_craftItem = 206.674057, sizeX_craftItem = 25.000009, sizeY_craftItem = 30.696273, strcpy(symbol_craftItem, "TLoU:weap_molotov");
			case 4:posX_craftItem = 322.666320, posY_craftItem = 253.303665, sizeX_craftItem = 21.000015, sizeY_craftItem = 28.622215, strcpy(symbol_craftItem, "TLoU:weap_nailbomb");
			case 5:posX_craftItem = 322.666320, posY_craftItem = 302.837188, sizeX_craftItem = 17.666677, sizeY_craftItem = 23.229625, strcpy(symbol_craftItem, "TLoU:weap_smkbmb");
		}

	    ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i] = CreatePlayerTextDraw(playerid, posX_craftItem, posY_craftItem, symbol_craftItem);
		PlayerTextDrawLetterSize	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], sizeX_craftItem, sizeY_craftItem);
		PlayerTextDrawAlignment		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], 1);
		PlayerTextDrawColor			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], COLOR_GRAY);
		PlayerTextDrawSetShadow		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], 0);
		PlayerTextDrawSetOutline	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], 0);
		PlayerTextDrawFont			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItems][i], 4);

		ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i] = CreatePlayerTextDraw(playerid, 296.000000, posY_wrench_backpack, "TLoU:wrench");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 25.666673, 24.474071);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], -1);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_Wrench][i], 4);

		posY_wrench_backpack += 45.0;
	}
//#endregion

//#region Left side-bar : Amount number next to the icons of items to craft.
	new Float:posY_craftItemAmount = 125.0;
	for(new i = 0; i != 5; i++, posY_craftItemAmount += 47.0)
	{
		ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i] = CreatePlayerTextDraw(playerid, 341.666503, posY_craftItemAmount, "0");
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 0.190999, 2.014811);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 1);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], -1);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 51);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 2);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Ls_CraftItemsAmount][i], 1);
	}
//#endregion

//#region Right side-bar : Backpack category name
	ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName] = CreatePlayerTextDraw(playerid, 380.999969, 65.540710, "CRAFTING");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 0.318666, 2.238814);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 2);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CatName], 1);
//#endregion

//#region Right side-bar : Viewing item to craft/upgrade
	ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0] = CreatePlayerTextDraw(playerid, 382.333435, 91.259246, "TLoU:weap_medkit");
	PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 0.000000, 0.000000);
	PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 51.333297, 63.051837);
	PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 1);
	PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], -1);
	PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 0);
	PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 0);
	PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][0], 4);

	ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1] = CreatePlayerTextDraw(playerid, 439.000000, 90.185173, "HEALTH KIT");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 0.284666, 2.242959);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 2);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][1], 1);

	ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2] = CreatePlayerTextDraw(playerid, 439.333221, 113.170364, "Crafts a health kit");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 0.240666, 1.425776);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 1);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_CurrentItem][2], 1);
//#endregion

//#region Right side-bar : Ingredient boxes (outlines) && Ingredient icons

	new 
		Float:posY_ingrBox,
		Float:posX_ingrIcon, Float:posY_ingrIcon,
		symbol_ingr[32];

	for(new i = 0; i != 19; i++)
	{
		switch(i)
		{
			// Blade.
			case 0:posX_ingrIcon = 462.0, posY_ingrIcon = 158.8, posY_ingrBox = 157.2, strcpy(symbol_ingr, "TLoU:ingr_blade_full");
			case 1:posX_ingrIcon = 486.0, posY_ingrIcon = 158.8, posY_ingrBox = 157.2, strcpy(symbol_ingr, "TLoU:ingr_blade_quater");
			case 2:posX_ingrIcon = 510.0, posY_ingrIcon = 158.8, posY_ingrBox = 157.2, strcpy(symbol_ingr, "TLoU:ingr_blade");

			// Binding.
			case 3:posX_ingrIcon = 462.0, posY_ingrIcon = 190.1, posY_ingrBox = 188.5, strcpy(symbol_ingr, "TLoU:ingr_binding_quater");
			case 4:posX_ingrIcon = 486.0, posY_ingrIcon = 190.1, posY_ingrBox = 188.5, strcpy(symbol_ingr, "TLoU:ingr_binding");
			case 5:posX_ingrIcon = 510.0, posY_ingrIcon = 190.1, posY_ingrBox = 188.5, strcpy(symbol_ingr, "TLoU:ingr_binding");

			// Rag.
			case 6:posX_ingrIcon = 462.0, posY_ingrIcon = 221.4, posY_ingrBox = 219.8, strcpy(symbol_ingr, "TLoU:ingr_rag_full");
			case 7:posX_ingrIcon = 486.0, posY_ingrIcon = 221.4, posY_ingrBox = 219.8, strcpy(symbol_ingr, "TLoU:ingr_rag_full");
			case 8:posX_ingrIcon = 510.0, posY_ingrIcon = 221.4, posY_ingrBox = 219.8, strcpy(symbol_ingr, "TLoU:ingr_rag_3quaters");

			// Alcohol.
			case 9:posX_ingrIcon = 462.0, posY_ingrIcon = 252.7, posY_ingrBox = 251.1, strcpy(symbol_ingr, "TLoU:ingr_alcohol_3quaters");
			case 10:posX_ingrIcon = 486.0, posY_ingrIcon = 252.7, posY_ingrBox = 251.1, strcpy(symbol_ingr, "TLoU:ingr_alcohol");
			case 11:posX_ingrIcon = 510.0, posY_ingrIcon = 252.7, posY_ingrBox = 251.1, strcpy(symbol_ingr, "TLoU:ingr_alcohol");

			// Explosive.
			case 12:posX_ingrIcon = 462.0, posY_ingrIcon = 284.0, posY_ingrBox = 282.4, strcpy(symbol_ingr, "TLoU:ingr_explosive_half");
			case 13:posX_ingrIcon = 486.0, posY_ingrIcon = 284.0, posY_ingrBox = 282.4, strcpy(symbol_ingr, "TLoU:ingr_explosive");
			case 14:posX_ingrIcon = 510.0, posY_ingrIcon = 284.0, posY_ingrBox = 282.4, strcpy(symbol_ingr, "TLoU:ingr_explosive");

			// Sugar.
			case 15:posX_ingrIcon = 462.0, posY_ingrIcon = 315.3, posY_ingrBox = 313.7, strcpy(symbol_ingr, "TLoU:ingr_sugar_full");
			case 16:posX_ingrIcon = 486.0, posY_ingrIcon = 315.3, posY_ingrBox = 313.7, strcpy(symbol_ingr, "TLoU:ingr_sugar_3quaters");
			case 17:posX_ingrIcon = 510.0, posY_ingrIcon = 315.3, posY_ingrBox = 313.7, strcpy(symbol_ingr, "TLoU:ingr_sugar");

			// Melee.
			case 18:posX_ingrIcon = 510.0, posY_ingrIcon = 346.6, posY_ingrBox = 345, strcpy(symbol_ingr, "TLoU:ingr_melee");
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i] = CreatePlayerTextDraw(playerid, posX_ingrIcon, posY_ingrIcon, symbol_ingr);
		PlayerTextDrawLetterSize	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 18.666645, 20.325935);
		PlayerTextDrawAlignment		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 1);
		PlayerTextDrawColor			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], COLOR_GRAY);
		PlayerTextDrawSetShadow		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 0);
		PlayerTextDrawSetOutline	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 0);
		PlayerTextDrawFont			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrIcon][i], 4);

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i] = CreatePlayerTextDraw(playerid, posX_ingrIcon, posY_ingrBox, "TLoU:ingrbox");
		PlayerTextDrawLetterSize	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 18.666656, 23.644443);
		PlayerTextDrawAlignment		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 1);
		PlayerTextDrawColor			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], COLOR_GRAY);
		PlayerTextDrawSetShadow		(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 0);
		PlayerTextDrawSetOutline	(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 0);
		PlayerTextDrawFont			(playerid, 	ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrBox][i], 4);
	}
//#endregion

//#region Right side-bar : Ingredient names
	new 
		Float:posX_ingrName = 455.333282,
		Float:posY_ingrName = 162.192718,
		symbol_ingrName[24];

	for(new i = 0; i != 7; i++)
	{
		switch(i)
		{
			case 0:strcpy(symbol_ingrName, "BLADE");
			case 1:strcpy(symbol_ingrName, "BINDING");
			case 2:strcpy(symbol_ingrName, "RAG");
			case 3:strcpy(symbol_ingrName, "ALCOHOL");
			case 4:strcpy(symbol_ingrName, "EXPLOSIVE");
			case 5:strcpy(symbol_ingrName, "SUGAR");
			case 6:posX_ingrName = 504.666625, strcpy(symbol_ingrName, "MELEE WEAPON");
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i] = CreatePlayerTextDraw(playerid, posX_ingrName, posY_ingrName, symbol_ingrName);
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 0.202665, 1.446519);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 3);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], COLOR_GRAY);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 51);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 2);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_IngrName][i], 1);

		posY_ingrName += 31.7;
	}
//#endregion
}

PlayerTextDraw_Backpack_Skills(playerid)
{
//#region Right side-bar : Supplements amount && Icon
	ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount] = CreatePlayerTextDraw(playerid, 551.333435, 63.881473, "50");
	PlayerTextDrawLetterSize	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 0.259000, 2.566519);
	PlayerTextDrawAlignment	 	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 1);
	PlayerTextDrawColor			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], COLOR_GRAY);
	PlayerTextDrawSetShadow		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 0);
	PlayerTextDrawSetOutline	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 1);
	PlayerTextDrawBackgroundColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 51);
	PlayerTextDrawFont			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 2);
	PlayerTextDrawSetProportional(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsAmount], 1);

	ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon] = CreatePlayerTextDraw(playerid, 531.999694, 70.688903, "TLoU:supplement");
	PlayerTextDrawLetterSize	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 0.000000, 0.000000);
	PlayerTextDrawTextSize		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 16.000015, 15.762984);
	PlayerTextDrawAlignment		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 1);
	PlayerTextDrawColor			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], COLOR_GRAY);
	PlayerTextDrawSetShadow		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 0);
	PlayerTextDrawSetOutline	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 0);
	PlayerTextDrawFont			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcon], 4);
//#endregion

//#region Right side-bar : Supplements names
	new 
		Float:posY_skillName = 95.822189,
		skillName[24];

	for(new i = 0; i != 6; i++)
	{
		switch(i)
		{
			case 0:strcpy(skillName, "MAXIMUM HEALTH");
			case 1:strcpy(skillName, "LISTEN MODE DISTANCE");
			case 2:strcpy(skillName, "CRAFTING SPEED");
			case 3:strcpy(skillName, "HEALING SPEED");
			case 4:strcpy(skillName, "WEAPON SWAY");
			case 5:strcpy(skillName, "SHIV MASTER");
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i] = CreatePlayerTextDraw(playerid, 402.666687, posY_skillName, skillName);
		PlayerTextDrawLetterSize	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 0.181997, 1.234961);
		PlayerTextDrawAlignment		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 1);
		PlayerTextDrawColor			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], COLOR_GRAY);
		PlayerTextDrawSetShadow		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 0);
		PlayerTextDrawSetOutline	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 1);
		PlayerTextDrawBackgroundColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 51);
		PlayerTextDrawFont			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 2);
		PlayerTextDrawSetProportional(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Names][i], 1);

		posY_skillName += 37.503723;
	}
//#endregion

//#region Right side-bar : Supplements icons + require amounts
	new
		skillRequire[3],
		Float:posY_supplementIcon,
		Float:posY_supplementRequirmentAmount;

	for(new i = 0; i != 6; i++)
	{
		switch(i)
		{
			case 0:strcpy(skillRequire, "50"), posY_supplementIcon = 99.555534, posY_supplementRequirmentAmount = 94.992599;
			case 1:strcpy(skillRequire, "20"), posY_supplementIcon = 136.644409, posY_supplementRequirmentAmount = 132.081466;
			case 2:strcpy(skillRequire, "20"), posY_supplementIcon = 173.733291, posY_supplementRequirmentAmount = 169.170349;
			case 3:strcpy(skillRequire, "20"), posY_supplementIcon = 210.822158, posY_supplementRequirmentAmount = 206.259231;
			case 4:strcpy(skillRequire, "50"), posY_supplementIcon = 247.911026, posY_supplementRequirmentAmount = 243.348098;
			case 5:strcpy(skillRequire, "75"), posY_supplementIcon = 284.999908, posY_supplementRequirmentAmount = 280.436981;
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i] = CreatePlayerTextDraw(playerid, 543.333129, posY_supplementIcon, "TLoU:supplement");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 8.999958, 9.125926);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], -1);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsIcons][i], 4);

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i] = CreatePlayerTextDraw(playerid, 555.000183, posY_supplementRequirmentAmount, skillRequire);
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 0.146999, 1.508738);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 1);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], -1);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 51);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 2);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_SuppsRequire][i], 1);
	}
//#endregion

//#region Right side-bar : Supplements progress bars/status
	new 
		Float:posX_skillProgressBar,
		Float:posY_skillProgressBar,
		Float:textSize_X_skillProgressBar;

	for(new i = 0; i != 15; i++)
	{
		switch(i)
		{
			// MAXHP
			case 0:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 114.744422, textSize_X_skillProgressBar = 404.3;
			case 1:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 114.744422, textSize_X_skillProgressBar = 416.0;

			// LISTEN MODE DISTANCE
			case 2:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 152.662918, textSize_X_skillProgressBar = 404.3;
			case 3:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 152.662918, textSize_X_skillProgressBar = 416.0;
			case 4:posX_skillProgressBar = 431.000000, posY_skillProgressBar = 152.662918, textSize_X_skillProgressBar = 428.3;

			// CRAFTING SPEED
			case 5:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 189.751800, textSize_X_skillProgressBar = 404.3;
			case 6:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 189.751800, textSize_X_skillProgressBar = 416.0;
			case 7:posX_skillProgressBar = 431.000000, posY_skillProgressBar = 189.751800, textSize_X_skillProgressBar = 428.3;

			// HEALING SPEED
			case 8:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 227.670257, textSize_X_skillProgressBar = 404.3;
			case 9:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 227.670257, textSize_X_skillProgressBar = 416.0;
			case 10:posX_skillProgressBar = 431.000000, posY_skillProgressBar = 227.670257, textSize_X_skillProgressBar = 428.3;

			// WEAPON SWAY
			case 11:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 264.759216, textSize_X_skillProgressBar = 404.3;
			case 12:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 264.759216, textSize_X_skillProgressBar = 416.0;

			// SHIV MASTER
			case 13:posX_skillProgressBar = 407.000000, posY_skillProgressBar = 302.677673, textSize_X_skillProgressBar = 404.3;
			case 14:posX_skillProgressBar = 419.000000, posY_skillProgressBar = 302.677673, textSize_X_skillProgressBar = 416.0;
		}

		ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i] = CreatePlayerTextDraw(playerid, posX_skillProgressBar, posY_skillProgressBar, "usebox");
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 0.000000, 0.052051);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], textSize_X_skillProgressBar, 0.0);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 0);
		PlayerTextDrawUseBox		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], true);
		PlayerTextDrawBoxColor		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], -1);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_SkillProg_Status][i], 0);
	}
//#endregion

	// Description of the skill.
	ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description] = CreatePlayerTextDraw(playerid, 477.333312, 327.703857, "Reduces crafting time");
	PlayerTextDrawLetterSize	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 0.262333, 1.442370);
	PlayerTextDrawAlignment		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 2);
	PlayerTextDrawColor			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], COLOR_GRAY);
	PlayerTextDrawSetShadow		 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 0);
	PlayerTextDrawSetOutline	 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 1);
	PlayerTextDrawBackgroundColor(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 51);
	PlayerTextDrawFont			 (playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 1);
	PlayerTextDrawSetProportional(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Skills_Description], 1);
}

PlayerTextDraw_Backpack_Colls(playerid)
{
//#region Right side-bar : Collectibles titles

	ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox] = CreatePlayerTextDraw(playerid, 483.333068, 124.348159, "TLoU:selectbox");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 93.000022, 20.325904);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], -1);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_SelectBox], 4);

	new Float:posY_collTitle = 127.348159;
	for(new i = 0; i < 9; i++)
	{
		ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i] = CreatePlayerTextDraw(playerid, 491.666778, posY_collTitle, "");
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 0.223995, 1.239109);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 1);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], -1);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 51);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 1);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_Bp_Rs_Coll_Titles][i], 1);

		posY_collTitle += 24.0;
	}

//#endregion
}

PlayerTextDraw_WeaponMenu(playerid)
{
//#region Weapon Menu : Wrench icons
	new Float:wrench_posY;
	for(new i = 0; i != 4; i++)
	{
		switch(i)
		{
			case 0:wrench_posY = 147.674041;
			case 1:wrench_posY = 194.133255;
			case 2:wrench_posY = 323.140838;
			case 3:wrench_posY = 368.355590;
		}

		ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i] = CreatePlayerTextDraw(playerid, 425.000000, wrench_posY, "TLoU:wrench");
		PlayerTextDrawLetterSize(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 19.999969, 18.666685);
		PlayerTextDrawAlignment	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 1);
		PlayerTextDrawColor		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], -1);
		PlayerTextDrawSetShadow	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 0);
		PlayerTextDrawSetOutline(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 0);
		PlayerTextDrawFont		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_wrench][i], 4);
	}
//#endregion

//#region Weapon Menu : Icons
	new
		Float:posX_weapIcon,
		Float:posY_weapIcon,
		Float:sizeX_weapIcon,
		Float:sizeY_weapIcon,
		symbol_weapIcon[24];

	for(new i = 0; i != 9; i++)
	{
		switch(i)
		{
			case 0:posX_weapIcon = 437.333312, posY_weapIcon = 150.577774, sizeX_weapIcon = 24.333345, sizeY_weapIcon = 29.866668, strcpy(symbol_weapIcon, "TLoU:weap_nailbomb");
			case 1:posX_weapIcon = 435.666717, posY_weapIcon = 192.229675, sizeX_weapIcon = 27.333349, sizeY_weapIcon = 32.770374, strcpy(symbol_weapIcon, "TLoU:weap_medkit");
			case 2:posX_weapIcon = 437.333343, posY_weapIcon = 279.096160, sizeX_weapIcon = 24.333347, sizeY_weapIcon = 30.281484, strcpy(symbol_weapIcon, "TLoU:weap_brick");
			case 3:posX_weapIcon = 438.000000, posY_weapIcon = 321.163085, sizeX_weapIcon = 25.333343, sizeY_weapIcon = 31.111112, strcpy(symbol_weapIcon, "TLoU:weap_molotov");
			case 4:posX_weapIcon = 439.666625, posY_weapIcon = 366.133392, sizeX_weapIcon = 20.000007, sizeY_weapIcon = 26.962972, strcpy(symbol_weapIcon, "TLoU:weap_smkbmb");
			
			case 5:posX_weapIcon = 335.999969, posY_weapIcon = 225.892562, sizeX_weapIcon = 46.000007, sizeY_weapIcon = 51.022228, strcpy(symbol_weapIcon, "TLoU:weap_bow");
			case 6:posX_weapIcon = 384.666656, posY_weapIcon = 220.500000, sizeX_weapIcon = 41.000015, sizeY_weapIcon = 52.681484, strcpy(symbol_weapIcon, "TLoU:weap_shot");
			case 7:posX_weapIcon = 472.333404, posY_weapIcon = 232.800003, sizeX_weapIcon = 28.333322, sizeY_weapIcon = 36.918544, strcpy(symbol_weapIcon, "TLoU:weap_rev");
			case 8:posX_weapIcon = 518.666809, posY_weapIcon = 232.800003, sizeX_weapIcon = 28.333322, sizeY_weapIcon = 36.918544, strcpy(symbol_weapIcon, "TLoU:weap_9mm");
		}

		ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i] = CreatePlayerTextDraw(playerid, posX_weapIcon, posY_weapIcon, symbol_weapIcon);
		PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], 0.000000, 0.000000);
		PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], sizeX_weapIcon, sizeY_weapIcon);
		PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], 1);
		PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], COLOR_GRAY);
		PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], 0);
		PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], 0);
		PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Icons][i], 4);
	}
//#endregion

//#region Weapon Menu : Amounts
	new
		Float:posX_weapAmount,
		Float:posY_weapAmount;

	for(new i = 0; i != 9; i++)
	{
		switch(i)
		{
			case 0:posX_weapAmount = 462.000000, posY_weapAmount = 165.925918;
			case 1:posX_weapAmount = 462.000000, posY_weapAmount = 208.822189;
			case 2:posX_weapAmount = 462.000000, posY_weapAmount = 294.444519;
			case 3:posX_weapAmount = 462.000000, posY_weapAmount = 335.681671;
			case 4:posX_weapAmount = 462.000000, posY_weapAmount = 379.407775;
			case 5:posX_weapAmount = 372.666320, posY_weapAmount = 251.500000;
			case 6:posX_weapAmount = 422.666412, posY_weapAmount = 251.500000;
			case 7:posX_weapAmount = 496.999969, posY_weapAmount = 251.500000;
			case 8:posX_weapAmount = 549.333251, posY_weapAmount = 251.500000;
		}

		ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i] = CreatePlayerTextDraw(playerid, posX_weapAmount, posY_weapAmount, "0");
		PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 0.268999, 2.130964);
		PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 2);
		PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], COLOR_GRAY);
		PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 0);
		PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 1);
		PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 51);
		PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 2);
		PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_Amounts][i], 1);
	}
//#endregion

//#region Weapon Menu : Hold to swap
	ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0] = CreatePlayerTextDraw(playerid, 468.666717, 278.755615, "HOLD~n~TO SWAP");
	PlayerTextDrawLetterSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 0.173996, 1.479701);
	PlayerTextDrawAlignment			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 1);
	PlayerTextDrawColor				(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], -1);
	PlayerTextDrawSetShadow			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 0);
	PlayerTextDrawSetOutline		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 1);
	PlayerTextDrawBackgroundColor	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 51);
	PlayerTextDrawFont				(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 2);
	PlayerTextDrawSetProportional	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][0], 1);

	ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1] = CreatePlayerTextDraw(playerid, 492.666625, 281.244445, "TLoU:mouseL");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 5.999967, 11.614809);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], -1);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_howToSwap][1], 4);
//#endregion

//#region Weapon Menu : Swap weapon icons
	ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0] = CreatePlayerTextDraw(playerid, 384.666656, 180.500000, "above_icon");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 41.000015, 52.681484);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], COLOR_GRAY_TRANS);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][0], 4);

	ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1] = CreatePlayerTextDraw(playerid, 384.666656, 260.500000, "below_icon");
	PlayerTextDrawLetterSize	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 0.000000, 0.000000);
	PlayerTextDrawTextSize		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 41.000015, 52.681484);
	PlayerTextDrawAlignment		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 1);
	PlayerTextDrawColor			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], COLOR_GRAY_TRANS);
	PlayerTextDrawSetShadow		(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 0);
	PlayerTextDrawSetOutline	(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 0);
	PlayerTextDrawFont			(playerid, ePlayerTextDrawInfo[playerid][e_weapMenu_swapWeapIcons][1], 4);
//#endregion
}