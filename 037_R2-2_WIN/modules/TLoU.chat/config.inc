// MAXIMAL VALUES.
#define		MAX_CHAT_LINES			(10)

// TIMERS.
#define		TIMER_HIDECHAT_DELAY	(5000)

enum CP_ALIGNMENT
{
    CP_ALIGN_UNKNOWN,
    CP_ALIGN_LEFT,
    CP_ALIGN_CENTERED,
    CP_ALIGN_RIGHT
}

enum CP_POSITION
{
    CP_POS_TOP_LEFT,
    CP_POS_TOP_RIGHT,
    CP_POS_BOTTOM_LEFT
}

new PlayerCustomChatTextString[MAX_PLAYERS][MAX_CHAT_LINES][128];
new Iterator:PlayerChatMessages<MAX_PLAYERS, MAX_CHAT_LINES>;

stock const RestrictedTextCharactersList[] =
{
    "~", 
    "|", 
    "%"
};