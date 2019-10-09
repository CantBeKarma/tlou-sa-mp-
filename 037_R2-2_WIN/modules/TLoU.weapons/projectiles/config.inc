#define     MAX_PROJECTILE_SPEED            (12.0)

#define     TICK_RATE_NAILBMB_UPDATE        (600)
#define     NAILBMB_TRIGGERED_EXPLODE       (750)
#define     TICK_RATE_SMOKEBMB_EFFECT       (10000)
#define     TICK_RATE_PROJECTILE_THROW      (1100)

#define     NAILBOMB_TRIGGER_RANGE          (2.0)
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

#define		MAX_THROWABLES			    (MAX_PLAYERS * 3)

#define     OBJ_MODEL_NAILBOMB_SMOKE    (18673)
#define     OBJ_MODEL_FIRE_EXPLOSION    (18686)
#define     OBJ_MODEL_EXPLOSION         (18680)
#define     OBJ_MODEL_SMOKE             (18715)

new Iterator:NailBmbsList<MAX_THROWABLES>;

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