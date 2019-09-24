CMD:cmd(playerid, params[])
{
    static icmdTip[73] = "Type: /icmd (command_name) to find more information about a command.\n\n";
    static cmdListRow1[67] = "/ls, /hp, /kill, /unfreeze, /anim, /clearanim, /skin, /windnoise\n";
    static cmdListRow2[52] = "/itemlist, /createitem, /createmelee, /createnote\n";
    static cmdListRow3[47] = "/giveitem, /givemelee, /myitems, /removeitem\n";
    static cmdListRow4[43] = "/skills, /myskills, /setskill, /setsupps\n";
    static cmdListRow5[66] = "/ugradeitems, /setitemlvl, /ingrs, /allin, /setteam, /removeatt";

    new string[ sizeof(icmdTip) + sizeof(cmdListRow1) + sizeof(cmdListRow2) + sizeof(cmdListRow3) + sizeof(cmdListRow4) + sizeof(cmdListRow5) + 1];
    strcat(string, icmdTip, sizeof(string));
    strcat(string, cmdListRow1, sizeof(string));
    strcat(string, cmdListRow2, sizeof(string));
    strcat(string, cmdListRow3, sizeof(string));
    strcat(string, cmdListRow4, sizeof(string));
    strcat(string, cmdListRow5, sizeof(string));

    ShowDialogMessage(playerid, "Available commands list", string);
    return 1;
}

YCMD:icmd(playerid, params[], help)
{
    if(help)
    {
        return ShowDialogMessage(playerid, "icmd > icmd", "Shows information about the required server command.\n/cmd - for all server commands.");
    }

    if(isnull(params))
    {
        ShowDialogMessage(playerid, "Usage..", "Type: /icmd (command_name)");
    }
    else
    {
        Command_ReProcess(playerid, params, true);
    }
    return 1;
}

//#region ROW1

    CMD:ls(playerid, params[])
    {
        SetPlayerPos(playerid, 2466.2358,-1659.0228,13.2723);
        return 1;
    }

    CMD:hp(playerid, params[])
    {
        new Float:hp;
        if(sscanf(params, "f", hp))return ShowDialogMessage(playerid, "Usage..", "Type: /hp (health_points)");
        {
            SetPlayerHealthEx(playerid, hp);
        }
        return 1;
    }

    CMD:kill(playerid, params[])
    {
        SetPlayerHealthEx(playerid, 0.0);
        return 1;
    }

    CMD:unfreeze(playerid, params[])
    {
        UnfreezePlayer(playerid);
        return 1;
    }

    CMD:anim(playerid, params[])
    {
        new forPlayerid, animLib[32], animName[32], Float:aFloat, aRepeat, aX, aY, aFreeze, aTime;
        if(sscanf(params, "ds[32]s[32]fddddd", forPlayerid, animLib, animName, aFloat, aRepeat, aX, aY, aFreeze, aTime))return ShowDialogMessage(playerid, "Usage..", "Type: /anim [forPlayerid] [library] [name] [float] [powt.] [x] [y] [freeze] [time]");
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

    YCMD:skin(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > skin", "Changes your character model.\nCustom TLoU models start from ID: 312");
        }

        new skinId;
        if(sscanf(params, "d", skinId))return ShowDialogMessage(playerid, "Usage..", "Type: /skin (skin_id)");
        {
            SetPlayerSkinEx(playerid, skinId);
        }
        return 1;
    }

    CMD:windnoise(playerid, params[])
    {
        new windNoise;
        if(sscanf(params, "d", windNoise))return ShowDialogMessage(playerid, "Usage..", "Type: /windnoise (0/1)");
        {
            if(windNoise == 0)
            {
                PlayerPlaySound(playerid, 1, 0.0, 0.0, 0.0);
            }
            else if(windNoise == 1)
            {
                PlayerPlaySound(playerid, 0, 0.0, 0.0, 0.0);
            }
        }
        return 1;
    }

//#endregion

