//#region Register & Login

    Dialog_ShowRegister(playerid)
    {
        static tit_signUp[8] = "Sign up";
        static msg_signUp[142] = "Welcome to the \"The Last of Us (SA-MP) Project\"\nWe haven't found you in our database. Please enter your account password below to sign up:";

        inline Response(pid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused pid, dialogid, response, listitem, inputtext
            if(!response)
            {
                Kick(playerid);
                return false;
            }

            if(!ValidatePassword(inputtext))
            {
                new msg_signUpFailed[142 + 170];
                strcat(msg_signUpFailed, msg_signUp, sizeof(msg_signUpFailed));
                strcat(msg_signUpFailed, "\n\n{FF0000}You've either entered too short/long password (min. 5, max. 20) OR you've used restricted characters!", sizeof(msg_signUpFailed));
                strcat(msg_signUpFailed, "\n{FF0000}Allowed text characters: a-z, A-Z, 0-9, !?@#$", sizeof(msg_signUpFailed));

                Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signUp, msg_signUpFailed, "Sign up", "Quit");
                return false;
            }
            HashPlayerPassword(playerid, inputtext);
        }
        Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signUp, msg_signUp, "Sign up", "Quit");
        return true;
    }

    Dialog_ShowLogin(playerid, bool:wrongPassword = false)
    {
        static tit_signIn[8] = "Sign in";
        static msg_signIn[144] = "Welcome back to the \"The Last of Us (SA-MP) Project\"\nWe have found you in our database. Please enter your account password below to sign in:";
        new msg_signInFailed[144 + 41];
        
        if(wrongPassword)
        {
            strcat(msg_signInFailed, msg_signIn, sizeof(msg_signInFailed));
            strcat(msg_signInFailed, "\n\n\t\t\t\t       {FF0000}Wrong password!", sizeof(msg_signInFailed));
        }

        inline Response(pid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused pid, dialogid, listitem
            if(!response)
            {
                Kick(playerid);
                return false;
            }

            if(isnull(inputtext))
            {
                Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signIn, msg_signIn, "Sign in", "Quit");
                return false;
            }

            DB_TryLoginPlayerToAccount(playerid, inputtext);
        }
        Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_PASSWORD, tit_signIn, (wrongPassword) ? (msg_signInFailed) : (msg_signIn), "Sign in", "Quit");
        wrongPassword = false;
        return true;
    }

//#endregion

