#ifndef _D2CONF_
#define _D2CONF_

#include <stdio.h>
#include <string.h>
#include <wchar.h>
#include <malloc.h>

struct d2larray {
	long count;
	long size;
	char** data;
};

#define d2_fetch_set         0
#define d2_fetch_cur         1
#define d2_fetch_end         2
#define d2_fetch_prev        3
#define d2_fetch_next        4
struct d2fetchd {
	long curr;
	long prev;
};

struct d2conf {
	wchar_t* path;
	FILE* fp;
	struct d2fetchd fd;
	struct d2larray arr;
};

void __cdecl d2confi(struct d2conf* conf, const wchar_t* const filename);
void __cdecl d2confu(struct d2conf* conf);
int __cdecl d2confl(struct d2conf* conf);
long __cdecl d2confs(struct d2conf* conf, const char* key);
long __cdecl d2conff(struct d2conf* conf, long mode);
const char* __cdecl d2confg(const struct d2conf* const conf, long idx);
int __cdecl d2confr(struct d2conf* conf, long idx, const char* const str);
int __cdecl d2confw(struct d2conf* conf, const wchar_t* filename);

#endif/*_D2CONF_*/