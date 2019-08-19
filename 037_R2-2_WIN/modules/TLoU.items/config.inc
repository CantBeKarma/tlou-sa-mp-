// Other
#define     MAX_ITEMS_DATA          (27)
#define     NO_OWNER_UID            (-1)
#define     ITEM_LABEL_DRAW_DIST    (2.5)
#define     HEAL_ITEM_RECOVER_HP    (5.0)

// Types of the items available.
#define     ITEM_TYPE_MELEE         (0)
#define     ITEM_TYPE_WEAPON        (1)
#define     ITEM_TYPE_HEAL          (2)
#define     ITEM_TYPE_INGREDIENT    (3)
#define     ITEM_TYPE_PROJECTILE    (4)
#define     ITEM_TYPE_THROWABLE     (5)
#define     ITEM_TYPE_COLLECTIBLE   (6)
#define     ITEM_TYPE_AMMO          (7)
#define     ITEM_TYPE_MISC          (8)

//#region Item models.
    // -> Craftable
    #define     ITEM_MODEL_SHIV         (15071)
    #define     ITEM_MODEL_MEDKIT       (15072)
    #define     ITEM_MODEL_BOMB         (15111)
    #define     ITEM_MODEL_MOLOTOV      (15112)
    #define     ITEM_MODEL_SMKBMB       (15113)

    // -> Ingredients
    #define     ITEM_MODEL_BLADE        (15073)
    #define     ITEM_MODEL_BINDING      (15074)
    #define     ITEM_MODEL_RAG          (15075)
    #define     ITEM_MODEL_ALCOHOL      (15076)
    #define     ITEM_MODEL_EXPLOSIVE    (15077)
    #define     ITEM_MODEL_SUGAR        (15078)

    // -> Weapons.
    #define     ITEM_MODEL_PIPE         (336)
    #define     ITEM_MODEL_SCSR_PIPE    (337)
    #define     ITEM_MODEL_HATCHET      (321)
    #define     ITEM_MODEL_MACHETE      (323)

    #define     ITEM_MODEL_9MM          (347)
    #define     ITEM_MODEL_REVOLVER     (348)
    #define     ITEM_MODEL_SHORTY       (350)

    #define     ITEM_MODEL_SHOTGUN      (349)
    #define     ITEM_MODEL_RIFLE        (357)
    #define     ITEM_MODEL_ASSAULT      (356)
    #define     ITEM_MODEL_BOW          (15109)

    #define     ITEM_MODEL_BRICK        (15101)
    #define     ITEM_MODEL_BOTTLE       (1509)

    #define     ITEM_MODEL_ARROW        (15110)

    // -> Collectibles
    #define     ITEM_MODEL_PAPER        (2953)
    #define     ITEM_MODEL_NOTES        (2059)
//#endregion

// Weapon upgrade types.
#define     UPGRADE_TYPE_CLIPSIZE       (0)

enum ITEM_IDX
{
    ITEM_IDX_BLADE,
    ITEM_IDX_BINDING,
    ITEM_IDX_RAG,
    ITEM_IDX_ALCOHOL,
    ITEM_IDX_EXPLOSIVE,
    ITEM_IDX_SUGAR,

    ITEM_IDX_PIPE,
    ITEM_IDX_SCSR_PIPE,
    ITEM_IDX_HATCHET,
    ITEM_IDX_MACHETE,

    ITEM_IDX_9MM,
    ITEM_IDX_REVOLVER,
    ITEM_IDX_SHORTY,

    ITEM_IDX_SHOTGUN,
    ITEM_IDX_RIFLE,
    ITEM_IDX_ASSAULT_RIFLE,
    ITEM_IDX_BOW,
    ITEM_IDX_ARROW,

    ITEM_IDX_SHIV,
    ITEM_IDX_MEDKIT,
    ITEM_IDX_MOLOTOV,
    ITEM_IDX_NAIL_BOMB,
    ITEM_IDX_SMOKE_BOMB,

    ITEM_IDX_BRICK,
    ITEM_IDX_BOTTLE
};

