#include "d2str.h"

char* __cdecl strbtw(char* buf, long cb, const char* const str, const char* const enc) {
	char* p = (char*)(0);
	int ch = 0;
	int gate = 0;
	p = (char*)(str);
	if (!cb) { return(p); }
	while ((ch=*p++)) {
		if (ch == enc[0]) { if (gate) { gate = 2; break; } else { gate = 1; } }
		else if (ch == enc[1]) { if (gate) { gate = 2; break; } }
		else { if (gate==1) { if (buf && (--cb > 0)) { *buf++ = ch; } } }
	}
	if (buf) { *buf = '\0'; }
	if (!gate) { return((char*)(str)); }
	return(p);
}

void  __cdecl stripv4(char* buf, unsigned int ipv4) {
	unsigned addr0 = 0, addr1 = 0, addr2 = 0, addr3 = 0;
	addr0 = (ipv4 & 0xFF000000) >> 24;
	addr1 = (ipv4 & 0x00FF0000) >> 16;
	addr2 = (ipv4 & 0x0000FF00) >> 8;
	addr3 = (ipv4 & 0x000000FF);
	itoa(addr0,buf,10); buf += strlen(buf); *buf++ = '.';
	itoa(addr1,buf,10); buf += strlen(buf); *buf++ = '.';
	itoa(addr2,buf,10); buf += strlen(buf); *buf++ = '.';
	itoa(addr3,buf,10); buf += strlen(buf); *buf++ = '\0';
}

int __cdecl strkvp(const char* const pair, int designator, char * key, char* value) {
	char* pk = NULL, * pv = NULL, * pd = NULL;
	pk = (char*)(pair);
	pd = strchr(pk,designator);
	if (!pd || pd==pk) { return(-1); }
	pv = pd+1;
	if (key) { do { *key++ = *pk; } while (++pk != pd); }
	if (value) { do { *value++ = *pv; } while (*pv++); }
	return(0);
}