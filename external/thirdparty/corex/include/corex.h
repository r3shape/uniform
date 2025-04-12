#ifndef __COREX_H__
#define __COREX_H__

// ==== corex/corex_def.h ====
#ifndef COREX_NO_STDLIB
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <stdarg.h>
#include <stddef.h>
#endif

typedef char*               str;
typedef const char*         cstr;

typedef float               f32;
typedef double              f64;

typedef signed char         i8;
typedef signed short        i16;
typedef signed int          i32;
typedef signed long long    i64;

typedef unsigned char       u8;
typedef unsigned short      u16;
typedef unsigned int        u32;
typedef unsigned long long  u64;

typedef intptr_t            iptr;
typedef uintptr_t           uptr;

#define COREX_TRUE   1
#define COREX_FALSE  0

#define COREX_SWAP(t, a, b) { t temp = a; (a) = (b); (b) = temp; }

#define COREX_HEAP_ARR(t, a) t* n
#define COREX_STACK_ARR(t, a, m) t* a[m]

#define COREX_IP2(x) ((x & (x - 1)) == 0)
#define COREX_ALIGN(p, a) (((p) + ((a) - 1)) & ~((a) - 1))

#define COREX_IGNORE(value)              ((void)v)
#define COREX_DEFAULT(type)              (type){0}
#define COREX_LITERAL(type, value)       (type){value}
#define COREX_DEREF(type, value)         *(type*)value
#define COREX_LITERAL_PTR(type, value)   &(type){value}

#define COREX_WRAPI(value, limit) ((value) % (limit))
#define COREX_WRAPF(value, limit) (fmod((f64)(value), (limit)))

#define COREX_MAX(a, b) ((a) > (b) ? a : b)
#define COREX_MIN(a, b) ((a) < (b) ? a : b)
#define COREX_CLAMP(value, min, max) ((value) > (max) ? (max) : (value) < (min) ? (min) : (value))

#define COREX_FORI(start, stop, step) for (u32 i = start; i < stop; i += step)
#define COREX_FORJ(start, stop, step) for (u32 j = start; j < stop; j += step)
#define COREX_FORK(start, stop, step) for (u32 k = start; k < stop; k += step)
#define COREX_FOR(type, iter, start, stop, step) for (type iter = start; iter < stop; iter += step)

#define COREX_KB  1024
#define COREX_MB  (COREX_KB * 1024)
#define COREX_GB  (COREX_MB * 1024)

#define COREX_FLIP_BIT(v, b)  ((v) ^ (1<<(b)))
#define COREX_SET_BITS(v, b)  ((v) | (1<<(b)))
#define COREX_GET_BITS(v, b)  ((v) & (1<<(b)))
#define COREX_REM_BITS(v, b)  ((v) & ~(1<<(b)))

#define COREX_FNPTR(t, n, ...) t (*n)(__VA_ARGS__)

#define COREX_IMPLEMENT(n, ...)          \
    void n##_impl(void) {               \
        __VA_ARGS__                     \
    }                                   \

#define COREX_DEFINE_ENUM(n, ...)        \
    typedef enum n {                    \
        __VA_ARGS__                     \
    } n                                 \

#define COREX_DEFINE_UNION(n, ...)       \
    typedef union n {                   \
        __VA_ARGS__                     \
    } n                                 \

#define COREX_DEFINE_STRUCT(n, ...)      \
    typedef struct n {                  \
        __VA_ARGS__                     \
    } n                                 \

#ifdef DLL_EXPORT
    #ifdef _MSC_VER
        #define COREX_EXPOSE __declspec(dllexport)
    #elif #defined (__GNUC__) || defined (__clang__)
        #define COREX_EXPOSE __attribute__((visibility("default")))
    #else
        #define COREX_EXPOSE
    #endif
#else
    #ifdef _MSC_VER
        #define COREX_EXPOSE __declspec(dllimport)
    #elif defined(__GNUC__) || defined(__clang__)
        #define COREX_EXPOSE __attribute__((visibility("default")))
    #else
        #define COREX_EXPOSE
    #endif
#endif

// ==== corex/memx/corex_memx.h ====
/**
 * A generic 16 byte `Blob` structure for safely passing and casting types.
 * - `void* data`: This field stores an 8 byte pointer to some data. 
 * - `u32 stride`: This field stores stride metadata about the `Blob` structure.
 * - `u32 size`: This field stores size metadata about the `Blob` structure.
 *
 * #### NOTE: The `Blob::data` field can be safely converted to a `uptr` or `iptr` type.
 */
typedef struct Blob {
    void* data;
    u64 size;
} Blob;

typedef enum Alloc_Type {
    ALLOC_DEFAULT,
    ALLOC_ARENA,
    ALLOC_TYPES
} Alloc_Type;

typedef struct Allocator {
    Alloc_Type type;
    union context {
        struct arena {
            void* buffer;
            u64 offset;
            u64 commit;
            u64 max;
        } arena;
    } context;
} Allocator;

typedef struct Memx {
    u8 init;
    void (*dealloc)(void* ptr);
    void (*zero)(void* ptr, u64 size);
    void* (*alloc)(u64 size, u64 align);
    void* (*realloc)(void* ptr, u64 size, u64 align);

    u8 (*blob_alloc)(Blob* blob, u64 align);
    u8 (*blob_realloc)(Blob* blob, u64 size, u64 align);
    u8 (*blob_dealloc)(Blob* blob);

    u8 (*create_allocator)(Alloc_Type type, u64 max, u64 align, Allocator* allocator);
    u8 (*destroy_allocator)(Allocator* allocator);

    void* (*arena_alloc)(u64 size, u64 align, Allocator* allocator);
    void (*arena_reset)(Allocator* allocator);
} Memx;

u8 _corex_init_memx(void);
void _corex_cleanup_memx(void);

// ==== corex/dsx/corex_dsx.h ====
/* ---------------- ARRAY ---------------- */
typedef struct Array_Head {
    u32 count, max;
    u32 size, stride;
} Array_Head;

typedef enum Array_Head_Field {
    ARRAY_SIZE_FIELD = 0,
    ARRAY_STRIDE_FIELD,
    ARRAY_COUNT_FIELD,
    ARRAY_MAX_FIELD,
    ARRAY_FIELDS,
} Array_Head_Field;
/* ---------------- ARRAY ---------------- */


/* ---------------- LINKED ARRAY ---------------- */
typedef struct Linked_Array {
    struct Linked_Array* last;
    struct Linked_Array* next;
    void* array;
    Array_Head meta;
} Linked_Array;
/* ---------------- LINKED ARRAY ---------------- */


/* ---------------- HASH ARRAY ---------------- */
typedef struct Key_Value {
    void* value;
    cstr key;
} Key_Value;

typedef struct Hash_Array {
    Key_Value* map;
    Array_Head meta;
} Hash_Array;
/* ---------------- HASH ARRAY ---------------- */

typedef struct Dsx {
    u8 init;
    struct array {
        void (*destroy_array)(void* array);

        /**
         * `push_array` is used to ensure a value is inserted at the next available slot in an array BASED ON THE CURRENT NUMBER OF ELEMENTS within the array.
         * Be careful using `push_array` after `n` calls to `put_array` as `push_array` will skip the first `n` slots in an array, and may over-write the inserts.
         */
        void (*push_array)(void* array, void* invalue);

        /**
         * `pop_array` is used to ensure a value is returned from the last occupied slot in an array BASED ON THE CURRENT NUMBER OF ELEMENTS within the array.
         * Be careful using `pop_array` after `n` calls to `put_array` as `pop_array` will start at index `n - 1` in an array, and fail to return inserted values.
         */
        void (*pop_array)(void* array, void* outvalue);
        
        void* (*create_array)(u32 stride, u32 max);
        void* (*resize_array)(void* array, u32 max);
        
        /**
         * `put_array` is used to manually write data to an array.
         * The data written with `put_array` is over-written with `n` calls to `push_array`, where `n` is equal to the `index` inserted to, ONLY IF the `index` inserted to is greater than the number of elements in the array.
         * Calling `push_array` after `n` calls to `put_array` ensures the first `n` slots of the array are never over-written by subsequent calls to `push_array`.
         */
        void (*put_array)(void* array, u32 index, void* invalue);
        
        /**
         * `pull_array` is used to return a value from a specific index in an array
         * Calling `pull_array` DOES shift/reorder elements in the array, instead the value at the removed index is set to 0, maintaining the integrity of the structure of data within the array.
         */
        void (*pull_array)(void* array, u32 index, void* outvalue);

        /**
         * `get_array_head` is used to return the metadata about an array that is stored in memory along with the array.
         */
        Array_Head (*get_array_head)(void* array);

        /**
         * `create_linked_array` dynamically allocates and returns a doubly-linked `Linked_Array` structure which is handy for linking blocks of memory containing *n* elements.
         * Any `Linked_Array` structure created with `create_linked_array` has its array field allocated via `create_array`, thus all other array API functions are applicable.  
         */
        Linked_Array* (*create_linked_array)(Linked_Array* array, u32 stride, u32 max);
        
        /**
         * `destroy_linked_array` destroys a specific link within a linked array structure whilst maintaining structural integrity.
         * Use `collapse_linked_array` to recursively destroy all links in a linked array structure from any link in the structure.
         */
        void (*destroy_linked_array)(Linked_Array* array);

        /**
         * `collapse_linked_array` recursively collapses an entire linked array structure from any link in the structure.
         * Use `destroy_linked_array` to destroy a specific link whilst maintaining structural integrity.
         */
        void (*collapse_linked_array)(Linked_Array* array);

        Hash_Array* (*create_hash_array)(u32 max);
        u8 (*put_hash_array)(Hash_Array* array, cstr key, void* value);
        void* (*get_hash_array)(Hash_Array* array, cstr key);
        
        /**
         * `get_hash_array_keys` iterates a given `Hash_Array` and populates a heap-allocated array
         * containing all the keys stored in the `Hash_Array`.
         * 
         * Note: Don't forget to call `structx->destroy_array()` on the array returned from `get_hash_array_keys` when no longer needed!
         */
        cstr* (*get_hash_array_keys)(Hash_Array* array);
        
        /**
         * `get_hash_array_values` iterates a given `Hash_Array` and populates a heap-allocated array
         * containing all the values stored in the `Hash_Array`.
         * 
         * Note: Don't forget to call `structx->destroy_array()` on the array returned from `get_hash_array_values` when no longer needed!
         */
        void** (*get_hash_array_values)(Hash_Array* array);
        u8 (*pull_hash_array)(Hash_Array* array, cstr key, Key_Value* out);
        void (*destroy_hash_array)(Hash_Array* array);

    } array;
} Dsx;

u8 _corex_init_dsx(void);
void _corex_cleanup_dsx(void);

// ==== corex/genx/corex_genx.h ====
typedef enum Buffer_Type {
    BUFFER_TYPE_NONE,
    BUFFER_TYPE_PTR,
    BUFFER_TYPE_CHAR,
    BUFFER_TYPE_UCHAR,
    BUFFER_TYPE_SCHAR,
    BUFFER_TYPE_INT8,
    BUFFER_TYPE_INT16,
    BUFFER_TYPE_INT32,
    BUFFER_TYPE_INT64,
    BUFFER_TYPE_UINT8,
    BUFFER_TYPE_UINT16,
    BUFFER_TYPE_UINT32,
    BUFFER_TYPE_UINT64,
    BUFFER_TYPE_FLOAT32,
    BUFFER_TYPE_FLOAT64,
    BUFFER_TYPES
} Buffer_Type;

typedef enum Buffer_Size {
    BUFFER_SIZE_NONE,
    BUFFER_SIZE_1B =    0x0001,
    BUFFER_SIZE_2B =    0x0002,
    BUFFER_SIZE_4B =    0x0004,
    BUFFER_SIZE_8B =    0x0008,
    BUFFER_SIZE_16B =   0x0010,
    BUFFER_SIZE_32B =   0x0020,
    BUFFER_SIZE_64B =   0x0040,
    BUFFER_SIZE_128B =  0x0080,
    BUFFER_SIZE_256B =  0x0100,
    BUFFER_SIZE_512B =  0x0200,
    BUFFER_SIZE_1KB =   0x0400,
    BUFFER_SIZE_2KB =   0x0800,
    BUFFER_SIZE_4KB =   0x1000,
    BUFFER_SIZE_MAX =   0x1000
} Buffer_Size;

