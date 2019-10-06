#define     MAX_NPC_TYPES           (4)
#define     MAX_NPC_TYPE_NAME       (8)
#define     MAX_NPC_SKINS           (4)

enum NPC_TYPE
{
    TYPE_RUNNER,
    TYPE_STALKER,
    TYPE_CLICKER,
    TYPE_BLOATER
};

stock const NpcSkinIds[MAX_NPC_SKINS] =
{
    SKIN_RUNNER1,
    230,
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