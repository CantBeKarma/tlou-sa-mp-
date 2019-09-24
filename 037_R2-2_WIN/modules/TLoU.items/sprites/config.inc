#define     MAX_SPRITES_AT_ONE_TIME		(10)
#define		ITEM_SPRITE_SPHERE_SIZE		(3.0)
#define		ITEM_SPRITE_STREAM_DIST		(3.0)
#define     TIMER_UPDATE_SPRITE         (10)

// Current list of sprites for items created for player.
new Iterator:SpritesCreated<MAX_PLAYERS, MAX_SPRITES_AT_ONE_TIME>;

enum e_PLAYER_2D_SPRITES
{
	e_iSprite_ObjectId[MAX_SPRITES_AT_ONE_TIME],
	Timer:e_iTimer_UpdateSprite[MAX_SPRITES_AT_ONE_TIME]
}
new ePlayerSpriteInfo[MAX_PLAYERS][e_PLAYER_2D_SPRITES];

stock const eItemsSpritesEx[MAX_ITEMS_DATA][25] =
{
    "ingr_blade_full",
    "ingr_binding_full",
    "ingr_rag_full",
    "ingr_alcohol_full",
    "ingr_explosive_full",
    "ingr_sugar_full",

    "weap_pipe",
    "weap_scsr_pipe",
    "weap_hatchet",
    "weap_machete",

    "weap_9mm",
    "weap_rev",
    "weap_shorty",

    "weap_shot",
    "weap_huntrif",
    "weap_assault",
    "weap_bow",
    "",

    "weap_shiv",
    "weap_medkit",
    "weap_molotov",
    "weap_nailbomb",
    "weap_smkbmb",

    "weap_brick",
    "weap_bottle",

    "",
    "",
    "",
    "",
    ""
};