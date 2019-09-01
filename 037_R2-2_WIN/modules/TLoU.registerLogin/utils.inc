// Save player's data
function SavePlayerData(playerid)
{
    if(!IsPlayerLogged(playerid))
    {
        return 1;
    }

	printf("%s left the server. Saving player's data..", PlayerName(playerid));

    new Float:x, Float:y, Float:z, Float:ang;
    GetPlayerPos(playerid, x, y, z);
    GetPlayerFacingAngle(playerid, ang);

    new 
		Float:playerHealth = GetPlayerHealthEx(playerid),
        playerInterior = GetPlayerInterior(playerid), 
        playerVirtualWorld = GetPlayerVirtualWorld(playerid),
		playerSupps = eCharsInfo[playerid][e_iSkills_SuppsAmount],
		playerSkill_MaxHp = Skill_GetLevel(playerid, SKILL_ITEM_MAXHP),
		playerSkill_MaxListenDist = Skill_GetLevel(playerid, SKILL_ITEM_LISTEN_DIST),
		playerSkill_CraftSpeed = Skill_GetLevel(playerid, SKILL_ITEM_CRAFT_SPEED),
		playerSkill_HealSpeed = Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED),
		playerSkill_WeapSway = Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY),
		playerSkill_ShivMaster = Skill_GetLevel(playerid, SKILL_ITEM_SHIV_MASTER);

	DB_SaveCharsData(playerid, playerHealth, x, y, z, ang, playerInterior, playerVirtualWorld, \
		playerSupps, playerSkill_MaxHp, playerSkill_MaxListenDist, playerSkill_CraftSpeed, playerSkill_HealSpeed, playerSkill_WeapSway, playerSkill_ShivMaster);

	DB_SavePlayerItems(playerid);

    return 0;
}

// Show appropriate dialog (sign in or sign up) 
function ShowRegisterLoginDialog(playerid, registered)
{
	if(registered)
	{
		inline Response(pid, dialogid, response, listitem, string:inputtext[])
		{
			#pragma unused pid, dialogid, response, listitem, inputtext
			if(!response)
				return Kick(playerid);

			if(!inputtext[0])
			{
				return Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD,\
					"Sign in", "Welcome back to the The Last of Us (SA-MP) Project!\nWe have found you in our database. Please enter your account password below to sign in:", "Sign in", "Quit");
			}
			LoginPlayerToAccount(playerid, inputtext);
		}
		Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD,\
			"Sign in", "Welcome back to the The Last of Us (SA-MP) Project!\nWe have found you in our database. Please enter your account password below to sign in:", "Sign in", "Quit");
	}
	else
	{
		inline Response(pid, dialogid, response, listitem, string:inputtext[])
		{
			#pragma unused pid, dialogid, response, listitem, inputtext
			if(!response)
				return Kick(playerid);

			if(!inputtext[0])
			{
				return Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD,\
					"Sign up", "Welcome to the \"The Last of Us (SA-MP) Project!\"\nWe haven't found you in our database. Please enter your account password below to sign up:", "Sign up", "Quit");
			}
			RegisterPlayerAccount(playerid, inputtext);
		}
		Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD,\
			"Sign up", "Welcome to the \"The Last of Us (SA-MP) Project!\"\nWe haven't found you in our database. Please enter your account password below to sign up:", "Sign up", "Quit");
	}
    return 0;
}

ResetVariables(playerid)
{
	Iter_Clear(ItemsToCraft<playerid>);
	Iter_Clear(SkillsToUpgrade<playerid>);
	Iter_Clear(CollectiblesOwned<playerid>);
	Iter_Clear(iAudioHandle3D<playerid>);

    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        ePlayerItemData[playerid][i][e_iItemId] = -1;
        ePlayerItemData[playerid][i][e_iItemAmount] = 0;
        ePlayerItemData[playerid][i][e_iItemClipAmount] = -1;

        if(i <= 3)
        {
            WeapMenu_Ls_ItemId(playerid, i) = -1;
        }

        if(i <= 2)
        {
            WeapMenu_Rs_ItemId(playerid, i) = -1;
        }
    }

	ePlayerBpInfo[playerid][e_iBackpackSelected_Craft] = 0;
	ePlayerBpInfo[playerid][e_iBackpackSelected_Skill] = 0;
	ePlayerBpInfo[playerid][e_iBackpackSelected_ViewColl] = 0;

    ePlayerWeapon[playerid][e_iArrowObjectId] = -1;

    ePlayerInfo[playerid][e_iCurrentEquippedItemId] = -1;
    ePlayerInfo[playerid][e_iLastEquippedItemId] = -1;
    
    eCharsInfo[playerid][e_iAttachSlot_Backpack] = -1;
    eCharsInfo[playerid][e_iAttachSlot_9mm] = -1;
    eCharsInfo[playerid][e_iAttachSlot_Revolver] = -1;
    eCharsInfo[playerid][e_iAttachSlot_LongGunOne] = -1;
    eCharsInfo[playerid][e_iAttachSlot_LongGunTwo] = -1;
    eCharsInfo[playerid][e_iAttachSlot_Melee] = -1;
    eCharsInfo[playerid][e_iAttachSlot_GasMask] = -1;
    eCharsInfo[playerid][e_iAttachSlot_Dynamic] = -1;
}