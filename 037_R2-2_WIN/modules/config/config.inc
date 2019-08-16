#include	<YSI\y_hooks>

// MAIN
#undef 		MAX_PLAYERS
#define		MAX_PLAYERS				(50)
#define		MAX_CHAT_LINES			(10)
#define		MAX_LOG_FILES			(7)

#define		MAX_PATHFINDER_THREADS	(1) // = MAX_FCNPCS (CHANGE WHEN NECESSARY SINCE I CAN'T ACCESS IT!)

// ON-CLIENT DISPLAY SETTINGS
//#define		USE_VPS
#define     SAMP_VERSION            "0.3.7-R3"

#define     GAMEMODE_HOST           "The Last of Us (SA-MP)"
#define     GAMEMODE_MODE           "Survival Horror"
#define     GAMEMODE_AREA           "Boston"
#define     GAMEMODE_WEBURL         "N/A"
#define     GAMEMODE_VERSION        "0.1.10"
#define     GAMEMODE_LANG           "English"

// FADE SCREEN TYPES
#define     FADE_NONE               (0)
#define     FADE_IN             	(1)
#define     FADE_OUT            	(2)

// LOG DATA INDEXES (keep that order, because that's an array of data).
#define 	LOG_INDEX_SERVER 		(0)
#define     LOG_INDEX_PLAYER        (1)
#define		LOG_INDEX_MYSQL			(2)
#define		LOG_INDEX_NPC			(3)
#define		LOG_INDEX_PUNISHMENTS	(4)
#define		LOG_INDEX_CHAT			(5)
#define		LOG_INDEX_CMD			(6)

//#region FILES LOCATIONS
	#define		PATH_LOG_MAIN				"server"
	#define     PATH_LOG_SERVER     		""PATH_LOG_MAIN"/server/"
	#define     PATH_LOG_PLAYERS    		""PATH_LOG_MAIN"/players/"
	#define     PATH_LOG_MYSQL        		""PATH_LOG_SERVER"/mysql/"
	#define     PATH_LOG_NPCS           	""PATH_LOG_MAIN"/npcs/"
	#define     PATH_LOG_PUNISHMENTS    	""PATH_LOG_PLAYERS"/punishments/"
	#define     PATH_LOG_CHAT       		""PATH_LOG_PLAYERS"/chat/"
	#define     PATH_LOG_CMD        		""PATH_LOG_PLAYERS"/cmd/"
//#endregion

//#region COLORS
	#define     COLOR_BLACK             	0x000000FF
	#define		COLOR_BLACK_TRANS			0x000000DD
	#define		COLOR_BG_BLACK_TRANS		0xFFFFFFDD
	#define     COLOR_WHITE             	0xFFFFFFFF
	#define		COLOR_WHITE_TRANS			0xFFFFFF88
	#define     COLOR_GRAY             		0xBDB2A4FF
	#define     COLOR_GRAY_TRANS       		0xBDB2A477
	#define		COLOR_RED					0xFF0000FF
	#define		COLOR_RED_TRANS				0xFF000077
	//#define		COLOR_DARK_RED			0xCC0000FF
	#define		COLOR_DARK_RED				0xA80000FF
	#define     COLOR_GOLD              	0xFFD700FF
	#define		COLOR_CHAT_FADE_FROM		0xE6E6E6E6
	#define		COLOR_CHAT_FADE_TO			0x6E6E6E6E

//#endregion

