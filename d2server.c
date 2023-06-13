#include "d2server.h"


BOOL                         DefaultInit(VOID);
ATOM                         MyRegisterClass(HINSTANCE hInstance);
BOOL                         InitInstance(HINSTANCE, int);
LRESULT CALLBACK             WndProc(HWND, UINT, WPARAM, LPARAM);
void                         InitControls(HWND hWnd);
void                         BuildConf(HWND hWnd);
DWORD WINAPI                 ThreadScanProcess(void*);
LRESULT                      GroupSubclassProc(HWND, UINT, WPARAM, LPARAM, UINT_PTR, DWORD_PTR);
void                         RunProcess(const WCHAR* const, const WCHAR* const, WORD);
void                         KillProcess(const WCHAR* const);


int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
	_In_opt_ HINSTANCE hPrevInstance,
	_In_ LPTSTR    lpCmdLine,
	_In_ int       nCmdShow)
{
	MSG msg;

	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	if (!DefaultInit()) { return(-1); };

	MyRegisterClass(hInstance);

	if (!InitInstance(hInstance, nCmdShow)) {
		return FALSE;
	}

	while (GetMessage(&msg, NULL, 0, 0)) {
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return (int)(msg.wParam);
}

BOOL DefaultInit(VOID) {
	struct d2conf conf;
	char buf[256];
	long idx = 0;

	ZeroMemory(&conf, sizeof(struct d2conf));
	d2confi(&conf, L".\\d2server.conf");
	if (d2confl(&conf) != 0) { return(FALSE); }

	idx = d2confs(&conf,"realm_conf_in");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.in_conf.realm), (int)(256)); } }
	idx = d2confs(&conf, "d2gs_reg_in");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.in_conf.d2gs), (int)(256)); } }
	idx = d2confs(&conf, "d2cs_conf_in");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.in_conf.d2cs), (int)(256)); } }
	idx = d2confs(&conf, "d2dbs_conf_in");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.in_conf.d2dbs), (int)(256)); } }
	idx = d2confs(&conf, "bnetd_conf_in");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.in_conf.bnetd), (int)(256)); } }

	idx = d2confs(&conf, "realm_conf_out");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.out_conf.realm), (int)(256)); } }
	idx = d2confs(&conf, "d2gs_reg_out");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.out_conf.d2gs), (int)(256)); } }
	idx = d2confs(&conf, "d2cs_conf_out");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.out_conf.d2cs), (int)(256)); } }
	idx = d2confs(&conf, "d2dbs_conf_out");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.out_conf.d2dbs), (int)(256)); } }
	idx = d2confs(&conf, "bnetd_conf_out");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.out_conf.bnetd), (int)(256)); } }

	idx = d2confs(&conf, "d2gs_dir");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_d.d2gs), (int)(256)); } }
	idx = d2confs(&conf, "pvpgn_dir");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_d.bnetd), (int)(256)); } }

	idx = d2confs(&conf, "d2gs_exe");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_f.d2gs), (int)(256)); } }
	idx = d2confs(&conf, "d2cs_exe");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_f.d2cs), (int)(256)); } }
	idx = d2confs(&conf, "d2dbs_exe");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_f.d2dbs), (int)(256)); } }
	idx = d2confs(&conf, "pvpgn_exe");
	if (idx >= 0) { if (!strkvp(d2confg(&conf, idx), '=', NULL, buf)) { MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(d2server.exe_f.bnetd), (int)(256)); } }

	d2confu(&conf);

	return(TRUE);
}

ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_D2SERVER));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW);
	wcex.lpszMenuName = NULL;
	wcex.lpszClassName = WND_CLASS;
	wcex.hIconSm = wcex.hIcon;

	return RegisterClassEx(&wcex);
}

BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
	HWND hWnd = CreateWindow(WND_CLASS, WND_TITLE, WS_SYSMENU | WS_MINIMIZEBOX,
		CW_USEDEFAULT, 0, 640, 415, NULL, NULL, hInstance, NULL);

	if (!hWnd) {
		return FALSE;
	}

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	g_hThreadScanProcess = CreateThread(NULL, 0, ThreadScanProcess, &hWnd, 0, NULL);
	if (!g_hThreadScanProcess) { return(-1); }

	return TRUE;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int id;
	PAINTSTRUCT ps;
	HDC hdc;
	HWND hGroup0, hGroup, hDummy;

	switch (message) {
	case(WM_APP + 100): {
		if (PROCESS_IS_RUNNING_D2GS()) { 
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10001), 1001), FALSE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10001), 1002), TRUE);
		}
		else { 
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10001), 1001), TRUE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10001), 1002), FALSE);
		}
		if (PROCESS_IS_RUNNING_D2CS()) {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10004), 1001), FALSE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10004), 1002), TRUE);
		}
		else {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10004), 1001), TRUE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10004), 1002), FALSE);
		}
		if (PROCESS_IS_RUNNING_D2DBS()) {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10003), 1001), FALSE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10003), 1002), TRUE);
		}
		else {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10003), 1001), TRUE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10003), 1002), FALSE);
		}
		if (PROCESS_IS_RUNNING_BNETD()) {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10002), 1001), FALSE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10002), 1002), TRUE);
		}
		else {
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10002), 1001), TRUE);
			EnableWindow(GetDlgItem(GetDlgItem(GetDlgItem(hWnd, 10000), 10002), 1002), FALSE);
		}
		break;
	}
	case WM_COMMAND: {
		id = LOWORD(wParam);
		switch (id)
		{
		case(1001): {
			BuildConf(GetDlgItem(hWnd, 10000));
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		break;
	}
	case(WM_CREATE): {
		/* Realm */
		hGroup0 = CreateWindowEx(0, WC_BUTTON, (LPCWSTR)(TEXT("Realm")), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 5, 615, 335, hWnd, (HMENU)((ULONG_PTR)(10000)), NULL, (LPARAM)((LPVOID)(NULL)));
		/*hDummy = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 590, 0, 20, 20, hGroup0, (HMENU)((ULONG_PTR)(1003)), NULL, (LPARAM)((LPVOID)(NULL)));*/
		hDummy = CreateWindowEx(0, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER, 5, 30, 300, 24, hGroup0, (HMENU)((ULONG_PTR)(2001)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0, WC_EDIT, NULL, WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL, 310, 30, 300, 24, hGroup0, (HMENU)((ULONG_PTR)(2002)), NULL, (LPARAM)((LPVOID)(NULL)));
		/* Diablo 2 Game Server Service */
		hGroup = CreateWindowEx(0, WC_BUTTON, (LPCWSTR)(TEXT("D2GS")), WS_CHILD | WS_VISIBLE | BS_GROUPBOX, 5, 75, 300, 255, hGroup0, (HMENU)((ULONG_PTR)(10001)), NULL, (LPARAM)((LPVOID)(NULL)));
		SetWindowSubclass(hGroup, GroupSubclassProc, 1001, 0);
		/*hDummy = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 0, 20, 20, hGroup, (HMENU)((ULONG_PTR)(1003)), NULL, (LPARAM)((LPVOID)(NULL)));*/
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Run"), WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1001)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_BUTTON,TEXT("Kill"),WS_CHILD | WS_VISIBLE | WS_BORDER,236, 0, 60, 20,hGroup,(HMENU)((ULONG_PTR)(1002)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_IPADDRESS,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,10, 30, 200, 24,hGroup,(HMENU)((ULONG_PTR)(2001)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,TEXT("4000"),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,215, 30, 75, 24,hGroup,(HMENU)((ULONG_PTR)(2002)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("MaxGames"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 70, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("MaxGameLife"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 100, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("AdminPort"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 130, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("MaxPreferUsers"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 160, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("MaxPacketPerSecond"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 190, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		CreateWindowEx(0,WC_STATIC,TEXT("MOTD"),WS_CHILD | WS_VISIBLE | SS_RIGHT,10, 220, 145, 24,hGroup,(HMENU)((ULONG_PTR)(0)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,160, 70, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2003)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,160, 100, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2004)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,160, 130, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2005)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,160, 160, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2006)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER,160, 190, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2007)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER | ES_AUTOHSCROLL,160, 220, 130, 24,hGroup,(HMENU)((ULONG_PTR)(2008)),NULL,(LPARAM)((LPVOID)(NULL)));
		/* PvP Game Network Service */
		hGroup = CreateWindowEx(0,WC_BUTTON,(LPCWSTR)(TEXT("PVPGN")),WS_CHILD | WS_VISIBLE | BS_GROUPBOX,310, 75, 300, 80,hGroup0,(HMENU)((ULONG_PTR)(10002)),NULL,(LPARAM)((LPVOID)(NULL)));
		SetWindowSubclass(hGroup, GroupSubclassProc, 1001, 0);
		/*hDummy = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 0, 20, 20, hGroup, (HMENU)((ULONG_PTR)(1003)), NULL, (LPARAM)((LPVOID)(NULL)));*/
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Run"), WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1001)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Kill"), WS_CHILD | WS_VISIBLE | WS_BORDER, 236, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1002)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_IPADDRESS,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,10, 30, 200, 24,hGroup,(HMENU)((ULONG_PTR)(2001)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,TEXT("6112"),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,215, 30, 75, 24,hGroup,(HMENU)((ULONG_PTR)(2002)),NULL,(LPARAM)((LPVOID)(NULL)));
		/* Database Service */
		hGroup = CreateWindowEx(0,WC_BUTTON,(LPCWSTR)(TEXT("D2DBS")),WS_CHILD | WS_VISIBLE | BS_GROUPBOX,310, 250, 300, 80,hGroup0,(HMENU)((ULONG_PTR)(10003)),NULL,(LPARAM)((LPVOID)(NULL)));
		SetWindowSubclass(hGroup, GroupSubclassProc, 1001, 0);
		/*hDummy = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 0, 20, 20, hGroup, (HMENU)((ULONG_PTR)(1003)), NULL, (LPARAM)((LPVOID)(NULL)));*/
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Run"), WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1001)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Kill"), WS_CHILD | WS_VISIBLE | WS_BORDER, 236, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1002)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_IPADDRESS,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,10, 30, 200, 24,hGroup,(HMENU)((ULONG_PTR)(2001)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,TEXT("6114"),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,215, 30, 75, 24,hGroup,(HMENU)((ULONG_PTR)(2002)),NULL,(LPARAM)((LPVOID)(NULL)));
		/* Control Service */
		hGroup = CreateWindowEx(0,WC_BUTTON,(LPCWSTR)(TEXT("D2CS")),WS_CHILD | WS_VISIBLE | BS_GROUPBOX,310, 160, 300, 80,hGroup0,(HMENU)((ULONG_PTR)(10004)),NULL,(LPARAM)((LPVOID)(NULL)));
		SetWindowSubclass(hGroup, GroupSubclassProc, 1001, 0);
		/*hDummy = CreateWindowEx(0, WC_BUTTON, NULL, WS_CHILD | WS_VISIBLE | BS_AUTOCHECKBOX, 150, 0, 20, 20, hGroup, (HMENU)((ULONG_PTR)(1003)), NULL, (LPARAM)((LPVOID)(NULL)));*/
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Run"), WS_CHILD | WS_VISIBLE | WS_BORDER, 175, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1001)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0, WC_BUTTON, TEXT("Kill"), WS_CHILD | WS_VISIBLE | WS_BORDER, 236, 0, 60, 20, hGroup, (HMENU)((ULONG_PTR)(1002)), NULL, (LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_IPADDRESS,NULL,WS_CHILD | WS_VISIBLE | WS_BORDER,10, 30, 200, 24,hGroup,(HMENU)((ULONG_PTR)(2001)),NULL,(LPARAM)((LPVOID)(NULL)));
		hDummy = CreateWindowEx(0,WC_EDIT,TEXT("6113"),WS_CHILD | WS_VISIBLE | WS_BORDER | ES_NUMBER | ES_CENTER,215, 30, 75, 24,hGroup,(HMENU)((ULONG_PTR)(2002)),NULL,(LPARAM)((LPVOID)(NULL)));
		/* Main */
		hDummy = CreateWindowEx(0,WC_BUTTON,TEXT("Build"),WS_CHILD | WS_VISIBLE | WS_BORDER,5, 345, 615, 24,hWnd,(HMENU)((ULONG_PTR)(1001)),NULL,(LPARAM)((LPVOID)(NULL)));
		InitControls(hGroup0);
		break;
	}
	case(WM_PAINT): {
		hdc = BeginPaint(hWnd, &ps);
		EndPaint(hWnd, &ps);
		break;
	}
	case WM_DESTROY: {
		PostQuitMessage(0);
		break;
	}
	default:
		return DefWindowProc(hWnd, message, wParam, lParam);
	}
	return 0;
}

