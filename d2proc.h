#ifndef _D2PROC_
#define _D2PROC_

#include <windows.h>
#include <psapi.h>
#include "d2str.h"

struct d2procl {
	long count;
	long size;
	DWORD id[1];
};

struct d2procl* d2nprocl(long count);
void d2dprocl(struct d2procl* pl);
DWORD d2lproc(struct d2procl* pl);
void d2ulproc(struct d2procl* pl);
DWORD d2qproc(const struct d2procl* const pl, const char* const list);
DWORD d2fproc(const struct d2procl* const pl, const char* const name);
DWORD d2fprocw(const struct d2procl* const pl, const WCHAR* const name);

#endif/*_D2PROC*/