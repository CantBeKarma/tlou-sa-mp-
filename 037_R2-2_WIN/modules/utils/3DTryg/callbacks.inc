OnPlayerScreen(playerid, targetid, Item3D_Type:target_type = item_player, element_orientation:orientation = o_front, Float:rx = INVALID_ROTATION, Float:rz = INVALID_ROTATION, \
                    Float:vrx = VERTICAL_CAMERA_RADIUS, Float:vrz = HORIZONTAL_CAMERA_RADIUS, bool:testLOS = true, bool:testVW = true)
{
	new 
        Float:x, Float:y, Float:z,
        Float:tx, Float:ty, Float:tz,
        Float:prx, Float:prz;

	GetPlayerCameraPos(playerid, x, y, z);
	GetPlayerCameraRotation(playerid, prx, prz);
	
	switch(orientation)
    {
		case o_left:	prx = CompressRotation(prx + 90.0);
		case o_right:	prx = CompressRotation(prx - 90.0);
		case o_back:	prx = CompressRotation(prx + 180.0);
	}
	
	if(rx != INVALID_ROTATION)
    {
        prx = rx;
    }

	if(rz != INVALID_ROTATION)
    {
        prz = rz;
    }
	
	if(!GetItemPos(targetid, target_type, tx, ty, tz))
    {
        return 0;
    }
	
	if(testVW)
    {
		if(GetItemVirtualWorld(targetid, target_type) != -1 
        && GetItemVirtualWorld(targetid, target_type) != GetPlayerVirtualWorld(playerid))
        {
            return 0;
        }
	}
	return PointInSphericalSectorEx(tx, ty, tz, x, y, z, prx, prz, 300.0, vrx, vrz, testLOS);
}