void InitControls(HWND hWnd) {
	struct d2conf conf;
	char* p = NULL;
	char buf[256] = { 0 };
	wchar_t wbuf[256] = { 0 };
	long i = 0;
	long x = 0;

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.realm);
	if (!d2confl(&conf)) {
		i = d2conff(&conf, d2_fetch_next);
		if (i >= 0) {
			p = d2confg(&conf, i);

			p = strbtw(buf, 256, p, "\"\"");
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(wbuf), (int)(256));
			SetWindowText(GetDlgItem(hWnd, 2001), wbuf);

			p = strbtw(buf, 256, p, "\"\"");
			MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(buf), (int)(-1), (LPWSTR)(wbuf), (int)(256));
			SetWindowText(GetDlgItem(hWnd, 2002), wbuf);
		}
	}
	d2confu(&conf);

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.d2gs);
	if (!d2confl(&conf)) {
		i = d2confs(&conf, "MaxGames");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2003), wbuf);
			}
		}

		i = d2confs(&conf, "MaxGameLife");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2004), wbuf);
			}
		}

		i = d2confs(&conf, "MaxGameLife");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2004), wbuf);
			}
		}

		i = d2confs(&conf, "AdminPort");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2005), wbuf);
			}
		}

		i = d2confs(&conf, "MaxPreferUsers");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2006), wbuf);
			}
		}

		i = d2confs(&conf, "MaxPacketPerSecond");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf) && !strkvp(buf, ':', NULL, &buf[128])) {
				x = strtol(&buf[128], NULL, 16);
				_ltow(x, wbuf, 10);
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2007), wbuf);
			}
		}

		i = d2confs(&conf, "MOTD");
		if (i >= 0) {
			p = d2confg(&conf, i);

			if (!strkvp(p, '=', NULL, buf)) {
				strbtw(&buf[128], 128, buf, "\"\"");
				MultiByteToWideChar(CP_UTF8, 0, (LPCCH)(&buf[128]), (int)(-1), (LPWSTR)(wbuf), (int)(256));
				SetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2008), wbuf);
			}
		}
	}
	d2confu(&conf);
}

