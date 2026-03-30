#include "field_info.h"

#include "complex_number.h"

#include <stdio.h>

static void int_add(const void* left, const void* right, void* out) {
    const int* a = (const int*)left;
    const int* b = (const int*)right;
    int* result = (int*)out;
    *result = *a + *b;
}

static void int_mul(const void* left, const void* right, void* out) {
    const int* a = (const int*)left;
    const int* b = (const int*)right;
    int* result = (int*)out;
    *result = (*a) * (*b);
}

static void int_print(const void* value) {
    printf("%d", *(const int*)value);
}

static int int_equals(const void* left, const void* right) {
    return *(const int*)left == *(const int*)right;
}

static int int_init(void* value) {
    int* x = (int*)value;
    *x = 0;
    return 1;
}

static void int_destroy(void* value) {
    (void)value;
}

static int int_copy(const void* src, void* dst) {
    *(int*)dst = *(const int*)src;
    return 1;
}

const FieldInfo* get_int_field_info(void) {
    static const FieldInfo info = {
        "int",
        sizeof(int),
        int_init,
        int_destroy,
        int_copy,
        int_add,
        int_mul,
        int_print,
        int_equals
    };
    return &info;
}

const FieldInfo* get_complex_field_info(void) {
    static const FieldInfo info = {
        "complex",
        sizeof(Complex),
        complex_init,
        complex_destroy_value,
        complex_copy,
        complex_add,
        complex_mul,
        complex_print,
        complex_equals
    };
    return &info;
}
