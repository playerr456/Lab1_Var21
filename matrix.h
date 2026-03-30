#ifndef MATRIX_H
#define MATRIX_H

#include <stddef.h>

#include "field_info.h"

typedef struct Matrix {
    size_t n;
    void* data;
    const FieldInfo* field;
} Matrix;

Matrix* matrix_create(size_t n, const FieldInfo* field);
void matrix_destroy(Matrix* matrix);

int matrix_set(Matrix* matrix, size_t row, size_t col, const void* value);
const void* matrix_get(const Matrix* matrix, size_t row, size_t col);

Matrix* matrix_add(const Matrix* left, const Matrix* right);
Matrix* matrix_mul(const Matrix* left, const Matrix* right);
Matrix* matrix_scalar_mul(const Matrix* matrix, const void* scalar);

int matrix_equals(const Matrix* left, const Matrix* right);
void matrix_print(const Matrix* matrix);

#endif
