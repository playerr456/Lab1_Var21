#ifndef FIELD_INFO_H
#define FIELD_INFO_H

#include <stddef.h>

typedef struct FieldInfo {
    const char* name;
    size_t size;
    int (*init)(void* value);
    void (*destroy)(void* value);
    int (*copy)(const void* src, void* dst);
    void (*add)(const void* left, const void* right, void* out);
    void (*mul)(const void* left, const void* right, void* out);
    void (*print)(const void* value);
    int (*equals)(const void* left, const void* right);
} FieldInfo;

const FieldInfo* get_int_field_info(void);
const FieldInfo* get_complex_field_info(void);

#endif
