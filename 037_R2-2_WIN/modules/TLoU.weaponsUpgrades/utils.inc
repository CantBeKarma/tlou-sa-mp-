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