#include "d2conf.h"

static int __cdecl d2larrayall(struct d2larray* arr, long size);
static void __cdecl d2larrayfr(struct d2larray* arr);
static int __cdecl d2larrayadd(struct d2larray* arr, const char* const line);

static int __cdecl d2larrayall(struct d2larray* arr,long size) {
	char** data = NULL;
	data = malloc(size *sizeof(char*));
	if (!data) { return(-3); };
	arr->data = data;
	arr->size = size;
	arr->count = 0;
	return(0);
}

static void __cdecl d2larrayfr(struct d2larray* arr) {
	long i = 0;
	for (i = 0; i < arr->count;i++) {
		free(arr->data[i]);
	}
	free(arr->data);
	arr->data = NULL;
	arr->size = 0;
	arr->count = 0;
}

static int __cdecl d2larrayadd (struct d2larray* arr, const char* const line){
	char** data = NULL;
	char* buf=NULL;
	if (arr->count >= arr->size) {
		data = realloc(arr->data, (arr->size * 2) * sizeof(char*));
		if (!data) { return(-3); }
		arr->data = data;
		arr->size *= 2;
	}
	buf = calloc(strlen(line) + 1, sizeof(char));
	if (!buf) { return(-3); }
	strcpy(buf, line);
	arr->data[arr->count++] = buf;
	return(0);
}



void __cdecl d2confi(struct d2conf* conf, const wchar_t* const filename) {
	conf->path = (wchar_t*)(filename);
	conf->fp = NULL;
	memset(&conf->fd, 0, sizeof(struct d2fetchd));
	d2larrayall(&conf->arr, 32);
}

void __cdecl d2confu(struct d2conf* conf) {
	if (conf->fp) { fclose(conf->fp); }
	d2larrayfr(&conf->arr);
}

int __cdecl d2confl(struct d2conf* conf) {
	FILE* fp=NULL;
	char* buf=NULL;
	int err = 0;
	fp = _wfopen(conf->path,L"rt");
	if (!fp) { err = (-1); goto Error; }
	buf = calloc(4096,sizeof(char));
	if (!buf) { err = (-3); goto Error; }
	while (!feof(fp)) {
		*buf = '\0';
		fgets(buf,4096, fp);
		if (*buf) {
			buf[strlen(buf) - 1] = '\0';
			d2larrayadd(&conf->arr, (const char* const)(buf));
		}
	}
Error:
	if (fp) { fclose(fp); fp = NULL; }
	if (buf) { free(buf); buf = NULL; }
	return(err);
}

long __cdecl d2confs(struct d2conf* conf, const char* key) {
	char* match = NULL;
	long i = 0;
	for (i = 0; i < conf->arr.count; i++) {
		match = strstr(conf->arr.data[i], key);
		if (match != NULL && *conf->arr.data[i] != '\0' && *conf->arr.data[i] != '#') {
			return(i);/*return(match)*/
		}
	}
	return(-1);
}

long __cdecl d2conff(struct d2conf* conf, long mode) {
	long start = 0;
	long idx = 0;
	switch (mode) {
	case(d2_fetch_set): {
		start = 0;
		break;
	}
	case(d2_fetch_cur): {
		start = conf->fd.curr;
		break;
	}
	case(d2_fetch_end): {
		start = conf->arr.count - 1;
		break;
	}
	case(d2_fetch_prev): {
		start = conf->fd.prev;
		break;
	}
	case(d2_fetch_next): {
		start = conf->fd.curr + 1;
		break;
	}
	}/*End Siwtch*/
	for (idx = start; idx < conf->arr.count; idx++) {
		if (*conf->arr.data[idx] != '\0' && *conf->arr.data[idx] != '#') {
			conf->fd.prev = conf->fd.curr;
			conf->fd.curr = idx;
			return(idx);
		}
	}
	return(-1);
}

const char* __cdecl d2confg(const struct d2conf* const conf, long idx) {
	return((const char*)(conf->arr.data[idx]));
}

int __cdecl d2confr(struct d2conf* conf, long idx, const char* const str) {
	char* p = NULL, * p2 = NULL;
	p = (char*)d2confg(conf, idx);
	if (p) {
		if (strlen(str) > strlen(p)) {
			p2 = calloc(strlen(str) + 1, sizeof(char));
			if (p2) {
				free(p);
				p = p2;
			}
			else { p = NULL; }
		} 
	}
	if (!p) { return(-3); }
	conf->arr.data[idx] = p;
	strcpy(p,str);
	return(0);
}

int __cdecl d2confw(struct d2conf* conf, const wchar_t* filename) {
	FILE* fp = NULL;
	long i = 0;
	fp = _wfopen(filename, L"wt");
	if (!fp) { return(-1); }
	for (i = 0; i < conf->arr.count; i++) {
		fputs(conf->arr.data[i], fp);
		fputs("\n", fp);
	}
	if (fp) { fclose(fp); fp = NULL; }
	return(0);
}