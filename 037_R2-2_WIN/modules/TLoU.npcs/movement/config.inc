#define     FCNPC_MOVE_WALK             (0)
#define     FCNPC_MOVE_RUN              (1)

#define     MAX_NPC_TYPES               (4)
#define		NPC_RUN_OFFSET				(0.9)

#define     RANGE_NPC_START_CHASE       (12.0)
#define     RANGE_NPC_LOOSE_PLAYER      (25.0)

stock const Float:NpcMovementSpeed[MAX_NPC_TYPES] =
{
    0.66444, 
    0.46444, 
    0.46444, 
    0.26444
};