typedef enum Buffer_Access {
    BUFFER_ACCESS_NONE,
    BUFFER_ACCESS_R,
    BUFFER_ACCESS_RW,
    BUFFER_ACCESS_TYPES
} Buffer_Access;

typedef enum Buffer_State {
    BUFFER_STATE_NONE,
    BUFFER_STATE_ALLOC,
    BUFFER_STATE_FREE,
    BUFFER_STATE_VALID,
    BUFFER_STATE_INVALID,
    BUFFER_STATES
} Buffer_State;

typedef enum Buffer_Flag {
    BUFFER_FLAG_AUTOCURSOR,
    BUFFER_FLAGS
} Buffer_Flag;

typedef enum Buffer_Cursor {
    BUFFER_CURSOR_NONE,
    BUFFER_CURSOR_BIT,
    BUFFER_CURSOR_BYTE,
    BUFFER_CURSOR_TYPES,
} Buffer_Cursor;

typedef enum Buffer_Field {
    BUFFER_FIELD_NONE,
    BUFFER_FIELD_TYPE,
    BUFFER_FIELD_SIZE,
    BUFFER_FIELD_ACCESS,
    BUFFER_FIELD_STATE,
    BUFFER_FIELD_WRITTEN,
    BUFFER_FIELD_BIT,
    BUFFER_FIELD_BYTE,
    BUFFER_FIELD_FLAGS,
    BUFFER_FIELDS
} Buffer_Field;

typedef struct Buffer_Layout {
    u8 type;
    u64 size;
    u8 access;
} Buffer_Layout;

typedef struct Buffer {
    void* data;
    struct meta {
        struct cursor {
            u64 written;
            u64 byte;
            u8 bit;
        } cursor;
        u64 size;
        u8 flags;
        u8 type;
        u8 state;
        u8 access;
    } meta;
} Buffer;

typedef struct Genx {
    struct bin {
        u8 (*depth8)(u8 value);
        u8 (*pack8)(u8 value1, u8 value2);
        u8* (*unpack8)(u8 value, u8 depth, u8* out);
    } bin;

    struct buffer {
        u8 (*create)(Buffer_Layout layout, Buffer* buffer);
        u8 (*reset)(Buffer* buffer);
        u8 (*destroy)(Buffer* buffer);
        
        u8 (*read)(u64 count, void* bytes, Buffer* buffer);
        u8 (*write)( u64 count, void* bytes, Buffer* buffer);
        u8 (*peek)(u64 offset, u64 count, void* bytes, Buffer* buffer);
        u8 (*inject)(u64 offset, u64 count, void* bytes, Buffer* buffer);
        
        u8 (*rewind)(Buffer_Cursor cursor, Buffer* buffer);
        u8 (*seek)(Buffer_Cursor cursor, u64 byte, Buffer* buffer);
        u8 (*seekr)(Buffer_Cursor cursor, u64 offset, Buffer* buffer);
        
        u8 (*set_flag)(Buffer_Flag flag, Buffer* buffer);
        u8 (*has_flag)(Buffer_Flag flag, Buffer* buffer);
        u8 (*rem_flag)(Buffer_Flag flag, Buffer* buffer);
    } buffer;

    u8 init;
} Genx;

u8 _corex_init_genx(void);
void _corex_cleanup_genx(void);

// ==== corex/ecsx/corex_ecsx.h ====
#define COMPONENT_MAX   (1 << 5)
#define SYSTEM_MAX      (1 << 3)
#define ENTITY_MAX      ((1 << 16) - 1)

typedef u8 (*COMPONENT_ADD_FPTR)(u32 entity);
typedef u8 (*COMPONENT_REM_FPTR)(u32 entity);
typedef u8 (*COMPONENT_GET_FPTR)(u32 entity, void* component);
typedef u8 (*COMPONENT_SYSTEM_FPTR)(u32 entity);

typedef struct Ecsx {
    u8 init;
    struct entity_manager {
        u32* queue;
        u32* mask;
        u32 count;
    } entity_manager;

    struct component_manager {
        void** component_system;
        void** component_storage;
        void** add_component_fptr;
        void** rem_component_fptr;
        void** get_component_fptr;
        u32 count;
    } component_manager;

    u32 (*create_entity)(void);
    u32 (*create_entity_with)(u8 count, u8* ids);
    void (*destroy_entity)(u32 entity);
    
    u8 (*register_component)(
        u8 id, void* storage,
        COMPONENT_ADD_FPTR add_func,
        COMPONENT_REM_FPTR rem_func,
        COMPONENT_GET_FPTR get_func);
    u8 (*unregister_component)(u8 id);

    u8 (*register_system)(u8 id, cstr name, void* system);
    u8 (*unregister_system)(u8 id, cstr name);
    u8 (*unregister_systems)(u8 id);
    
    /**
     * The `get_entities` function returns an array of all entities that have the specified component attached.
     * `get_entities` iterates over all active entities (n), thus the size of the returned array is: `n * sizeof(u32)`.
     * Any entities that do not have this component will be pushed onto the returned array as the value expanded from the `ENTITY_MAX` directive.
     * Note: The caller of `get_entities` must call `structx->destroy_array()` on the return value to ensure no memory is leaked.
     */
    u32* (*get_entities)(u8 id);
    u8 (*run_systems)(u8 id);
    u8 (*run_system)(u8 id, cstr name);
    u8 (*add_component)(u8 id, u32 entity);
    u8 (*rem_component)(u8 id, u32 entity);
    u8 (*has_component)(u8 id, u32 entity);
    u8 (*get_component)(u8 id, u32 entity, void* component);
} Ecsx;

u8 _corex_init_ecsx(void);
void _corex_cleanup_ecsx(void);

// ==== corex/mathx/corex_mathx.h ====
#include <math.h>


#define COREX_PI 3.14159265358979323846

/* ---------------- VECTOR ---------------- */
typedef struct Vec2 { f32 x, y; } Vec2;
typedef struct Vec3 { f32 x, y, z; } Vec3;
typedef struct Vec4 { f32 x, y, z, w; } Vec4;
/* ---------------- VECTOR ---------------- */

/* ---------------- MATRIX 4 ---------------- */
typedef struct Mat2 { f32  m[4]; } Mat2;
typedef struct Mat3 { f32  m[9]; } Mat3;
typedef struct Mat4 { f32 m[16]; } Mat4;
/* ---------------- MATRIX 4 ---------------- */

typedef struct Mathx {
    u8 init;
    struct scalar {
        // Scalar Math
        f32 (*radians)(f32 deg);
    } scalar;
    
    struct vec {
        // Utility
        void (*print2)(cstr name, Vec2 vec);
        void (*print3)(cstr name, Vec3 vec);
        void (*print4)(cstr name, Vec4 vec);
        
        // Vec2 Operations
        Vec2 (*vec2)(f32 x, f32 y);
        Vec2 (*scale2)(Vec2 v, f32 scale);
        Vec2 (*add2)(Vec2 v1, Vec2 v2);
        Vec2 (*sub2)(Vec2 v1, Vec2 v2);
        f32 (*dot2)(Vec2 v1, Vec2 v2);
        f32 (*mag2)(Vec2 v);
        Vec2 (*norm2)(Vec2 v);
    
        // Vec3 Operations
        Vec3 (*vec3)(f32 x, f32 y, f32 z);
        Vec3 (*scale3)(Vec3 v, f32 scale);
        Vec3 (*add3)(Vec3 v1, Vec3 v2);
        Vec3 (*sub3)(Vec3 v1, Vec3 v2);
        f32 (*dot3)(Vec3 a, Vec3 b);
        f32 (*mag3)(Vec3 v);
        Vec3 (*norm3)(Vec3 v);
        Vec3 (*cross3)(Vec3 v1, Vec3 v2);
    
        // Vec4 Operations
        Vec4 (*vec4)(f32 x, f32 y, f32 z, f32 w);
        Vec4 (*scale4)(Vec4 v, f32 scale);
        Vec4 (*add4)(Vec4 v1, Vec4 v2);
        Vec4 (*sub4)(Vec4 v1, Vec4 v2);
        f32 (*dot4)(Vec4 v1, Vec4 v2);
        f32 (*mag4)(Vec4 v);
        Vec4 (*norm4)(Vec4 v);
    } vec;

    /**
     * Right-Handed Column-Major Matrix Transformation API
     * (OpenGL Friendly)
     */
    struct mat {
        // Utility
        void (*print2)(cstr name, Mat2 mat);
        void (*print3)(cstr name, Mat3 mat);
        void (*print4)(cstr name, Mat4 mat);
        
        // Mat2 Operations
        Mat2 (*identity2)(void);
        Mat2 (*transpose2)(Mat2 mat);
        Mat2 (*mult2)(Mat2 a, Mat2 b);
        
        // Mat3 Operations
        Mat3 (*identity3)(void);
        Mat3 (*transpose3)(Mat3 mat);
        Mat3 (*mult3)(Mat3 a, Mat3 b);
    
        // Mat4 Operations
        Mat4 (*identity4)(void);
        Mat4 (*transpose4)(Mat4 mat);
        Mat4 (*trans4)(f32 x, f32 y, f32 z);
        Vec3 (*mult4v3)(Mat4 m, Vec3 v);
        Mat4 (*rot4)(Vec3 axis, f32 angle);
        Mat4 (*rotx4)(f32 angle);
        Mat4 (*roty4)(f32 angle);
        Mat4 (*rotz4)(f32 angle);
        Mat4 (*scale4)(f32 x, f32 y, f32 z);
        Mat4 (*mult4)(Mat4 a, Mat4 b);
        Mat4 (*lookat)(Vec3 eye, Vec3 center, Vec3 up);
        Mat4 (*ortho)(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far);

        /**
         * symmetric frustum using vertical FOV
        */
        Mat4 (*perspective)(f32 fov, f32 aspect, f32 near, f32 far);
    } mat;    
} Mathx;

u8 _corex_init_mathx(void);
void _corex_cleanup_mathx(void);

// ==== corex/filex/corex_filex.h ====
typedef struct Filex {
    u8 init;
    u8 (*exists)(cstr path);
    u64 (*size)(cstr path);
    u8 (*remove)(cstr path);
    str (*read)(cstr path, u64 size);
    u8 (*copy)(cstr src, cstr dest, u64 size);
    u8 (*copyb)(cstr src, cstr dest, u64 size);
    u8 (*readb)(cstr path, u64 size, void* out);
    u8 (*write)(cstr path, cstr lines, u8 preserve);
    u8 (*append)(cstr path, cstr lines, u8 newline);
    u8 (*writeb)(cstr path, u64 size, u8 preserve, void* ptr);
    u8 (*appendb)(cstr path, u64 size, u8 newline, void* ptr);
    u8 (*process)(cstr path, u64 lines, void (*callback)(cstr line));
} Filex;

u8 _corex_init_filex(void);
void _corex_cleanup_filex(void);

// ==== corex/corex.h ====
/*
----------------------------------------------------------
Corex Runtime Library
----------------------------------------------------------

Modules:
    - MEMX   : Memory utilities
    - MATHX  : Math library
    - DSX    : Data structures
    - GENX   : General utilities
    - FILEX  : File I/O
    - ECSX   : ECS framework (Entity-Component-System)

Usage:
    corex_init(COREX_MEMX | COREX_DSX);
    ...
    corex_cleanup();

corex is a singleton pointer to the active runtime.

----------------------------------------------------------
*/











typedef enum Corex_Api {
    COREX_NOAPI  = 0,
    COREX_MEMX   = (1 << 0),
    COREX_MATHX  = (1 << 1),
    COREX_DSX    = (1 << 2),
    COREX_GENX   = (1 << 3),
    COREX_FILEX  = (1 << 4),
    COREX_ECSX   = (1 << 5),
    COREX_ALL    = COREX_MEMX|COREX_MATHX|COREX_DSX|COREX_GENX|COREX_FILEX|COREX_ECSX,
} Corex_Api;

typedef struct Corex_Metadata {
    struct version {
        u32 major, minor, patch;
        cstr string;
        cstr quote;         // a funny random quote (might look into a string lib as well as generating random quote strings :o Corex::strx ?!?!?) 
    } version;

    struct usage {
        u8 apis;            // bitmask passed to corex_init
        u64 memx;
        u64 dsx;
        u64 genx;
        u64 ecsx;
        u64 mathx;
        u64 filex;
    } usage;
} Corex_Metadata;

