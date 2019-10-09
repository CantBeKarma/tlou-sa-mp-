// NPCs Configuration.
#define     TIMER_FCNPC_RESPAWN         (1000 * 6)
#define     TIMER_FCNPC_UPDATE          (200)

#define     RANGE_NPC_CHASE_BY_PATH     (30.0)
#define		RANGE_NPC_CHASE		        (10.0)
#define		RANGE_NPC_ATTACK	        (1.1)
#define     RANGE_NPC_DISTRACT          (30.0)

#define		NPC_ATTACK_DELAY			(500)

#define		PATHFINDER_Z_DIFF	    	(1.1)
#define		PATHFINDER_STEP_SIZE    	(1)
#define		PATHFINDER_STEP_LIMIT	    (-1)
#define		PATHFINDER_MAX_STEPS	    (50)

#define     MAX_FCNPCS                  (10)
#define     MAX_NPC_TYPES               (4)

#define     AREA_ROAM_SIZE              (3.0)

enum e_NPC_DATA
{
    NPC_TYPE:e_iType,
    NPC_STATUS:e_iCurrentStatus,

    e_iPlayingAnim_Tick,
    e_iPlayingAnim_Index,

    e_iFireCreatorId,

    Float:e_fHealth,

    e_iChaseId,
    FCNPC_MovePathId,

    e_iRoamAreaId,

    Timer:e_iUpdateTimer,
    e_iTickCounter_damageAnim
}
new eNpcInfo[MAX_PLAYERS][e_NPC_DATA];

enum e_NPC_FLAGS
{
    e_bIsUpdateTimerRunning
}
new BitArray:eNpcFlag[e_NPC_FLAGS]<MAX_PLAYERS>;