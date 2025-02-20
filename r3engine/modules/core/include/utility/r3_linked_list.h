#pragma once

#include "../r3_core_defines.h"
#include "../memory/r3_memory.h"

typedef enum R3_List_Header_Field {
    R3_LIST_HEADER_SIZE = 0,
    R3_LIST_HEADER_STRIDE,
    R3_LIST_HEADER_LENGTH,
    R3_LIST_HEADER_FIELDS,
} R3_List_Header_Field;

typedef struct R3_List_Header {
    ubyte4 size;
    ubyte4 stride;
    ubyte4 length;
} R3_List_Header;

typedef struct R3_List_Node {
    struct R3_List_Node* prev;
    struct R3_List_Node* next;
    void* data;
} R3_List_Node;

R3_API R3_List_Node* r3_create_list(ubyte4 dataStride);
R3_API void r3_destroy_list(R3_List_Node* startNode);

R3_API R3_List_Header r3_get_list_header(R3_List_Node* startNode);
R3_API ubyte4 r3_get_list_header_field(R3_List_Node* startNode, R3_List_Header_Field field);
R3_API ubyte r3_set_list_header_field(R3_List_Node* startNode, R3_List_Header_Field field, ubyte4 value);

R3_API ubyte r3_create_list_node(R3_List_Node* startNode);
R3_API ubyte r3_remove_list_node(R3_List_Node* startNode);
R3_API R3_List_Node* r3_query_list_node(R3_List_Node* startNode, ubyte4 index);
