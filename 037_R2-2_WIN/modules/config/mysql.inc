#include	<YSI\y_hooks>

//#define		USE_VPS

// MYSQL SERVER DETAILS
#if defined USE_VPS

	#define     MYSQL_HOST              "127.0.0.1"
	#define     MYSQL_USER              "server_536"
	#define     MYSQL_DATA              "server_536_tlou"
	#define     MYSQL_PASS              "kbq815rapf"

#else

	#define     MYSQL_HOST              "localhost"
	#define     MYSQL_USER              "root"
	#define     MYSQL_DATA              "tlou_prd"
	#define     MYSQL_PASS              ""

#endif

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    mysql_log(ALL);

	new MySQLOpt: option_id = mysql_init_options();
	mysql_set_option(option_id, AUTO_RECONNECT, true);
    
	Log(eLogs[e_Log_Index][LOG_INDEX_MYSQL], INFO, "Establishing connection to MySQL server...");
	eServerInfo[e_SqlHandler] = mysql_connect(MYSQL_HOST, MYSQL_USER, MYSQL_PASS, MYSQL_DATA);

	Log(eLogs[e_Log_Index][LOG_INDEX_MYSQL], 
		(mysql_errno() != 0) 
			? (ERROR) 
			: (INFO), 
			"%s", 
		(mysql_errno() != 0) 
			? "Couldn't connect to the database!" 
			: "Connection to the SQL successfully established!");

    return 1;
}