// NPCs Configuration.
#define     TIMER_FCNPC_RESPAWN         (1000 * 5)
#define     TIMER_FCNPC_UPDATE          (200)
#define     TICK_RATE_FCNPC_IDLE        (1000 * 6)

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

#define     AREA_ROAM_SIZE              (10.0)

enum e_NPC_DATA
{
    NPC_TYPE:e_iType,
    NPC_STATUS:e_iCurrentStatus,

    e_iPlayingAnim_Tick,
    e_iPlayingAnim_Index,

    Float:e_fHealth,

    e_iChaseId,
    FCNPC_MovePathId,

    Float:e_fDistractedX,
    Float:e_fDistractedY,
    Float:e_fDistractedZ,

    e_iRoamAreaId,

    Timer:e_iUpdateTimer,
    e_iTickCounter_damageAnim,
    e_iTickCounter_IdleActions
}
new eNpcInfo[MAX_PLAYERS][e_NPC_DATA];

enum e_NPC_FLAGS
{
    e_bIsUpdateTimerRunning
}
new BitArray:eNpcFlag[e_NPC_FLAGS]<MAX_PLAYERS>;