void BuildConf(HWND hWnd) {
	char* p = NULL;
	char buf[1024] = { 0 };
	wchar_t wbuf1[256] = { 0 };
	wchar_t wbuf2[256] = { 0 };
	char ipv4[16] = { 0 };
	wchar_t port[16] = { 0 };
	DWORD ipv4_d2gs = 0, ipv4_d2dbs = 0, ipv4_d2cs = 0, ipv4_bnetd = 0;
	WORD port_d2gs = 0, port_d2dbs = 0, port_d2cs = 0, port_bnetd = 0;
	struct d2conf conf;
	long i = 0;

	SendMessage(GetDlgItem(GetDlgItem(hWnd, 10001), 2001), IPM_GETADDRESS, 0, (LPARAM)(&ipv4_d2gs));
	SendMessage(GetDlgItem(GetDlgItem(hWnd, 10002), 2001), IPM_GETADDRESS, 0, (LPARAM)(&ipv4_bnetd));
	SendMessage(GetDlgItem(GetDlgItem(hWnd, 10003), 2001), IPM_GETADDRESS, 0, (LPARAM)(&ipv4_d2dbs));
	SendMessage(GetDlgItem(GetDlgItem(hWnd, 10004), 2001), IPM_GETADDRESS, 0, (LPARAM)(&ipv4_d2cs));

	GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2002), port, 16);
	port_d2gs = (WORD)wcstol(port, NULL, 10);
	GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10002), 2002), port, 16);
	port_bnetd = (WORD)wcstol(port, NULL, 10);
	GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10003), 2002), port, 16);
	port_d2dbs = (WORD)wcstol(port, NULL, 10);
	GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10004), 2002), port, 16);
	port_d2cs = (WORD)wcstol(port, NULL, 10);

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.realm);
	if (!d2confl(&conf)) {
		i = d2conff(&conf, d2_fetch_next);
		if (i >= 0) {
			GetWindowText(GetDlgItem(hWnd, 2001), wbuf1, 256);
			GetWindowText(GetDlgItem(hWnd, 2002), wbuf2, 256);
			stripv4(ipv4, ipv4_d2cs);
			sprintf(
				buf,
				"\"%ls\" \"%ls\" %s:%hu",
				wbuf1, wbuf2,
				ipv4,
				port_d2cs
			);
			d2confr(&conf, i, buf);
		}
		d2confw(&conf, d2server.out_conf.realm);
	}
	d2confu(&conf);

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.d2cs);
	if (!d2confl(&conf)) {
		i = d2confs(&conf, "realmname");
		if (i >= 0) {
			GetWindowText(GetDlgItem(hWnd, 2001), wbuf1, 256);
			sprintf(buf, "realmname=\"%ls\"", wbuf1);
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "gameservlist");
		if (i >= 0) {
			stripv4(ipv4, ipv4_d2gs);
			sprintf(buf, "gameservlist=%s", ipv4);
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "bnetdaddr");
		if (i >= 0) {
			stripv4(ipv4, ipv4_bnetd);
			sprintf(buf, "bnetdaddr=%s:%hu", ipv4, port_bnetd);
			d2confr(&conf, i, buf);
		}
		d2confw(&conf, d2server.out_conf.d2cs);
	}
	d2confu(&conf);

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.d2dbs);
	if (!d2confl(&conf)) {
		i = d2confs(&conf, "gameservlist");
		if (i >= 0) {
			stripv4(ipv4, ipv4_d2gs);
			sprintf(buf, "gameservlist=%s", ipv4);
			d2confr(&conf, i, buf);
		}
		d2confw(&conf, d2server.out_conf.d2dbs);
	}
	d2confu(&conf);

	memset(&conf, 0, sizeof(struct d2conf));
	d2confi(&conf, d2server.in_conf.d2gs);
	if (!d2confl(&conf)) {
		i = d2confs(&conf, "D2CSIP");
		if (i >= 0) {
			stripv4(ipv4, ipv4_d2cs);
			sprintf(buf, "\"D2CSIP\"=\"%s\"", ipv4);
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "D2CSPort");
		if (i >= 0) {
			sprintf(buf, "\"D2CSPort\"=dword:%08x", (unsigned int)(port_d2cs));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "D2DBSIP");
		if (i >= 0) {
			stripv4(ipv4, ipv4_d2dbs);
			sprintf(buf, "\"D2DBSIP\"=\"%s\"", ipv4);
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "D2DBSPort");
		if (i >= 0) {
			sprintf(buf, "\"D2DBSPort\"=dword:%08x", (unsigned int)(port_d2dbs));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "MaxGames");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2003), wbuf1, 256);
			sprintf(buf, "\"MaxGames\"=dword:%08x", (unsigned int)(wcstol(wbuf1, NULL, 10)));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "MaxGameLife");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2004), wbuf1, 256);
			sprintf(buf, "\"MaxGameLife\"=dword:%08x", (unsigned int)(wcstol(wbuf1, NULL, 10)));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "AdminPort");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2005), wbuf1, 256);
			sprintf(buf, "\"AdminPort\"=dword:%08x", (unsigned int)(wcstol(wbuf1, NULL, 10)));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "MaxPreferUsers");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2006), wbuf1, 256);
			sprintf(buf, "\"MaxPreferUsers\"=dword:%08x", (unsigned int)(wcstol(wbuf1, NULL, 10)));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "MaxPacketPerSecond");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2007), wbuf1, 256);
			sprintf(buf, "\"MaxPacketPerSecond\"=dword:%08x", (unsigned int)(wcstol(wbuf1, NULL, 10)));
			d2confr(&conf, i, buf);
		}
		i = d2confs(&conf, "MOTD");
		if (i >= 0) {
			GetWindowText(GetDlgItem(GetDlgItem(hWnd, 10001), 2008), wbuf1, 256);
			sprintf(buf, "\"MOTD\"=\"%ls\"", wbuf1);
			d2confr(&conf, i, buf);
		}
		d2confw(&conf, d2server.out_conf.d2gs);
	}
	d2confu(&conf);
}

