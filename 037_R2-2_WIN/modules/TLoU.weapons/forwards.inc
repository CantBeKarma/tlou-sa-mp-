forward OnPlayerWeaponChange(playerid, newItemId, oldItemId);

forward OnPlayerShivPlayer(playerid, shivedId);

forward OnPlayerTryAimingWeapon(playerid, itemId);
forward OnPlayerAimsWeapon(playerid, itemId);
forward OnPlayerStopAimingWeapon(playerid);
forward OnPlayerStartAimingBulletWeapon(playerid);
forward OnPlayerStopAimingBulletWeapon(playerid);
forward OnPlayerReloadWeapon(playerid, itemId, oldClipAmmo, newClipAmmo);
forward OnPlayerWeaponShoot(playerid, weaponId, oldAmmo, oldClipAmmo, newAmmo, newClipAmmo, Float:fX, Float:fY, Float:fZ);

forward OnPlayerStartAimingProjectile(playerid, itemId);
forward OnPlayerStopAimingProjectile(playerid);
forward OnPlayerThrowProjectile(playerid, itemId);
forward OnPlayerDropNailbomb(playerid);
forward OnPlayerTriggerNailbomb(triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z);

forward OnMolotovExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);
forward OnSmokebombExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);
forward OnThrowableDestroy(projid, throwerid, hitid, itemId, Float:x, Float:y, Float:z);
forward OnNailbombInitiate(nailBmbIdx, throwerid, Float:x, Float:y, Float:z);

forward OnPlayerStartAimingBow(playerid);
forward OnPlayerStopAimingBow(playerid);
forward OnPlayerShotArrow(playerid);
forward OnPlayerGetArrowShot(playerid, damagerid);