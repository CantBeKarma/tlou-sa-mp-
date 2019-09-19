forward OnPlayerOpenBackpack(playerid, category);
forward OnPlayerCloseBackpack(playerid);
forward OnPlayerUseBackpack(playerid, category, keys);
forward OnPlayerSwitchBpCtgry(playerid, leftRight);

forward OnPlayerCheckCollectibles(playerid, bool:readCollectible);
forward OnPlayerStopCheckingCollectible(playerid);
forward OnPlayerViewingCollectibles(playerid, updown);
forward OnPlayerReadCollectible(playerid, row_title[], row_message[]);
forward OnPlayerStopReadingCollectible(playerid);
forward OnPlayerCollectibleLoad(playerid, title[]);

forward OnPlayerStartedCrafting(playerid, craftItemId);
forward OnPlayerCanceledCrafting(playerid);
forward OnPlayerCraftedItem(playerid, craftItemId);

forward OnPlayerStartedUpgradingSkill(playerid, skillid);
forward OnPlayerCanceledUpgradingSkill(playerid);
forward OnPlayerUpgradedSkill(playerid, skillid);