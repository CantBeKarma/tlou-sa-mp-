// Clearing player's audio enum data.
ResetEnum_ePlayerAudio(playerid)
{
    static sBlank[e_PLAYER_AUDIO];
    ePlayerAudio[playerid] = sBlank;
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
    if(Iter_Free(iAudioHandle3D<playerid>) == INVALID_ITERATOR_SLOT)
    {
        return INVALID_ITERATOR_SLOT;
    }
    
    new audioHandle = Iter_Add(iAudioHandle3D<playerid>, Audio_Play(playerid, audioid, ppause, loop, downmix));
    Audio_Set3DPosition(playerid, audioHandle, x, y, z, distance);
    return audioHandle;
}

// Plays reload sound effect for player (and for nearby players 3D sound effect).
PlayPlayer3D_ReloadSFX(playerid, weaponId)
{
    for(new i = 0; i != MAX_RELOAD_SFX_AUDIOS; i++)
    {
        if(SFX_WeaponReloads[i][e_iWeaponId] != weaponId)
        {
            continue;
        }

        new Float:x, Float:y, Float:z;
        GetPlayerPos(playerid, x, y, z);

        new tmpAudioHandle = Audio_Play(playerid, SFX_WeaponReloads[i][e_iAudioId]);
        Audio_Set3DPosition(playerid, tmpAudioHandle, x, y, z, DIST3D_RELOAD_WEAPON_RANGE);
        break;
    }
}

// Plays player 3D Sound effect of destroyed projectile.
// SFX is played in the specific location (based on coordinates).
PlayPlayer3D_ProjectileSFX(playerid, itemId, Float:x, Float:y, Float:z)
{
    for(new i = 0; i != MAX_PROJECTILES_SFX; i++)
    {
        if(SFX_ProjectilesDestroy[i][e_iProjectileWeaponId] != GetItemNativeWeaponId(itemId))
            continue;

        if(IsPlayerInRangeOfPoint(playerid, SFX_ProjectilesDestroy[i][e_fProjectileDistanceSFX], x, y, z))
        {
            Audio3D_Play(playerid, SFX_ProjectilesDestroy[i][e_iProjectileDestroySFX], x, y, z, DIST3D_EXPLODE_RANGE);
        }
        break;
    }
}