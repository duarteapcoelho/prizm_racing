#pragma once

#ifdef GINT
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#include <gint/kmalloc.h>
#endif

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

#ifdef CE
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif

#ifdef SDL
#include "stdio.h"
#include "stdlib.h"
#include "string.h"
#endif
