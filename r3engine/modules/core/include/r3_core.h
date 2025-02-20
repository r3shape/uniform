#pragma once

#include "memory/r3_memory.h"

#include "platform/r3_input.h"
#include "platform/r3_event.h"
#include "platform/r3_platform.h"

#include "utility/r3_math.h"
#include "utility/r3_array.h"
#include "utility/r3_fileio.h"
#include "utility/r3_hashmap.h"
#include "utility/r3_linked_list.h"

#include "graphics/r3_gl.h"

R3_API void r3_init_core(void);
R3_API void r3_shutdown_core(void);
