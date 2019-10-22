//#region OnPlayerScreen functions

    GetItemPos(elementid, Item3D_Type:element_type, &Float:x, &Float:y, &Float:z)
    {
        switch(element_type)
        {
            case item_player, item_npc: GetPlayerPos(elementid, x, y, z);
            case item_actor:			GetActorPos(elementid, x, y, z);
            case item_object:			GetObjectPos(elementid, x, y, z);
            case item_vehicle:			GetVehiclePos(elementid, x, y, z);
            case item_dynamic_object:	GetDynamicObjectPos(elementid, x, y, z);
            case item_dynamic_pickup: 	Streamer_GetItemPos(STREAMER_TYPE_PICKUP, elementid, x, y, z);
            case item_dynamic_cp: 		Streamer_GetItemPos(STREAMER_TYPE_CP, elementid, x, y, z);
            case item_dynamic_racecp:	Streamer_GetItemPos(STREAMER_TYPE_RACE_CP, elementid, x, y, z);
            case item_dynamic_mapicon:	Streamer_GetItemPos(STREAMER_TYPE_MAP_ICON, elementid, x, y, z);
            case item_dynamic_3dtext: 	Streamer_GetItemPos(STREAMER_TYPE_3D_TEXT_LABEL, elementid, x, y, z);
            case item_dynamic_actor:	GetDynamicActorPos(elementid, x, y, z);
            case item_fcnpc: 			FCNPC_GetPosition(elementid, x, y, z);
        }
        return 1;
    }

    GetItemVirtualWorld(elementid, Item3D_Type:element_type)
    {
        switch(element_type)
        {
            case item_player, item_npc: return GetPlayerVirtualWorld(elementid);
            case item_actor:            return GetActorVirtualWorld(elementid);
            
            case item_vehicle:          return GetVehicleVirtualWorld(elementid);
            case item_dynamic_object:   return Streamer_GetIntData(STREAMER_TYPE_OBJECT, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_pickup:   return Streamer_GetIntData(STREAMER_TYPE_PICKUP, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_cp:       return Streamer_GetIntData(STREAMER_TYPE_CP, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_racecp:   return Streamer_GetIntData(STREAMER_TYPE_RACE_CP, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_mapicon:  return Streamer_GetIntData(STREAMER_TYPE_MAP_ICON, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_3dtext:   return Streamer_GetIntData(STREAMER_TYPE_3D_TEXT_LABEL, elementid, E_STREAMER_WORLD_ID);
            case item_dynamic_actor:    return GetDynamicActorVirtualWorld(elementid);
            case item_fcnpc:            return FCNPC_GetVirtualWorld(elementid);
        }
        return INVALID_VIRTUAL_WORLD;
    }

    PointInSphericalSectorEx(Float:px, Float:py, Float:pz, Float:x, Float:y, Float:z, Float:rx, Float:rz, Float:radius, Float:vrx, Float:vrz, bool:testLOS = true)
    {
        if(GetDistanceBetweenPoints3D(px, py, pz, x, y, z) > radius)
        {
            return 0;
        }
        
        if(testLOS)
        {
            new Float:x2, Float:y2, Float:z2;
            if(CA_RayCastLine(x, y, z, px, py, pz, x2, y2, z2))
            {
                return 0;
            }
        }
        
        vrx /= 2.0;
        vrz /= 2.0;
        
        new Float:trx, Float:trz;
        if(!GetRotationFor2Point3D(x, y, z, px, py, pz, trx, trz))
        {
            return 0;
        }
        return (IsRotationTest(trx, rx - vrx, rx + vrx) && IsRotationTest(trz, rz - vrz, rz + vrz));
    }

    function IsRotationTest(Float:rotation, Float:r_min, Float:r_max)
    {
        rotation = CompressRotation(rotation);
        r_min = CompressRotation(r_min);
        r_max = CompressRotation(r_max);

        if(r_min > r_max)
        {
            if((rotation >= 0.0 && rotation <= r_max) || (rotation >= r_min && rotation <= 360.0)) 
            {
                return 1;
            }
        } 
        else 
        {
            if(rotation >= r_min && rotation <= r_max)
            {
                return 1;
            }
        }
        return 0;
    }

    function GetPlayerCameraRotation(playerid, &Float:rx, &Float:rz)
    {
        new Float:mx, Float:my, Float:mz;
        GetPlayerCameraFrontVector(playerid, mx, my, mz);
        rx = CompressRotation(-(acos(mz)-90.0));
        rz = CompressRotation((atan2(my,mx)-90.0));
    }

    GetRotationFor2Point3D(Float:x,Float:y,Float:z,Float:tx,Float:ty,Float:tz,&Float:rx,&Float:rz)
    {
        new Float:radius = GetDistanceBetweenPoints3D(x,y,z,tx,ty,tz);
        return (radius <= 0.0) ? 0 : (0x1 & (1 | _:(rx = CompressRotation(-(acos((tz-z)/radius)-90.0)), rz = CompressRotation(atan2(ty-y,tx-x)-90.0))));
    }

//#endregion

//#region Moving correctly functions (for NPCs purposes to do not ghost through buildings etc.)

	Float:ColAndreasFindZ(Float:x, Float:y, &Float:z = 0.0)
	{
		if(x <= -3000.0 || x >= 3000.0 || y <= -3000.0 || y >= 3000.0)
		{
			z = 0.0;
		} 
		else
		{
			CA_FindZ_For2DCoord(x, y, z);
		}
		return z;
	}

	Float:UndergroundFindZ(Float:x, Float:y, &Float:z=0.0)
	{
		z = 0.0;
		CA_RayCastLine(x, y, -0.002, x, y, -100.0, x, y, z);
		return z;
	}
	
	IsPointInWater(Float:x, Float:y, Float:z = 0.0)
	{
		if(z > 0.0)
		{
			return 0;
		}

		new Float:tmpzA, Float:tmpzB;
		ColAndreasFindZ(x, y, tmpzA);
		UndergroundFindZ(x, y, tmpzB);

		return ((z >= tmpzB) && (tmpzB <= 0.0) && (tmpzA > -0.001 && tmpzA < 0.001));
	}

	Random_PointInDynamicArea(areaid, &Float:tx, &Float:ty, &Float:tz)
	{
		switch(GetDynamicAreaType(areaid))
		{
			case STREAMER_AREA_TYPE_CIRCLE: 
			{
				new Float:areaX, Float:areaY, Float:areaZ, Float:areaSize;
				Streamer_GetItemPos(STREAMER_TYPE_AREA, areaid, areaX, areaY, areaZ);
				Streamer_GetFloatData(STREAMER_TYPE_AREA, areaid, E_STREAMER_SIZE, areaSize);
				Random_PointInCircle(areaX, areaY, areaSize, tx, ty);
				tz = FLOAT_INFINITY;
			}
		}
	}

	Random_PointInCircle(Float:x, Float:y, Float:radius, &Float:tx, &Float:ty)
	{
		new Float:alfa = float(random(1000000) + 1) / 1000000.0, Float:beta = float(random(1000000) + 1) / 1000000.0;
		if(beta < alfa)
		{
			SwapInt(alfa, beta);
		}

		tx = x + (beta * radius * floatcos(2.0 * FLOAT_PI * alfa / beta));
		ty = y + (beta * radius * floatsin(2.0 * FLOAT_PI * alfa / beta));
	}

	Float:CompressRotation(Float:rotation)
	{
		return (rotation - floatround(rotation / 360.0, floatround_floor) * 360.0);
	}

	MovePointColCutLineEx(Float:sX, Float:sY, Float:sZ, Float:eX, Float:eY, Float:eZ, &Float:x, &Float:y, &Float:z, Float:cut_size = 0.0, bool:npcChasingPlayer = false)
	{
		new Float:radius, Float:rx, Float:rz;
		x = y = z = 0.0;
		CA_RayCastLine(sX, sY, sZ, eX, eY, eZ, x, y, z);

		if(x == 0.0) x = eX;
		if(y == 0.0) y = eY;
		if(z == 0.0) z = eZ;

		if((radius = GetDistanceBetweenPoints3D(sX, sY, sZ, x, y, z)) <= 0.0) 
		{
			return false;
		}

		rx = CompressRotation(-(acos((z-sZ) / radius) - 90.0));
		rz = CompressRotation((atan2(y - sY, x - sX) - 90.0));

		if((radius-cut_size > cut_size) && (radius-cut_size > 0.0))
		{
			radius -= cut_size;
		} 
		else // reached some place like fence in front of him or anything like it.
		{
			radius = FLOAT_DEFECT;

			if(npcChasingPlayer)
			{
				return false;
			}
		}

		GetPointInFront3D(sX, sY, sZ, rx, rz, radius, x, y, z);
		return true;
	}

	GetPointInFront3D(Float:x, Float:y, Float:z, Float:rx, Float:rz, Float:radius, &Float:tx, &Float:ty, &Float:tz)
	{
		tx = x - (radius * floatcos(rx,degrees) * floatsin(rz,degrees));
		ty = y + (radius * floatcos(rx,degrees) * floatcos(rz,degrees));
		tz = z + (radius * floatsin(rx,degrees));
	}

	// Changes object facing position towards specified coords.
	SetObjectFaceCoords3D(iObject, Float: fX, Float: fY, Float: fZ, Float: fRollOffset = 0.0, Float: fPitchOffset = 0.0, Float: fYawOffset = 0.0) 
	{
		new
			Float: fOX,
			Float: fOY,
			Float: fOZ,
			Float: fPitch
		;
		GetDynamicObjectPos(iObject, fOX, fOY, fOZ);

		fPitch = floatsqroot(floatpower(fX - fOX, 2.0) + floatpower(fY - fOY, 2.0));
		fPitch = floatabs(atan2(fPitch, fZ - fOZ));

		fZ = atan2(fY - fOY, fX - fOX) + 90.0;
		SetDynamicObjectRot(iObject, fRollOffset, fPitch + fPitchOffset, fZ + fYawOffset);
	}

//#endregion