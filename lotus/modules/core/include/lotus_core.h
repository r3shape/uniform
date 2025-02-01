#pragma once

#include "memory/lotus_memory.h"

#include "platform/lotus_input.h"
#include "platform/lotus_event.h"
#include "platform/lotus_platform.h"

#include "utility/lotus_math.h"
#include "utility/lotus_array.h"
#include "utility/lotus_fileio.h"
#include "utility/lotus_hashmap.h"
#include "utility/lotus_linked_list.h"

#include "graphics/lotus_gl.h"

LOTUS_API void lotus_init_core(void);
LOTUS_API void lotus_shutdown_core(void);
