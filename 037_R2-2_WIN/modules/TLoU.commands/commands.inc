CMD:hp(playerid, params[])
{
    new Float:hp;
    if(sscanf(params, "f", hp))return SCM(playerid, -1, "/hp [value]");
    {
        SetPlayerHealthEx(playerid, hp);
    }
    return 1;
}

CMD:removeweap(playerid, params[])
{
    new weaponId;
    if(sscanf(params, "d", weaponId)) return SCM(playerid, -1, "/removeWeap [weapon_id]");
    {
        RemovePlayerWeapon(playerid, weaponId);
    }
    return 1;
}

CMD:freeze(playerid, params[])
{
    FreezePlayer(playerid, true, 5);
    return 1;
}

CMD:unfreeze(playerid, params[])
{
    UnfreezePlayer(playerid);
    return 1;
}

CMD:zdiff(playerid, params[])
{
	new targetId, Float:diff;
	if(sscanf(params, "df", targetId, diff)) return SCM(playerid, -1, "/zdiff [targetId] [targetDiff]");
	{
		new Float:x, Float:y, Float:z;
		GetPlayerPos(playerid, x, y, z);

		new Float:x2, Float:y2, Float:z2;
		FCNPC_GetPosition(targetId, x2, y2, z2);

		new zDiff = CompareZ(z, z2, diff);
		SCMF(playerid, -1, "Higher than target? : %d", zDiff);
	}
	return 1;
}

YCMD:icmd(playerid, params[], help)
{
    if(help)
    {
		Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "icmd > icmd", "Shows information about the required server command.\n/cmd - for all server commands.", "Close");
    }

    else
    {
        if(isnull(params))
        {
            Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, "Usage..", "Type: /icmd (command_name)", "Close", "");
        }

        else
        {
            Command_ReProcess(playerid, params, true);
        }
    }
    return 1;
}

CMD:clipsizelvl(playerid, params[])
{
    new weaponId, level;
    if(sscanf(params, "dd", weaponId, level))return SCM(playerid, -1, "/clipsizelvl [weaponId] [newLevel]");
    {
        SetPlayerWeaponUpgradeLvl(playerid, weaponId, level, UPGRADE_TYPE_CLIPSIZE);
    }
    return 1;
}

CMD:weapsdata(playerid, params[])
{
    new 
        str_weapsData[512],
        formattedStr[128];

    new 
        weaponId,
        weaponAmmo,
        weaponMaxAmmo,
        weaponClipAmmo;

    strcat(str_weapsData, "ID\tAmmo\tMax ammo\tClip ammo\n", sizeof(str_weapsData));

    for(new i = 0; i != MAX_WEAPON_SLOTS; i++)
    {
        weaponId = ePlayerWeapon[playerid][e_iWeaponsId][i];
        weaponAmmo = ePlayerWeapon[playerid][e_iWeaponsAmmo][i];
        weaponMaxAmmo = GetPlayerWeaponLvl_MaxAmmo(playerid, weaponId);
        weaponClipAmmo = ePlayerWeapon[playerid][e_iWeaponClipAmmo][i];

        SPAO(playerid, GetWeaponModelFromId(weaponId));

        format(formattedStr, sizeof(formattedStr), "%d\t%d\t%d\t%d\n", weaponId, weaponAmmo, weaponMaxAmmo, weaponClipAmmo);
        strcat(str_weapsData, formattedStr, sizeof(str_weapsData));
    }

    Dialog_Show(playerid, DIALOG_STYLE_TABLIST_HEADERS, "Your weapons data...", str_weapsData, "Close", "");
    return 1;
}

CMD:skin(playerid, params[])
{
    new skinId;
    if(sscanf(params, "d", skinId)) return SCMF(playerid, -1, "/skin [skinId] ( 312+ )");
    {
        SetPlayerSkinEx(playerid, skinId);
    }
    return 1;
}

CMD:mymelee(playerid, params[])
{
    new weaponId = GetPlayerMelee[playerid];
    SCMF(playerid, -1, "Melee weaponId: %d | CND: %d | Upgraded CND: %d", weaponId, GetPlayerMeleeCnd[playerid], GetPlayerMeleeExtCnd[playerid]);
    SCMF(playerid, -1, "default basic CND: %d | default upgraded CND: %d", GetMeleeWeapon_Durability(weaponId, false), GetMeleeWeapon_Durability(weaponId, true));
    return 1;
}

