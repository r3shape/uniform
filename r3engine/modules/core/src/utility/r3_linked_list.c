#include "../../include/utility/r3_linked_list.h"

R3_List_Node *r3_create_list(ubyte4 dataStride) {
    size_t headerSize = 3 * sizeof(ubyte4);
    ubyte4 *header = (ubyte4 *)r3_memory_api->alloc(headerSize + sizeof(R3_List_Node), 16);
    if (!header) {
        return NULL;    // error: run out of memory!
    }

    header[0] = headerSize + sizeof(R3_List_Node);
    header[1] = dataStride;
    header[2] = 1;

    R3_List_Node *node = (R3_List_Node *)((ubyte *)header + headerSize);
    node->data = NULL;
    node->prev = NULL;
    node->next = NULL;
    
    return node;
}

void r3_destroy_list(R3_List_Node *startNode) {
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    R3_List_Node *current = startNode->next;
    for (ubyte4 i = 1; i < length; i++) {
        if (!current) break;
        
        if (!current->next) {
            r3_memory_api->free(current);
            break;
        }
        
        R3_List_Node *next = current->next;
        r3_memory_api->free(current);
        current = next;
    }

    r3_memory_api->free(header);
}


R3_List_Header r3_get_list_header(R3_List_Node *startNode) {
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    return (R3_List_Header) {
        .size = header[0],
        .stride = header[1],
        .length = header[2]
    };
}

ubyte r3_set_list_header_field(R3_List_Node *startNode, R3_List_Header_Field field, ubyte4 value) {
    if (!startNode || field > R3_LIST_HEADER_FIELDS) { return R3_FALSE; }
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    header[field] = value;
    return R3_TRUE;
}

ubyte4 r3_get_list_header_field(R3_List_Node *startNode, R3_List_Header_Field field) {
    if (!startNode || field > R3_LIST_HEADER_FIELDS) { return R3_FALSE; }
    ubyte4 *header = (ubyte4 *)((ubyte *)startNode - (3 * sizeof(ubyte4)));
    return header[field];
}


ubyte r3_create_list_node(R3_List_Node *startNode) {
    if (!startNode) return R3_FALSE;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    R3_List_Node *newNode = (R3_List_Node*)r3_memory_api->alloc(sizeof(R3_List_Node), 16);
    if (!newNode) return R3_FALSE;  // error: out of memory!

    newNode->data = NULL;

    R3_List_Node *endNode = startNode;
    for (ubyte4 i = 0; i < length; i++) {
        if (!endNode->next) break;
        endNode = endNode->next;
    }

    newNode->prev = endNode;
    newNode->next = NULL;

    endNode->next = newNode;

    header[0] += sizeof(R3_List_Node);
    header[2]++;
    return R3_TRUE;
}

ubyte r3_remove_list_node(R3_List_Node *startNode) {
    if (!startNode) return R3_FALSE;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    ubyte4 length = header[2];

    R3_List_Node *endNode = startNode;
    for (ubyte4 i = 0; i < length; i++) {
        if (!endNode->next) break;
        endNode = endNode->next;
    }

    r3_memory_api->free(endNode);

    header[0] -= sizeof(R3_List_Node);
    header[2]--;
    return R3_TRUE;
}

R3_List_Node *r3_query_list_node(R3_List_Node *startNode, ubyte4 index) {
    if (!startNode) return NULL;
    ubyte4 *header = (ubyte4 *)((ubyte *)(startNode) - (3 * sizeof(ubyte4)));
    
    ubyte4 length = header[2];
    if (index >= length) return R3_FALSE; // error: index out of bounds!

    R3_List_Node *node = startNode;
    for (ubyte4 i = 0; i < index; i++) {
        if (!node->next) break;
        node = node->next;
    }

    return node;
}
