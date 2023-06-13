#include "d2proc.h"

struct d2procl* d2nprocl(long count) {
	struct d2procl* pl = NULL;
	DWORD cb = (DWORD)(count) * (DWORD)sizeof(DWORD);
	pl = (struct d2procl*)HeapAlloc(GetProcessHeap(), HEAP_ZERO_MEMORY, sizeof(long) + sizeof(long) + cb);
	if (!pl) { return(NULL); }
	pl->size = count;
	pl->count = 0;
	return(pl);
}

void d2dprocl(struct d2procl* pl) {
	if (pl && pl->size) { HeapFree(GetProcessHeap(), HEAP_ZERO_MEMORY, (LPVOID)(pl)); }
}

DWORD d2lproc(struct d2procl* pl) {
	DWORD cb = 0;
	EnumProcesses(pl->id, pl->size*sizeof(DWORD), &cb);
	pl->count = cb / sizeof(DWORD);
	return(pl->count);
}

void d2ulproc(struct d2procl* pl) {
	pl->count = 0;
}

DWORD d2qproc(const struct d2procl* const pl,const char *const list) {
	HANDLE hproc = NULL;
	char* p = NULL;
	char name1[256];
	char name2[256];
	long i = 0, j = 0;
	DWORD mask = 0;
	p = (char*)(list);
	do {
		p = strbtw(name1, 256, p, "[]");
		for (i = 0; i < pl->count;i++) {
			hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pl->id[i]);
			if (hproc) {
				GetModuleBaseNameA(hproc, NULL, name2, 256);
				CloseHandle(hproc);
				if (!lstrcmpiA(name1, name2)) { mask |= 0x1 << j; break; }
			}
		}
		j++;
	} while (p && *p);
	return(mask);
}

DWORD d2fproc(const struct d2procl* const pl, const char* const name) {
	HANDLE hproc = NULL;
	char name1[256];
	long i = 0;
	DWORD pid = 0;
	for (i = 0; i < pl->count; i++) {
		hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pl->id[i]);
		if (hproc) {
			GetModuleBaseNameA(hproc, NULL, name1, 256);
			CloseHandle(hproc);
			if (!lstrcmpiA(name1, name)) { pid = pl->id[i]; break; }
		}
	}
	return(pid);
}
DWORD d2fprocw(const struct d2procl* const pl, const WCHAR* const name) {
	HANDLE hproc = NULL;
	WCHAR name1[256];
	long i = 0;
	DWORD pid = 0;
	for (i = 0; i < pl->count; i++) {
		hproc = OpenProcess(PROCESS_QUERY_INFORMATION | PROCESS_VM_READ, FALSE, pl->id[i]);
		if (hproc) {
			GetModuleBaseNameW(hproc, NULL, name1, 256);
			CloseHandle(hproc);
			if (!lstrcmpiW(name1, name)) { pid = pl->id[i]; break; }
		}
	}
	return(pid);
}