#ifndef __R3_TREE_H__
#define __R3_TREE_H__

#include <r3kit/include/defines.h>

typedef enum Tree_Header_Field {
    ID_FIELD,
    MAX_FIELD,
    SIZE_FIELD,
    NODES_FIELD,
    STRIDE_FIELD,
    TREE_HEADER_FIELDS,
} Tree_Header_Field;

// bare 'wrapper' structure for type saftey
// the internal data field is ALWAYS allocated
// with 10 header bytes
typedef struct Tree {
    ptr nodes;
    ptr data;
} Tree;

R3_API u8 r3_tree_dealloc(Tree* in);
R3_API u16 r3_tree_field(Tree_Header_Field field, Tree* in);
R3_API u8 r3_tree_alloc(u16 id, u16 max, u16 stride, Tree* out);

#endif // __R3_TREE_H__
