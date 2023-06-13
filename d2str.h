#ifndef _D2STR_
#define _D2STR_

#include <stdlib.h>
#include <string.h>

char* __cdecl strbtw(char* buf, long cb, const char* const str, const char* const enc);
void  __cdecl stripv4(char* buf, unsigned int ipv4);
int __cdecl strkvp(const char* const pair, int designator, char* key, char* value);

#endif/*_D2STR_*/