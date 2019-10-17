// Get player health value.
Float:GetPlayerHealthEx(playerid)
{
    return eCharsInfo[playerid][e_fHealth];
}

// Sets player's health to specified value. Value can be set by another player killerId/issuerId with specified itemId & bodypart.
// Those params will be provided to callback OnPlayerDie() if player's health is set to 0.0 or below.
SetPlayerHealthEx(playerid, Float:health, killerId = INVALID_PLAYER_ID, itemId = -1, bodypart = -1)
{
    if(playerid == INVALID_PLAYER_ID || IsPlayerDead(playerid))
    {
        return;
    }

    new Float:oldHealth = eCharsInfo[playerid][e_fHealth];
    new Float:newHealth = eCharsInfo[playerid][e_fHealth] = (health <= 0.0 ? 0.0 : health >= 100.0 ? 100.0 : health);

    OnPlayerHealthChange(playerid, oldHealth, newHealth);

    if(GetPlayerHealthEx(playerid) <= 0.0)
    {
        SetPlayerStatus(playerid, PLAYER_STATUS:PLAYER_STATUS_DEAD);
        OnPlayerDie(playerid, killerId, itemId, bodypart);
    }
}

// Changes player health bar length
// (calculates its length so it doesn't go over its limit).
AdjustPlayerHealthBar(playerid)
{
    PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], pb_percent(528.9, 35.3, 100.0, GetPlayerHealthEx(playerid)), 0.0);
}

// (recursive) Function which makes player heal himself with a medkit.
function HealMyself(playerid, healSpeed, Float:oldHealth, Float:destHealth)
{
    if(!IsPlayerHealing(playerid))
    {
        return;
    }

    if(IsPlayerHoldingKey(playerid, KEY_FIRE))
    {
        OnPlayerIsHealing(playerid, healSpeed, oldHealth, destHealth);
    }
    else
    {
        OnPlayerCanceledHealing(playerid, oldHealth);
    }
}