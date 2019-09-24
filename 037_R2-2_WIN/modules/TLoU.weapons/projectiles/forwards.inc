forward OnThrowableDestroy(projid, throwerid, hitid, itemId, Float:x, Float:y, Float:z);
forward OnMolotovExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);
forward OnSmokebombExplode(projid, throwerid, hitid, Float:x, Float:y, Float:z);

forward OnNailbombInitiate(nailBmbIdx, throwerid, Float:x, Float:y, Float:z);
forward OnPlayerTriggerNailbomb(triggererid, throwerid, nailBmbIdx, Float:x, Float:y, Float:z);

forward OnPlayerThrowProjectile(playerid, itemId);
forward OnPlayerDropNailbomb(playerid);