// -> Skins.
#define     SKIN_JOEL               (312)
#define     SKIN_ELLIE              (313)
#define     SKIN_ELLIE2             (314)
#define     SKIN_TESS               (315)
#define     SKIN_SARAH              (316)
#define     SKIN_HAZMAT             (317)
#define     SKIN_CLICKER            (318)
#define     SKIN_RUNNER1            (319)

enum e_CHARS_DATA
{
	e_iUid,
	e_iSkinId,
	Float:e_fHealth,
	Float:e_fPosX,
	Float:e_fPosY,
	Float:e_fPosZ,
	Float:e_fPosAng,
	e_iPosInt,
	e_iPosVw,

	e_iSkills_SuppsAmount,
	e_iSkillLevel[MAX_SKILLS_ITEMS],

	e_iAttachSlot_Dynamic
}
new eCharsInfo[MAX_PLAYERS][e_CHARS_DATA];