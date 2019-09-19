// Load player's data from database.
LoadPlayerCharacterData(playerid)
{
	DB_LoadPlayerCharacter(playerid);
}

// Save player's character's data.
SavePlayerCharacterData(playerid)
{
    if(!IsPlayerLogged(playerid))
    {
        return false;
    }

	DB_SavePlayerCharacter(playerid);
    return true;
}

// Function is called during first spawn. Prepares player to sign in/up.
// It freezes player in specified position, clearing chat and performing connection intro procedure.
// Also it gets player name and saves it to variable, hides all HUD components and removes all buildings defined internally.
PreparePlayerToSign(playerid)
{
    FreezePlayer(playerid, false, -1);

    SetPlayerVirtualWorld(playerid, playerid);
    SetPlayerPos(playerid, -260.3906, 674.5479, 280.5438);
    SetPlayerCameraPos(playerid, -258.3556, 670.9673, 277.2066);
    SetPlayerCameraLookAt(playerid, -257.3723, 671.1423, 277.3569);

    ClearChat(playerid);
    CreatePlayerIntroTds(playerid);
    ShowPlayerBlackScreen(playerid);
    DisableDefaultAmbientNoise(playerid);

    GetPlayerName(playerid, ePlayerInfo[playerid][e_sName], MAX_PLAYER_NAME);
    ToggleHUDComponentForPlayer(playerid, HUD_COMPONENT_ALL, false);

    for(new i = 0; i != sizeof(RemoveObjectsData); i++)
    {
        RemoveBuildingForPlayer(playerid, RemoveObjectsData[i], 0.0, 0.0, 0.0, 6000.0);
	}
}

// Reseting all variables, iterators (overall player data).
// Usually happens only in sign in/up.
ResetVariables(playerid)
{
	Iter_Clear(ItemsToCraft<playerid>);
	Iter_Clear(SkillsToUpgrade<playerid>);
	Iter_Clear(ArtifactsOwned<playerid>);
	Iter_Clear(PendantsOwned<playerid>);
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

//#region Validation

	// Check if password cointains restricted characters.
	PassContainsRestrictedChars(const password[])
	{
		for(new i = 0; password[i] != EOS; i++)
		{
			switch(password[i])
			{
				case 
					'0'..'9', 
					'A'..'Z', 
					'a'..'z',
					'!',
					'?',
					'@',
					'#',
					'$':
					{
						continue;
					}

				default:
				{
					return false;
				}
			}
		}
		return true;
	}

	// Validating player's password input.
	ValidatePlayerPassword(playerid, const password[])
	{
		if(isnull(password)
		|| strlen(password) < MIN_PASSWORD_LEN
		|| strlen(password) > MAX_PASSWORD_LEN)
		{
			return false;
		}

		if(PassContainsRestrictedChars(password))
		{
			SCMF(playerid, -1, "Restricted characters detected! Allowed ones: %s", AllowedPasswordCharacters);
			return false;
		}

		return true;
	}

//#endregion