typedef struct Corex {
    Memx memx;
    Dsx dsx;
    Genx genx;
    Ecsx ecsx;
    Mathx mathx;
    Filex filex;
    Corex_Metadata meta;

    void (*dump)(void);
} Corex;
extern Corex* corex;

COREX_EXPOSE u8 corex_init(Corex_Api mask);
COREX_EXPOSE u8 corex_deinit(Corex_Api mask);
COREX_EXPOSE u8 corex_cleanup(void);

#ifdef COREX_IMPL

// ==== corex/memx/corex_memx.c ====
/* ---------------- STANDARD ---------------- */
void _dealloc_impl(void* ptr) {
	if (!ptr) return;	// error: null ptr!
	// retrieve the pointer difference stored before the ptr
	u16 diff = *((u16*)ptr - 1);

	// retrieve+free the original pointer by shifting the aligned pointer
	free((void*)((u64)ptr - diff));
}

void _zero_impl(void* ptr, u64 size) {
	if (!ptr || !size) return;	// error: null ptr/value error!
	memset(ptr, 0, size);
}

void* _alloc_impl(u64 size, u64 align) {
	if (!size || !COREX_IP2(align)) return NULL;	// error: value error!

	// allocate 2 extra bytes for pointer difference
	// allocate align-1 extra bytes for pointer alignment
	void* optr = malloc(sizeof(u16) + (align - 1) + size);
	if (!optr) return NULL;	// error: out of memory!

	// move 2 bytes from optr align, zero, and return that address
	void* aptr = memset((void*)COREX_ALIGN((u64)((u16*)optr + 1), align), 0, size);

	// store pointer diff/offset at the head of the aligned pointer;
	*((u16*)aptr - 1) = (u16)((u64)aptr - (u64)optr);

	return aptr;
}

void* _realloc_impl(void* ptr, u64 size, u64 align) {
	if (!ptr) return corex->memx.alloc(size, align);	// error: null ptr!
	if (!size || !COREX_IP2(align)) return NULL;	// error: value error!
	
	// retrieve the pointer difference stored before the ptr
	u16 diff = *((u16*)ptr - 1);
	
	// retrieve the original pointer by shifting the aligned pointer
	void* optr = (void*)((u64)ptr - diff);

	// allocate 2 extra bytes for pointer difference
	// allocate align-1 extra bytes for pointer alignment
	void* nptr = realloc((void*)optr, sizeof(u16) + (align - 1) + size);
	if (!nptr) return NULL;

	// move 2 bytes from optr align, and assign that address
	void* aptr = (void*)COREX_ALIGN((u64)((u16*)nptr + 1), align);

	// store pointer diff/offset at the head of the aligned pointer;
	*((u16*)aptr - 1) = (u16)((u64)aptr - (u64)nptr);

	return aptr;
}
/* ---------------- STANDARD ---------------- */


/* ---------------- ARENA ALLOCATOR ---------------- */
void _arena_reset_impl(Allocator* allocator) {
	if (!allocator) return;	// error: null ptr!
	allocator->context.arena.offset = 0;
}

void* _arena_alloc_impl(u64 size, u64 align, Allocator* allocator) {
	if (!allocator || !allocator->context.arena.buffer) return NULL;	// error: null ptr!

	u64 aoffset = COREX_ALIGN(allocator->context.arena.offset, align);
	if (aoffset + size > allocator->context.arena.max) return NULL;	// error: out of memory!

	allocator->context.arena.commit += size;
	allocator->context.arena.offset = aoffset + size;
	return (u8*)allocator->context.arena.buffer + aoffset;
}
/* ---------------- ARENA ALLOCATOR ---------------- */

/* ---------------- ALLOCATOR ---------------- */
u8 _create_allocator_impl(Alloc_Type type, u64 max, u64 align, Allocator* allocator) {
	if (type >= ALLOC_TYPES || !max || !COREX_IP2(align) || !allocator) return COREX_FALSE;	// error value error!
	switch (type) {
		case ALLOC_DEFAULT:
		case ALLOC_ARENA: {
			allocator->type = type;
	
			allocator->context.arena.buffer = corex->memx.alloc(max, align);
			if (!allocator->context.arena.buffer) return COREX_FALSE;
		
			allocator->context.arena.offset = 0;
			allocator->context.arena.max = max;
			return COREX_TRUE;
		}
		default: break;
	}
}

u8 _destroy_allocator_impl(Allocator* allocator) {
	if (!allocator || allocator->type >= ALLOC_TYPES) return COREX_FALSE;	// error: value error!
	switch (allocator->type)
	{
	case ALLOC_DEFAULT:
	case ALLOC_ARENA: {
		if (!allocator->context.arena.buffer) return COREX_FALSE;	// error: null ptr!
		corex->memx.dealloc(allocator->context.arena.buffer);
		allocator->context.arena.buffer = NULL;
		allocator->context.arena.offset = 0;
		allocator->context.arena.max = 0;
		return COREX_TRUE;
	}
	default: break;
	}
	return COREX_TRUE;
}
/* ---------------- ALLOCATOR ---------------- */

/* -------------------- GENERICS ------------------ */
u8 _blob_alloc_impl(Blob* blob, u64 align) {
	if (!blob || !COREX_IP2(align) || !blob->size) {
		blob->size = 0;
		return COREX_FALSE;	// error: null ptr/value error!
	}
	
	blob->data = corex->memx.alloc(blob->size, align);
	if (!blob->data) {
		blob->size = 0;
		return COREX_FALSE;	// error: out of memory!
	}

	return COREX_TRUE;
}

u8 _blob_realloc_impl(Blob* blob, u64 size, u64 align) {
	if (!blob || !blob->data || !size || size > INT_MAX || !COREX_IP2(align)) return COREX_FALSE;	// error: null ptr/value error!

	void* temp = corex->memx.realloc(blob->data, size, align);
	if (!temp) return COREX_FALSE;	// error: out of memory!

	blob->data = temp;
	blob->size = size;

	return COREX_TRUE;
}

u8 _blob_dealloc_impl(Blob* blob) {
	if (!blob || !blob->data || !blob->size) return COREX_FALSE;	// error: null ptr/value error!
	corex->memx.dealloc(blob->data);
	blob->data = (void*)0;
	blob->size = 0;
	return COREX_TRUE;
}
/* -------------------- GENERICS ------------------ */

/* ---------------- API ---------------- */
u8 _corex_init_memx(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->memx.init == COREX_TRUE) return COREX_TRUE;    // redundant call: memx API already initialized!

	corex->memx.alloc = _alloc_impl;
	corex->memx.zero = _zero_impl;
	corex->memx.dealloc = _dealloc_impl;
	corex->memx.realloc = _realloc_impl;
	
	corex->memx.blob_alloc = _blob_alloc_impl;
	corex->memx.blob_dealloc = _blob_dealloc_impl;
	corex->memx.blob_realloc = _blob_realloc_impl;
	
	corex->memx.create_allocator = _create_allocator_impl;
	corex->memx.arena_alloc = _arena_alloc_impl;
	corex->memx.arena_reset = _arena_reset_impl;
	corex->memx.destroy_allocator = _destroy_allocator_impl;
	
	corex->meta.usage.memx = sizeof(Memx);
	corex->memx.init = COREX_TRUE;
	return COREX_TRUE;
}

void _corex_cleanup_memx(void) {
    if (corex->memx.init == COREX_FALSE) return;    // error: memx API not initialized!
	corex->meta.usage.apis &= ~COREX_MEMX;
	corex->memx.init = COREX_FALSE;
	corex->meta.usage.memx = 0;
	corex->memx	= (Memx){0};
}
/* ---------------- API ---------------- */

// ==== corex/dsx/corex_dsx.c ====
/* ---------------- ARRAY ---------------- */
#define ARRAY_HEAD_SIZE (sizeof(u32) * 4)

void* _create_array_impl(u32 stride, u32 max) {
    u32* head = (u32*)corex->memx.alloc(ARRAY_HEAD_SIZE + (stride * max), 8);
    if (!head) return NULL; // error: out of memory!

    head[ARRAY_SIZE_FIELD] = ARRAY_HEAD_SIZE + (stride * max);
    head[ARRAY_STRIDE_FIELD] = stride;
    head[ARRAY_COUNT_FIELD] = 0;
    head[ARRAY_MAX_FIELD] = max;

    return (void*)((u8*)head + ARRAY_HEAD_SIZE);
}

void _put_array_impl(void* array, u32 index, void* invalue) {
    if (!array || !invalue || index < 0) return;
	
    u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    if (index >= head[ARRAY_MAX_FIELD] || (head[ARRAY_COUNT_FIELD] + 1) > head[ARRAY_MAX_FIELD]) return;

    u32 offset = (head[ARRAY_STRIDE_FIELD] * index);

    void* slot = ((u8*)array + offset);
    memcpy(slot, invalue, head[ARRAY_STRIDE_FIELD]);

    head[ARRAY_COUNT_FIELD]++;
}

void _pull_array_impl(void* array, u32 index, void* outvalue) {
    if (!array || outvalue == NULL) return;
	
    u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    if (index >= head[ARRAY_MAX_FIELD] || (head[ARRAY_COUNT_FIELD] - 1) < 0) return;
    
    u32 stride = head[ARRAY_STRIDE_FIELD];
    u32 count = head[ARRAY_COUNT_FIELD];
    u32 offset = (stride * index);

    void* slot = ((u8*)array + offset);
    memcpy(outvalue, slot, stride);
    
    // shift array down
    if (index < count) {
        memmove(slot, (u8*)slot + stride, stride * (count - index));
        memset((u8*)slot + stride * (count - index), 0, stride);  // zero the last slot
    }
    
    head[ARRAY_COUNT_FIELD]--;
}

void _push_array_impl(void* array, void* invalue) {
    if (!array || !invalue) return;
	
	u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    u32 index = head[ARRAY_COUNT_FIELD];

    corex->dsx.array.put_array(array, index, invalue);
}

void _pop_array_impl(void* array, void* outvalue) {
    if (!array || outvalue == NULL) return;
	
	u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    if (!head[ARRAY_COUNT_FIELD]) return;

    u32 index = (head[ARRAY_COUNT_FIELD] - 1);

    corex->dsx.array.pull_array(array, index, outvalue);
}

void* _resize_array_impl(void* array, u32 max) {
    if (!array || !max) return NULL;
    
	u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    
    u32 count = head[ARRAY_COUNT_FIELD];
    u32 stride = head[ARRAY_STRIDE_FIELD];
    u32 newsize = ARRAY_HEAD_SIZE + (stride * max);

    u32* newhead = (u32*)corex->memx.realloc(head, newsize, 8);
    if (!newhead) return NULL;

    newhead[ARRAY_SIZE_FIELD] = ARRAY_HEAD_SIZE + (stride * max);
    newhead[ARRAY_STRIDE_FIELD] = stride;
    newhead[ARRAY_COUNT_FIELD] = count;
    newhead[ARRAY_MAX_FIELD] = max;

    return (void*)((u8*)newhead + ARRAY_HEAD_SIZE);
}

void _destroy_array_impl(void* array) {
    corex->memx.dealloc((void*)((u8*)array - ARRAY_HEAD_SIZE));
}

Array_Head _get_array_head_impl(void* array) {
    Array_Head arrhead = {0, 0, 0, 0};
    if (!array) return arrhead; // error: null ptr!

    u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    
    arrhead.size = head[ARRAY_SIZE_FIELD];
    arrhead.stride = head[ARRAY_STRIDE_FIELD];
    arrhead.count = head[ARRAY_COUNT_FIELD];
    arrhead.max = head[ARRAY_MAX_FIELD];

    return arrhead;
}

void _set_array_head_impl(void* array, u32 field, u32 value) {
    if (!array || field >= ARRAY_FIELDS) return;
    u32* head = (u32*)((u8*)array - ARRAY_HEAD_SIZE);
    head[field] = value;
}
/* ---------------- ARRAY ---------------- */


/* ---------------- LINKED ARRAY ---------------- */
Linked_Array* _create_linked_array_impl(Linked_Array* array, u32 stride, u32 max) {
    if (!stride || !max) return NULL;   // error: value error!

    Linked_Array* new = (Linked_Array*)corex->memx.alloc(sizeof(Linked_Array), 8);
    if (!new) return NULL;  // error: out of memory!

    new->array = corex->dsx.array.create_array(stride, max);
    if (!new->array) return NULL;    // error: out of memory!

    new->last = NULL;
    new->next = NULL;
    if (array) {
        new->last = array;
        if (array->next) {
            new->next = array->next;
            array->next->last = new;
        }
        array->next = new;
    }

    new->meta = corex->dsx.array.get_array_head(new->array);
    return new;
}

