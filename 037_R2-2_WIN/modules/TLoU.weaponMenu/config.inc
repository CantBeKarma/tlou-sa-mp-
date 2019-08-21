// Weapon menu, weapons Ids.
#define     WEAPON_MENU_V_BOMB        (0)
#define     WEAPON_MENU_V_MEDKIT      (1)
#define     WEAPON_MENU_V_BRICK       (2)
#define     WEAPON_MENU_V_MOLOTOV     (3)
#define     WEAPON_MENU_V_SMOKE_BOMB  (4)

#define     WEAPON_MENU_H_LEFT0       (0)
#define     WEAPON_MENU_H_LEFT1       (1)
#define     WEAPON_MENU_H_RIGHT0      (2)
#define     WEAPON_MENU_H_RIGHT1      (3)

#define     MAX_WEAPMENU_ITEMS        (9)

// Other.
#define     WeapMenu_GetSelectionV(%0)      ePlayerWeapMenu[%0][e_iWeapMenu_SelectedItemV]
#define     WeapMenu_GetSelectionH(%0)      ePlayerWeapMenu[%0][e_iWeapMenu_SelectedItemH]
#define     WeapMenu_Ls_WeaponId(%0,%1)     ePlayerWeapMenu[%0][e_iWeapMenu_H_Ls_Weapons][%1]
#define     WeapMenu_Rs_WeaponId(%0,%1)     ePlayerWeapMenu[%0][e_iWeapMenu_H_Rs_Weapons][%1]

#define     WeapMenu_Ls_ItemId(%0,%1)     ePlayerWeapMenu[%0][e_iWeapMenu_H_Ls_ItemId][%1]
#define     WeapMenu_Rs_ItemId(%0,%1)     ePlayerWeapMenu[%0][e_iWeapMenu_H_Rs_ItemId][%1]

enum e_WEAPMENU_DATA
{
    Float:e_fSelBoxX,
    Float:e_fSelBoxY
};
stock const eWeapMenu_Items[MAX_WEAPMENU_ITEMS][e_WEAPMENU_DATA] =
{
    // Vertical (from top to bottom)
    {427.340118, 140.622161},
    {427.340118, 183.622161},
    {427.340118, 269.903686},
    {427.340118, 312.903686},
    {427.340118, 355.903686},

    // Horizontal (from left to right)
    {339.006608, 225.659194},
    {385.336608, 225.659194},
    {466.0,      225.659194},
    {512.33,     225.659194}
};

enum e_PLAYER_WEAPMENU_DATA
{
    e_iWeapMenu_KeysUpDown,
	e_iWeapMenu_KeysLeftRight,
    e_iWeapMenu_SelectedItemV,
    e_iWeapMenu_SelectedItemH,
    e_iWeapMenu_H_Ls_Weapons[4],
    e_iWeapMenu_H_Rs_Weapons[3],

    e_iWeapMenu_H_Ls_ItemId[4],
    e_iWeapMenu_H_Rs_ItemId[3]
};
new ePlayerWeapMenu[MAX_PLAYERS][e_PLAYER_WEAPMENU_DATA];