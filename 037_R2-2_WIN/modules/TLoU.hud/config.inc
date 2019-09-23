// TIMERS.
#define		TIMER_HIDE_HUD			(5000)
#define		TIMER_HUD_FLASH_RATE	(100)

// MAX/MIN definitions.
#define		MAX_HUD_WEAPONS				(17)
#define		MIN_HEALTH_FLASH_HUD		(30.0)

// Hud menu items data (icons, sizes, positions of icon/total bullet count)
enum e_HUDMENU_DATA
{
    ITEM_IDX:e_iHud_itemId,
    e_sHud_WeapIcon[24],
    Float:e_fHud_WeapIconTextSize_X,
    Float:e_fHud_WeapIconTextSize_Y,

    Float:e_fHud_WeapIconPos_X,
    Float:e_fHud_WeapIconPos_Y,

    Float:e_fHud_WeapAmmoPos_X,
    Float:e_fHud_WeapAmmoPos_Y
};
stock const eHudMenu_Items[MAX_HUD_WEAPONS][e_HUDMENU_DATA] =
{
    {ITEM_IDX_9MM,              "TLoU:weap_9mm", 			 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_REVOLVER,  	 	"TLoU:weap_rev", 			 27.333326, 33.600013, 530.666748, 338.488983, 556.333557, 352.177368},
    {ITEM_IDX_SHORTY,	  	 	"TLoU:weap_shorty",			 38.333293, 46.044479, 517.333007, 330.607055, 558.000183, 355.080993},
    {ITEM_IDX_SHOTGUN,    		"TLoU:weap_shot_horiz", 	 50.333328, 50.607410, 507.333221, 324.385223, 559.333557, 352.177368},
    {ITEM_IDX_RIFLE,	    	"TLoU:weap_huntrif_horiz", 	 48.666625, 52.266689, 512.666503, 332.266540, 556.333557, 357.155120},
    {ITEM_IDX_ASSAULT_RIFLE,	"TLoU:weap_assault_horiz", 	 53.666641, 66.785194, 506.666748, 316.503479, 557.000244, 346.784698},
    {ITEM_IDX_BOW,              "TLoU:weap_bow",			 36.666618, 53.096324, 525.999633, 328.118286, 557.000183, 355.080993},
    {ITEM_IDX_NAIL_BOMB,  		"TLoU:weap_nailbomb", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_MEDKIT,			"TLoU:weap_medkit", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_BRICK, 	    	"TLoU:weap_brick", 		 	 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_BOTTLE,	    	"TLoU:weap_bottle", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_MOLOTOV,    		"TLoU:weap_molotov", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_SMOKE_BOMB, 		"TLoU:weap_smkbmb", 		 26.666658, 32.355571, 531.333374, 336.414886, 556.333557, 352.177368},
    {ITEM_IDX_PIPE, 			"TLoU:weap_pipe",	   	 	 36.666633, 44.800029, 527.999816, 331.436859, 0.0, 0.0},
    {ITEM_IDX_SCSR_PIPE, 		"TLoU:weap_scsr_pipe",	 	 40.999946, 44.385219, 516.999755, 331.436798, 0.0, 0.0},
    {ITEM_IDX_HATCHET,			"TLoU:weap_hatchet",		 33.999958, 48.533359, 528.333129, 326.044158, 0.0, 0.0},
    {ITEM_IDX_MACHETE,			"TLoU:weap_machete",		 36.666633, 44.800029, 527.999816, 331.436859, 0.0, 0.0}
};