enum e_ITEM_DATA
{
    e_sItem_Name[24],
    e_iItem_Type,
    e_iItem_ModelId,
    e_iItem_SlotId,
    //e_iItem_MaxClipSize,
    e_iItem_MaxAmount,
    e_iItem_AudioPickupId,
    e_iItem_SA_WeaponId,

    Float:e_iItem_BodyDmg,
    Float:e_iItem_HeadDmg,

    e_iItem_CraftItemId,
    e_iItem_CraftIngrRequired1,
    e_iItem_CraftIngrRequired2
};
stock const eItemsData[MAX_ITEMS_DATA][e_ITEM_DATA] =
{
    {"Blade",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BLADE,       -1,                         300,        AUDIO_PICKUP_BLADE,         -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Binding",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_BINDING,     -1,                         300,        AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Rag",                 ITEM_TYPE_INGREDIENT,   ITEM_MODEL_RAG,         -1,                         300,        AUDIO_PICKUP_RAG,           -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Alcohol",             ITEM_TYPE_INGREDIENT,   ITEM_MODEL_ALCOHOL,     -1,                         300,        AUDIO_PICKUP_ALCOHOL,       -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Explosive",           ITEM_TYPE_INGREDIENT,   ITEM_MODEL_EXPLOSIVE,   -1,                         300,        AUDIO_PICKUP_EXPLOSIVE,     -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Sugar",               ITEM_TYPE_INGREDIENT,   ITEM_MODEL_SUGAR,       -1,                         300,        AUDIO_PICKUP_SUGAR,         -1,                     0.0,    0.0,        -1,                     -1,                     -1},

    {"Pipe",                ITEM_TYPE_MELEE,        ITEM_MODEL_PIPE,        WEAPON_SLOT_MELEE,          1,          -1,                         WEAPON_PIPE,            12.0,   12.0,       CRAFT_ITEM_MELEE,       -1,                     -1},
    {"Scissor pipe",        ITEM_TYPE_MELEE,        ITEM_MODEL_SCSR_PIPE,   WEAPON_SLOT_MELEE,          1,          -1,                         WEAPON_SCISSOR_PIPE,    20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Hatchet",             ITEM_TYPE_MELEE,        ITEM_MODEL_HATCHET,     WEAPON_SLOT_MELEE,          1,          -1,                         WEAPON_HATCHET,         20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Machete",             ITEM_TYPE_MELEE,        ITEM_MODEL_MACHETE,     WEAPON_SLOT_MELEE,          1,          -1,                         WEAPON_MACHETE,         20.0,   20.0,       CRAFT_ITEM_MELEE,       INGR_ITEM_BLADE,        INGR_ITEM_BINDING},

    {"9mm pistol",          ITEM_TYPE_WEAPON,       ITEM_MODEL_9MM,         WEAPON_SLOT_9MM,            -1,         -1,                         WEAPON_9MM,             20.0,   60.0,       -1,                     -1,                     -1},
    {"Revolver",            ITEM_TYPE_WEAPON,       ITEM_MODEL_REVOLVER,    WEAPON_SLOT_REVOLVER,       18,         -1,                         WEAPON_REVOLVER,        35.0,   75.0,       -1,                     -1,                     -1},
    {"Shorty",              ITEM_TYPE_WEAPON,       ITEM_MODEL_SHORTY,      WEAPON_SLOT_SHORTY,         -1,         -1,                         WEAPON_SHORTY,          35.0,   35.0,       -1,                     -1,                     -1},

    {"Shotgun",             ITEM_TYPE_WEAPON,       ITEM_MODEL_SHOTGUN,     WEAPON_SLOT_SHOTGUN,        -1,         -1,                         WEAPON_SHOTGUN,         40.0,   80.0,       -1,                     -1,                     -1},
    {"Rifle",               ITEM_TYPE_WEAPON,       ITEM_MODEL_RIFLE,       WEAPON_SLOT_RIFLE,          -1,         -1,                         WEAPON_RIFLE,           70.0,   100.0,      -1,                     -1,                     -1},
    {"Assault rifle",       ITEM_TYPE_WEAPON,       ITEM_MODEL_ASSAULT,     WEAPON_SLOT_ASSAULT,        90,         -1,                         WEAPON_ASSAULT,         20.0,   60.0,       -1,                     -1,                     -1},
    {"Bow",                 ITEM_TYPE_WEAPON,       ITEM_MODEL_BOW,         WEAPON_SLOT_BOW,            9,          -1,                         WEAPON_BOW,             45.0,   35.0,       -1,                     -1,                     -1},
    {"Arrow",               ITEM_TYPE_AMMO,         ITEM_MODEL_ARROW,       -1,                         -1,         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1},

    {"Shiv",                ITEM_TYPE_MISC,         ITEM_MODEL_SHIV,        WEAPON_SLOT_SHIV,           3,          -1,                         WEAPON_SHIV,            0.0,    0.0,        CRAFT_ITEM_SHIV,        INGR_ITEM_BLADE,        INGR_ITEM_BINDING},
    {"Medkit",              ITEM_TYPE_HEAL,         ITEM_MODEL_MEDKIT,      WEAPON_SLOT_MEDKIT,         3,          AUDIO_PICKUP_MEDKIT,        WEAPON_MEDKIT,          0.0,    0.0,        CRAFT_ITEM_MEDKIT,      INGR_ITEM_RAG,          INGR_ITEM_ALCOHOL},
    {"Molotov Cocktail",    ITEM_TYPE_PROJECTILE,   ITEM_MODEL_MOLOTOV,     WEAPON_SLOT_MOLOTOV,        3,          -1,                         WEAPON_MOLOTOV,         5.0,    5.0,        CRAFT_ITEM_MOLOTOV,     INGR_ITEM_RAG,          INGR_ITEM_ALCOHOL},
    {"Nail bomb",           ITEM_TYPE_PROJECTILE,   ITEM_MODEL_BOMB,        WEAPON_SLOT_BOMB,           3,          -1,                         WEAPON_NAIL_BOMB,       85.0,   85.0,       CRAFT_ITEM_BOMB,        INGR_ITEM_BLADE,        INGR_ITEM_EXPLOSIVE},
    {"Smoke Bomb",          ITEM_TYPE_PROJECTILE,   ITEM_MODEL_SMKBMB,      WEAPON_SLOT_SMOKE_BOMB,     3,          -1,                         WEAPON_SMOKE_BOMB,      0.0,    0.0,        CRAFT_ITEM_SMOKE_BOMB,  INGR_ITEM_EXPLOSIVE,    INGR_ITEM_SUGAR},

    {"Brick",               ITEM_TYPE_THROWABLE,    ITEM_MODEL_BRICK,       WEAPON_SLOT_BRICK_BOTTLE,   1,          AUDIO_BRICK_PICKUP,         WEAPON_BRICK,           10.0,   10.0,       -1,                     -1,                     -1},
    {"Bottle",              ITEM_TYPE_THROWABLE,    ITEM_MODEL_BOTTLE,      WEAPON_SLOT_BRICK_BOTTLE,   1,          AUDIO_BOTTLE_PICKUP,        WEAPON_BOTTLE,          10.0,   10.0,       -1,                     -1,                     -1},

    {"Note #1",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_PAPER,       -1,                         -1,         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1},
    {"Note #2",             ITEM_TYPE_COLLECTIBLE,  ITEM_MODEL_NOTES,       -1,                         -1,         -1,                         -1,                     0.0,    0.0,        -1,                     -1,                     -1}
};

// Player item's data.
enum e_PLAYER_ITEM_DATA
{
    e_iItemId,
    e_iItemAmount,
    e_iItemUpgradedCND
};
new ePlayerItemData[MAX_PLAYERS][MAX_ITEMS_DATA][e_PLAYER_ITEM_DATA];

// Item upgrade's data.
enum e_ITEM_UPGRADES_DATA
{
    e_iItemId,
    e_iItem_UpgradeLvl,
    e_iItem_UpgradeType,

    e_iItem_ClipSize,
    e_iItem_MaxAmount,

    e_iItem_RequiredParts,
    e_iItem_RequiredToolsLvl
};
stock const eItemsUpgrades[][e_ITEM_UPGRADES_DATA] =
{
    {_:ITEM_IDX_9MM, 0, UPGRADE_TYPE_CLIPSIZE,  6, 20,  0, 0},
    {_:ITEM_IDX_9MM, 1, UPGRADE_TYPE_CLIPSIZE,  8, 22, 15, 0},
    {_:ITEM_IDX_9MM, 2, UPGRADE_TYPE_CLIPSIZE, 10, 24, 20, 1},
    {_:ITEM_IDX_9MM, 3, UPGRADE_TYPE_CLIPSIZE, 12, 26, 30, 2},
    {_:ITEM_IDX_9MM, 4, UPGRADE_TYPE_CLIPSIZE, 14, 28, 40, 3},

    {_:ITEM_IDX_SHORTY, 0, UPGRADE_TYPE_CLIPSIZE, 2, 20, 20, 2},
    {_:ITEM_IDX_SHORTY, 1, UPGRADE_TYPE_CLIPSIZE, 3, 22, 30, 4},
    {_:ITEM_IDX_SHORTY, 2, UPGRADE_TYPE_CLIPSIZE, 4, 24, 40, 4},
    {_:ITEM_IDX_SHORTY, 3, UPGRADE_TYPE_CLIPSIZE, 5, 26, 50, 5},

    {_:ITEM_IDX_SHOTGUN, 0, UPGRADE_TYPE_CLIPSIZE, 4, 14,  0, 0},
    {_:ITEM_IDX_SHOTGUN, 1, UPGRADE_TYPE_CLIPSIZE, 5, 15, 15, 2},
    {_:ITEM_IDX_SHOTGUN, 2, UPGRADE_TYPE_CLIPSIZE, 6, 16, 30, 3},
    {_:ITEM_IDX_SHOTGUN, 3, UPGRADE_TYPE_CLIPSIZE, 7, 17, 40, 4},
    {_:ITEM_IDX_SHOTGUN, 4, UPGRADE_TYPE_CLIPSIZE, 8, 18, 50, 5},

    {_:ITEM_IDX_RIFLE, 0, UPGRADE_TYPE_CLIPSIZE, 1, 6,  0, 0},
    {_:ITEM_IDX_RIFLE, 1, UPGRADE_TYPE_CLIPSIZE, 2, 7, 20, 1},
    {_:ITEM_IDX_RIFLE, 2, UPGRADE_TYPE_CLIPSIZE, 3, 9, 40, 4}
};

IsValidItemId(itemId)
{
    if(itemId < 0 || itemId > MAX_ITEMS_DATA)
    {
        return false;
    }
    return true;
}

HasPlayerGotItem(playerid, itemId)
{
    return (IsValidItemId(itemId) && ePlayerItemData[playerid][itemId][e_iItemId] != -1);
}

GetItemType(itemId)
{
    return eItemsData[itemId][e_iItem_Type];
}

GetPlayerItemAmount(playerid, itemId)
{
    if(!HasPlayerGotItem(playerid, itemId))
    {
        return -1;
    }

    return ePlayerItemData[playerid][itemId][e_iItemAmount];
}

GetPlayerItemMaxAmount(playerid, itemId)
{
    new result = -1;

    result = eItemsData[itemId][e_iItem_MaxAmount];

    if(result == -1 && GetItemType(itemId) == ITEM_TYPE_WEAPON)
    {
        new playerItemLvl_MaxAmount = 0; //GetPlayerWeaponUpgradeLvl(playerid, weaponId, UPGRADE_TYPE_CLIPSIZE);

        for(new i = 0; i != sizeof(eItemsUpgrades); i++)
        {
            if(eItemsUpgrades[i][e_iItemId] == itemId
            && eItemsUpgrades[i][e_iItem_UpgradeLvl] == playerItemLvl_MaxAmount
            && eItemsUpgrades[i][e_iItem_UpgradeType] == UPGRADE_TYPE_CLIPSIZE)
            {
                result = eItemsUpgrades[i][e_iItem_MaxAmount];
                break;
            }
        }
    }
    return result;
}

IsPlayerFullOnItem(playerid, itemId)
{
    if(!HasPlayerGotItem(playerid, itemId))
    {
        return false;
    }

    new bool:result = false;

    if(eItemsData[itemId][e_iItem_MaxAmount] == -1 && GetItemType(itemId) == ITEM_TYPE_WEAPON)
    {
        result = (GetPlayerItemAmount(playerid, itemId) >= GetPlayerItemMaxAmount(playerid, itemId));
    }
    else
    {
        result = (GetPlayerItemAmount(playerid, itemId) >= eItemsData[itemId][e_iItem_MaxAmount]);
    }

    return result;
}

GivePlayerItem(playerid, itemId)
{
    if(HasPlayerGotItem(playerid, itemId))
    {
        return false;
    }

    ePlayerItemData[playerid][itemId][e_iItemId] = itemId;
    return true;
}

SetPlayerItemAmountEx(playerid, itemId, amount)
{
    if(!HasPlayerGotItem(playerid, itemId) || IsPlayerFullOnItem(playerid, itemId))
    {
        return false;
    }

    if(_:ITEM_IDX_BLADE <= itemId <= _:ITEM_IDX_SUGAR)
    {
        if((amount % 25) != 0)
        {
            return false;
        }
    }

    new maxItemAmount = GetPlayerItemMaxAmount(playerid, itemId);
    if(amount > maxItemAmount)
    {
        amount = maxItemAmount;
    }

    ePlayerItemData[playerid][itemId][e_iItemAmount] = amount;
    return true;
}

SetPlayerMeleeWeaponCND(playerid, itemId, cnd, upgradedCnd = 0)
{
    if(!HasPlayerGotItem(playerid, itemId) || eItemsData[itemId][e_iItem_Type] != ITEM_TYPE_MELEE)
    {
        return false;
    }

    if(cnd <= 0)
    {
        //RemoveWeapon
    }
    else
    {
        ePlayerItemData[playerid][itemId][e_iItemAmount] = amount;
    }
    

    if(upgradedCnd > 0)
    {
        ePlayerItemData[playerid][itemId][e_iItemUpgradedCND] = upgradedCnd;
    }
    return true;
}

CMD:giveitem(playerid, params[])
{
    new itemId;
    if(sscanf(params, "d", itemId))return SCM(playerid, -1, "/giveitem [itemId]");
    {
        GivePlayerItem(playerid, itemId);
    }
    return 1;
}

CMD:itemamount(playerid, params[])
{
    new itemId, amount;
    if(sscanf(params, "dd", itemId, amount))return SCM(playerid, -1, "/itemamount [itemId] [amount]");
    {
        SetPlayerItemAmountEx(playerid, itemId, amount);
    }
    return 1;
}

CMD:myitems(playerid, params[])
{
    SCM(playerid, -1, "My items:");

    for(new i = 0; i != MAX_ITEMS_DATA; i++)
    {
        if(!HasPlayerGotItem(playerid, i))
        {
            continue;
        }

        SCMF(playerid, -1, "Item Name: %s | Item Idx: %d | Amount: %d", eItemsData[i][e_sItem_Name], i, GetPlayerItemAmount(playerid, i));
    }
    return 1;
}

CMD:equipitem(playerid, params[])
{
    new itemId;
    if(sscanf(params, "d", itemId))return SCM(playerid, -1, "/equipitem [item_index] (/myitems)");
    {
        if(!HasPlayerGotItem(playerid, itemId))
        {
            SCM(playerid, -1, "You've tried to equip item that you either don't have or is invalid!");
            return 1;
        }

        if(GetItemType(itemId) == ITEM_TYPE_WEAPON)
        {
            GivePlayerWeapon(playerid, eItemsData[itemId][e_iItem_SA_WeaponId], GetPlayerItemAmount(playerid, itemId));
        }
    }
    return 1;
}