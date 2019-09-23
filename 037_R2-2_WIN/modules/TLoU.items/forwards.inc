forward OnPlayerLookAtItem(playerid, item_objectId);

forward OnPlayerTryPickupItem(playerid, item_objectId);
forward OnPlayerPickUpItem(playerid, itemId, itemAmount, itemExtAmount, item_objectId, itemUid);
forward OnPlayerPickUpIngredient(playerid, itemId, itemAmount, &dropItemAmount);
forward OnPlayerPickUpHealItem(playerid, itemId, itemAmount);
forward OnPlayerPickUpMeleeItem(playerid, itemId, itemAmount, itemExtAmount, &dropItemId, &dropItemAmount, &dropItemExtCnd);
forward OnPlayerPickUpThrowableItem(playerid, itemId, &dropItemId);
forward OnPlayerPickUpProjectileItem(playerid, itemId, itemAmount);
forward OnPlayerPickUpWeapon(playerid, itemId, itemAmount, &dropItemAmount);
forward OnPlayerPickUpCollectible(playerid, itemId, itemUid, itemTypeId, item_objectId);

forward OnItemCreate(itemId, itemType, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId);
forward OnItemDestroy(uid_item);
forward OnItemUpdate(updateItemUid, itemType, newItemId, newItemAmount, newMeleeExtCnd, Float:x, Float:y, Float:z, Float:rX, Float:rY, Float:rZ, virtualWorld, interiorId, updaterId);