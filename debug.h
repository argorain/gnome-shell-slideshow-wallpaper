#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
#define DBG(msg) printf("[DEBUG] %s\n", msg);
#else
#define DBG(msg)
#endif

#endif // __DEBUG_H__