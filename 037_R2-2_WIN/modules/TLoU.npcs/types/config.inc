#define     MAX_NPC_TYPES           (4)
#define     MAX_NPC_TYPE_NAME       (8)
#define     MAX_NPC_SKINS           (15)

enum NPC_TYPE
{
    TYPE_RUNNER,
    TYPE_STALKER,
    TYPE_CLICKER,
    TYPE_BLOATER
};

stock const NpcSkinIds[MAX_NPC_SKINS] =
{
    289, 253, 230, 212, 213, 210, 209, 
    195, 194, 193, 192, 191, 190,
    SKIN_CLICKER,
    5
};

stock const NpcTypeNames[MAX_NPC_TYPES][MAX_NPC_TYPE_NAME] =
{
    "Runner",
    "Stalker",
    "Clicker",
    "Bloater"
};