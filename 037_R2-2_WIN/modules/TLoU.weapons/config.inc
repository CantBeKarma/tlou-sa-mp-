//#region Weapon reasons (not defined by default).
    #define 	WEAPON_VEHICLE_M4 		(19)
    #define 	WEAPON_VEHICLE_MINIGUN  (20)
    #define		WEAPON_PISTOLWHIP 		(48)
    #define	 	WEAPON_HELIBLADES 		(50)
    #define     WEAPON_EXPLOSION        (51)
    #define 	WEAPON_CARPARK 			(52)
    #define     WEAPON_COLLISION        (54)
    #define     WEAPON_FIRE				WEAPON_FLAMETHROWER
//#endregion

//#region Weapon IDs.
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
//#endregion

//#region Projectiles.
    #define     MAX_PROJECTILE_SPEED            (12.0)

    #define     TICK_RATE_NAILBMB_UPDATE        (600)
    #define     NAILBMB_TRIGGERED_EXPLODE       (750)
    #define     TICK_RATE_SMOKEBMB_EFFECT       (10000)
    #define     TICK_RATE_PROJECTILE_THROW      (1100)

    #define     NAILBOMB_INITIATE_RANGE         (2.0)
    #define     NAILBOMB_EXPLODE_RANGE          (5.0)
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
    //#define		MAX_ARROW_SPEED			    (40.0)
    #define		MAX_ARROW_DISTANCE		    (40.0)
    #define     MAX_ARROW_STRETCH_TIME      (2000)

    #define     SHIV_LABEL_DRAW_DIST        (3.0)
    #define     SHIV_ATTACK_DIST            (3.0)

    #define     OBJ_MODEL_NAILBOMB_SMOKE    (18673)
    #define     OBJ_MODEL_FIRE_EXPLOSION    (18686)
    #define     OBJ_MODEL_EXPLOSION         (18680)
    #define     OBJ_MODEL_SMOKE             (18715)
//#endregion

// Other.
#define     MAX_WEAPON_SLOTS            (14)

#define     MIN_NOAMMO_CLICKS           (2)
#define     MAX_NOAMMO_CLICKS           (5)

#define		MAX_THROWABLES			(MAX_PLAYERS * 3)
#define		MAX_ARROWS				(MAX_PLAYERS * 9)

#define     TICK_RATE_HIDE_CROSSHAIR_SYMBOL     (200)

//#region Helpers.

    #define     PlayerWeaponBodyDamage[%0]          ePlayerWeapon[%0][e_fWeapon_BodyDamage]
    #define     PlayerWeaponHeadDamage[%0]          ePlayerWeapon[%0][e_fWeapon_HeadDamage]

//#endregion

// Projectiles data.
enum e_SERVER_PROJECTILES_DATA
{
    e_iProjectile_ThrowerId,
    e_iProjectile_ObjectId,

    e_iNailBmb_ObjectId,
    e_iNailBmb_SmokeObjectId,
    Timer:e_iNailBmb_Timer
};
new eProjectileData[MAX_THROWABLES][e_SERVER_PROJECTILES_DATA];

new Iterator:NailBmbsList<MAX_THROWABLES>;

// Player weapon's data.
enum e_PLAYER_WEAPONS_DATA
{
    Float:e_fWeapon_BodyDamage,
    Float:e_fWeapon_HeadDamage,
    
    e_iArrowObjectId,

    e_iTickCounter_bowString,
    e_iTickCounter_projectileThrow,
    e_iTickCounter_NoAmmoClicks, 

    bool:e_bAimingArrow
};
new ePlayerWeapon[MAX_PLAYERS][e_PLAYER_WEAPONS_DATA];