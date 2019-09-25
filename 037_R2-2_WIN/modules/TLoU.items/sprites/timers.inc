// Update created sprite timer and face it towards player.
timer UpdateSpriteTimer[TIMER_UPDATE_SPRITE](playerid, updateSpriteObjectId, updateSpriteTextObjectId)
{
	new Float:x, Float:y, Float:z;
	GetPlayerCameraPos(playerid, x, y, z);

	if(IsValidDynamicObject(updateSpriteObjectId))
	{
		SetObjectFaceCoords3D(updateSpriteTextObjectId, x, y, z, 0.0, -90.0, -90.0);
	}

	if(IsValidDynamicObject(updateSpriteTextObjectId))
	{
		SetObjectFaceCoords3D(updateSpriteObjectId, x, y, z, 0.0, -90.0, -90.0);
	}
}