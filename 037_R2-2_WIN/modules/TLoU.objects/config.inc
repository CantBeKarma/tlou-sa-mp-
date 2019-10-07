// Maximal values.
#define     MAX_REMOVE_OBJECTS      (38)

enum e_OBJECT_DATA
{
    e_iObj_Uid,
    e_iObj_ItemId,
    e_iObj_TypeId,
    Float:e_fObj_X,
    Float:e_fObj_Y,
    Float:e_fObj_Z,
    e_iObj_Vw,
    e_iObj_Int,
    e_iObj_ItemAmount,
	e_iObj_MeleeUpgradedCnd,
    e_sObj_Title[MAX_COLL_TIT_LEN],
    e_sObj_InnerMessage[MAX_COLL_MSG_LEN],
    e_iObj_SpriteAreaId,
	bool:e_bObj_Created
};
new eObjData[e_OBJECT_DATA];

// Removed objects by default from the SA world.
stock const RemoveObjectsData[MAX_REMOVE_OBJECTS] =
{
	// Vending machines
	1302, 1209, 955, 956, 1775, 1776, 1977,

	// Traffic lights.
	1283, 1284, 1315, 1350,

	// Lamp posts.
	1226, 1290, 1294, 1297, 1298, 3853, 1223, 1232,

	// Gang Tags.
	1531, 1530, 1529, 1528, 1527, 1526, 1525, 1524, 490,
	
	// Palm Trees.
	619, 620, 621, 710, 712, 716, 718, 739, 645,
	
	// Hydrants.
	1211
};