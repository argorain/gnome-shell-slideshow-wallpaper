#ifndef __DEBUG_H__
#define __DEBUG_H__

#include <stdio.h>

#define DEBUG

#ifdef DEBUG
inline void dbg(char *msg){printf("[DEBUG] %s\n", msg);}
#else
inline void dbg(char *msg){}
#endif

#endif // __DEBUG_H__