void _destroy_linked_array_impl(Linked_Array* array) {
    if (!array) return; // error: null ptr!
    
    if (array->next) array->next->last = array->last;
    if (array->last) array->last->next = array->next;
    
    corex->dsx.array.destroy_array(array->array);
    corex->memx.dealloc(array);
    
    array->meta = (Array_Head){0};
    array->array = NULL;
    array->last = NULL;
    array->next = NULL;
}

void _collapse_linked_array_impl(Linked_Array* array) {
    if (!array) return; // error: null ptr!
    
    Linked_Array* next = array->next;
    while (next) {
        Linked_Array* temp = next->next;
        corex->dsx.array.destroy_linked_array(next);
        next = temp;
    }

	Linked_Array* last = array->last;
    while (last) {
        Linked_Array* temp = last->last;
        corex->dsx.array.destroy_linked_array(last);
        last = temp;
    }
    
	corex->dsx.array.destroy_linked_array(array);
}
/* ---------------- LINKED ARRAY ---------------- */


/* ---------------- HASH ARRAY ---------------- */
i32 _fnv1a_hash(cstr string) {
    if (!string) return -1;
    u32 hash = 2166136261u; // FNV-1a offset basis
    while (*string) {
        hash ^= (u8)*string++;  // XOR with current character
        hash *= 16777619;       // multiply by FNV-1a prime
    }; return hash;
}

i32 _probe_key_hash_array(Hash_Array* array, cstr key) {
    if (!array || !key) return -1;

    Array_Head meta = corex->dsx.array.get_array_head(array->map);

    u32 index = _fnv1a_hash(key) % meta.max;
    u32 start = index;

    COREX_FORI(0, meta.max, 1) {
        if (array->map[index].key)
            if (strcmp(array->map[index].key, key) == 0) return index;
        index = (index + 1) % meta.max;
        if (index == start) return -1;
    }
    return index;
}

i32 _probe_slot_hash_array(Hash_Array* array, cstr key) {
    if (!array || !key) return -1;
    
    Array_Head meta = corex->dsx.array.get_array_head(array->map);
    
    i32 index = _fnv1a_hash(key) % meta.max;
    i32 start = index;
    
    COREX_FORI(0, meta.max, 1) {
        if (array->map[index].key) {
            if (strcmp(array->map[index].key, key) == 0) return index;
        } else return index;
        index = (index + 1) % meta.max;
        if (index == start) return -1;
    }
    return index;
}

Hash_Array* _create_hash_array_impl(u32 max) {
    Hash_Array* array = (Hash_Array*)corex->memx.alloc(sizeof(Hash_Array), 8);
    if (!array) return NULL;  // error: out of memory!

    array->map = (Key_Value*)corex->dsx.array.create_array(sizeof(Key_Value), max);
    if (!array->map) {   // error: out of memory!
        corex->memx.dealloc(array);
        return NULL;
    }

    COREX_FORI(0, max, 1) {
        array->map[i] = (Key_Value){
            .value = NULL,
            .key = NULL
        };
    }

    array->meta = corex->dsx.array.get_array_head(array->map);
    return array;
}

u8 _put_hash_array_impl(Hash_Array* array, cstr key, void* value) {
    if (!array || !key || !value) return COREX_FALSE;  // error: null ptr!

    Array_Head meta = corex->dsx.array.get_array_head(array->map);
    if (meta.count > (u32)(meta.max * 0.7)) {
        Key_Value* temp = (Key_Value*)corex->dsx.array.create_array(sizeof(Key_Value), meta.max * 2);
        if (!temp) return COREX_FALSE;  // error: out of memory!

        ((u32*)((u8*)temp - ARRAY_HEAD_SIZE))[ARRAY_COUNT_FIELD] = meta.count;
        memcpy(temp, array->map, meta.stride * meta.max);
        
        corex->dsx.array.destroy_array(array->map);
        array->map = temp;
    }
    
    i32 index = _probe_slot_hash_array(array, key);
    if (index == -1) return COREX_FALSE;

    if (array->map[index].key && strcmp(array->map[index].key, key) == 0) {
        array->map[index].value = value;
    } else {
        corex->dsx.array.put_array(array->map, index, &(Key_Value){ .value = value, .key = strdup(key)});
    }
    
    array->meta = corex->dsx.array.get_array_head(array->map);
    return COREX_TRUE;
}

void* _get_hash_array_impl(Hash_Array* array, cstr key) {
    if (!array || !key) return NULL;  // error: null ptr!
    
    i32 index = _probe_key_hash_array(array, key);
    if (index == -1) return NULL;

    return array->map[index].value;
}

u8 _pull_hash_array_impl(Hash_Array* array, cstr key, Key_Value* out) {
    if (!array || !key) return COREX_FALSE;  // error: null ptr!

    i32 index = _probe_key_hash_array(array, key);
    if (index == -1) return COREX_FALSE;

    corex->dsx.array.pull_array(array->map, index, out);
    array->meta = corex->dsx.array.get_array_head(array->map);
    return COREX_TRUE;
}

void _destroy_hash_array_impl(Hash_Array* array) {
    if (!array) return;   // error: null ptr!
    corex->dsx.array.destroy_array(array->map);
    corex->memx.dealloc(array);
}

cstr* _get_hash_array_keys_impl(Hash_Array* array) {
    u8 key = 0;
	
    cstr* keys = corex->dsx.array.create_array(sizeof(cstr), array->meta.count);
    if (!keys) return NULL; // error: out of memory!

	COREX_FORI(0, array->meta.max, 1)
		if (array->map[i].key)
            keys[key++] = array->map[i].key;
	
    ((u32*)((u8*)keys - ARRAY_HEAD_SIZE))[ARRAY_COUNT_FIELD] = array->meta.count;
    return keys;
}

void** _get_hash_array_values_impl(Hash_Array* array) {
    u8 value = 0;
	
    void** values = corex->dsx.array.create_array(sizeof(void*), array->meta.count);
    if (!values) return NULL; // error: out of memory!

	COREX_FORI(0, array->meta.max, 1)
		if (array->map[i].key)
            values[value++] = array->map[i].value;
	
    ((u32*)((u8*)values - ARRAY_HEAD_SIZE))[ARRAY_COUNT_FIELD] = array->meta.count;
    return values;
}
/* ---------------- HASH ARRAY ---------------- */


/* ---------------- API ---------------- */
u8 _corex_init_dsx(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->dsx.init == COREX_TRUE) return COREX_TRUE;    // redundant call: Dsx API already initialized!

    if (corex->memx.init == COREX_FALSE) {
        printf("Corex Memx api not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Memx API!
    }

    corex->dsx.array.create_array = _create_array_impl;
    corex->dsx.array.pop_array = _pop_array_impl;
    corex->dsx.array.put_array = _put_array_impl;
    corex->dsx.array.push_array = _push_array_impl;
    corex->dsx.array.pull_array = _pull_array_impl;
    corex->dsx.array.resize_array = _resize_array_impl;
    corex->dsx.array.destroy_array = _destroy_array_impl;
    corex->dsx.array.get_array_head = _get_array_head_impl;

    corex->dsx.array.create_linked_array = _create_linked_array_impl;
    corex->dsx.array.destroy_linked_array = _destroy_linked_array_impl;
    corex->dsx.array.collapse_linked_array = _collapse_linked_array_impl;

    corex->dsx.array.create_hash_array = _create_hash_array_impl;
    corex->dsx.array.put_hash_array = _put_hash_array_impl;
    corex->dsx.array.get_hash_array = _get_hash_array_impl;
    corex->dsx.array.get_hash_array_keys = _get_hash_array_keys_impl;
    corex->dsx.array.get_hash_array_values = _get_hash_array_values_impl;
    corex->dsx.array.pull_hash_array = _pull_hash_array_impl;
    corex->dsx.array.destroy_hash_array = _destroy_hash_array_impl;
    
    corex->meta.usage.dsx = sizeof(Dsx);
	corex->dsx.init = COREX_TRUE;
    return 1;
}

void _corex_cleanup_dsx(void) {
    if (corex->dsx.init == COREX_FALSE) return;    // error: Dsx API not initialized!
	corex->meta.usage.apis &= ~COREX_DSX;
    corex->dsx.init = COREX_FALSE;
    corex->meta.usage.dsx = 0;
    corex->dsx = (Dsx){0};
}
/* ---------------- API ---------------- */

// ==== corex/genx/corex_genx.c ====
#define BUFFER_VALID(b) ((b) && (b)->data && (b)->meta.state == BUFFER_STATE_VALID)
#define BUFFER_CAN_WRITE(b) ((b)->meta.access == BUFFER_ACCESS_RW)
#define BUFFER_CAN_READ(b) ((b)->meta.access != BUFFER_ACCESS_NONE)

u8 _depth8_impl(u8 value) {
u8 depth = 0;
    COREX_FOR(u8, bit, 0, 8, 1) {
        depth = (COREX_GET_BITS(value, bit)) ? bit+1 : depth;
    }
    return depth;
}

u8 _pack8_impl(u8 value1, u8 value2) {
    u8 depth = corex->genx.bin.depth8(value2);
    u8 packed = 0;
    packed = value1 << depth;
    packed |= value2;
    return packed;
}

u8* _unpack8_impl(u8 value, u8 depth, u8* out) {
    u8 lmask = (1 << depth) - 1;
    out[0] = value >> depth;
    out[1] = value & lmask;
    return out;
}


u8 _create_buffer_impl(Buffer_Layout layout, Buffer* buffer) {
    if (!buffer ||
        !layout.size || layout.size > BUFFER_SIZE_MAX   ||
        !layout.type || layout.type > BUFFER_TYPES      ||
        !layout.access || layout.access > BUFFER_ACCESS_TYPES) {
        return COREX_FALSE;  // error: null ptr/value error!
    }

    buffer->data = corex->memx.alloc(layout.size, 8);
    if (!buffer->data) return COREX_FALSE;   // error: out of memory!

    buffer->meta.type = layout.type;
    buffer->meta.size = layout.size;
    buffer->meta.access = layout.access;
    buffer->meta.state = BUFFER_STATE_VALID;
    
    buffer->meta.cursor.written = 0;
    buffer->meta.cursor.bit = 0;
    buffer->meta.cursor.byte = 0;
    buffer->meta.flags = 0;

    return COREX_TRUE;
}

u8 _reset_impl(Buffer* buffer) {
    if (!BUFFER_VALID(buffer)) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }
    
    corex->memx.zero(buffer->data, buffer->meta.size);
    buffer->meta.cursor.written = 0;
    buffer->meta.cursor.byte = 0;
    buffer->meta.cursor.bit = 0;
    return COREX_TRUE;
}

u8 _destroy_buffer_impl(Buffer* buffer) {
    if (!BUFFER_VALID(buffer)) return COREX_FALSE;  // error: null ptr!

    corex->memx.dealloc(buffer->data);

    buffer->meta.type = BUFFER_TYPE_NONE;
    buffer->meta.size = BUFFER_SIZE_NONE;
    buffer->meta.access = BUFFER_ACCESS_NONE;
    buffer->meta.state = BUFFER_STATE_NONE;
    
    buffer->meta.cursor.written = 0;
    buffer->meta.cursor.bit = 0;
    buffer->meta.cursor.byte = 0;
    buffer->meta.flags = 0;

    return COREX_TRUE;
}


u8 _read_buffer_impl(u64 count, void* bytes, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_READ(buffer) || !count || !bytes) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }

    if (COREX_GET_BITS(buffer->meta.flags, BUFFER_FLAG_AUTOCURSOR)) {
        buffer->meta.cursor.byte = 0;
    }
    COREX_FOR(u64, byte, 0, count, 1) {
        if (!((u8*)buffer->data + byte)) break;    // error: null byte encountered!
        *((u8*)bytes + byte) = *(((u8*)buffer->data + buffer->meta.cursor.byte) + byte);
    }
    
    return COREX_TRUE;
}

u8 _write_buffer_impl(u64 count, void* bytes, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_READ(buffer) || !count || !bytes) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }

    if (count > buffer->meta.size - buffer->meta.cursor.written) {
        return COREX_FALSE;  // error: not enough space for this write!
    }
    
    COREX_FOR(u64, byte, 0, count, 1) {
        if (!((u8*)bytes + byte)) break;    // error: null byte encountered!
        *(((u8*)buffer->data + buffer->meta.cursor.byte) + byte) = *((u8*)bytes + byte);
    }
    
    buffer->meta.cursor.byte += (COREX_GET_BITS(buffer->meta.flags, BUFFER_FLAG_AUTOCURSOR)) ? count : 0;
    buffer->meta.cursor.written+=count;
    return COREX_TRUE;
}

