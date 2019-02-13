// Clearing player's audio enum data.
ResetEnum_ePlayerAudio(playerid)
{
    static sBlank[e_PLAYER_AUDIO];
    ePlayerAudio[playerid] = sBlank;

    print("ResetEnum_ePlayerAudio");
}

// Play audio and assign it to player's handle.
Audio_PlayEx(playerid, audioid, bool:ppause = false, bool:loop = false, bool:downmix = false)
{
    ePlayerAudio[playerid][e_iAudioHandle] = Audio_Play(playerid, audioid, ppause, loop, downmix);
}

// Stop audio based on current handle.
Audio_StopEx(playerid)
{
    Audio_Stop(playerid, ePlayerAudio[playerid][e_iAudioHandle]);
    ePlayerAudio[playerid][e_iAudioHandle] = NO_AUDIO;
}

// Plays 3D Audio sound to the next available free 3D slot effect.
Audio3D_Play(playerid, audioid, Float:x, Float:y, Float:z, Float:distance, bool:ppause = false, bool:loop = false, bool:downmix = false)
{
    if(Iter_Alloc(iAudioHandle3D<playerid>) == INVALID_ITERATOR_SLOT)
        return INVALID_ITERATOR_SLOT;

    new audioHandle = Iter_Add(iAudioHandle3D<playerid>, Audio_Play(playerid, audioid, ppause, loop, downmix));
    Audio_Set3DPosition(playerid, audioHandle, x, y, z, distance);

    return audioHandle;
}

// Plays 3D STREAM audio sound to the next available free 3D slot effect.
// Audio3D_Stream(playerid, const url[], Float:x, Float:y, Float:z, Float:distance, bool:ppause = false, bool:loop = false, bool:downmix = false)
// {
//     if(Iter_Alloc(iAudioHandle3D<playerid>) == INVALID_ITERATOR_SLOT)
//         return INVALID_ITERATOR_SLOT;

//     new audioHandle = Iter_Add(iAudioHandle3D<playerid>, Audio_PlayStreamed(playerid, url, ppause, loop, downmix));
//     Audio_Set3DPosition(playerid, audioHandle, x, y, z, distance);

//     return audioHandle;
// }

// Plays reload sound effect for player (and for nearby players 3D sound effect).
PlayPlayerReloadSound(playerid, weaponId)
{
	new tmpAudioId = -1;

	switch(weaponId)
	{
		case WEAPON_9MM: 
			tmpAudioId = AUDIO_RELOAD_9MM;

		case WEAPON_REVOLVER: 
			tmpAudioId = AUDIO_RELOAD_REVOLVER;

		case WEAPON_SHOTGUN:
			tmpAudioId = AUDIO_RELOAD_SHOTGUN;

		case WEAPON_RIFLE:
			tmpAudioId = AUDIO_RELOAD_RIFLE;

		case WEAPON_ASSAULT: 
			tmpAudioId = AUDIO_RELOAD_ASSAULT;
	}

    if(tmpAudioId == -1)
        return;

    new Float:x, Float:y, Float:z;
    GetPlayerPos(playerid, x, y, z);

    new tmpAudioHandle = Audio_Play(playerid, tmpAudioId);
    Audio_Set3DPosition(playerid, tmpAudioHandle, x, y, z, DIST3D_RELOAD_WEAPON_RANGE);
}