Dialog_CreateItemList(playerid)
{
    inline ItemListResponse(pid, dialogid, response, listitem, string:inputtext[])
    {
        #pragma unused pid, dialogid, response, listitem, inputtext
        if(!response)
            return 1;

        inline ItemListToCreate(pid2, dialogid2, response2, listitem2, string:inputtext2[])
        {
            #pragma unused pid2, dialogid2, response2, listitem2, inputtext2
            if(!response2)
                return 1;

            CreateOrUpdateItem(listitem2, strval(inputtext2), .creatorId = pid2);
            SendChatMessage(pid2, -1, "Item created");
        }
        Dialog_ShowCallback(playerid, using inline ItemListToCreate, DIALOG_STYLE_INPUT, "icmd > create item", "Enter the amount of item to create:", "Create", "Close");
    }

    new string[350];
    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        format(string, sizeof(string), "%s%d\t%s\n", string, i, eItemsData[i][e_sItem_Name]);
    }
    Dialog_ShowCallback(playerid, using inline ItemListResponse, DIALOG_STYLE_LIST, "icmd > items list", string, "Create", "Close");
    return 0;
}

YCMD:createitem(playerid, params[], help)
{
	if(help)
	{
        Dialog_CreateItemList(playerid);
	}
	else
	{
        new itemId, itemAmount, meleeExtCnd, title[32], innerMessage[256];
        if(sscanf(params, "ddds[32]s[256]", itemId, itemAmount, meleeExtCnd, title, innerMessage))return SCM(playerid, -1, "/createitem [itemId] [itemAmount] [meleeExtCnd] [title] [innerMsg]");
        {
            CreateOrUpdateItem(itemId, itemAmount, meleeExtCnd, playerid, title, innerMessage);
        }
	}
	return 1;
}

CMD:ls(playerid, params[])
{
    SetPlayerPos(playerid, 2466.2358,-1659.0228,13.2723);
    return 1;
}

CMD:velocity(playerid, params[])
{
    new Float:oldNpcVelX, Float:oldNpcVelY, Float:oldNpcVelZ;

    new Float:x, Float:y, Float:z, targetid;
    if(sscanf(params, "dfff", targetid, x, y, z))return SCM(playerid, -1, "/velocity [targetid] [x] [y] [z]");
    {
        if(IsPlayerNPC(targetid))
        {
            FCNPC_GetVelocity(targetid, oldNpcVelX, oldNpcVelY, oldNpcVelZ);
            FCNPC_GiveVelocity(targetid, oldNpcVelX + x, oldNpcVelY + y, z, false);
            //FCNPC_GetVelocity(targetid, oldNpcVelX, oldNpcVelY, oldNpcVelZ);
            //FCNPC_SetVelocity(targetid, oldNpcVelX + x, oldNpcVelY + y, oldNpcVelZ + z);

            //wait_ms(500);
            //FCNPC_SetVelocity(targetid, oldNpcVelX, oldNpcVelY, oldNpcVelZ);
        }

        else
        {
            SetPlayerVelocity(playerid, x, y, z);
        }
    }
    return 1;
}

CMD:setskill(playerid, params[])
{
    new skillId, level;
    if(sscanf(params, "dd", skillId, level))return SCM(playerid, -1, "/setskill [skillId] [value (0-3)]");
    {
        Skill_SetLevel(playerid, skillId, level);
        SCMF(playerid, -1, "Remember to use /showskillbars to show them later on!");
    }
    return 1;
}

CMD:setteam(playerid, params[])
{
	new targetPlayerId, teamId;
	if(sscanf(params, "dd", targetPlayerId, teamId))return SCMF(playerid, -1, "/setteam [playerid] [teamId]");
	{
		SetPlayerTeam(targetPlayerId, teamId);
	}
	return 1;
}

CMD:setingr(playerid, params[])
{
    new ingrId, ingrAmount;
    if(sscanf(params, "dd", ingrId, ingrAmount))return SCM(playerid, -1, "/setingr [ingr_id] [ingr_amount]");
    {
        SetPlayerItemAmount(playerid, ingrId, ingrId);
    }
    return 1;
}