u8 _peek_impl(u64 offset, u64 count, void* bytes, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_READ(buffer)) return COREX_FALSE;
    if (!bytes || !count ||
        count > buffer->meta.size ||
        buffer->meta.cursor.byte + offset > buffer->meta.size
    ) return COREX_FALSE;
    buffer->meta.cursor.byte += offset;
    u8 result = corex->genx.buffer.read(count, bytes, buffer);
    buffer->meta.cursor.byte -= offset;
    return result;
}

u8 _inject_impl(u64 offset, u64 count, void* bytes, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_WRITE(buffer)) return COREX_FALSE;
    if (!bytes || !count ||
        count > buffer->meta.size ||
        buffer->meta.cursor.byte + offset > buffer->meta.size
    ) return COREX_FALSE;
        
    buffer->meta.cursor.byte += offset;
    u8 result = corex->genx.buffer.write(count, bytes, buffer);
    buffer->meta.cursor.byte -= offset;
    return result;
}


u8 _rewind_impl(Buffer_Cursor cursor, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_WRITE(buffer)) return COREX_FALSE;
    if (cursor < 0 || cursor > BUFFER_CURSOR_TYPES) return COREX_FALSE;
    switch (cursor) {
        case BUFFER_CURSOR_BIT: {
            buffer->meta.cursor.bit = 0;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_BYTE: {
            buffer->meta.cursor.byte = 0;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_NONE:    // fall through to default
        case BUFFER_CURSOR_TYPES:   // fall through to default
        default: return COREX_FALSE;
    }
}

u8 _seek_impl(Buffer_Cursor cursor, u64 offset, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_WRITE(buffer)) return COREX_FALSE;
    if (offset < 0 || cursor < 0 || cursor > BUFFER_CURSOR_TYPES) return COREX_FALSE;
    switch (cursor) {
        case BUFFER_CURSOR_BIT: {
            if (offset >= 8) return COREX_FALSE;
            buffer->meta.cursor.bit = offset;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_BYTE: {
            if (offset >= buffer->meta.size) return COREX_FALSE;
            buffer->meta.cursor.byte = offset;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_NONE:    // fall through to default
        case BUFFER_CURSOR_TYPES:   // fall through to default
        default: return COREX_FALSE;
    }
}

u8 _seekr_impl(Buffer_Cursor cursor, u64 offset, Buffer* buffer) {
    if (!BUFFER_VALID(buffer) || !BUFFER_CAN_WRITE(buffer)) return COREX_FALSE;
    if (offset < 0 || cursor < 0 || cursor > BUFFER_CURSOR_TYPES) return COREX_FALSE;
    switch (cursor) {
        case BUFFER_CURSOR_BIT: {
            if (
                offset >= 8 ||
                buffer->meta.cursor.bit + offset < 0 ||
                buffer->meta.cursor.bit + offset >= 8
            ) return COREX_FALSE;
            buffer->meta.cursor.bit += offset;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_BYTE: {
            if (
                offset >= buffer->meta.size ||
                buffer->meta.cursor.byte + offset < 0 ||
                buffer->meta.cursor.byte + offset >= buffer->meta.size
                ) return COREX_FALSE;
            buffer->meta.cursor.byte += offset;
            return COREX_TRUE;
        }
        case BUFFER_CURSOR_NONE:    // fall through to default
        case BUFFER_CURSOR_TYPES:   // fall through to default
        default: return COREX_FALSE;
    }
}


u8 _set_flag_impl(Buffer_Flag flag, Buffer* buffer) {
    if (!BUFFER_VALID(buffer)) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }
    buffer->meta.flags = COREX_SET_BITS(buffer->meta.flags, flag);
    return COREX_TRUE;
}

u8 _has_flag_impl(Buffer_Flag flag, Buffer* buffer) {
    if (!BUFFER_VALID(buffer)) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }
    return (COREX_GET_BITS(buffer->meta.flags, flag)) ? COREX_TRUE : COREX_FALSE;
}

u8 _rem_flag_impl(Buffer_Flag flag, Buffer* buffer) {
    if (!BUFFER_VALID(buffer)) {
        return COREX_FALSE;  // error: null ptr/buffer in a "bad" state!
    }
    buffer->meta.flags = COREX_REM_BITS(buffer->meta.flags, flag);
    return COREX_TRUE;
}


u8 _corex_init_genx(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->genx.init == COREX_TRUE) return COREX_TRUE;    // redundant call: Genx API already initialized!

    if (corex->memx.init == COREX_FALSE) {
        printf("Corex Memx API not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Memx API!
    }
    
    if (corex->dsx.init == COREX_FALSE) {
        printf("Corex Dsx API not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Dsx API!
    }

    corex->genx.buffer.create = _create_buffer_impl;
    corex->genx.buffer.reset = _reset_impl;
    corex->genx.buffer.destroy = _destroy_buffer_impl;
    
    corex->genx.buffer.read = _read_buffer_impl;
    corex->genx.buffer.write = _write_buffer_impl;
    
    corex->genx.buffer.peek = _peek_impl;
    corex->genx.buffer.inject = _inject_impl;
    
    corex->genx.buffer.rewind = _rewind_impl;
    corex->genx.buffer.seek = _seek_impl;
    corex->genx.buffer.seekr = _seekr_impl;
    
    corex->genx.buffer.set_flag = _set_flag_impl;
    corex->genx.buffer.has_flag = _has_flag_impl;
    corex->genx.buffer.rem_flag = _rem_flag_impl;
    
    corex->genx.bin.depth8 = _depth8_impl;
    corex->genx.bin.pack8 = _pack8_impl;
    corex->genx.bin.unpack8 = _unpack8_impl;

    corex->meta.usage.genx = sizeof(Genx);
	corex->genx.init = COREX_TRUE;
    return COREX_TRUE;
}

void _corex_cleanup_genx(void) {
    if (corex->genx.init == COREX_FALSE) return;    // error: Genx API not initialized!
	corex->meta.usage.apis &= ~COREX_GENX;
	corex->genx.init = COREX_FALSE;
    corex->meta.usage.genx = 0;
	corex->genx	= (Genx){0};
}

// ==== corex/ecsx/corex_ecsx.c ====
u32 _create_entity_impl(void) {
    u32 entity;

    Array_Head qhead = corex->dsx.array.get_array_head(corex->ecsx.entity_manager.queue);
    if (qhead.count > 0) {
        corex->dsx.array.pop_array(corex->ecsx.entity_manager.queue, &entity);
    } else {
        return -1;  // error: array exhausted!
    }
    qhead = corex->dsx.array.get_array_head(corex->ecsx.entity_manager.queue);
    
    corex->ecsx.entity_manager.mask[entity] = 0;
    corex->ecsx.entity_manager.count++;
    return entity;
}

u32 _create_entity_with_impl(u8 count, u8* ids) {
    if (count < 0 || count > COMPONENT_MAX) return COREX_FALSE;
    u8 result = 0;
    u32 entity = corex->ecsx.create_entity();
    COREX_FORI(0, count, 1) {
        result = corex->ecsx.add_component(ids[i], entity);
        if (!result) printf("failed to create (entity)%d with component%d:\n", entity, ids[i]);
    }
    return entity;
}

void _destroy_entity_impl(u32 entity) {
    if (entity >= ENTITY_MAX || entity < 0) return;
    
    corex->dsx.array.push_array(corex->ecsx.entity_manager.queue, &(u32){entity});
    
    corex->ecsx.entity_manager.mask[entity] = 0;
    corex->ecsx.entity_manager.count--;
}


u8 _register_component_impl(
    u8 id, void* storage,
    COMPONENT_ADD_FPTR add_func,
    COMPONENT_REM_FPTR rem_func,
    COMPONENT_GET_FPTR get_func) {
    
    if (id < 0 || id > COMPONENT_MAX || !storage ||
        !add_func || !rem_func || !get_func
    ) return COREX_FALSE; // error: value error!
    
    ((Hash_Array**)corex->ecsx.component_manager.component_system)[id] = corex->dsx.array.create_hash_array(SYSTEM_MAX);
    if (!((Hash_Array**)corex->ecsx.component_manager.component_system)[id]) return COREX_FALSE;   // error: out of memory!

    corex->ecsx.component_manager.component_storage[id] = storage;
    corex->ecsx.component_manager.add_component_fptr[id] = add_func;
    corex->ecsx.component_manager.rem_component_fptr[id] = rem_func;
    corex->ecsx.component_manager.get_component_fptr[id] = get_func;
    
    corex->ecsx.component_manager.count++;
    return COREX_TRUE;
}

u8 _unregister_component_impl(u8 id) {
    if (id < 0 || id > COMPONENT_MAX) return COREX_FALSE; // error: value error!
    if (!corex->ecsx.component_manager.component_storage[id]) return COREX_FALSE;  // error: component not registered!

    // TODO: remove this component from all entities with it
    corex->ecsx.component_manager.component_storage[id] = NULL;
    corex->dsx.array.destroy_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id]);
    corex->ecsx.component_manager.add_component_fptr[id] = NULL;
    corex->ecsx.component_manager.rem_component_fptr[id] = NULL;
    corex->ecsx.component_manager.get_component_fptr[id] = NULL;

    corex->ecsx.component_manager.count--;
    return COREX_TRUE;
}

u8 _register_system_impl(u8 id, cstr name, void* system) {
    if (id < 0 || id > COMPONENT_MAX || !system) return COREX_FALSE; // error: null ptr/value error!
    
    Array_Head head = corex->dsx.array.get_array_head(((Hash_Array**)corex->ecsx.component_manager.component_system)[id]);
    if (head.count == SYSTEM_MAX) return COREX_FALSE;    // error: maximum systems reached for this component!

    corex->dsx.array.put_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id], name, system);

    return COREX_TRUE;
}

u8 _unregister_system_impl(u8 id, cstr name) {
    if (id < 0 || id > COMPONENT_MAX || !name) return COREX_FALSE; // error: null ptr/value error!
    Key_Value temp = {NULL};
    return corex->dsx.array.pull_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id], name, &temp);
}

u8 _unregister_systems_impl(u8 id) {
    if (id < 0 || id > COMPONENT_MAX) return COREX_FALSE; // error: null ptr/value error!
    Key_Value temp = {NULL};
    
    cstr* keys = corex->dsx.array.get_hash_array_keys(((Hash_Array**)corex->ecsx.component_manager.component_system)[id]);
    if (!keys) return COREX_FALSE;   // error: failed to get hash array keys!

    COREX_FORI(0, ((Hash_Array**)corex->ecsx.component_manager.component_system)[id]->meta.count, 1) {
        if (!corex->dsx.array.pull_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id], keys[i], &temp)) return COREX_FALSE;   // error: failed to unregister a system!
    }
    
    if (keys != NULL) corex->dsx.array.destroy_array(keys);
    return COREX_TRUE;
}


u8 _run_system_impl(u8 id, cstr name) {
    if (id < 0 || id > COMPONENT_MAX || !name) return COREX_FALSE; // error: null ptr/value error!
    
    COMPONENT_SYSTEM_FPTR system = corex->dsx.array.get_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id], name);
    if (!system) return COREX_FALSE; // error: failed to get system from hash array!

    COREX_FORI(0, corex->ecsx.entity_manager.count, 1) {
        if ((corex->ecsx.entity_manager.mask[i] & (1<<id)) == (1<<id)) {
            if (!system(i)) return COREX_FALSE;  // error: system failed to run!
        }
    }

}

u8 _run_systems_impl(u8 id) {
    if (id < 0 || id > COMPONENT_MAX) return COREX_FALSE; // error: null ptr/value error!
    
    cstr* keys = NULL;
    COREX_FORI(0, corex->ecsx.entity_manager.count, 1) {
        if ((corex->ecsx.entity_manager.mask[i] & (1<<id)) != (1<<id)) continue;  // skip entities with invalid masks
        
        keys = corex->dsx.array.get_hash_array_keys(((Hash_Array**)corex->ecsx.component_manager.component_system)[id]);
        if (!keys) return COREX_FALSE;   // error: failed to get hash array keys!

        COREX_FORJ(0, ((Hash_Array**)corex->ecsx.component_manager.component_system)[id]->meta.count, 1) {
            COMPONENT_SYSTEM_FPTR system = corex->dsx.array.get_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[id], keys[j]);
            if (!system) return COREX_FALSE; // error: failed to get system from hash array!
            if (!system(i)) return COREX_FALSE;  // error: system failed to run!
        }
    }
    
    if (keys != NULL) corex->dsx.array.destroy_array(keys);
    return COREX_TRUE;
}

