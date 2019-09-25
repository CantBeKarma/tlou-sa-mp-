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

forward OnItemCreate(item_objectId, itemId, itemType, Float:x, Float:y, Float:z, virtualWorld, interiorId, itemAmount, meleeExtCnd, itemTitle[], innerMsg[], creatorId);
forward OnItemDestroy(item_objectId, bool:updating);
forward OnItemUpdate(item_objectId, itemId, itemType, Float:x, Float:y, Float:z, virtualWorld, interiorId, newItemAmount, newMeleeExtCnd, updateItemUid, updaterId);