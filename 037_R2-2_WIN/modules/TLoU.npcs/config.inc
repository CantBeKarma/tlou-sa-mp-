#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// NPCs Configuration.
#define     TIMER_FCNPC_RESPAWN         (1000 * 5)
#define     TIMER_FCNPC_UPDATE          (200)
#define     TICK_RATE_FCNPC_IDLE        (1000 * 6)

#define     RANGE_NPC_CHASE_BY_PATH     (30.0)
#define		RANGE_NPC_CHASE		        (10.0)
#define		RANGE_NPC_ATTACK	        (1.1)
#define     RANGE_Z_DIFF_CHASE          (1.4)
#define     RANGE_NPC_DISTRACT          (30.0)

#define		NPC_RUN_OFFSET				(0.9)
#define		NPC_ATTACK_DELAY			(500)

#define		PATHFINDER_Z_DIFF	    	(1.1)
#define		PATHFINDER_STEP_SIZE    	(1)
#define		PATHFINDER_STEP_LIMIT	    (-1)
#define		PATHFINDER_MAX_STEPS	    (50)

#define     MAX_FCNPCS                  (10)
#define     MAX_NPC_TYPES               (4)

stock const NpcDamageAnims[MAX_DAMAGE_ANIMS] =
{
    PED_DAM_ARML_FRMBK, PED_DAM_ARML_FRMFT, PED_DAM_ARML_FRMLT,
    PED_DAM_ARMR_FRMBK, PED_DAM_ARMR_FRMFT, PED_DAM_ARMR_FRMRT,

    PED_DAM_LEGL_FRMBK, PED_DAM_LEGL_FRMFT, PED_DAM_LEGL_FRMLT,
    PED_DAM_LEGR_FRMBK, PED_DAM_LEGR_FRMFT, PED_DAM_LEGR_FRMRT,

    PED_DAM_STOMACH_FRMBK, PED_DAM_STOMACH_FRMFT, PED_DAM_STOMACH_FRMLT, PED_DAM_STOMACH_FRMRT
};

stock const NpcIdleAnimsList[5] =
{
    PED_FLEE_LKAROUND_01,
    PED_DAM_ARML_FRMFT,
    PED_DAM_ARMR_FRMFT,
    FLOWERS_FLOWER_HIT,
    KISSING_GF_CARARGUE_02
};

stock const NpcAttackAnims[3] =
{
	KNIFE_KNIFE_PART,
    SWORD_SWORD_PART,
    PED_SHOVE_PARTIAL  
};

stock const Float:NpcDamageValues[MAX_NPC_TYPES] =
{
    5.0, 
    8.0, 
    30.0, 
    100.0
};

stock const Float:NpcMovementSpeed[MAX_NPC_TYPES] =
{
    0.66444, 
    0.46444, 
    0.46444, 
    0.26444
};

enum NPC_TYPE
{
    TYPE_RUNNER,
    TYPE_STALKER,
    TYPE_CLICKER,
    TYPE_BLOATER
};

stock const NpcTypeNames[MAX_NPC_TYPES][8] =
{
    "Runner",
    "Stalker",
    "Clicker",
    "Bloater"
};

stock const NpcSkinIds[15] =
{
    289, 253, 230, 212, 213, 210, 209, 
    195, 194, 193, 192, 191, 190,
    SKIN_CLICKER,
    5
};

enum NPC_STATUS
{
    STATUS_NONE,
    STATUS_IDLE,
    STATUS_ROAM,
    STATUS_CHASE,
    STATUS_ATTACK,
    STATUS_DISTRACTED,
    STATUS_BURNING,
    STATUS_STUNNED,
    STATUS_DEAD,
    STATUS_PLAYING_ANIM
};

enum e_NPC_DATA
{
    NPC_TYPE:e_iType,
    NPC_STATUS:e_iCurrentStatus,

    e_iPlayingAnim_Tick,
    e_iPlayingAnim_Index,

    Float:e_fHealth,

    e_iChasedId,
    FCNPC_MovePathId,

    Float:e_fDistractedX,
    Float:e_fDistractedY,
    Float:e_fDistractedZ,

    e_iRoamAreaId,
    Float:e_iRoamAreaCenterX,
    Float:e_iRoamAreaCenterY,

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

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeInit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeInit()
{
    SendRconCommandf("maxnpc %d", MAX_FCNPCS);

    Log(eLogs[e_Log_Index][LOG_INDEX_SERVER], INFO, "Creating FCNPCs...");
    //FCNPC_CreateInfected();
    return 1;
}

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//--------------------- OnGameModeExit --------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    foreach(new npcid : FCNPC)
	{
	    FCNPC_Destroy(npcid);
	}
    return 1;
}