#ifndef _D2GLOBALS_
#define _D2GLOBALS_

#include <windows.h>

#define PROCESS_MASK_D2GS              0x00000001
#define PROCESS_MASK_BNETD             0x00000002
#define PROCESS_MASK_D2CS              0x00000004
#define PROCESS_MASK_D2DBS             0x00000008

extern DWORD g_dwProcessMask;

#define PROCESS_IS_RUNNING_D2GS()      (g_dwProcessMask & PROCESS_MASK_D2GS)
#define PROCESS_IS_RUNNING_BNETD()     (g_dwProcessMask & PROCESS_MASK_BNETD)
#define PROCESS_IS_RUNNING_D2CS()      (g_dwProcessMask & PROCESS_MASK_D2CS)
#define PROCESS_IS_RUNNING_D2DBS()     (g_dwProcessMask & PROCESS_MASK_D2DBS)


extern HANDLE g_hThreadScanProcess;


struct D2FCOLL {
	WCHAR d2gs[256];
	WCHAR d2cs[256];
	WCHAR d2dbs[256];
	WCHAR bnetd[256];
	WCHAR realm[256];
};

struct D2DCOLL {
	WCHAR d2gs[256];
	WCHAR bnetd[256];
};

struct D2SERVER {
	struct D2FCOLL in_conf;
	struct D2FCOLL out_conf;
	struct D2FCOLL exe_f;
	struct D2DCOLL exe_d;
};

extern struct D2SERVER d2server;

#endif/*_D2GLOBALS_*/