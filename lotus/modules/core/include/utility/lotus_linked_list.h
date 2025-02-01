#pragma once

#include "../lotus_core_defines.h"
#include "../memory/lotus_memory.h"

typedef enum Lotus_List_Header_Field {
    LOTUS_LIST_HEADER_SIZE = 0,
    LOTUS_LIST_HEADER_STRIDE,
    LOTUS_LIST_HEADER_LENGTH,
    LOTUS_LIST_HEADER_FIELDS,
} Lotus_List_Header_Field;

typedef struct Lotus_List_Header {
    ubyte4 size;
    ubyte4 stride;
    ubyte4 length;
} Lotus_List_Header;

typedef struct Lotus_List_Node {
    struct Lotus_List_Node* prev;
    struct Lotus_List_Node* next;
    void* data;
} Lotus_List_Node;

LOTUS_API Lotus_List_Node* lotus_create_list(ubyte4 dataStride);
LOTUS_API void lotus_destroy_list(Lotus_List_Node* startNode);

LOTUS_API Lotus_List_Header lotus_get_list_header(Lotus_List_Node* startNode);
LOTUS_API ubyte4 lotus_get_list_header_field(Lotus_List_Node* startNode, Lotus_List_Header_Field field);
LOTUS_API ubyte lotus_set_list_header_field(Lotus_List_Node* startNode, Lotus_List_Header_Field field, ubyte4 value);

LOTUS_API ubyte lotus_create_list_node(Lotus_List_Node* startNode);
LOTUS_API ubyte lotus_remove_list_node(Lotus_List_Node* startNode);
LOTUS_API Lotus_List_Node* lotus_query_list_node(Lotus_List_Node* startNode, ubyte4 index);
