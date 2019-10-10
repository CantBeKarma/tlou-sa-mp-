// Max definitions.
#define     MIN_FIRES_PER_MOLOTOV   (3)
#define     MAX_FIRES_PER_MOLOTOV   (5)

// Fire delay configurations.
#define     FIRE_DESTROY_DELAY      (60 * 1000)
#define     FIRE_PUT_OUT_DELAY      (5 * 1000)

// Fire model.
#define     FIRE_MODEL_ID           (18689)

// Other.
#define     FIRE_RANGE              (3.5)
#define     FIRE_DAMAGE_AMOUNT      (2.0)
#define     FIRE_DAMAGE_RATE        (200)

enum e_FIRE_DATA
{
    e_iFireAreaId,
    e_iFireCreatorId,
    bool:e_bFireCreated
}
new eFireData[e_FIRE_DATA];