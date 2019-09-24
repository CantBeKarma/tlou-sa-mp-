// Update created sprite timer and face it towards player.
timer UpdateSpriteTimer[TIMER_UPDATE_SPRITE](playerid, updateSpriteObjectId)
{
	if(IsValidDynamicObject(updateSpriteObjectId))
	{
		new Float:x, Float:y, Float:z;
		GetPlayerCameraPos(playerid, x, y, z);
		SetObjectFaceCoords3D(updateSpriteObjectId, x, y, z, 0.0, -90.0, -90.0);
	}
}