//#region ROW2

    CMD:itemlist(playerid, params[])
    {
        new string[350];
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            format(string, sizeof(string), "%s%d\t%s\n", string, i, eItemsData[i][e_sItem_Name]);
        }

        ShowDialogMessage(playerid, "Available items list", string);
        return 1;
    }

    YCMD:createitem(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > createitem", "Creates item at your position. Available items list: /itemlist");
        }

        new itemId, itemAmount;
        if(sscanf(params, "dd", itemId, itemAmount))return ShowDialogMessage(playerid, "Usage..", "Type: /createitem (item_id) (amount)");
        {
            if(IsItemType(itemId, ITEM_TYPE_MELEE))
            {
                return ShowDialogMessage(playerid, "WARNING!", "Please use /createmelee command to create a melee item.");
            }

            CreateOrUpdateItem(itemId, itemAmount, .creatorId = playerid);
        }
        return 1;
    }

    YCMD:createmelee(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > createmelee", "Creates melee item at your position. Available items list: /itemlist");
        }

        new itemId, meleeCnd, meleeExtraCnd;
        if(sscanf(params, "ddd", itemId, meleeCnd, meleeExtraCnd))return ShowDialogMessage(playerid, "Usage..", "Type: /createmelee (item_id) (CND) (Upgraded_CND = 0 by default)");
        {
            if(!IsItemType(itemId, ITEM_TYPE_MELEE))
            {
                return ShowDialogMessage(playerid, "WARNING!", "Please use /createitem command to create other items than melee.");
            }

            CreateOrUpdateItem(itemId, meleeCnd, meleeExtraCnd, playerid);
        }
        return 1;
    }

    YCMD:createnote(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > createnote", "Creates collectible item.");
        }

        Dialog_EnterArtifactTitle(playerid);
        ApplyAnimation(playerid, "BOMBER", "BOM_PLANT_IN", 4.0, 0, 1, 1, 1, 0, 1);
        return 1;
    }

    CMD:skills(playerid, params[])
    {
        new skillName[32];
        new string[350];

        for(new i = 0; i != MAX_SKILLS_ITEMS; i++)
        {
            switch(i)
            {
                case 0: skillName = "MAX HP";
                case 1: skillName = "LISTEN MODE DISTANCE";
                case 2: skillName = "CRAFTING SPEED";
                case 3: skillName = "HEALING SPEED";
                case 4: skillName = "WEAPON SWAY";
                case 5: skillName = "SHIV MASTER";
            }
            format(string, sizeof(string), "%s%s | ID: %d | MAX LVL: %d\n", string, skillName, i, Skill_GetMaxLevel(i));
        }
        ShowDialogMessage(playerid, "Skills list", string);
        return 1;
    }

    CMD:myskills(playerid, params[])
    {
        new skillName[32];
        new string[350];
        
        for(new i = 0; i != MAX_SKILLS_ITEMS; i++)
        {
            switch(i)
            {
                case 0: skillName = "MAX HP";
                case 1: skillName = "LISTEN MODE DISTANCE";
                case 2: skillName = "CRAFTING SPEED";
                case 3: skillName = "HEALING SPEED";
                case 4: skillName = "WEAPON SWAY";
                case 5: skillName = "SHIV MASTER";
            }
            format(string, sizeof(string), "%s%s - Level: %d\n", string, skillName, Skill_GetPlayerLevel(playerid, i));
        }
        ShowDialogMessage(playerid, "Your skill levels", string);
        return 1;
    }

    YCMD:setskill(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > setskill", "Changes character skill (from skill tree). /skills");
        }

        new skillId, level;
        if(sscanf(params, "dd", skillId, level))return ShowDialogMessage(playerid, "Usage..", "/setskill (skill_id) (level)");
        {
            Skill_SetPlayerLevel(playerid, skillId, level);
            SkillsTd_FillPlayerSkillBars(playerid);
        }
        return 1;
    }

    YCMD:setsupps(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > setsupps", "Changes your amount of supplements to increase your skill level in backpack.");
        }

        new suppsAmount;
        if(sscanf(params, "d", suppsAmount))return ShowDialogMessage(playerid, "Usage..", "Type: /setsupps (amount)");
        {
            SetPlayerSupplements(playerid, suppsAmount);
        }
        return 1;
    }

//#endregion