//#region HELPERS.
	#if !defined isnull
		#define	 	isnull(%1) 				((!(%1[0])) || (((%1[0]) == '\1') && (!(%1[1]))))
	#endif

	#define 	function%0(%1) 			forward%0(%1); public%0(%1)

	#if !defined FLOAT_INFINITY
		#define 	FLOAT_INFINITY   		(Float:0x7F800000)
	#endif

	#if !defined FLOAT_NEG_INFINITY
		#define 	FLOAT_NEG_INFINITY 		(Float:0xFF800000)
	#endif
	
	#define     SCM             		SendClientMessage
	#define     SCMF             		SendClientMessagef
	#define		SCM_ALL					SendClientMessageToAll
	#define		SCMF_ALL				SendClientMessageToAllf
	#define		CLF						CallLocalFunction
	//#define     SPAO              		SetPlayerAttachedObject

	#define 	PRESSED(%0) 			(((newkeys & (%0)) == (%0)) && ((oldkeys & (%0)) != (%0)))
	#define 	RELEASED(%0) 			(((newkeys & (%0)) != (%0)) && ((oldkeys & (%0)) == (%0)))
	#define 	HOLDING(%0) 			((newkeys & (%0)) == (%0))

	#define		KEY_AIM				KEY_HANDBRAKE
	#define		KEY_ENTER			KEY_SECONDARY_ATTACK
//#endregion

// Server enumerator.
enum e_SERVER_DATA
{
	MySQL:e_SqlHandler
}
new eServerInfo[e_SERVER_DATA];

// Log files data.
enum e_LOGS
{
	e_Log_Path[32],
	Logger:e_Log_Index[MAX_LOG_FILES]
};
new Logger:eLogs[e_LOGS];

stock const eLogsPathsData[MAX_LOG_FILES][32] =
{
	""PATH_LOG_SERVER"",
	""PATH_LOG_PLAYERS"",
	""PATH_LOG_MYSQL"",
	""PATH_LOG_NPCS"",
	""PATH_LOG_PUNISHMENTS"",
	""PATH_LOG_CHAT"",
	""PATH_LOG_CMD""
};

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
	static year, month, day;
	getdate(year, month, day);

	// Create log files instances.
	static log_filePathAndName[32];
	for(new i = 0; i != MAX_LOG_FILES; i++)
	{
		format(log_filePathAndName, sizeof(log_filePathAndName), "%s%02d-%02d-%02d", eLogsPathsData[i], year, month, day);
		eLogs[e_Log_Index][i] = CreateLog(log_filePathAndName);	
	}

	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Setting up server display settings on SA-MP Client. Please wait...");

	#if defined USE_VPS

		//SendRconCommand("hostname "GAMEMODE_HOST"");
	    SendRconCommand("gamemodetext "GAMEMODE_MODE"");
	   	SendRconCommand("mapname "GAMEMODE_AREA"");
	   	SendRconCommand("weburl "GAMEMODE_WEBURL"");

	#else
 
		SendRconCommand("hostname [DEBUG] The Last of Us (SA-MP) project"); 
	    SendRconCommand("gamemodetext DEBUG MODE");
	   	SendRconCommand("mapname San Andreas");
	   	SendRconCommand("weburl "GAMEMODE_WEBURL"");

   	#endif

	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Setting up in-game global game settings. Please wait...");

	CA_Init();
	//MapAndreas_Init(MAP_ANDREAS_MODE_FULL);
	//PathFinder_Init(MapAndreas_GetAddress(), MAX_PATHFINDER_THREADS);

	// SetWorldTime(hour);
    ShowNameTags(false);
	ShowPlayerMarkers(false);
    DisableInteriorEnterExits();
    EnableVehicleFriendlyFire();
    EnableStuntBonusForAll(false);
    AddPlayerClass(0, 0.0, 0.0, 0.0, 0.0, 0, 0, 0, 0, 0, 0);
	return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeExit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " ===================================================== ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |                                                   | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |              The Last of Us (SA-MP)               | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |   All rights reserved Copyrights to Naughty Dog   | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |                                                   | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |                 Script: Riddick                   | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |         Models: Riddick | Website: N/A            | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |                   Mapping: N/A                    | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " |                                                   | ");
	Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, " ===================================================== ");

	// Destroying logs.
	for(new i = 0; i != MAX_LOG_FILES; i++)
	{
		DestroyLog(eLogs[e_Log_Index][i]);
	}
	return 1;
}