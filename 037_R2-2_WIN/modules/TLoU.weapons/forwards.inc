forward OnPlayerWeaponChange(playerid, newItemId, oldItemId);

forward OnPlayerReloadWeapon(playerid, itemId, oldClipAmmo, newClipAmmo);
forward OnPlayerWeaponShoot(playerid, weaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ);

forward OnPlayerTryAimingWeapon(playerid, itemId);
forward OnPlayerAimsWeapon(playerid, itemId);
forward OnPlayerStopAimingWeapon(playerid);

forward OnPlayerStartAimingBulletWeapon(playerid);
forward OnPlayerStopAimingBulletWeapon(playerid);

forward OnPlayerStartAimingProjectile(playerid, itemId);
forward OnPlayerStopAimingProjectile(playerid);

forward OnPlayerStartAimingBow(playerid);
forward OnPlayerStopAimingBow(playerid);