//#region ROW3

    YCMD:giveitem(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > giveitem", "Instead of creating item, you can give it to yourself. Available items list: /itemlist");
        }

        new itemId, amount;
        if(sscanf(params, "dd", itemId, amount))return ShowDialogMessage(playerid, "Usage..", "Type: /giveitem (item_id) (amount)");
        {
            if(IsItemType(itemId, ITEM_TYPE_MELEE))
            {
                return ShowDialogMessage(playerid, "WARNING!", "Please use /givemelee command to give a melee item.");
            }

            GivePlayerItem(playerid, itemId);
            SetPlayerItemAmount(playerid, itemId, amount);
        }
        return 1;
    }

    YCMD:givemelee(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > givemelee", "Instead of creating melee, you can give it to yourself. Available items list: /itemlist");
        }

        new itemId, meleeCnd, meleeExtraCnd;
        if(sscanf(params, "ddd", itemId, meleeCnd, meleeExtraCnd))return ShowDialogMessage(playerid, "Usage..", "Type: /givemelee (item_id) (CND) (Upgraded_CND = 0 by default)");
        {
            if(!IsItemType(itemId, ITEM_TYPE_MELEE))
            {
                return ShowDialogMessage(playerid, "WARNING!", "Please use /giveitem command to give other items than melee.");
            }

            GivePlayerItem(playerid, itemId);
            SetPlayerItemAmount(playerid, itemId, meleeCnd, meleeExtraCnd);
        }
        return 1;
    }

    CMD:myitems(playerid, params[])
    {
        new string[1024];
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(!HasPlayerGotItem(playerid, i))
            {
                continue;
            }

            if(IsItemType(i, ITEM_TYPE_MELEE))
            {
                format(string, sizeof(string), "%s%s (ID: %d) (CND: %d) (Upgraded CND: %d)\n", string, eItemsData[i][e_sItem_Name], i, GetPlayerItemAmount(playerid, i), GetPlayerMeleeExtraCnd(playerid, i));
            }
            else
            {
                format(string, sizeof(string), "%s%s (ID: %d) (Amount: %d) (Upgrade LVL: %d)\n", string, eItemsData[i][e_sItem_Name], i, GetPlayerItemAmount(playerid, i), GetPlayerItemUpgradeLvl(playerid, i, UPGRADE_TYPE_CLIPSIZE));
            }
        }

        ShowDialogMessage(playerid, "My items", string);
        return 1;
    }

    YCMD:removeitem(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > removeitem", "To check your items use: /myitems.\nrecreate_in_world param means if you want to create item on the ground once removed");
        }

        new itemId, recreate;
        if(sscanf(params, "dd", itemId, recreate))return ShowDialogMessage(playerid, "Usage..", "/removeitem (item_id) (recreate_in_world - 0/1)");
        {
            RemovePlayerItem(playerid, itemId, (recreate == 1) ? (true) : (false));
        }
        return 1;
    }

//#endregion

//#region ROW4

    CMD:upgradeitems(playerid, params[])
    {
        new string[350];
        for(new i = 0; i != MAX_ITEMS_DATA; i++)
        {
            if(!IsItemUpgradeable(i, UPGRADE_TYPE_CLIPSIZE))
            {
                continue;
            }

            format(string, sizeof(string), "%s%s\tID: %d\t|\tMAX LVL: %d\n", string, eItemsData[i][e_sItem_Name], i, GetItemMaxUpgradeLvl(i, UPGRADE_TYPE_CLIPSIZE));
        }

        ShowDialogMessage(playerid, "Available items list", string);
        return 1;
    }

    YCMD:setitemlvl(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > setitemlvl", "Sets specified item upgrade level (clip size). Check: /upgradeitems");
        }
        
        new itemId, level;
        if(sscanf(params, "dd", itemId, level))return ShowDialogMessage(playerid, "Usage..", "Type: /setitemlvl (item_id) (level)");
        {
            if(!SetPlayerItemUpgradeLvl(playerid, itemId, level, UPGRADE_TYPE_CLIPSIZE))
            {
                ShowDialogMessage(playerid, "WARNING", "Either wrong item id or level");
            }
        }
        return 1;
    }

    CMD:ingrs(playerid, params[])
    {
        for(new i = INGR_ITEM_BLADE; i <= INGR_ITEM_SUGAR; i++)
        {
            GivePlayerItem(playerid, i);
            SetPlayerItemAmount(playerid, i, 300);
        }
        ShowDialogMessage(playerid, "INFO", "You were given all ingredients.");
        return 1;
    }

    CMD:allin(playerid, params[])
    {
        for(new i = 0; i < MAX_ITEMS_DATA; i++)
        {
            if(!GivePlayerItem(playerid, i))
            {
                continue;
            }

            new 
                maxItemAmount = 0,
                maxMeleeCnd = 0;                

            if(IsItemType(i, ITEM_TYPE_MELEE))
            {
                maxItemAmount = GetMeleeItemMaxCND(i, .extraCnd = false);
                maxMeleeCnd = GetMeleeItemMaxCND(i, .extraCnd = true);
            }
            else
            {
                maxItemAmount = GetPlayerItemMaxAmount(playerid, i);
            }
                
            SetPlayerItemAmount(playerid, i, maxItemAmount, maxMeleeCnd);
        }

        ShowDialogMessage(playerid, "INFO", "You were given all possible items and their maximal amounts.");
        return 1;
    }

    YCMD:setteam(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > setteam", "Changes specified player his team. Players on the same team cannot kill eachother.");
        }
        new targetPlayerId, teamId;
        if(sscanf(params, "dd", targetPlayerId, teamId))return ShowDialogMessage(playerid, "Usage..", "/setteam (playerid) (team_id)");
        {
            SetPlayerTeam(targetPlayerId, teamId);
        }
        return 1;
    }

    YCMD:removeatt(playerid, params[], help)
    {
        if(help)
        {
            return ShowDialogMessage(playerid, "icmd > removeatt", "Removes all your attached objects to character.");
        }

        for(new i = 0; i != MAX_PLAYER_ATTACHED_OBJECTS; i++)
        {
            RemovePlayerAttachedObj(playerid, i);
        }
        return 1;
    }

