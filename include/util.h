#pragma once

#ifdef PRIZM
#include <fxcg/heap.h>
#include <fxcg/misc.h>
#include "string.h"
#include <stdlib.h>
#include <stdio.h>

#define malloc sys_malloc
#define strcpy sys_strcpy
#define rand sys_rand
#define srand sys_srand
#endif

#ifdef SDL
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif
