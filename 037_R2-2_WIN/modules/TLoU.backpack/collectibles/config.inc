// MAXIMAL VALUES.
#define     MAX_COLL_ITEMS              (2) // 3 for comics, but there are no comics yet and there may never be.
#define     MAX_COLLECTIBLES_DATA       (3)
#define     MAX_PLAYER_COLLECTIBLES     (100)
#define     MAX_DISPLAY_COLLECTIBLES    (9)

// COLLECTIBLES ITEM IDs.
#define     COLL_ITEM_ARTIFACTS     (0)
#define     COLL_ITEM_FIREFLY_PEND  (1)

// Backpacks data : COLLECTIBLES

// Collectibles backpack section information (X, Y positions of select box).
enum e_BACKPACK_COLL_DATA
{
    Float:e_fSelBoxX,
    Float:e_fSelBoxY
};
stock const eBackpack_CollItemsData[MAX_COLL_ITEMS][e_BACKPACK_COLL_DATA] =
{
    {376.333312, 123.703620},
    {376.333312, 152.703613}
};

// Player's collectibles data information (Artifacts & Firefly Pendants).
new Iterator:ArtifactsOwned<MAX_PLAYERS, MAX_PLAYER_COLLECTIBLES>;
new Iterator:PendantsOwned<MAX_PLAYERS, MAX_PLAYER_COLLECTIBLES>;

// List of available types of collectibles.
enum e_COLLECTIBLE_TYPES
{
    e_iCollectibleType,
    e_iCollectible_ModelId,
    e_iCollectible_AudioPickupId
};
stock const eCollectiblesData[MAX_COLLECTIBLES_DATA][e_COLLECTIBLE_TYPES] =
{
    { ITEM_TYPE_ARTIFACT,       ITEM_MODEL_PAPER,       AUDIO_PICKUP_NOTE },
    { ITEM_TYPE_ARTIFACT,       ITEM_MODEL_NOTES,       AUDIO_PICKUP_NOTE },
    { ITEM_TYPE_FIREFLY_PEND,   ITEM_MODEL_PENDANT,     AUDIO_PICKUP_PENDANT }
};