//#endregion

//#region DEBUG CMDs

    CMD:movecam(playerid, params[])
    {
        new Float:test1, Float:test2;
        if(sscanf(params, "ff", test1, test2))return SCM(playerid, -1, "/movcam [f1] [f2] (defaults: -1.1, 0.2)");
        {
            MoveCameraNextToPlayer(playerid, test1, test2, 500);
        }
        return 1;
    }

    new dynamicNpcsCount = 0;

    CMD:npc(playerid, params[])
    {
        new option[32], npcid, NPC_TYPE:type;
        if(sscanf(params, "s[32]D(-1)D(-1)", option, npcid, _:type))return SCM(playerid, -1, "/npc [create/spawn/type/move]");
        {
            if(!strcmp(option, "create", false))
            {
                new npcName[32];
                format(npcName, sizeof(npcName), "FCNPC_Dyn_%d", dynamicNpcsCount);
                FCNPC_Create(npcName);

                dynamicNpcsCount++;
                SCMF(playerid, -1, "Created %s", npcName);
            }
            else if(!strcmp(option, "spawn", false))
            {
                if(npcid == -1 || npcid == INVALID_PLAYER_ID)
                {
                    SCMF(playerid, -1, "/npc spawn [id]");
                    return 1;
                }

                new Float:x, Float:y, Float:z;
                GetPlayerPos(playerid, x, y, z);
                FCNPC_Spawn(npcid, FCNPC_GetSkin(npcid), x, y, z);
            }
            else if(!strcmp(option, "type", false))
            {
                if(npcid == -1 || npcid == INVALID_PLAYER_ID)
                {
                    SCMF(playerid, -1, "/npc spawn [id] [type]");
                    return 1;
                }
                else if(_:type == -1)
                {
                    SCMF(playerid, -1, "/npc spawn [id] [type]");
                    return 1;
                }

                FCNPC_SetType(npcid, type);
            }
            else if(!strcmp(option, "move", false))
            {
                if(npcid == -1 || npcid == INVALID_PLAYER_ID)
                {
                    SCMF(playerid, -1 , "/npc move [id]");
                    return 1;
                }

                new Float:x, Float:y, Float:z;
                GetPlayerPos(playerid, x, y, z);

                switch(random(2))
                {
                    case 0: FCNPC_RunTo(npcid, x, y, z);
                    case 1: FCNPC_WalkTo(npcid, x, y, z);
                }
            }
        }
        return 1;
    }

    CMD:npcstatus(playerid, params[])
    {
        new npcid, NPC_STATUS:status;
        if(sscanf(params, "dd", npcid, _:status))return SCM(playerid, -1, "/npcstatus [id] [status]");
        {
            FCNPC_SetStatus(npcid, status);
        }
        return 1;
    }

    CMD:mystatus(playerid, params[])
    {
        new PLAYER_STATUS:status;
        if(sscanf(params, "d", _:status))return SCM(playerid, -1, "/mystatus [status]");
        {
            SetPlayerStatus(playerid, status);
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

    CMD:align(playerid, params[])
    {
        new align[10];

        if(sscanf(params, "s[10]", align))
            return SCM(playerid, -1, "/align [left/right/center]");

        if(strcmp(align, "left", false) == 0)
        {
            SetPlayerChatPanelAlignment(playerid, CP_ALIGNMENT:CP_ALIGN_LEFT);
        }
        else if(strcmp(align, "right", false) == 0)
        {
            SetPlayerChatPanelAlignment(playerid, CP_ALIGNMENT:CP_ALIGN_RIGHT);
        }
        else if(strcmp(align, "center", false) == 0)
        {
            SetPlayerChatPanelAlignment(playerid, CP_ALIGNMENT:CP_ALIGN_CENTERED);
        }
        
        return 1;
    }

    CMD:chatpos(playerid, params[])
    {
        new position[10], position2[10];

        if(sscanf(params, "s[10]s[10]", position, position2))
            return SCM(playerid, -1, "/chatpos [top/bottom] [left/right]");


        if(strcmp(position, "top", false) == 0)
        {
            if(strcmp(position2, "left", false) == 0)
            {
                SetPlayerChatPosition(playerid, CP_POSITION:CP_POS_TOP_LEFT);
            }
            else if(strcmp(position2, "right", false) == 0)
            {

            }
        }
        else if(strcmp(position, "bottom", false) == 0)
        {
            if(strcmp(position2, "left", false) == 0)
            {
                SetPlayerChatPosition(playerid, CP_POSITION:CP_POS_BOTTOM_LEFT);
            }
            else if(strcmp(position2, "right", false) == 0)
            {

            }
        }
        
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
            }
            else
            {
                SetPlayerVelocity(playerid, x, y, z);
            }
        }
        return 1;
    }

//#endregion