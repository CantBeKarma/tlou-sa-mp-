// -> Skins.
#define     SKIN_JOEL               (312)
#define     SKIN_ELLIE              (313)
#define     SKIN_ELLIE2             (314)
#define     SKIN_TESS               (315)
#define     SKIN_SARAH              (316)
#define     SKIN_HAZMAT             (317)
#define     SKIN_CLICKER            (318)

// -> Attachment slots.
// #define     SLOT_BACKPACK    		(0)
// #define     SLOT_BURNING			(1)
// #define     SLOT_WEAPON_PISTOL      (1)
// #define     SLOT_WEAPON_REVOLVER    (2)
// #define     SLOT_WEAPON_LONGGUN1    (3)
// #define		SLOT_WEAPON_LONGGUN2    (4)
// #define     SLOT_WEAPON_MELEE       (5)
// #define     SLOT_GASMASK            (6)
// #define     SLOT_FLASHLIGHT         (8)
// #define     SLOT_DYNAMIC            (9)

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

	e_iAttachSlot_Backpack,
	e_iAttachSlot_9mm,
	e_iAttachSlot_Revolver,
	e_iAttachSlot_LongGunOne,
	e_iAttachSlot_LongGunTwo,
	e_iAttachSlot_Melee,
	e_iAttachSlot_GasMask,
	e_iAttachSlot_Dynamic
}
new eCharsInfo[MAX_PLAYERS][e_CHARS_DATA];