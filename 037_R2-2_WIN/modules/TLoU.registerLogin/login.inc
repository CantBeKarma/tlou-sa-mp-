// Load player and character data
function LoginPlayerToAccount(playerid, password[])
{
    new hashStr[65];
    SHA256_PassHash(password, "tlou2014", hashStr, sizeof(hashStr));
    DB_RequestPlayerData(playerid, hashStr);

    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s is trying to sign in to account of Guid: %d", PlayerName(playerid), GetPlayerCharUid(playerid));
}

// Prepare player's data during sign in.
function PrepareDataAfterLogin(playerid)
{
    LoadCache(playerid);

    new Float:charHealth;

    cache_get_value_name_int(0, "uid", eCharsInfo[playerid][e_iUid]);
    cache_get_value_float(0, "health", charHealth);
    cache_get_value_name_int(0, "skinid", eCharsInfo[playerid][e_iSkinId]);
    cache_get_value_float(0, "last_x", eCharsInfo[playerid][e_fPosX]); 
    cache_get_value_float(0, "last_y", eCharsInfo[playerid][e_fPosY]); 
    cache_get_value_float(0, "last_z", eCharsInfo[playerid][e_fPosZ]);
    cache_get_value_float(0, "last_ang", eCharsInfo[playerid][e_fPosAng]);
    cache_get_value_name_int(0, "last_int", eCharsInfo[playerid][e_iPosInt]);
    cache_get_value_name_int(0, "last_vw", eCharsInfo[playerid][e_iPosVw]);
    cache_get_value_name_int(0, "supplements", eCharsInfo[playerid][e_iSkills_SuppsAmount]);
    cache_get_value_name_int(0, "skill_maxhealth", Skill_GetLevel(playerid, SKILL_ITEM_MAXHP));
    cache_get_value_name_int(0, "skill_listenmode", Skill_GetLevel(playerid, SKILL_ITEM_LISTEN_DIST));
    cache_get_value_name_int(0, "skill_crafting", Skill_GetLevel(playerid, SKILL_ITEM_CRAFT_SPEED));
    cache_get_value_name_int(0, "skill_healing", Skill_GetLevel(playerid, SKILL_ITEM_HEAL_SPEED));
    cache_get_value_name_int(0, "skill_weaponsway", Skill_GetLevel(playerid, SKILL_ITEM_WEAP_SWAY));
    cache_get_value_name_int(0, "skill_shivmaster", Skill_GetLevel(playerid, SKILL_ITEM_SHIV_MASTER));

//#region Reset variables
    for(new i = 0; i != MAX_WEAPON_SLOTS; i++)
    {
        ePlayerWeapon[playerid][e_iWeaponClipAmmo][i] = -1;

        if(i < 4)
        {
            WeapMenu_Ls_ItemId(playerid, i) = -1;
        }

        if(i < 3)
        {
            WeapMenu_Rs_ItemId(playerid, i) = -1;
        }
    }

    Iter_Clear(ItemsToCraft<playerid>);
    Iter_Clear(SkillsToUpgrade<playerid>);
    Iter_Clear(iAudioHandle3D<playerid>);

    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        ePlayerItemData[playerid][i][e_iItemId] = -1;
        ePlayerItemData[playerid][i][e_iItemAmount] = -1;
        ePlayerItemData[playerid][i][e_iItemClipAmount] = -1;
    }

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
//#endregion

    DB_LoadPlayerItems(playerid, eCharsInfo[playerid][e_iUid]);

    TogglePlayerAction(playerid, PLAYER_ACTION_FIRE_WEAPON, false);
    TogglePlayerAction(playerid, PLAYER_ACTION_SWITCH_WEAPON, false);

    SkillTd_FillPlayerSkillBars(playerid);

    SetPlayerHealth(playerid, FLOAT_INFINITY);
    SetPlayerHealthEx(playerid, (charHealth <= 0.0) ? (25.0) : (charHealth));

    CreatePlayerLabel(playerid);
    SetPlayerVirtualWorld(playerid, eCharsInfo[playerid][e_iPosVw]);
    SetSpawnInfoEx(playerid, GetPlayerSkinEx(playerid), eCharsInfo[playerid][e_fPosX], eCharsInfo[playerid][e_fPosY], eCharsInfo[playerid][e_fPosZ], eCharsInfo[playerid][e_fPosAng]);

    Bit_Let(ePlayerFlag[e_bLogged], playerid);
    Log(eLogs[e_Log_Index][LOG_INDEX_PLAYER], INFO, "%s (charUid: %d) has logged in successfully!", PlayerName(playerid), GetPlayerCharUid(playerid));

    SpawnPlayer(playerid);
    return 0;
}

CreatePlayerLabel(playerid)
{
    ePlayerInfo[playerid][e_iPlayerLabel] = CreateDynamic3DTextLabel(PlayerName(playerid), COLOR_WHITE_TRANS, 0.0, 0.0, 0.12, 10.0, playerid, INVALID_VEHICLE_ID, true);
}