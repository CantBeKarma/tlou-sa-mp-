#define     MAX_NPC_TYPES           (4)
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

stock const NpcTypeNames[MAX_NPC_TYPES][8] =
{
    "Runner",
    "Stalker",
    "Clicker",
    "Bloater"
};

stock const Float:NpcHealthValues[MAX_NPC_TYPES] =
{
    100.0,
    120.0,
    140.0,
    160.0
};