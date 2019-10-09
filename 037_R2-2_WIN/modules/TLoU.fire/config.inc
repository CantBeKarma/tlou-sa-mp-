// Max definitions.
#define     MAX_POSSIBLE_FIRES      (300)

#define     MIN_FIRES_PER_MOLOTOV   (3)
#define     MAX_FIRES_PER_MOLOTOV   (5)

// Fire delay configurations.
#define     FIRE_DESTROY_DELAY      (60 * 1000)
#define     FIRE_PUT_OUT_DELAY      (5 * 1000)

// Fire model.
#define     FIRE_MODEL_ID           (18689)

// Other.
#define     FIRE_RANGE              (5.0)
#define     FIRE_DAMAGE_AMOUNT      (2.0)
#define     FIRE_DAMAGE_RATE        (200)

enum e_FIRE_DATA
{
    e_iFireObjectId[MAX_FIRES_PER_MOLOTOV],
    Float:e_fFirePos_X,
    Float:e_fFirePos_Y,
    Float:e_fFirePos_Z,
    e_iFireAreaId,
    e_iFireCreatorId
}
new eFireData[MAX_POSSIBLE_FIRES][e_FIRE_DATA];

new Iterator:ServerFires<MAX_POSSIBLE_FIRES>;