#pragma once
#include <Windows.h>
#include <windowsx.h>
#include <tchar.h>
#include <tlhelp32.h>
#include "resource.h"

class TaskManager
{
public:
	TaskManager(void);
	~TaskManager(void);
	static BOOL CALLBACK DlgProc(HWND hWnd, UINT mes, WPARAM wp, LPARAM lp);
	BOOL Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam);
	void Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify);
	void Cls_OnClose(HWND hwnd);
	void create_process_list(HWND hWnd, int mode, TCHAR* buf = TEXT(""));
	enum { SHOW, DEL };
private:
	static TaskManager* ptr;
	HWND hTaskManagerDlg, hInit, hEnd, hRight, hLeft, hRefresh;
	HWND hProc, hListLeft, hListRight;
};