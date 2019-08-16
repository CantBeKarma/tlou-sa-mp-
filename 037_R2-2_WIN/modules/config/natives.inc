//native print_s(AmxString:string) = print;

native GetPlayerName_s(playerid, AmxStringBuffer:name, len) = GetPlayerName;

native PlayerTextDrawSetStr_s(playerid, PlayerText:text, AmxString:string) = PlayerTextDrawSetString;

native strcat_s_impl(AmxString:dest, const source[], maxlength) = strcat;
stock strcat_s(StringTag:dest, const source[])
{
    return strcat_s_impl(dest, source, str_len(dest) + 1);
}