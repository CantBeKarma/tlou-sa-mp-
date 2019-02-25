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

// Weapon slots.
#define     WEAPON_SLOT_MELEE        (0)
#define     WEAPON_SLOT_SHIV         (1)
#define     WEAPON_SLOT_MEDKIT       (2)
#define     WEAPON_SLOT_MOLOTOV      (3)
#define     WEAPON_SLOT_BOMB         (4)
#define     WEAPON_SLOT_SMOKE_BOMB   (5)
#define     WEAPON_SLOT_9MM          (6)
#define     WEAPON_SLOT_REVOLVER     (7)
#define     WEAPON_SLOT_SHORTY       (8)
#define     WEAPON_SLOT_BOW          (9)
#define		WEAPON_SLOT_SHOTGUN		 (10)
#define     WEAPON_SLOT_ASSAULT      (11)
#define     WEAPON_SLOT_RIFLE        (12) 
#define		WEAPON_SLOT_BRICK_BOTTLE (13)

// Projectiles.
#define     MAX_PROJECTILE_SPEED            (12.0)

#define     TICK_RATE_NAILBMB_UPDATE        (600)
#define     TICK_RATE_SMOKEBMB_EFFECT       (10000)
#define     TICK_RATE_PROJECTILE_THROW      (1100)

#define     NAILBMB_EXPLODE_RANGE           (2.0)
#define     SMOKEBMB_AFFECT_RANGE           (5.5)

#define     PROJ_PHYS_SPHERECOL_RADIUS      (0.19)
#define     PROJ_PHYS_GROUND_FRICTION       (50.0)
#define     PROJ_PHYS_COLL_FRICTION         (0.3)
#define     PROJ_PHYS_AIR_ASSISTANCE        (0.5)
#define     PROJ_PHYS_GRAVITY               (30.0)
#define     PROJ_PHYS_PLAYER_COL_RADIUS     (0.8)
#define     PROJ_PHYS_COLLIDE_SIMULATION    (false)
#define     PROJ_PHYS_MASS                  (1.0)

#define     PROJ_PHYSDROP_SPHERECOL_RADIUS      (0.19)
#define     PROJ_PHYSDROP_GROUND_FRICTION       (30.0)
#define     PROJ_PHYSDROP_COLL_FRICTION         (0.3)
#define     PROJ_PHYSDROP_AIR_ASSISTANCE        (0.5)
#define     PROJ_PHYSDROP_GRAVITY               (40.0)
#define     PROJ_PHYSDROP_PLAYER_COL_RADIUS     (0.8)
#define     PROJ_PHYSDROP_COLLIDE_SIMULATION    (false)
#define     PROJ_PHYSDROP_MASS                  (1.0)

#define     ARROW_RANGE_DAMAGE          (3.0)
#define		MAX_ARROW_SPEED			    (40.0)
#define		MAX_ARROW_DISTANCE		    (40.0)
#define     TICK_RATE_ARROW_SHOOT       (1250)

#define     SHIV_LABEL_DRAW_DIST        (3.0)
#define     SHIV_ATTACK_DIST            (3.0)

#define     OBJ_MODEL_NAILBOMB_SMOKE    (18673)
#define     OBJ_MODEL_FIRE_EXPLOSION    (18686)
#define     OBJ_MODEL_EXPLOSION         (18680)
#define     OBJ_MODEL_SMOKE             (18715)

// Other.
#define     MAX_WEAPON_SLOTS            (14)

#define     MIN_NOAMMO_CLICKS           (2)
#define     MAX_NOAMMO_CLICKS           (5)

// Helpers.
#define     CurrentPlayerWeaponId[%0]           ePlayerWeapon[%0][e_iWeaponId]
#define     CurrentPlayerWeaponClipSize[%0]     ePlayerWeapon[%0][e_iWeaponClipSize]
#define     CurrentPlayerWeaponAmmo[%0]         ePlayerWeapon[%0][e_iWeaponAmmo]

#define     LastPlayerWeaponId[%0]              ePlayerWeapon[%0][e_iLastWeaponId]

#define     PlayerWeaponBodyDamage[%0]          ePlayerWeapon[%0][e_fWeapon_BodyDamage]
#define     PlayerWeaponHeadDamage[%0]          ePlayerWeapon[%0][e_fWeapon_HeadDamage]
#define     PlayerWeaponSlotAmmo[%0,%1]         ePlayerWeapon[%0][e_iWeaponsAmmo][%1]

#define     IsPlayerMeleeUpgraded[%0]           ePlayerWeapon[%0][e_iWeaponMeleeUpgraded]
#define     GetPlayerMelee[%0]                  ePlayerWeapon[%0][e_iWeaponsId][WEAPON_SLOT_MELEE]
#define     GetPlayerMeleeCnd[%0]               GetPlayerWeaponAmmo(%0, GetPlayerMelee[%0])
#define     GetPlayerMeleeExtCnd[%0]            ePlayerWeapon[%0][e_iMeleeUpgradedCnd]
#define     GetPlayerShivCnd[%0]                ePlayerWeapon[%0][e_iShivCnd]

#define     GetPlayerThrowable[%0]              ePlayerWeapon[%0][e_iWeaponsId][WEAPON_SLOT_BRICK_BOTTLE]
#define     GetPlayerArrows[%0]                 GetPlayerWeaponAmmo(%0, WEAPON_BOW)
#define     GetPlayerMedkits[%0]                GetPlayerWeaponAmmo(%0, WEAPON_MEDKIT)
#define     GetPlayerShivs[%0]                  GetPlayerWeaponAmmo(%0, WEAPON_SHIV)

// Projectiles data.
enum e_SERVER_PROJECTILES_DATA
{
    e_iSmkBmbSmoke_ObjectId,
    e_iNailBmbSmoke_ObjectId,
    Timer:e_iNailBmb_Timer
};
new eProjectileData[MAX_THROWABLES][e_SERVER_PROJECTILES_DATA];

// Player weapon's data.
enum e_PLAYER_WEAPONS_DATA
{
    e_iWeaponId,
    e_iWeaponClipSize,
	e_iWeaponAmmo,

    e_iLastWeaponId,
    e_iSwitch_LastWeapon,
    e_iMeleeUpgradedCnd,
    e_iShivCnd,
    bool:e_iWeaponMeleeUpgraded,
    Float:e_fWeapon_BodyDamage,
    Float:e_fWeapon_HeadDamage,

	e_iWeaponsId[MAX_WEAPON_SLOTS],
	e_iWeaponsAmmo[MAX_WEAPON_SLOTS],
    e_iWeaponClipAmmo[MAX_WEAPON_SLOTS],
    e_iWeaponsUpgradeLvl_ClipSize[MAX_WEAPON_SLOTS],
    
    e_iArrowObjectId,

    e_iTickCounter_bowShooting,
    e_iTickCounter_projectileThrow,
    e_iTickCounter_NoAmmoClicks
};
new ePlayerWeapon[MAX_PLAYERS][e_PLAYER_WEAPONS_DATA];

//new Iterator:PlayerWeapons[MAX_PLAYERS]<MAX_WEAPON_SLOTS, MAX_WEAPON_SLOTS>;

// new 
//     Iterator:PlayerWeapons<MAX_PLAYERS, MAX_WEAPON_SLOTS>,
//     Iterator:PlayerWeaponsAmmo<MAX_PLAYERS, MAX_WEAPON_SLOTS>;