DWORD WINAPI ThreadScanProcess(void* lp) {
	struct d2procl* pl = NULL;
	char list[256] = {0};
	pl = d2nprocl(4096);
	if (!pl) { return(0xFFFFFFFF); }
	sprintf(list,"[%ls][%ls][%ls][%ls]",d2server.exe_f.d2gs,d2server.exe_f.bnetd,d2server.exe_f.d2cs,d2server.exe_f.d2dbs);
	while (d2lproc(pl)) {
		g_dwProcessMask = d2qproc(pl, list);
		d2ulproc(pl);
		if (lp && *((HWND*)(lp))) { SendMessage((HWND) * ((HWND*)(lp)), WM_APP + 100, (WPARAM)(0), (LPARAM)(0)); }
		Sleep(5000);
	}
	d2dprocl(pl);
	return(0);
}

LRESULT GroupSubclassProc(HWND hWnd, UINT uMsg, WPARAM wParam, LPARAM lParam, UINT_PTR uIdSubclass, DWORD_PTR dwRefData) {
	WCHAR* pwcDirectory=NULL;
	WCHAR wcFilename[256];
	ZeroMemory(wcFilename, sizeof(wcFilename));
	switch (uMsg) {
	case(WM_COMMAND): {
		switch (LOWORD(wParam)) {
		case(1001): {
			switch (GetDlgCtrlID(hWnd)) {
			case(10001): {
				swprintf(wcFilename, 256, L"%s\\%s", d2server.exe_d.d2gs, d2server.exe_f.d2gs);
				pwcDirectory = (WCHAR*)(d2server.exe_d.d2gs);
				break;
			}
			case(10002): {
				swprintf(wcFilename, 256, L"%s\\%s", d2server.exe_d.bnetd, d2server.exe_f.bnetd);
				pwcDirectory = (WCHAR*)(d2server.exe_d.bnetd);
				break;
			}
			case(10003): {
				swprintf(wcFilename, 256, L"%s\\%s", d2server.exe_d.bnetd, d2server.exe_f.d2dbs);
				pwcDirectory = (WCHAR*)(d2server.exe_d.bnetd);
				break;
			}
			case(10004): {
				swprintf(wcFilename, 256, L"%s\\%s", d2server.exe_d.bnetd, d2server.exe_f.d2cs);
				pwcDirectory = (WCHAR*)(d2server.exe_d.bnetd);
				break;
			}
			}/*End Switch*/
			RunProcess(wcFilename, pwcDirectory, SW_HIDE);
			break;
		}
		case(1002): {
			switch (GetDlgCtrlID(hWnd)) {
			case(10001): {
				KillProcess(d2server.exe_f.d2gs);
				break;
			}
			case(10002): {
				KillProcess(d2server.exe_f.bnetd);
				break;
			}
			case(10003): {
				KillProcess(d2server.exe_f.d2dbs);
				break;
			}
			case(10004): {
				KillProcess(d2server.exe_f.d2cs);
				break;
			}
			}/*End Switch*/
			break;
		}
		}/*End Switch*/
		return(0);
	}
	}/*End Switch*/
	return(DefSubclassProc(hWnd,uMsg,wParam,lParam));
}