u8 _add_component_impl(u8 id, u32 entity) {
    if (entity >= ENTITY_MAX || entity < 0) return COREX_FALSE;  // error: value error!
    if ((corex->ecsx.entity_manager.mask[entity] & (1<<id)) == (1<<id)) return COREX_FALSE; // error: entity already has this component!
    
    u8 result = ((COMPONENT_ADD_FPTR)corex->ecsx.component_manager.add_component_fptr[id])(entity);
    corex->ecsx.entity_manager.mask[entity] |= (1 << id);
    return result;
}

u8 _rem_component_impl(u8 id, u32 entity) {
    if (entity >= ENTITY_MAX || entity < 0) return COREX_FALSE;  // error: value error!
    if ((corex->ecsx.entity_manager.mask[entity] & (1<<id)) != (1<<id)) return COREX_FALSE; // error: entity does not have this component!

    u8 result = ((COMPONENT_REM_FPTR)corex->ecsx.component_manager.rem_component_fptr[id])(entity);
    corex->ecsx.entity_manager.mask[entity] &= ~(1<<id);
    return result;
}

u8 _has_component_impl(u8 id, u32 entity) {
    if (entity >= ENTITY_MAX || entity < 0) return COREX_FALSE;  // error: null ptr/value error!
    return ((corex->ecsx.entity_manager.mask[entity] & (1<<id)) == (1<<id)) ? COREX_TRUE : COREX_FALSE;
}

u8 _get_component_impl(u8 id, u32 entity, void* component) {
    if (entity >= ENTITY_MAX || entity < 0 || !component) return COREX_FALSE;  // error: null ptr/value error!
    if ((corex->ecsx.entity_manager.mask[entity] & (1<<id)) != (1<<id)) return COREX_FALSE;

    return ((COMPONENT_GET_FPTR)corex->ecsx.component_manager.get_component_fptr[id])(entity, component);
}

u32* _get_entities_impl(u8 id) {
    u32* out = corex->dsx.array.create_array(sizeof(u32), corex->ecsx.entity_manager.count);

    COREX_FORI(0, corex->ecsx.entity_manager.count, 1) {
        corex->dsx.array.push_array(out, ((corex->ecsx.entity_manager.mask[i] & (1<<id)) == (1<<id)) ? &(u32){i} : &(u32){ENTITY_MAX});
    }

    return out;
}


u8 _corex_init_ecsx(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->ecsx.init == COREX_TRUE) return COREX_TRUE;    // redundant call: Ecsx API already initialized!

    if (corex->memx.init == COREX_FALSE) {
        printf("Corex Memx API not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Memx API!
    }
    
    if (corex->dsx.init == COREX_FALSE) {
        printf("Corex Dsx API not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Dsx API!
    }

    corex->ecsx.entity_manager.queue = corex->dsx.array.create_array(sizeof(u32), ENTITY_MAX);
    if (!corex->ecsx.entity_manager.queue) {
        return COREX_FALSE;  // error: out of memory!
    }
    
    corex->ecsx.entity_manager.mask = corex->dsx.array.create_array(sizeof(u32), ENTITY_MAX);
    if (!corex->ecsx.entity_manager.mask) {
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        return COREX_FALSE;  // error: out of memory!
    }
    
    COREX_FORI(ENTITY_MAX-1, -1, -1) {
        corex->dsx.array.push_array(corex->ecsx.entity_manager.queue, &(u32){i});
    }
    corex->ecsx.entity_manager.count = 0;
    
    corex->ecsx.component_manager.component_storage = corex->dsx.array.create_array(sizeof(void*), COMPONENT_MAX);
    if (!corex->ecsx.component_manager.component_storage) {
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
        return COREX_FALSE;  // error: out of memory!
    }
    
    corex->ecsx.component_manager.component_system = corex->dsx.array.create_array(sizeof(Hash_Array*), COMPONENT_MAX);
    if (!corex->ecsx.component_manager.component_system) {
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_storage);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
        return COREX_FALSE;  // error: out of memory!
    }

    corex->ecsx.component_manager.add_component_fptr = corex->dsx.array.create_array(sizeof(void*), COMPONENT_MAX);
    if (!corex->ecsx.component_manager.add_component_fptr) {
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_storage);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_system);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
        return COREX_FALSE;  // error: out of memory!
    }
    
    corex->ecsx.component_manager.rem_component_fptr = corex->dsx.array.create_array(sizeof(void*), COMPONENT_MAX);
    if (!corex->ecsx.component_manager.rem_component_fptr) {
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_storage);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_system);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.add_component_fptr);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
        return COREX_FALSE;  // error: out of memory!
    }
    
    corex->ecsx.component_manager.get_component_fptr = corex->dsx.array.create_array(sizeof(void*), COMPONENT_MAX);
    if (!corex->ecsx.component_manager.get_component_fptr) {
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_storage);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_system);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.add_component_fptr);
        corex->dsx.array.destroy_array(corex->ecsx.component_manager.rem_component_fptr);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);
        corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
        return COREX_FALSE;  // error: out of memory!
    }
    
    corex->ecsx.component_manager.count = 0;

    corex->ecsx.create_entity = _create_entity_impl;
    corex->ecsx.create_entity_with = _create_entity_with_impl;
    corex->ecsx.destroy_entity = _destroy_entity_impl;

    corex->ecsx.register_component = _register_component_impl;
    corex->ecsx.unregister_component = _unregister_component_impl;
    
    corex->ecsx.register_system = _register_system_impl;
    corex->ecsx.unregister_system = _unregister_system_impl;
    corex->ecsx.unregister_systems = _unregister_systems_impl;
    
    corex->ecsx.run_system = _run_system_impl;
    corex->ecsx.run_systems = _run_systems_impl;
    corex->ecsx.add_component = _add_component_impl;
    corex->ecsx.get_component = _get_component_impl;
    corex->ecsx.rem_component = _rem_component_impl;
    corex->ecsx.has_component = _has_component_impl;
    corex->ecsx.get_entities = _get_entities_impl;
    
    corex->meta.usage.ecsx= sizeof(Ecsx);
	corex->ecsx.init = COREX_TRUE;
    return COREX_TRUE;
}

void _corex_cleanup_ecsx(void) {
    if (corex->ecsx.init == COREX_FALSE) return;    // error: ecsx API not initialized!

    corex->dsx.array.destroy_array(corex->ecsx.entity_manager.mask);
    corex->dsx.array.destroy_array(corex->ecsx.entity_manager.queue);

    Array_Head head = corex->dsx.array.get_array_head(corex->ecsx.component_manager.component_system);
    COREX_FORI(0, head.count, 1) {
        if (((Hash_Array**)corex->ecsx.component_manager.component_system)[i]) {
            corex->dsx.array.destroy_hash_array(((Hash_Array**)corex->ecsx.component_manager.component_system)[i]);
        }
    }

    corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_storage);
    corex->dsx.array.destroy_array(corex->ecsx.component_manager.component_system);
    corex->dsx.array.destroy_array(corex->ecsx.component_manager.add_component_fptr);
    corex->dsx.array.destroy_array(corex->ecsx.component_manager.rem_component_fptr);
    corex->dsx.array.destroy_array(corex->ecsx.component_manager.get_component_fptr);

	corex->meta.usage.apis &= ~COREX_ECSX;
	corex->ecsx.init = COREX_FALSE;
    corex->meta.usage.ecsx = 0;
	corex->ecsx	= (Ecsx){0};
}

// ==== corex/mathx/corex_mathx.c ====
/* ---------------- UTILITY ------------- */
void _print_vec2_impl(cstr name, Vec2 vec) {
    printf("%s: <%0.1f, %0.1f>\n", name,
        vec.x, vec.y);
}

void _print_vec3_impl(cstr name, Vec3 vec) {
    printf("%s: <%0.1f, %0.1f, %0.1f>\n", name,
        vec.x, vec.y, vec.z);
}

void _print_vec4_impl(cstr name, Vec4 vec) {
    printf("%s: <%0.1f, %0.1f, %0.1f, %0.1f>\n", name,
        vec.x, vec.y, vec.z, vec.w);
}

void _print_mat2_impl(cstr name, Mat2 mat) {
    printf("%s\n", name);
    u32 r = 0;
    COREX_FORI(0, 4, 1) {
        printf("%0.3f ", mat.m[i]);
        if (r++ == 1) {
            printf("\n");
            r = 0;
        }
    }
}

void _print_mat3_impl(cstr name, Mat3 mat) {
    printf("%s\n", name);
    u32 r = 0;
    COREX_FORI(0, 9, 1) {
        printf("%0.3f ", mat.m[i]);
        if (r++ == 2) {
            printf("\n");
            r = 0;
        }
    }
}

void _print_mat4_impl(cstr name, Mat4 mat) {
    printf("%s\n", name);
    u32 r = 0;
    COREX_FORI(0, 16, 1) {
        printf("%0.3f ", mat.m[i]);
        if (r++ == 3) {
            printf("\n");
            r = 0;
        }
    }
}
/* ---------------- UTILITY ------------- */


/* ---------------- SCALAR -------------- */
f32 _to_radians_impl(f32 deg) {
    return deg * COREX_PI/180.0;
}
/* ---------------- SCALAR -------------- */


/* ---------------- VEC2 ---------------- */
// Creates a Vec2
Vec2 _new_vec2_impl(f32 x, f32 y) {
    return (Vec2){x, y};
}

// Scales a Vec2 by a scalar value
Vec2 _scale_vec2_impl(Vec2 v, f32 scale) {
    return (Vec2){v.x * scale, v.y * scale};
}

// Adds two Vec2s
Vec2 _add_vec2_impl(Vec2 v1, Vec2 v2) {
    return (Vec2){v1.x + v2.x, v1.y + v2.y};
}

// Subtracts one Vec2 from another
Vec2 _sub_vec2_impl(Vec2 v1, Vec2 v2) {
    return (Vec2){v1.x - v2.x, v1.y - v2.y};
}

// Dot product of two Vec2s
f32 _dot_vec2_impl(Vec2 v1, Vec2 v2) {
    return v1.x * v2.x + v1.y * v2.y;
}

// Return the magnitude of a Vec2
f32 _mag_vec2_impl(Vec2 v) {
    return sqrtf(v.x * v.x + v.y * v.y);
}

// Normalize a Vec2
Vec2 _norm_vec2_impl(Vec2 v) {
    f32 length = corex->mathx.vec.mag2(v);
    return corex->mathx.vec.scale2(v, 1.0f / length);
}
/* ---------------- VEC2 ---------------- */


/* ---------------- VEC3 ---------------- */
// Creates a Vec3
Vec3 _new_vec3_impl(f32 x, f32 y, f32 z) {
    return (Vec3){x, y, z};
}
// Scales a vec by a scalar value
Vec3 _scale_vec3_impl(Vec3 v, f32 scale) {
    Vec3 result;
    result.x = v.x * scale;
    result.y = v.y * scale;
    result.z = v.z * scale;
    return result;
}

// Adds two vectors
Vec3 _add_vec3_impl(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x + v2.x;
    result.y = v1.y + v2.y;
    result.z = v1.z + v2.z;
    return result;
}

// Subtracts one vec from another
Vec3 _sub_vec3_impl(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.x - v2.x;
    result.y = v1.y - v2.y;
    result.z = v1.z - v2.z;
    return result;
}

f32 _dot_vec3_impl(Vec3 a, Vec3 b) {
    return a.x * b.x + a.y * b.y + a.z * b.z;
}

// Return the magnitude of a Vec3
f32 _mag_vec3_impl(Vec3 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z);
}

// Normalize a vec
Vec3 _norm_vec3_impl(Vec3 v) {
    f32 length = corex->mathx.vec.mag3(v);
    return corex->mathx.vec.scale3(v, 1.0f / length);
}

// Cross product of two vectors
Vec3 _cross_vec3_impl(Vec3 v1, Vec3 v2) {
    Vec3 result;
    result.x = v1.y * v2.z - v1.z * v2.y;
    result.y = v1.z * v2.x - v1.x * v2.z;
    result.z = v1.x * v2.y - v1.y * v2.x;
    return result;
}
/* ---------------- VEC3 ---------------- */


