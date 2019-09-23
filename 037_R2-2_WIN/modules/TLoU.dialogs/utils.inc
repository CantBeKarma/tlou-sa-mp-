// Used to display player a message in a dialog (INFO/WARNING/ERROR).
ShowDialogMessage(playerid, caption[], message[])
{
	return Dialog_Show(playerid, DIALOG_STYLE_MSGBOX, caption, message, "Close");
}