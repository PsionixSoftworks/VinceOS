#ifndef _VERSION_H
#define _VERSION_H

typedef struct
{
	const char *kernel_name;
	const char *kernel_version;
} kernel_info_t;

#define OS_SETUP(name, version)	static kernel_info_t info = {.kernel_name=name, .kernel_version=version};

#endif