/* ---------------- VEC4 ---------------- */
// Creates a Vec4
Vec4 _new_vec4_impl(f32 x, f32 y, f32 z, f32 w) {
    return (Vec4){x, y, z, w};
}

// Scales a Vec4 by a scalar value
Vec4 _scale_vec4_impl(Vec4 v, f32 scale) {
    return (Vec4){v.x * scale, v.y * scale, v.z * scale, v.w * scale};
}

// Adds two Vec4s
Vec4 _add_vec4_impl(Vec4 v1, Vec4 v2) {
    return (Vec4){v1.x + v2.x, v1.y + v2.y, v1.z + v2.z, v1.w + v2.w};
}

// Subtracts one Vec4 from another
Vec4 _sub_vec4_impl(Vec4 v1, Vec4 v2) {
    return (Vec4){v1.x - v2.x, v1.y - v2.y, v1.z - v2.z, v1.w - v2.w};
}

// Dot product of two Vec4s
f32 _dot_vec4_impl(Vec4 v1, Vec4 v2) {
    return v1.x * v2.x + v1.y * v2.y + v1.z * v2.z + v1.w * v2.w;
}

// Return the magnitude of a Vec4
f32 _mag_vec4_impl(Vec4 v) {
    return sqrtf(v.x * v.x + v.y * v.y + v.z * v.z + v.w * v.w);
}

// Normalize a Vec4
Vec4 _norm_vec4_impl(Vec4 v) {
    f32 length = corex->mathx.vec.mag4(v);
    return corex->mathx.vec.scale4(v, 1.0f / length);
}
/* ---------------- VEC4 ---------------- */


/* ---------------- MAT2 ---------------- */
Mat2 _identity2_impl(void) {
    return (Mat2){.m={
        1, 0,
        0, 1
    }};
}

Mat2 _transpose2_impl(Mat2 mat) {
    Mat2 result = mat;
    COREX_SWAP(f32, result.m[1], result.m[2]);   // (0,1) <-> (1,0)
    return result;
}

Mat2 _mul_mat2_impl(Mat2 a, Mat2 b) {
    Mat2 result = {0};
    for (int row = 0; row < 2; ++row) {
        for (int col = 0; col < 2; ++col) {
            for (int i = 0; i < 2; ++i) {
                result.m[row * 2 + col] += a.m[row * 2 + i] * b.m[i * 2 + col];
            }
        }
    }
    return result;
}
/* ---------------- MAT2 ---------------- */


/* ---------------- MAT3 ---------------- */
Mat3 _identity3_impl(void) {
    return (Mat3){.m={
        1, 0, 0,
        0, 1, 0,
        0, 0, 1
    }};
}

Mat3 _transpose3_impl(Mat3 mat) {
    Mat3 result = mat;
    COREX_SWAP(f32, result.m[1], result.m[3]);   // (0,1) <-> (1,0)
    COREX_SWAP(f32, result.m[2], result.m[6]);   // (0,2) <-> (2,0)
    COREX_SWAP(f32, result.m[5], result.m[7]);   // (1,2) <-> (2,1)
    return result;
}

Mat3 _mul_mat3_impl(Mat3 a, Mat3 b) {
    Mat3 result = {0};
    for (int row = 0; row < 3; ++row) {
        for (int col = 0; col < 3; ++col) {
            for (int i = 0; i < 3; ++i) {
                result.m[row * 3 + col] += a.m[row * 3 + i] * b.m[i * 3 + col];
            }
        }
    }
    return result;
}
/* ---------------- MAT3 ---------------- */


/* ---------------- MAT4 ---------------- */
Mat4 _identity4_impl(void) {
    return (Mat4){.m={
        1.0f, 0.0f, 0.0f, 0.0f,
        0.0f, 1.0f, 0.0f, 0.0f,
        0.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 1.0f
    }};
}

Mat4 _transpose4_impl(Mat4 mat) {
    Mat4 result = mat;
    COREX_SWAP(f32, result.m[1], result.m[4]);   // (0,1) <-> (1,0)
    COREX_SWAP(f32, result.m[2], result.m[8]);   // (0,2) <-> (2,0)
    COREX_SWAP(f32, result.m[3], result.m[12]);  // (0,3) <-> (3,0)
    COREX_SWAP(f32, result.m[6], result.m[9]);   // (1,2) <-> (2,1)
    COREX_SWAP(f32, result.m[7], result.m[13]);  // (1,3) <-> (3,1)
    COREX_SWAP(f32, result.m[11],result.m[14]);  // (2,3) <-> (3,2)
    return result;
}

Vec3 _mult4v3_impl(Mat4 m, Vec3 v) {
    Vec3 result;
    result.x = m.m[0] * v.x + m.m[4] * v.y + m.m[8] * v.z + m.m[12];
    result.y = m.m[1] * v.x + m.m[5] * v.y + m.m[9] * v.z + m.m[13];
    result.z = m.m[2] * v.x + m.m[6] * v.y + m.m[10] * v.z + m.m[14];
    return result;
}

Mat4 _scale4_impl(f32 x, f32 y, f32 z) {
    Mat4 result = {
        x, 0, 0, 0,
        0, y, 0, 0,
        0, 0, z, 0,
        0, 0, 0, 1
    };
    return result;
}

Mat4 _trans4_impl(f32 x, f32 y, f32 z) {
    Mat4 result = {
        1, 0, 0, 0,
        0, 1, 0, 0,
        0, 0, 1, 0,
        x, y, z, 1
    };
    return result;
}

Mat4 _rotx4_impl(f32 angle) {
    f32 rad = corex->mathx.scalar.radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);
    Mat4 result = {
        1, 0, 0, 0,
        0, cos_angle, sin_angle, 0,
        0,-sin_angle, cos_angle, 0,
        0, 0, 0, 1
    };
    return result;
}

Mat4 _roty4_impl(f32 angle) {
    f32 rad = corex->mathx.scalar.radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);
    Mat4 result = {
        cos_angle, 0,-sin_angle, 0,
        0, 1, 0, 0,
        sin_angle, 0, cos_angle, 0,
        0, 0, 0, 1
    };
    return result;
}

Mat4 _rotz4_impl(f32 angle) {
    f32 rad = corex->mathx.scalar.radians(angle);
    f32 cos_angle = cosf(rad);
    f32 sin_angle = sin(rad);
    Mat4 result = {
        cos_angle, sin_angle, 0, 0,
       -sin_angle, cos_angle, 0, 0,
        0, 0, 1, 0,
        0, 0, 0, 1
    };
    return result;
}

Mat4 _rot4_impl(Vec3 axis, f32 angle) {
    Mat4 result = corex->mathx.mat.identity4();
    f32 rad = corex->mathx.scalar.radians(angle);
    f32 cos_theta = cosf(rad);
    f32 sin_theta = sinf(rad);

    f32 axis_len = corex->mathx.vec.mag3(axis);
    if (axis_len > 0.0f) {
        axis.x /= axis_len;
        axis.y /= axis_len;
        axis.z /= axis_len;
    }

    result.m[0] = cos_theta + (1 - cos_theta) * axis.x * axis.x;
    result.m[1] = (1 - cos_theta) * axis.x * axis.y + sin_theta * axis.z;
    result.m[2] = (1 - cos_theta) * axis.x * axis.z - sin_theta * axis.y;

    result.m[4] = (1 - cos_theta) * axis.y * axis.x - sin_theta * axis.z;
    result.m[5] = cos_theta + (1 - cos_theta) * axis.y * axis.y;
    result.m[6] = (1 - cos_theta) * axis.y * axis.z + sin_theta * axis.x;

    result.m[8] = (1 - cos_theta) * axis.z * axis.x + sin_theta * axis.y;
    result.m[9] = (1 - cos_theta) * axis.z * axis.y - sin_theta * axis.x;
    result.m[10] = cos_theta + (1 - cos_theta) * axis.z * axis.z;

    return result;
}


Mat4 _mult4_impl(Mat4 a, Mat4 b) {
    Mat4 result = {0};
    for (int row = 0; row < 4; ++row) {
        for (int col = 0; col < 4; ++col) {
            for (int i = 0; i < 4; ++i) {
                result.m[row * 4 + col] += a.m[row * 4 + i] * b.m[i * 4 + col];
            }
        }
    }
    return result;
}

Mat4 _lookat_impl(Vec3 eye, Vec3 center, Vec3 up) {
    Vec3 target = corex->mathx.vec.add3(eye, center);
    
    Vec3 f = corex->mathx.vec.norm3(corex->mathx.vec.sub3(target, eye)); 
    Vec3 s = corex->mathx.vec.norm3(corex->mathx.vec.cross3(f, up));
    Vec3 u = corex->mathx.vec.norm3(corex->mathx.vec.cross3(s, f));

    Mat4 result = {
        s.x, u.x,-f.x, 0.0f,
        s.y, u.y,-f.y, 0.0f,
        s.z, u.z,-f.z, 0.0f,
       -corex->mathx.vec.dot3(s, eye), -corex->mathx.vec.dot3(u, eye), corex->mathx.vec.dot3(f, eye), 1.0f,
    };
    
    return result;
}

Mat4 _ortho_impl(f32 left, f32 right, f32 bottom, f32 top, f32 near, f32 far) {
    f32 rml = right - left;
    f32 tmb = top - bottom;
    f32 fmn = far - near;
    
    f32 rpl = right + left;
    f32 tpb = top + bottom;
    f32 fpn = far + near;
    
    Mat4 result = {
        2.0f / rml,  0.0f,       0.0f,       0.0f,
        0.0f,        2.0f / tmb, 0.0f,       0.0f,
        0.0f,        0,         -2.0f / fmn, 0.0f,
        -rpl / rml, -tpb / tmb, -fpn / fmn, 1.0f

    };

    return result;
}

Mat4 _perspective_impl(f32 fovy, f32 aspect, f32 near, f32 far) {
    f32 tfov = tanf(corex->mathx.scalar.radians(fovy) / 2.0f);
    f32 fpn = far + near;
    f32 fmn = far - near;
    f32 fn = far * near;
    
    Mat4 result = {
        1.0f / (aspect * tfov),   0,                0,                       0,
        0,                        1.0f / tfov,      0,                       0,
        0,                        0,               -fpn / fmn,              -1.0f,
        0,                        0,               -(2.0f * fn) / fmn,       1.0f
    };

    return result;
}
/* ---------------- MAT4 ---------------- */

u8 _corex_init_mathx(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->mathx.init == COREX_TRUE) return COREX_TRUE;    // redundant call: Mathx API already initialized!

    // SCALAR API INIT
    corex->mathx.scalar.radians = _to_radians_impl;

    // VECTOR API INIT
    corex->mathx.vec.print2 = _print_vec2_impl;
    corex->mathx.vec.print3 = _print_vec3_impl;
    corex->mathx.vec.print4 = _print_vec4_impl;
    
    corex->mathx.vec.vec2 = _new_vec2_impl;
    corex->mathx.vec.add2 = _add_vec2_impl;
    corex->mathx.vec.sub2 = _sub_vec2_impl;
    corex->mathx.vec.dot2 = _dot_vec2_impl;
    corex->mathx.vec.norm2 = _norm_vec2_impl;
    corex->mathx.vec.mag2 = _mag_vec2_impl;
    corex->mathx.vec.scale2 = _scale_vec2_impl;
    
    corex->mathx.vec.vec3 = _new_vec3_impl;
    corex->mathx.vec.add3 = _add_vec3_impl;
    corex->mathx.vec.sub3 = _sub_vec3_impl;
    corex->mathx.vec.dot3 = _dot_vec3_impl;
    corex->mathx.vec.norm3 = _norm_vec3_impl;
    corex->mathx.vec.mag3 = _mag_vec3_impl;
    corex->mathx.vec.scale3 = _scale_vec3_impl;
    corex->mathx.vec.cross3 = _cross_vec3_impl;
    
    corex->mathx.vec.vec4 = _new_vec4_impl;
    corex->mathx.vec.add4 = _add_vec4_impl;
    corex->mathx.vec.sub4 = _sub_vec4_impl;
    corex->mathx.vec.dot4 = _dot_vec4_impl;
    corex->mathx.vec.norm4 = _norm_vec4_impl;
    corex->mathx.vec.mag4 = _mag_vec4_impl;
    corex->mathx.vec.scale4 = _scale_vec4_impl;
    
    corex->mathx.mat.print2 = _print_mat2_impl;
    corex->mathx.mat.print3 = _print_mat3_impl;
    corex->mathx.mat.print4 = _print_mat4_impl;

    // MATRIX 2
    corex->mathx.mat.identity2 = _identity2_impl;
    corex->mathx.mat.transpose2 = _transpose2_impl;
    
    corex->mathx.mat.mult2 = _mul_mat2_impl;
    
    // MATRIX 3
    corex->mathx.mat.identity3 = _identity3_impl;
    corex->mathx.mat.transpose3 = _transpose3_impl;
    
    corex->mathx.mat.mult3 = _mul_mat3_impl;

    // MATRIX 4
    corex->mathx.mat.identity4 = _identity4_impl;
    corex->mathx.mat.transpose4 = _transpose4_impl;
    
    corex->mathx.mat.rot4 = _rot4_impl;
    corex->mathx.mat.rotx4 = _rotx4_impl;
    corex->mathx.mat.roty4 = _roty4_impl;
    corex->mathx.mat.rotz4 = _rotz4_impl;
    corex->mathx.mat.trans4 = _trans4_impl;
    corex->mathx.mat.scale4 = _scale4_impl;
    
    corex->mathx.mat.mult4 = _mult4_impl;
    corex->mathx.mat.mult4v3 = _mult4v3_impl;
    
    corex->mathx.mat.lookat = _lookat_impl;
    
    corex->mathx.mat.ortho = _ortho_impl;
    corex->mathx.mat.perspective = _perspective_impl;
    
    corex->meta.usage.mathx = sizeof(Mathx);
	corex->mathx.init = COREX_TRUE;
    return COREX_TRUE;
}

