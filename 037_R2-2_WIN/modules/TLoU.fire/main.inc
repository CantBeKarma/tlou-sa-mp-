#include <TLoU.fire\forwards>
#include <TLoU.fire\callbacks>
#include <YSI\y_hooks>

//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
//-------------------- OnGameModeExit ---------------------
//=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=-=
hook OnGameModeExit()
{
    foreach(new fireId : ServerFires)
    {
        DestroyFire(fireId);
    }
    Iter_Clear(ServerFires);
    return 1;
}