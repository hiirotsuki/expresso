#include <windows.h>

#define WM_SYSICON (WM_USER + 1)
#define ID_TRAY_EXIT 1001
#define ID_TRAY_TOGGLE 1002

NOTIFYICONDATA nid;
HWND hwnd;
BOOL toggleState = TRUE;
EXECUTION_STATE es;

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_CREATE:
			nid.cbSize = sizeof(NOTIFYICONDATA);
			nid.hWnd = hwnd;
			nid.uID = 1;
			nid.uFlags = NIF_ICON | NIF_MESSAGE | NIF_TIP;
			nid.uCallbackMessage = WM_SYSICON;
			nid.hIcon = LoadIcon(NULL, IDI_APPLICATION);
			lstrcpy(nid.szTip, "Expresso");

			Shell_NotifyIcon(NIM_ADD, &nid);
		break;

		case WM_SYSICON:
			if (lParam == WM_RBUTTONUP || lParam == WM_LBUTTONUP)
			{
				POINT pt;
				GetCursorPos(&pt);
				HMENU hMenu = CreatePopupMenu();

				InsertMenu(hMenu, 0, MF_BYPOSITION | MF_STRING, ID_TRAY_TOGGLE,
						toggleState ? TEXT("Disable") : TEXT("Enable"));
				InsertMenu(hMenu, 1, MF_BYPOSITION | MF_STRING, ID_TRAY_EXIT, TEXT("Exit"));

				SetForegroundWindow(hwnd);
				TrackPopupMenu(hMenu, TPM_BOTTOMALIGN | TPM_LEFTALIGN, pt.x, pt.y, 0, hwnd, NULL);
				DestroyMenu(hMenu);
			}
		break;

		case WM_COMMAND:
			switch (LOWORD(wParam))
			{
				case ID_TRAY_TOGGLE:
					if(toggleState)
					{
						SetThreadExecutionState(es);
						toggleState = FALSE;
					}
					else
					{
						es = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);
						toggleState = TRUE;
					}
				break;

				case ID_TRAY_EXIT:
					Shell_NotifyIcon(NIM_DELETE, &nid);
					PostQuitMessage(0);
				break;
			}
		break;

		case WM_DESTROY:
			SetThreadExecutionState(es); /* lets be kind and follow the knowledge base recommendation */
			Shell_NotifyIcon(NIM_DELETE, &nid);
			PostQuitMessage(0);
		break;

		default:
			return DefWindowProc(hwnd, uMsg, wParam, lParam);
	}
	return 0;
}

/* we don't need the C runtime startup bits. */
/* TODO: add StartupInfo code... */
int WINAPI WinMainCRTStartup(void)
{
#if 0
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpCmdLine, int nCmdShow)
{
#endif
	WNDCLASS wc = {0};

	wc.lpfnWndProc = WindowProc;
	wc.hInstance = NULL;
	wc.lpszClassName = TEXT("Expresso");

	RegisterClass(&wc);

	hwnd = CreateWindowEx(
	    0,
		wc.lpszClassName,
		TEXT("Expresso"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,
		NULL,
		NULL,
		NULL, //hInstance
		NULL
	);

	ShowWindow(hwnd, SW_HIDE);

	es = SetThreadExecutionState(ES_CONTINUOUS | ES_SYSTEM_REQUIRED | ES_DISPLAY_REQUIRED);

	MSG msg;
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return msg.wParam;
}
