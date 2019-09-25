// 2D Sprites (texture names) are located in "doNotUse.txd" file

#define     MAX_2D_ITEM_SPRITES         MAX_ITEMS_DATA
#define     MAX_2D_INGR_SPRITES         (24)
#define     MAX_2D_COLL_SPRITES         (2)

#define     MAX_SPRITES_AT_ONE_TIME		(10)
#define		ITEM_SPRITE_SPHERE_SIZE		(3.0)
#define		ITEM_SPRITE_STREAM_DIST		(3.0)
#define     TIMER_UPDATE_SPRITE         (10)

// Current list of sprites for items created for player.
new Iterator:SpritesCreated<MAX_PLAYERS, MAX_SPRITES_AT_ONE_TIME>;

enum e_PLAYER_2D_SPRITES
{
	e_iSprite_ObjectId[MAX_SPRITES_AT_ONE_TIME],
    e_iSpriteText_ObjectId[MAX_SPRITES_AT_ONE_TIME],
	Timer:e_iTimer_UpdateSprite[MAX_SPRITES_AT_ONE_TIME]
}
new ePlayerSpriteInfo[MAX_PLAYERS][e_PLAYER_2D_SPRITES];

// ITEM SPRITES - List of ITEM in-game sprites (textures) names.
stock const Sprites2D_Items[MAX_2D_ITEM_SPRITES][25] =
{
    "",
    "",
    "",
    "",
    "",
    "",

    "sprite_weap_pipe",
    "sprite_weap_scsr_pipe",
    "sprite_weap_hatchet",
    "sprite_weap_machete",

    "sprite_weap_9mm",
    "sprite_weap_rev",
    "sprite_weap_shorty",

    "sprite_weap_shot",
    "sprite_weap_hunt_rif",
    "sprite_weap_assault",
    "sprite_weap_bow",
    "",

    "sprite_weap_shiv",
    "sprite_weap_medkit",
    "sprite_weap_molotov",
    "sprite_weap_nailbomb",
    "sprite_weap_smkbmb",

    "sprite_weap_brick",
    "sprite_weap_bottle",

    "",
    "",
    "",
    "",
    ""
};

// INGREDIENT SPRITES - List of INGREDIENT in-game sprites (textures) names.
enum e_2D_INGR_SPRITES
{
    ITEM_IDX:e_iIngrId,
    e_iIngrValue,
    e_sIngrSprite[24]
};
stock const Sprites2D_Ingredients[MAX_2D_INGR_SPRITES][e_2D_INGR_SPRITES] =
{
    {ITEM_IDX_BLADE,        25,     "ingr_blade_quater"},
    {ITEM_IDX_BLADE,        50,     "ingr_blade_half"},
    {ITEM_IDX_BLADE,        75,     "ingr_blade_3quaters"},
    {ITEM_IDX_BLADE,        100,    "ingr_blade_full"},

    {ITEM_IDX_BINDING,      25,     "ingr_binding_quater"},
    {ITEM_IDX_BINDING,      50,     "ingr_binding_half"},
    {ITEM_IDX_BINDING,      75,     "ingr_binding_3quaters"},
    {ITEM_IDX_BINDING,     100,     "ingr_binding_full"},

    {ITEM_IDX_RAG,          25,     "ingr_rag_quater"},
    {ITEM_IDX_RAG,          50,     "ingr_rag_half"},
    {ITEM_IDX_RAG,          75,     "ingr_rag_3quaters"},
    {ITEM_IDX_RAG,         100,     "ingr_rag_full"},

    {ITEM_IDX_ALCOHOL,      25,     "ingr_alcohol_quater"},
    {ITEM_IDX_ALCOHOL,      50,     "ingr_alcohol_half"},
    {ITEM_IDX_ALCOHOL,      75,     "ingr_alcohol_3quaters"},
    {ITEM_IDX_ALCOHOL,     100,     "ingr_alcohol_full"},

    {ITEM_IDX_EXPLOSIVE,    25,     "ingr_explosive_quater"},
    {ITEM_IDX_EXPLOSIVE,    50,     "ingr_explosive_half"},
    {ITEM_IDX_EXPLOSIVE,    75,     "ingr_explosive_3quaters"},
    {ITEM_IDX_EXPLOSIVE,   100,     "ingr_explosive_full"},

    {ITEM_IDX_SUGAR,        25,     "ingr_sugar_quater"},
    {ITEM_IDX_SUGAR,        50,     "ingr_sugar_half"},
    {ITEM_IDX_SUGAR,        75,     "ingr_sugar_3quaters"},
    {ITEM_IDX_SUGAR,       100,     "ingr_sugar_full"}
};

// COLLECTIBLE SPRITES - List of COLLECTIBLE in-game sprites (textures) names.
stock const Sprites2D_Collectibles[MAX_2D_COLL_SPRITES][25] =
{
    "sprite_collectible",
    "sprite_firefly"
};