//#region Collectible creation (artifacts/notes)

    Dialog_EnterArtifactTitle(playerid)
    {
        static caption_createTitle[17] = "Artifact's title";
        static msg_createTitle[35] = "Enter your artifact's title below:";

        inline Response(pid, dialogid, response, listitem, string:inputtext[])
        {
            #pragma unused pid, dialogid, listitem
            if(!response)
            {
                ePlayerCmdData[playerid][e_sCreateNoteTitle] = EOS;
                ePlayerCmdData[playerid][e_sCreateNoteInnerMsg] = EOS;
                ePlayerCmdData[playerid][e_iCreateNotePageCount] = 0;
                return true;
            }

            if(response)
            {
                if(isnull(inputtext) || strlen(inputtext) > MAX_COLL_TIT_LEN)
                {
                    new msg_createTitleFailed[sizeof(msg_createTitle) + 68];
                    strcat(msg_createTitleFailed, msg_createTitle, sizeof(msg_createTitleFailed));
                    strcat(msg_createTitleFailed, "\n{FF0000}Invalid input! Title cannot be longer than 32 characters", sizeof(msg_createTitleFailed));

                    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, caption_createTitle, msg_createTitleFailed, "Done", "Cancel");
                    return false;
                }

                strcpy(ePlayerCmdData[playerid][e_sCreateNoteTitle], inputtext, MAX_COLL_TIT_LEN);

                Dialog_EnterArtifactInnerMsg(playerid);
            }
        }
        Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, caption_createTitle, msg_createTitle, "Done", "Cancel");
        return true;
    }

    Dialog_EnterArtifactInnerMsg(playerid)
    {
        static caption_createInnerMsg[19] = "Artifact's message";

        new msg_createInnerMsg[42 + 50];
        new playerMsgLen = strlen(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]);
        format(msg_createInnerMsg, sizeof(msg_createInnerMsg), "Enter your artifact's message (MAX: 512):\n\nYour current characters count is: %s%d", \
                    (playerMsgLen >= 0 && playerMsgLen < 256) ? "{00FF00}" 
                : (playerMsgLen >= 256 && playerMsgLen < 384) ? "{FFFF00}" 
                : "{FF0000}", playerMsgLen);

        new buttonName1[12];
        format(buttonName1, sizeof(buttonName1), "%s", (ePlayerCmdData[playerid][e_iCreateNotePageCount] < 4) ? "Next" : "Finish");
        if(!strcmp(buttonName1, "Next", false))
        {
            format(buttonName1, sizeof(buttonName1), "Next (%d/4)", ePlayerCmdData[playerid][e_iCreateNotePageCount]);
        }

        inline Response(pid, dialogid, response, listitem, string:inputtext[])
        {
            if(!response)
            {
                ePlayerCmdData[playerid][e_sCreateNoteTitle] = EOS;
                ePlayerCmdData[playerid][e_sCreateNoteInnerMsg] = EOS;
                ePlayerCmdData[playerid][e_iCreateNotePageCount] = 0;
                return true;
            }

            #pragma unused pid, dialogid, listitem
            if(ePlayerCmdData[playerid][e_iCreateNotePageCount] >= 4)
            {
                new playerInnerMsgLen = strlen(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]);
                new createItemId = (playerInnerMsgLen < 128) ? (0) : (1); // 0 - little piece of paper, 1 - two books next to each other.

                if(CreateCollectible(createItemId, ePlayerCmdData[playerid][e_sCreateNoteTitle], ePlayerCmdData[playerid][e_sCreateNoteInnerMsg], .creatorId = playerid))
                {
                    ShowDialogMessage(playerid, "Create collectible", "Your note has been created!");
                }

                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_PaperNote]);
                TextDrawHideForPlayer(playerid, eTextDrawInfo[e_Bp_Rs_Coll_MessageText][playerid]);

                ePlayerCmdData[playerid][e_sCreateNoteTitle] = EOS;
                ePlayerCmdData[playerid][e_sCreateNoteInnerMsg] = EOS;
                ePlayerCmdData[playerid][e_iCreateNotePageCount] = 0;
                return true;
            }

            if(response)
            {
                new msg_createInnerMsgFailed[sizeof(msg_createInnerMsg) + 95];
                if(isnull(inputtext) || (isnull(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]) && strlen(inputtext) < MIN_COLL_MSG_LEN))
                {
                    strcat(msg_createInnerMsgFailed, msg_createInnerMsg, sizeof(msg_createInnerMsgFailed));
                    strcat(msg_createInnerMsgFailed, "\n\n{FF0000}Invalid input!\nMessage cannot be shorter than 64 AND longer than 512 characters!", sizeof(msg_createInnerMsgFailed));
                }

                if(isnull(inputtext) && isnull(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]))
                {
                    Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, caption_createInnerMsg, msg_createInnerMsgFailed, buttonName1, "Cancel");
                    return false;
                }

                if(isnull(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]))
                {
                    if(strlen(inputtext) < MIN_COLL_MSG_LEN)
                    {
                        Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, caption_createInnerMsg, msg_createInnerMsgFailed, buttonName1, "Cancel");
                        return false;
                    }
                }

                if(!isnull(inputtext))
                {
                    strcat(ePlayerCmdData[playerid][e_sCreateNoteInnerMsg], inputtext, MAX_COLL_MSG_LEN);
                    DisplayCollectibleContents(playerid, COLL_ITEM_ARTIFACTS, ePlayerCmdData[playerid][e_sCreateNoteTitle], ePlayerCmdData[playerid][e_sCreateNoteInnerMsg]);
                }

                ePlayerCmdData[playerid][e_iCreateNotePageCount] += 1;
                Dialog_EnterArtifactInnerMsg(playerid);
            }
        }
        Dialog_ShowCallback(playerid, using inline Response, DIALOG_STYLE_INPUT, caption_createInnerMsg, msg_createInnerMsg, buttonName1, "Cancel");
        return true;
    }

//#endregion