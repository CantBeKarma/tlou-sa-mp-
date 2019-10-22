#define     FLOAT_PI		 							            (3.14159265358979323846)
#define     FLOAT_DEFECT											(0.000001)
#define     GetDistanceBetweenPoints1D(%1,%2)						VectorSize((%1)-(%2),0.0,0.0)
#define     GetDistanceBetweenPoints2D(%1,%2,%3,%4)					VectorSize((%1)-(%3),(%2)-(%4),0.0)
#define     GetDistanceBetweenPoints3D(%1,%2,%3,%4,%5,%6)			VectorSize((%1)-(%4),(%2)-(%5),(%3)-(%6))
#define     SwapInt(%0,%1)                                          (((%0) ^= (%1)), ((%1) ^= (%0)), ((%0) ^= (%1)))

#define     INVALID_ROTATION            (-1000.0)
#define     INVALID_VIRTUAL_WORLD       (-2000)
#define     VERTICAL_CAMERA_RADIUS      (55.0) // 27.5 * 2
#define     HORIZONTAL_CAMERA_RADIUS    (70.0) // 35.0 * 2

enum Item3D_Type 
{
	item_player,
	item_npc,
	item_actor,
	item_object,
	item_vehicle,
	item_dynamic_object,
	item_dynamic_pickup,
	item_dynamic_cp,
	item_dynamic_racecp,
	item_dynamic_mapicon,
	item_dynamic_3dtext,
	item_dynamic_actor,
	item_dynamic_vehicle,
	item_fcnpc
}

enum element_orientation 
{
	o_left,
	o_right,
	o_up,
	o_down,
	o_front,
	o_back
}