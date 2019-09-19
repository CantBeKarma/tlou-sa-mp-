#include	<YSI\y_hooks>

// Password requirments.
#define		MIN_PASSWORD_LEN	(5)
#define		MAX_PASSWORD_LEN	(20)

#define		BCRYPT_COST			        (12)
#define     MAX_BCRYPT_THREAD_LIMIT     (3)

new const AllowedPasswordCharacters[21] = "a-z, A-Z, 0-9, !?@#$";

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
	bcrypt_set_thread_limit(MAX_BCRYPT_THREAD_LIMIT);
	return 1;
}