#include "../../include/utility/lotus_linked_list.h"

Lotus_List_Node *lotus_create_list(ubyte4 dataStride) {
    size_t headerSize = 3 * sizeof(ubyte4);
    ubyte4 *header = (ubyte4 *)lotus_memory_api->alloc(headerSize + sizeof(Lotus_List_Node), 16);
    if (!header) {
        return NULL;    // error: run out of memory!
    }

    header[0] = headerSize + sizeof(Lotus_List_Node);
    header[1] = dataStride;
    header[2] = 1;

    Lotus_List_Node *node = (Lotus_List_Node *)((ubyte *)header + headerSize);
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void lotus_destroy_list(Lotus_List_Node *startNode) {
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    Lotus_List_Node *current = startNode->next;
    for (ubyte4 i = 1; i < length; i++) {
        if (!current) break;
        
        if (!current->next) {
            lotus_memory_api->free(current);
            break;
        }
        
        Lotus_List_Node *next = current->next;
        lotus_memory_api->free(current);
        current = next;
    }

    lotus_memory_api->free(header);
}


Lotus_List_Header lotus_get_list_header(Lotus_List_Node *startNode) {
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    return (Lotus_List_Header) {
        .size = header[0],
        .stride = header[1],
        .length = header[2]
    };
}

ubyte lotus_set_list_header_field(Lotus_List_Node *startNode, Lotus_List_Header_Field field, ubyte4 value) {
    if (!startNode || field > LOTUS_LIST_HEADER_FIELDS) { return LOTUS_FALSE; }
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    header[field] = value;
    return LOTUS_TRUE;
}

ubyte4 lotus_get_list_header_field(Lotus_List_Node *startNode, Lotus_List_Header_Field field) {
    if (!startNode || field > LOTUS_LIST_HEADER_FIELDS) { return LOTUS_FALSE; }
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    return header[field];
}


ubyte lotus_create_list_node(Lotus_List_Node *startNode) {
    if (!startNode) return LOTUS_FALSE;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    Lotus_List_Node *newNode = (Lotus_List_Node*)lotus_memory_api->alloc(sizeof(Lotus_List_Node), 16);
    if (!newNode) return LOTUS_FALSE;  // error: out of memory!

    newNode->data = NULL;

    Lotus_List_Node *endNode = startNode;
    for (ubyte4 i = 0; i < length; i++) {
        if (!endNode->next) break;
        endNode = endNode->next;
    }

    newNode->prev = endNode;
    newNode->next = NULL;

    endNode->next = newNode;

    header[0] += sizeof(Lotus_List_Node);
    header[2]++;
    return LOTUS_TRUE;
}

ubyte lotus_remove_list_node(Lotus_List_Node *startNode) {
    if (!startNode) return LOTUS_FALSE;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    Lotus_List_Node *endNode = startNode;
    for (ubyte4 i = 0; i < length; i++) {
        if (!endNode->next) break;
        endNode = endNode->next;
    }

    lotus_memory_api->free(endNode);

    header[0] -= sizeof(Lotus_List_Node);
    header[2]--;
    return LOTUS_TRUE;
}

Lotus_List_Node *lotus_query_list_node(Lotus_List_Node *startNode, ubyte4 index) {
    if (!startNode) return NULL;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    
    ubyte4 length = header[2];
    if (index >= length) return LOTUS_FALSE; // error: index out of bounds!

    Lotus_List_Node *node = startNode;
    for (ubyte4 i = 0; i < index; i++) {
        if (!node->next) break;
        node = node->next;
    }

    return node;
}
