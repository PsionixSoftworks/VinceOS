#ifndef _CONFIG_H
#define _CONFIG_H

#define OS_VERSION_MAJOR	0
#define OS_VERSION_MINOR	0
#define OS_VERSION_BUILD	1
#define OS_VERSION_RELEASE	20
#define OS_NAME				"VinceOS"
#define OS_VERSION_STR		"0.01u"
#define OS_VERSION			(OS_VERSION_MAJOR * OS_VERSION_MINOR + OS_VERSION_BUILD * OS_VERSION_RELEASE)

#ifndef _VERSION_H
#include "version.h"
#endif

#endif
