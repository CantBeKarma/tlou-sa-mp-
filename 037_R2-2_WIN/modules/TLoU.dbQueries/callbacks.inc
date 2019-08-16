//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//---------------------- OnQueryError ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
public OnQueryError(errorid, const error[], const callback[], const query[], MySQL:handle)
{
	switch(errorid)
	{
		case CR_SERVER_GONE_ERROR:
		{
			Log(eLogs[e_Log_Index][LOG_INDEX_MYSQL], ERROR, "Lost connection to server, trying reconnect...");
		}
		case ER_SYNTAX_ERROR:
		{
			Log(eLogs[e_Log_Index][LOG_INDEX_MYSQL], ERROR, "Wrong SQL syntax. Callback: %s | Query: %s", callback, query);
		}
	}
	return 1;
}