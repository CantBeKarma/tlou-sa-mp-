// Weapon reasons (not defined by default).
#define 	WEAPON_VEHICLE_M4 		(19)
#define 	WEAPON_VEHICLE_MINIGUN  (20)
#define		WEAPON_PISTOLWHIP 		(48)
#define	 	WEAPON_HELIBLADES 		(50)
#define     WEAPON_EXPLOSION        (51)
#define 	WEAPON_CARPARK 			(52)
#define     WEAPON_COLLISION        (54)
#define     WEAPON_FIRE				WEAPON_FLAMETHROWER

// Weapon IDs.
#define     NO_MELEE                (0)
#define     NO_THROWABLE            (0)
#define     WEAPON_UNARMED          (0)
#define     WEAPON_SHIV             (205)
#define     WEAPON_MEDKIT           (206)
#define     WEAPON_BRICK            (207)
#define     WEAPON_BOTTLE           (212)
#define     WEAPON_MOLOTOV          (208)
#define     WEAPON_NAIL_BOMB        (209)
#define     WEAPON_SMOKE_BOMB       (210)
#define		WEAPON_BOW				(211)
#define     WEAPON_PIPE             WEAPON_BAT
#define     WEAPON_SCISSOR_PIPE     WEAPON_SHOVEL
#define     WEAPON_HATCHET          WEAPON_DILDO
#define     WEAPON_MACHETE          WEAPON_VIBRATOR
#define     WEAPON_9MM              WEAPON_SILENCED
#define     WEAPON_REVOLVER         WEAPON_DEAGLE
#define     WEAPON_SHORTY			WEAPON_SAWEDOFF
#define     WEAPON_ASSAULT          WEAPON_M4

// Other.
#define     MIN_NOAMMO_CLICKS           (2)
#define     MAX_NOAMMO_CLICKS           (5)

#define     TICK_RATE_HIDE_CROSSHAIR_SYMBOL     (200)

#define     PlayerWeaponBodyDamage[%0]  ePlayerWeapon[%0][e_fWeapon_BodyDamage]
#define     PlayerWeaponHeadDamage[%0]  ePlayerWeapon[%0][e_fWeapon_HeadDamage]

// Player's currently equipped weapon data.
enum e_PLAYER_WEAPONS_DATA
{
    Float:e_fWeapon_BodyDamage,
    Float:e_fWeapon_HeadDamage,

    e_iTickCounter_bowString,
    e_iTickCounter_projectileThrow,
    e_iTickCounter_NoAmmoClicks, 

    bool:e_bAimingArrow
};
new ePlayerWeapon[MAX_PLAYERS][e_PLAYER_WEAPONS_DATA];