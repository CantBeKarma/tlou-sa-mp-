#include	".\modules\includes\YSI-Includes-4.x\YSI\y_hooks.inc"

// NPCs Configuration.
#define     TIMER_FCNPC_RESPAWN         (1000 * 5)
#define     TIMER_FCNPC_UPDATE          (200)

#define     NPC_TYPE_RUNNER             (0)
#define		NPC_TYPE_STALKER			(1)
#define     NPC_TYPE_CLICKER            (2)
#define		NPC_TYPE_BLOATER			(3)

#define     RANGE_NPC_CHASE_BY_PATH     (30.0)
#define		RANGE_NPC_CHASE		        (10.0)
#define		RANGE_NPC_ATTACK	        (1.1)
#define     RANGE_Z_DIFF_CHASE          (1.4)
#define     RANGE_NPC_DISTRACT          (30.0)

#define		NPC_RUN_OFFSET				(0.9)
#define		NPC_ATTACK_DELAY			(500)

#define     NPC_STATUS_IDLE             (0)
#define     NPC_STATUS_CHASING          (1)
#define     NPC_STATUS_ATTACKING        (2)
#define     NPC_STATUS_DISTRACTED       (3)
#define     NPC_STATUS_CHASING_BY_PATH  (4)

#define		PATHFINDER_Z_DIFF	    	(1.1)
#define		PATHFINDER_STEP_SIZE    	(1)
#define		PATHFINDER_STEP_LIMIT	    (-1)
#define		PATHFINDER_MAX_STEPS	    (50)

#define     MAX_FCNPCS                  (1)

// NPC taking damage animations.
new const NpcDamageAnims[MAX_DAMAGE_ANIMS] =
{
    PED_DAM_ARML_FRMBK, PED_DAM_ARML_FRMFT, PED_DAM_ARML_FRMLT,
    PED_DAM_ARMR_FRMBK, PED_DAM_ARMR_FRMFT, PED_DAM_ARMR_FRMRT,

    PED_DAM_LEGL_FRMBK, PED_DAM_LEGL_FRMFT, PED_DAM_LEGL_FRMLT,
    PED_DAM_LEGR_FRMBK, PED_DAM_LEGR_FRMFT, PED_DAM_LEGR_FRMRT,

    PED_DAM_STOMACH_FRMBK, PED_DAM_STOMACH_FRMFT, PED_DAM_STOMACH_FRMLT, PED_DAM_STOMACH_FRMRT
};

// NPC idle animations (while roaming around).
new const NpcIdleAnims[5] =
{
    PED_FLEE_LKAROUND_01,
    PED_DAM_ARML_FRMFT,
    PED_DAM_ARMR_FRMFT,
    FLOWERS_FLOWER_HIT,
    KISSING_GF_CARARGUE_02
};

// NPC attack animations.
new const NpcAttackAnims[3] =
{
	KNIFE_KNIFE_PART,
    SWORD_SWORD_PART,
    PED_SHOVE_PARTIAL  
};

new const Float:NpcDamageValues[4] =
{
    // Runner, Stalker, Clicker, Bloater
    5.0, 8.0, 30.0, 100.0
};

new const Float:NpcMovementSpeed[4] =
{
    // Runner, Stalker, Clicker, Bloater
    0.66444, 0.46444, 0.46444, 0.26444
};

// NPC skin IDs.
new const NpcSkins[15] =
{
    // Runner
    289, 253, 230, 212, 213, 210, 209, 

    // Stalker
    195, 194, 193, 192, 191, 190,

    // Clicker
    SKIN_CLICKER,

    // Bloater
    5
};

enum e_NPC_DATA
{
    e_iType,
    e_iCurrentStatus,
    Float:e_fHealth,
    e_iAnimIdx,
    e_iAnimTick,
    e_iTickCounter_damageAnim,
    Timer:e_iUpdateTimer,
    e_iChasedId,
    FCNPC_MovePathId,

    Float:e_fDistractedX,
    Float:e_fDistractedY,
    Float:e_fDistractedZ
}
new eNpcInfo[MAX_PLAYERS][e_NPC_DATA];

enum e_NPC_FLAGS
{
    e_bAnimPlaying,
    e_bIsBurning,
    e_bIsStunned,
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
    FCNPC_CreateInfected();
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