CMD:first(playerid, params[])
{
    for(new i = 0; i != 6; i++)
    {
        DB_GivePlayerItem(i, 0, .ownerUid = eCharsInfo[playerid][e_iUid]);
    }
    return 1;
}

CMD:setsupps(playerid, params[])
{
    new suppsAmount;
    if(sscanf(params, "d", suppsAmount))return SCM(playerid, -1, "/setsupps [amount]");
    {
        SetPlayerSupplements(playerid, suppsAmount);
    }
    return 1;
}

CMD:setitem(playerid, params[])
{
    new itemId, itemAmount;
    if(sscanf(params, "dd", itemId, itemAmount))return SCM(playerid, -1, "/setitem [itemId] [itemAmount]");
    {
        SetPlayerItemAmount(playerid, itemId, itemAmount);
    }
    return 1;
}

CMD:movecam(playerid, params[])
{
    new Float:test1, Float:test2;
    if(sscanf(params, "ff", test1, test2))return SCM(playerid, -1, "/movcam [f1] [f2] (defaults: -1.1, 0.2)");
    {
        MoveCameraNextToPlayer(playerid, test1, test2, 500);
    }
    return 1;
}

CMD:ambientnoise(playerid, params[])
{
    new ambient;
    if(sscanf(params, "d", ambient))return SCM(playerid, -1, "/ambient [0/1]");
    {
        if(ambient == 0)
        {
            PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
        }
        else if(ambient == 1)
        {
            PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
        }
    }
    return 1;
}

CMD:npcupdate(playerid, params[])
{
    new npcid, update, bool:force;
    if(sscanf(params, "ddd", npcid, update, force))return SCM(playerid, -1, "/npcupdate [npcid] [0/1] [force-(0/1)]");
    {
        if(update) FCNPC_StartUpdateTimer(npcid);
        else FCNPC_StopUpdateTimer(npcid, force);
    }
    return 1;
}

CMD:allin(playerid, params[])
{
    // Ingredients.
    for(new i = 0; i <= 5; i++)
    {
        SetPlayerItemAmount(playerid, i, 300);
    }

    // Melee weapon.
    new 
        randomMelee = RandomEx(6, 9),
        weaponId = eItemsData[randomMelee][e_iItem_SA_WeaponId],
        meleeCnd = GetMeleeWeapon_Durability(weaponId, false),
        meleeExtCnd = GetMeleeWeapon_Durability(weaponId, true);

    SetPlayerItemAmount(playerid, randomMelee, meleeCnd, meleeExtCnd);

    // Other weapons.
    for(new i = 9; i != MAX_ITEMS_DATA; i++)
    {
        weaponId = eItemsData[i][e_iItem_SA_WeaponId];

        if(eItemsData[i][e_iItem_Type] == ITEM_TYPE_COLLECTIBLE 
        || eItemsData[i][e_iItem_Type] == ITEM_TYPE_AMMO
        || weaponId == WEAPON_BOTTLE)
        {
            continue;
        }

        SetPlayerItemAmount(playerid, i, GetPlayerWeaponLvl_MaxAmmo(playerid, weaponId));
    }
    return 1;
}

CMD:face(playerid, params[])
{
	new targetid;
	if(sscanf(params, "d", targetid))return SCM(playerid, -1, "/face [playerid]");
	{
    	SetPlayerToFacePlayer(playerid, targetid);
	}
	return 1;
}

CMD:anim(playerid, params[])
{
    new forPlayerid, animLib[32], animName[32], Float:aFloat, aRepeat, aX, aY, aFreeze, aTime;
    if(sscanf(params, "ds[32]s[32]fddddd", forPlayerid, animLib, animName, aFloat, aRepeat, aX, aY, aFreeze, aTime))return SCM(playerid, -1, "/anim [forPlayerid] [library] [name] [float] [powt.] [x] [y] [freeze] [time]");
    {
        ApplyAnimation(forPlayerid, animLib, animName, aFloat, aRepeat, aX, aY, aFreeze, aTime);
    }
    return 1;
}

CMD:clearanim(playerid, params[])
{
    ClearAnimations(playerid);
    return 1;
}

CMD:removeatt(playerid, params[])
{
    for(new i = 0; i != MAX_PLAYER_ATTACHED_OBJECTS; i++)
    {
        RemovePlayerAttachedObj(playerid, i);
    }
    return 1;
}