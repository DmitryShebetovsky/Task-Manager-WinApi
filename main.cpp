#include "task.h"

int WINAPI WinMain(HINSTANCE hInst, HINSTANCE hPrev, LPSTR lpszCmdLine, int nCmdShow)
{
	TaskManager dlg;
	return DialogBox(hInst, MAKEINTRESOURCE(IDD_TMDIALOG), NULL, TaskManager::DlgProc);
}