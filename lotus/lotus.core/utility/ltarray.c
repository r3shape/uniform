#include "ltarray.h"

#include "../memory/ltmemory.h"
#include "../platform/ltlogger.h"

/* LT VALUE */

void ltDestroyValue(LTvalue* v) {
    v->type = LOTUS_NONE;
    ltMemFree(v->value, sizeof(v->value), LOTUS_MEMTAG_VALUE);
    ltMemFree(v, sizeof(LTvalue), LOTUS_MEMTAG_VALUE);
}

LTvalue* ltMakeValue(LTvalueType type, void* value) {
    LTvalue* v = (LTvalue*)ltMemAlloc(sizeof(LTvalue), LOTUS_MEMTAG_VALUE);
    if (!v) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic value");
        return NULL;
    }
    v->type = type;
    v->value = value;
    return v;
}

LTvalue* ltMakeInt(int value) {
    LTvalue* v = (LTvalue*)ltMemAlloc(sizeof(LTvalue), LOTUS_MEMTAG_VALUE);
    if (!v) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic value");
        return NULL;
    }

    v->type = LOTUS_INT;
    
    v->value = (int*)ltMemAlloc(sizeof(int), LOTUS_MEMTAG_VALUE);
    if (!v->value) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate integer");
        v->value = NULL;
        return v;
    }; *(int*)v->value = value;
    
    return v;
}

LTvalue* ltMakeFloat(float value) {
    LTvalue* v = (LTvalue*)ltMemAlloc(sizeof(LTvalue), LOTUS_MEMTAG_VALUE);
    if (!v) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic value");
        return NULL;
    }

    v->type = LOTUS_FLOAT;
    
    v->value = (float*)ltMemAlloc(sizeof(float), LOTUS_MEMTAG_VALUE);
    if (!v->value) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate float");
        v->value = NULL;
        return v;
    }; *(float*)v->value = value;
    
    return v;
}

LTvalue* ltMakeString(const char* value) {
    LTvalue* v = (LTvalue*)ltMemAlloc(sizeof(LTvalue), LOTUS_MEMTAG_VALUE);
    if (!v) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic value");
        return NULL;
    }

    v->type = LOTUS_STRING;
    v->value = strdup(value);
    return v;
}

int ltGetInt(LTvalue* v) {
    return *(int*)v->value;
}

char* ltGetString(LTvalue* v) {
    return (char*)v->value;
}

float ltGetFloat(LTvalue* v) {
    return *(float*)v->value;
}

/* LT ARRAY */
LTarray* ltMakeArray(int max, int resize) {
    LTarray* arr = (LTarray*)ltMemAlloc(sizeof(LTarray), LOTUS_MEMTAG_ARRAY);
    if (!arr) {
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic array");
        return NULL;
    }

    arr->count = 0;
    arr->max = max;
    arr->resize = resize;

    arr->arr = (LTvalue**)ltMemAlloc(max*sizeof(void*), LOTUS_MEMTAG_ARRAY);
    if (!arr->arr) {
        ltMemFree(arr, sizeof(LTarray), LOTUS_MEMTAG_ARRAY);
        ltSetLogLevel(LOTUS_LOG_ERROR);
        ltLogError("failed to allocate dynamic array");
        return NULL;
    }; return arr;
}

void ltDestroyArray(LTarray* inArr) {
    // free associeced data
    for(int i = 0; i < inArr->count; i++) {
        ltDestroyValue(inArr->arr[i]);
    }; ltMemFree(inArr->arr, inArr->max * sizeof(LTvalue*), LOTUS_MEMTAG_ARRAY);
    ltMemFree(inArr, inArr->max * sizeof(LTvalue*), LOTUS_MEMTAG_ARRAY);
}

LTerrorType ltResizeArray(LTarray* inArr) {
    LTerrorType err = LOTUS_ERR_NONE;
    
    LTvalue** temp = (LTvalue**)realloc(inArr->arr, inArr->max+inArr->resize * sizeof(LTvalue*));
    if (!temp) {
        err = LOTUS_ERR_MALLOC;
        return err;
    }
    
    inArr->max += inArr->resize;
    inArr->arr = temp;
    return err;
}

LTerrorType ltInsertArray(LTarray* inArr, int index, LTvalue* value) {
    LTerrorType err = LOTUS_ERR_NONE;
    if (!inArr || index < 0) { return err; }

    if (index >= inArr->max || inArr->count == inArr->max) {
        err = ltResizeArray(inArr);
    }

    if (!err) {
        inArr->arr[index] = value;
        inArr->count++;
    }; return err;
}

LTerrorType ltInsertArrayV(LTarray* inArr, int count, LTvalue** values) {
    LTerrorType err = LOTUS_ERR_NONE;
    if (inArr->count + count >= inArr->max || inArr->count == inArr->max) {
        err = ltResizeArray(inArr);
    }
    if (!err) {
        for (int i = 0; i < count; i++) {
            inArr->arr[inArr->count] = values[i];
            inArr->count++;
        }
    }; return err;
}

LTvalue* ltPopArray(LTarray* inArr, int index) {
    LTerrorType err = LOTUS_ERR_NONE;
    
    if (!inArr || index >= inArr->max) {
        return NULL;
    }

    void* value = inArr->arr[index];

    for (int i = index; i < inArr->count-1; i++) {
        inArr->arr[i] = inArr->arr[i + 1];
    };inArr->arr[inArr->count-1] = NULL;
    
    inArr->count--;
    return value;
}

LTvalue* ltQueryArray(LTarray* inArr, int index) {
    LTerrorType err = LOTUS_ERR_NONE;
    
    if (!inArr || index >= inArr->max) {
        return NULL;
    }
    
    return inArr->arr[index];
}

int ltQueryArrayInt(LTarray* inArr, int index) {
    return ltGetInt(ltQueryArray(inArr, index));
}

float ltQueryArrayFloat(LTarray* inArr, int index) {
    return ltGetFloat(ltQueryArray(inArr, index));
}

char* ltQueryArrayString(LTarray* inArr, int index) {
    return ltGetString(ltQueryArray(inArr, index));
}