void RunProcess(const WCHAR* const filename, const WCHAR* const directory, WORD show) {
	PROCESS_INFORMATION pi;
	STARTUPINFO si;
	if (!filename || !*filename) { return; }
	ZeroMemory(&pi, sizeof(PROCESS_INFORMATION));
	ZeroMemory(&si, sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO);
	si.dwFlags = STARTF_USESHOWWINDOW;
	si.wShowWindow = show;
	if (CreateProcess(filename, NULL, NULL, NULL, FALSE, 0, NULL, directory, &si, &pi)) {
		if (pi.hProcess) { CloseHandle(pi.hProcess); }
		if (pi.hThread) { CloseHandle(pi.hThread); }
	}
}

void KillProcess(const WCHAR* const name) {
	struct d2procl* pl=NULL;
	HANDLE hproc=NULL;
	DWORD pid= 0;
	pl=d2nprocl(4096);
	if (!pl) { return; }
	d2lproc(pl);
	pid = d2fprocw(pl,name);
	if (pid) { 
		hproc = OpenProcess(PROCESS_TERMINATE, FALSE, pid);
		if (hproc) {
			TerminateProcess(hproc, 0);
			CloseHandle(hproc);
		}
	}
	if (pl) { d2dprocl(pl); }
	pl = NULL;
}