void _corex_cleanup_mathx(void) {
    if (corex->mathx.init == COREX_FALSE) return;    // error: Mathx API not initialized!
	corex->meta.usage.apis &= ~COREX_MATHX;
	corex->mathx.init = COREX_FALSE;
    corex->meta.usage.mathx = 0;
	corex->mathx	= (Mathx){0};
}

// ==== corex/filex/corex_filex.c ====
u8 _exists_impl(cstr path) {
    FILE* file = fopen(path, "rb");
    if (!file) return COREX_FALSE;
    fclose(file);
    return COREX_TRUE;
}

u8 _copy_impl(cstr src, cstr dest, u64 size) {
    FILE* srcf = fopen(src, "rb");
    if (!srcf) return COREX_FALSE;   // error: file not found!
    
    FILE* destf = fopen(dest, "wb");
    if (!destf) {
        fclose(srcf);
        return COREX_FALSE;  // error: file not found!
    }

    u64 read = 0;
    char buffer[size];
    do fwrite(buffer, sizeof(char), read, destf);
    while((read = fread(buffer, sizeof(char), size, srcf)) > 0);
    
    fclose(srcf);
    fclose(destf);
    return COREX_TRUE;
}

u64 _size_impl(cstr path) {
    FILE* file = fopen(path, "rb");
    if (!file) return 0;    // error: file not found!
    
    fseek(file, 0, SEEK_END);
    u64 read = ftell(file);
    fclose(file);

    return read;
}

str _read_impl(cstr path, u64 size) {
    FILE* file = fopen(path, "rb");
    if (!file) return NULL; // error: file not found!

    if (!size) size = corex->filex.size(path);

    str out = corex->memx.alloc(size+1, 8);
    if (!out) return NULL;  // error: out of memory!
    
    u64 read = 0;
    read = fread(out, sizeof(char), size, file);
    if (read != size) {
        corex->memx.dealloc(out);
        fclose(file);
        return NULL;    // error: failed to read file!
    }
    
    out[size] = '\0';
    fclose(file);
    return out;
}

u8 _readb_impl(cstr path, u64 size, void* out) {
    FILE* file = fopen(path, "rb");
    if (!file) return COREX_FALSE; // error: file not found!

    u64 read = 0;
    read = fread(out, size, 1, file);
    if (read != size) {
        fclose(file);
        return COREX_FALSE;    // error: failed to fully read data!
    }
    
    fclose(file);
    return COREX_TRUE;
}

u8 _remove_impl(cstr path) {
    if (remove(path) != COREX_FALSE) return COREX_FALSE;  // error: failed to delete file!
    return COREX_TRUE;
}

u8 _write_impl(cstr path, cstr lines, u8 preserve) {
    FILE* file = fopen(path, preserve ? "ab" : "wb");
    if (!file) return COREX_FALSE;   // error: failed to write to file!

    u64 size = strlen(lines);
    if (!size) {
        fclose(file);
        return COREX_FALSE;  // error: failed to get size of `lines`!
    }

    u64 written = fwrite(lines, sizeof(char), size, file);
    if (written != size) {
        fclose(file);
        return COREX_FALSE;  // error: failed to write all lines!
    }
    
    fclose(file);
    return COREX_TRUE;
}

u8 _writeb_impl(cstr path, u64 size, u8 preserve, void* ptr) {
    FILE* file = fopen(path, preserve ? "ab" : "wb");
    if (!file) return COREX_FALSE;   // error: failed to write to file!

    u64 written = fwrite(ptr, size, 1, file);
    if (written != size) {
        fclose(file);
        return COREX_FALSE;  // error: failed to write all lines!
    }
    
    fclose(file);
    return COREX_TRUE;
}

u8 _append_impl(cstr path, cstr lines, u8 newline) {
    u8 result = corex->filex.write(path, lines, 1);
    if (newline) result = corex->filex.write(path, "\n", 1);
    return result;
}

u8 _appendb_impl(cstr path, u64 size, u8 newline, void* ptr) {
    u8 result = corex->filex.writeb(path, size, 1, ptr);
    if (newline) result = corex->filex.write(path, "\n", 1);
    return result;
}

u8 _process_impl(cstr path, u64 lines, void (*callback)(cstr line)) {
    FILE* file = fopen(path, "rb");
    if (!file) return COREX_FALSE;    // error: failed to open file!

    str buffer = corex->memx.alloc(lines, 8);
    if (!buffer) return COREX_FALSE; // error: failed to allocate temporary buffer!
    
    do callback((cstr)buffer);
    while(fgets(buffer, lines ,file));
    
    corex->memx.dealloc(buffer);
    fclose(file);

    return COREX_TRUE;
}


u8 _corex_init_filex(void) {
    if (!corex) return COREX_FALSE; // error: null ptr!
    if (corex->filex.init == COREX_TRUE) return COREX_TRUE;    // redundant call: Filex API already initialized!

    if (corex->memx.init == COREX_FALSE) {
        printf("corex Memx api not initialized!\n");
        return COREX_FALSE; // error: failed to initialize Memx api!
    }
    
    corex->filex.exists = _exists_impl;
    corex->filex.copy = _copy_impl;
    corex->filex.size = _size_impl;
    corex->filex.read = _read_impl;
    corex->filex.readb = _readb_impl;
    corex->filex.write = _write_impl;
    corex->filex.remove = _remove_impl;
    corex->filex.append = _append_impl;
    corex->filex.writeb = _writeb_impl;
    corex->filex.appendb = _appendb_impl;
    corex->filex.process = _process_impl;

    corex->meta.usage.filex = sizeof(Filex);
	corex->filex.init = COREX_TRUE;
    return COREX_TRUE;
}

void _corex_cleanup_filex(void) {
    if (corex->filex.init == COREX_FALSE) return;    // error: Filex API not initialized!
	corex->meta.usage.apis &= ~COREX_FILEX;
	corex->filex.init = COREX_FALSE;
    corex->meta.usage.filex = 0;
	corex->filex	= (Filex){0};
}

// ==== corex/corex.c ====
Corex* corex = NULL;

void _dump_impl(void) {
    u64 total = corex->meta.usage.memx + corex->meta.usage.dsx + corex->meta.usage.genx + corex->meta.usage.ecsx + corex->meta.usage.mathx + corex->meta.usage.filex;
    printf("---Corex %s---\n", corex->meta.version.string);
    printf("\"%s\"\n", corex->meta.version.quote);
    printf("|Memx  %d: %lluB\n", corex->memx.init, corex->meta.usage.memx);
    printf("|Dsx   %d: %lluB\n", corex->dsx.init, corex->meta.usage.dsx);
    printf("|Genx  %d: %lluB\n", corex->genx.init, corex->meta.usage.genx);
    printf("|Ecsx  %d: %lluB\n", corex->ecsx.init, corex->meta.usage.ecsx);
    printf("|Mathx %d: %lluB\n", corex->mathx.init, corex->meta.usage.mathx);
    printf("|Filex %d: %lluB", corex->filex.init, corex->meta.usage.filex);
    printf("\n-----Total %lluB-----\n", total);
}

u8 corex_init(Corex_Api mask) {
    if (!mask) return COREX_FALSE;

    if (corex == NULL) {
        corex = malloc(sizeof(Corex));
        if (!corex) return COREX_FALSE;	// error: out of memory!

        corex->meta.version.major = COREX_VERSION_MAJOR;
        corex->meta.version.minor = COREX_VERSION_MINOR;
        corex->meta.version.patch = COREX_VERSION_PATCH;
        corex->meta.version.string = COREX_VERSION_STRING;
        corex->meta.version.quote = "Welcome to Li-, uh, Corex!";
        
        corex->meta.usage.apis = 0;
        corex->meta.usage.memx = 0;
        corex->meta.usage.mathx = 0;
        corex->meta.usage.dsx = 0;
        corex->meta.usage.genx = 0;
        corex->meta.usage.filex = 0;
        corex->meta.usage.ecsx = 0;
        
        corex->memx.init = COREX_FALSE;
        corex->mathx.init = COREX_FALSE;
        corex->dsx.init = COREX_FALSE;
        corex->genx.init = COREX_FALSE;
        corex->filex.init = COREX_FALSE;
        corex->ecsx.init = COREX_FALSE;

        corex->dump = _dump_impl;
    }

    // check if any new modules were requested
    Corex_Api new_mask = mask & ~corex->meta.usage.apis;
    if (!new_mask) return COREX_TRUE; // error: requested apis already initialized!

    corex->meta.usage.apis |= new_mask;

    if ((new_mask & COREX_ALL) == COREX_ALL) {
        if (!_corex_init_memx()   ||
            !_corex_init_mathx()  ||
            !_corex_init_dsx()    ||
            !_corex_init_genx()   ||
            !_corex_init_ecsx()   ||
            !_corex_init_filex()) {
            return COREX_FALSE;
        }
        return COREX_TRUE;
    }

    if (new_mask & COREX_MEMX)  { if (!_corex_init_memx())   return COREX_FALSE; }
    if (new_mask & COREX_MATHX) { if (!_corex_init_mathx())  return COREX_FALSE; }
    if (new_mask & COREX_DSX)   { if (!_corex_init_dsx())    return COREX_FALSE; }
    if (new_mask & COREX_GENX)  { if (!_corex_init_genx())   return COREX_FALSE; }
    if (new_mask & COREX_FILEX) { if (!_corex_init_filex())  return COREX_FALSE; }
    if (new_mask & COREX_ECSX)  { if (!_corex_init_ecsx())   return COREX_FALSE; }

    return COREX_TRUE;
}

u8 corex_deinit(Corex_Api mask) {
    if (!corex) return COREX_FALSE;
    if (!mask) return COREX_FALSE;

    if (mask == COREX_ALL) {
        _corex_cleanup_ecsx();
        _corex_cleanup_filex();
        _corex_cleanup_genx();
        _corex_cleanup_dsx();
        _corex_cleanup_mathx();
        _corex_cleanup_memx();
        return COREX_TRUE;
    }
   
    if (mask & COREX_ECSX)  { _corex_cleanup_ecsx(); }
    if (mask & COREX_FILEX) { _corex_cleanup_filex(); }
    if (mask & COREX_GENX)  { _corex_cleanup_genx(); }
    if (mask & COREX_DSX)   { _corex_cleanup_dsx(); }
    if (mask & COREX_MATHX) { _corex_cleanup_mathx(); }
    if (mask & COREX_MEMX)  { _corex_cleanup_memx(); }
    
    return COREX_TRUE;
}

u8 corex_cleanup(void) {
    if (!corex) return COREX_TRUE;
    corex_deinit(corex->meta.usage.apis);
    free(corex);
    corex = NULL;
    return COREX_TRUE;
}

#endif // COREX_IMPL
#endif // __COREX_H__