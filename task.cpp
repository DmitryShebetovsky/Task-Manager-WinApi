#include "task.h"

TaskManager* TaskManager::ptr = nullptr;

TaskManager::TaskManager(void)
{
	ptr = this;
}

TaskManager::~TaskManager(void)
{}

void TaskManager::Cls_OnClose(HWND hwnd)
{
	EndDialog(hwnd, 0);
}

BOOL TaskManager::Cls_OnInitDialog(HWND hwnd, HWND hwndFocus, LPARAM lParam)
{
	hTaskManagerDlg = GetDlgItem(hwnd, IDD_TMDIALOG);
	hInit = GetDlgItem(hwnd, IDC_BUTTON_INIT);
	hEnd = GetDlgItem(hwnd, IDC_BUTTON_END);
	hRight = GetDlgItem(hwnd, IDC_BUTTON_R);
	hLeft = GetDlgItem(hwnd, IDC_BUTTON_L);
	hRefresh = GetDlgItem(hwnd, IDC_BUTTON_REFRESH);
	hProc = GetDlgItem(hwnd, IDC_EDIT_PROCCESS);
	hListLeft = GetDlgItem(hwnd, IDC_LIST_CUR_PROC);
	hListRight = GetDlgItem(hwnd, IDC_LIST_CHOSEN_PROC);

	create_process_list(hwnd, SHOW);

	return TRUE;
}

void TaskManager::Cls_OnCommand(HWND hwnd, int id, HWND hwndCtl, UINT codeNotify)
{
	switch (id)
	{
	case IDC_BUTTON_INIT:
	{
		STARTUPINFO s = { sizeof(STARTUPINFO) };
		PROCESS_INFORMATION p;
		TCHAR buffer[32];
		GetWindowText(hProc, buffer, 31);
		if (buffer[0] != '\0')
		{
			BOOL fs = CreateProcess(NULL, buffer, NULL, NULL, FALSE, 0, NULL, NULL, &s, &p);
			if (fs)
			{
				CloseHandle(p.hThread);
				CloseHandle(p.hProcess);
			}
			else {
				MessageBox(hwnd, TEXT("ERROR! Check process name"), TEXT("INFO"), MB_OK);
			}
		}
	}
	break;
	case IDC_BUTTON_END:
	{
		int count = SendMessage(hListRight, LB_GETCOUNT, 0, 0);
		while (count)
		{
			TCHAR buffer[32];
			memset(buffer, '\0', 1);
			SendMessage(hListRight, LB_GETTEXT, WPARAM(0), LPARAM(buffer));
			create_process_list(hwnd, DEL, buffer);
			SendMessage(hListRight, LB_DELETESTRING, WPARAM(0), 0);
			count = SendMessage(hListRight, LB_GETCOUNT, 0, 0);
		}
		create_process_list(hwnd, SHOW);
		SendMessage(hListRight, LB_RESETCONTENT, 0, 0);
	}
	break;
	case IDC_BUTTON_R:
	{
		int pos = SendMessage(hListLeft, LB_GETCURSEL, 0, 0);
		TCHAR buffer[32];
		SendMessage(hListLeft, LB_GETTEXT, WPARAM(pos), LPARAM(buffer));
		SendMessage(hListRight, LB_ADDSTRING, 0, LPARAM(buffer));
		SendMessage(hListLeft, LB_DELETESTRING, WPARAM(pos), 0);
	}
	break;
	case IDC_BUTTON_L:
	{
		int pos = SendMessage(hListRight, LB_GETCURSEL, 0, 0);
		TCHAR buffer[32];
		SendMessage(hListRight, LB_GETTEXT, WPARAM(pos), LPARAM(buffer));
		SendMessage(hListLeft, LB_ADDSTRING, 0, LPARAM(buffer));
		SendMessage(hListRight, LB_DELETESTRING, WPARAM(pos), 0);
	}
	break;
	case IDC_BUTTON_REFRESH:
		create_process_list(hwnd, SHOW);
		SendMessage(hListRight, LB_RESETCONTENT, 0, 0);
		break;
	}
}

BOOL CALLBACK TaskManager::DlgProc(HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	switch (message)
	{
		HANDLE_MSG(hwnd, WM_INITDIALOG, ptr->Cls_OnInitDialog);
		HANDLE_MSG(hwnd, WM_CLOSE, ptr->Cls_OnClose);
		HANDLE_MSG(hwnd, WM_COMMAND, ptr->Cls_OnCommand);
	}
	return FALSE;
}

void TaskManager::create_process_list(HWND hWnd, int mode, TCHAR* buf)
{
	SendMessage(hListLeft, LB_RESETCONTENT, 0, 0);

	HANDLE hProcessSnap;
	HANDLE hProcess;
	PROCESSENTRY32 pe32;
	DWORD dwPriorityClass;

	hProcessSnap = CreateToolhelp32Snapshot(TH32CS_SNAPPROCESS, 0);
	if (hProcessSnap == INVALID_HANDLE_VALUE)
	{
		MessageBox(hWnd, TEXT("CreateToolhelp32Snapshot (of processes)"), TEXT("INFO"), MB_OK);
		return;
	}

	pe32.dwSize = sizeof(PROCESSENTRY32);

	if (!Process32First(hProcessSnap, &pe32))
	{
		MessageBox(hWnd, TEXT("Process32First"), TEXT("INFO"), MB_OK);
		CloseHandle(hProcessSnap);
		return;
	}

	do
	{
		SendMessage(hListLeft, LB_ADDSTRING, 0, LPARAM(pe32.szExeFile));

		dwPriorityClass = 0;
		hProcess = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
		if (hProcess == NULL)
			;//MessageBox(hWnd, TEXT("OpenProcess"), TEXT("INFO"), MB_OK);
		else
		{
			if (mode == DEL)
			{
				if (!lstrcmp(buf, pe32.szExeFile))
				{
					HANDLE hPR = OpenProcess(PROCESS_ALL_ACCESS, FALSE, pe32.th32ProcessID);
					TerminateProcess(hPR, 0);
				}
			}
		}

	} while (Process32Next(hProcessSnap, &pe32));

	CloseHandle(hProcessSnap);
}