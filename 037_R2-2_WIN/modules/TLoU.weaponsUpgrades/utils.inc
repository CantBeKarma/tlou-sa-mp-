// Returns maximal possible weapon clipsize that player can have for a specific weapon.
// GetPlayerWeaponLvl_MaxClipSize(playerid, weaponId)
// {
// 	new playerWeaponLvl_ClipSize = GetPlayerWeaponUpgradeLvl(playerid, weaponId, UPGRADE_TYPE_CLIPSIZE);

// 	for(new i = 0; i != MAX_WEAPONS_UPGRADES_DATA; i++)
//     {       
//         if(eWeaponsUpgrades[i][e_iWeaponId] == weaponId
//         && eWeaponsUpgrades[i][e_iWeapon_UpgradeLvl] == playerWeaponLvl_ClipSize
//         && eWeaponsUpgrades[i][e_iWeapon_UpgradeType] == UPGRADE_TYPE_CLIPSIZE)
//         {
// 			return eWeaponsUpgrades[i][e_iWeapon_ClipSize];
//         }
//     }
// 	return 0;
// }

// Returnes MeleeWeapon durability.
GetMeleeWeapon_Durability(weaponId, upgradedDurability)
{
   	for(new i = 0; i != MAX_WEAPONS_MELEE_DATA; i++)
    {       
        if(eMeleeWeaponsData[i][e_iWeaponId] == weaponId)
        {
            if(!upgradedDurability)
			    return eMeleeWeaponsData[i][e_iWeaponDurability];
            else
                return eMeleeWeaponsData[i][e_iWeaponExtDurability];
        }
    }
    return 0;
}