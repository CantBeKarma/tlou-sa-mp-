// Gets player's maximal health value based on player's skill level of MAX HP.
Float:GetPlayerMaxHealth(playerid)
{
    new maxHpLvl = Skill_GetPlayerLevel(playerid, SKILL_ITEM_MAXHP);

    return (maxHpLvl == 0) ? (SKILL_MAXHP_LVL0) 
         : (maxHpLvl == 1) ? (SKILL_MAXHP_LVL1) 
         : (SKILL_MAXHP_LVL2);
}

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

    new Float:maxPlayerHealth = GetPlayerMaxHealth(playerid);
    new Float:oldHealth = eCharsInfo[playerid][e_fHealth];
    new Float:newHealth = eCharsInfo[playerid][e_fHealth] = (health <= 0.0 ? 0.0 : health >= maxPlayerHealth ? maxPlayerHealth : health);

    AdjustPlayerHealthBar(playerid, maxPlayerHealth);
    OnPlayerHealthChange(playerid, oldHealth, newHealth);

    if(GetPlayerHealthEx(playerid) <= 0.0)
    {
        SetPlayerStatus(playerid, PLAYER_STATUS_DEAD);
        OnPlayerDie(playerid, killerId, itemId, bodypart);
    }
}

// Changes player health bar length
// (calculates its length so it doesn't go over its limit).
AdjustPlayerHealthBar(playerid, Float:maxPlayerHealth)
{
    PlayerTextDrawTextSize(playerid, ePlayerTextDrawInfo[playerid][e_Hud_Hp_Status], pb_percent(528.9, 35.3, maxPlayerHealth, GetPlayerHealthEx(playerid)), 0.0);
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