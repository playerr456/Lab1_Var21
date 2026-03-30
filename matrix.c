#include "matrix.h"

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static int matrix_compatible(const Matrix* left, const Matrix* right) {
    if (left == NULL || right == NULL) {
        return 0;
    }
    if (left->field == NULL || right->field == NULL) {
        return 0;
    }
    if (left->field != right->field) {
        return 0;
    }
    return left->n == right->n;
}

static size_t matrix_index(const Matrix* matrix, size_t row, size_t col) {
    return row * matrix->n + col;
}

static void* matrix_get_mut(Matrix* matrix, size_t row, size_t col) {
    if (matrix == NULL || matrix->data == NULL || matrix->field == NULL) {
        return NULL;
    }
    if (row >= matrix->n || col >= matrix->n) {
        return NULL;
    }
    return (char*)matrix->data + matrix_index(matrix, row, col) * matrix->field->size;
}

static int matrix_init_cells(Matrix* matrix) {
    size_t i = 0;
    size_t count = matrix->n * matrix->n;
    for (i = 0; i < count; i ++) {
        void* cell = (char*)matrix->data + i * matrix->field->size;
        if (!matrix->field->init(cell)) {
            size_t j = 0;
            for (j = 0; j < i; j ++) {
                void* prev = (char*)matrix->data + j * matrix->field->size;
                matrix->field->destroy(prev);
            }
            return 0;
        }
    }
    return 1;
}

Matrix* matrix_create(size_t n, const FieldInfo* field) {
    Matrix* matrix = NULL;
    size_t bytes = 0;

    if (n == 0 || field == NULL) {
        return NULL;
    }

    matrix = (Matrix*)malloc(sizeof(Matrix));
    if (matrix == NULL) {
        return NULL;
    }

    bytes = n * n * field->size;
    matrix->data = calloc(1, bytes);
    if (matrix->data == NULL) {
        free(matrix);
        return NULL;
    }

    matrix->n = n;
    matrix->field = field;

    if (!matrix_init_cells(matrix)) {
        free(matrix->data);
        free(matrix);
        return NULL;
    }
    return matrix;
}

void matrix_destroy(Matrix* matrix) {
    size_t i = 0;
    size_t count = 0;
    if (matrix == NULL) {
        return;
    }

    if (matrix->field != NULL && matrix->data != NULL) {
        count = matrix->n * matrix->n;
        for (i = 0; i < count; i ++) {
            void* cell = (char*)matrix->data + i * matrix->field->size;
            matrix->field->destroy(cell);
        }
    }
    free(matrix->data);
    free(matrix);
}

int matrix_set(Matrix* matrix, size_t row, size_t col, const void* value) {
    void* cell = NULL;
    if (value == NULL) {
        return 0;
    }
    cell = matrix_get_mut(matrix, row, col);
    if (cell == NULL) {
        return 0;
    }
    return matrix->field->copy(value, cell);
}

const void* matrix_get(const Matrix* matrix, size_t row, size_t col) {
    if (matrix == NULL || matrix->data == NULL || matrix->field == NULL) {
        return NULL;
    }
    if (row >= matrix->n || col >= matrix->n) {
        return NULL;
    }
    return (const char*)matrix->data + matrix_index(matrix, row, col) * matrix->field->size;
}

Matrix* matrix_add(const Matrix* left, const Matrix* right) {
    Matrix* result = NULL;
    size_t i = 0;
    size_t count = 0;

    if (!matrix_compatible(left, right)) {
        return NULL;
    }

    result = matrix_create(left->n, left->field);
    if (result == NULL) {
        return NULL;
    }

    count = left->n * left->n;
    for (i = 0; i < count; i ++) {
        const void* a = (const char*)left->data + i * left->field->size;
        const void* b = (const char*)right->data + i * right->field->size;
        void* out = (char*)result->data + i * result->field->size;
        left->field->add(a, b, out);
    }

    return result;
}

Matrix* matrix_mul(const Matrix* left, const Matrix* right) {
    Matrix* result = NULL;
    void* prev = NULL;
    void* curr = NULL;
    void* next = NULL;
    void* zero = NULL;
    size_t i = 0;
    size_t j = 0;
    size_t k = 0;
    size_t elem_size = 0;

    if (!matrix_compatible(left, right)) {
        return NULL;
    }

    result = matrix_create(left->n, left->field);
    if (result == NULL) {
        return NULL;
    }

    elem_size = left->field->size;
    prev = calloc(1, elem_size);
    curr = calloc(1, elem_size);
    next = calloc(1, elem_size);
    zero = calloc(1, elem_size);
    if (prev == NULL || curr == NULL || next == NULL || zero == NULL) {
        free(prev);
        free(curr);
        free(next);
        free(zero);
        matrix_destroy(result);
        return NULL;
    }

    if (!left->field->init(prev) || !left->field->init(curr) || !left->field->init(next) || !left->field->init(zero)) {
        left->field->destroy(prev);
        left->field->destroy(curr);
        left->field->destroy(next);
        left->field->destroy(zero);
        free(prev);
        free(curr);
        free(next);
        free(zero);
        matrix_destroy(result);
        return NULL;
    }

    for (i = 0; i < left->n; i ++) {
        for (j = 0; j < left->n; j ++) {
            left->field->copy(zero, prev);
            for (k = 0; k < left->n; ++k) {
                const void* a = matrix_get(left, i, k);
                const void* b = matrix_get(right, k, j);

                left->field->mul(a, b, curr);
                left->field->add(prev, curr, next);
                left->field->copy(next, prev);
            }
            matrix_set(result, i, j, prev);
        }
    }

    left->field->destroy(prev);
    left->field->destroy(curr);
    left->field->destroy(next);
    left->field->destroy(zero);
    free(prev);
    free(curr);
    free(next);
    free(zero);
    return result;
}

Matrix* matrix_scalar_mul(const Matrix* matrix, const void* scalar) {
    Matrix* result = NULL;
    size_t i = 0;
    size_t count = 0;

    if (matrix == NULL || matrix->field == NULL || scalar == NULL) {
        return NULL;
    }

    result = matrix_create(matrix->n, matrix->field);
    if (result == NULL) {
        return NULL;
    }

    count = matrix->n * matrix->n;
    for (i = 0; i < count; i ++) {
        const void* value = (const char*)matrix->data + i * matrix->field->size;
        void* out = (char*)result->data + i * result->field->size;
        matrix->field->mul(value, scalar, out);
    }

    return result;
}

int matrix_equals(const Matrix* left, const Matrix* right) {
    size_t i = 0;
    size_t count = 0;

    if (!matrix_compatible(left, right)) {
        return 0;
    }

    count = left->n * left->n;
    for (i = 0; i < count; i ++) {
        const void* a = (const char*)left->data + i * left->field->size;
        const void* b = (const char*)right->data + i * right->field->size;
        if (!left->field->equals(a, b)) {
            return 0;
        }
    }
    return 1;
}

void matrix_print(const Matrix* matrix) {
    size_t i = 0;
    size_t j = 0;
    if (matrix == NULL || matrix->field == NULL) {
        printf("(null matrix)\n");
        return;
    }

    for (i = 0; i < matrix->n; i ++) {
        printf("[ ");
        for (j = 0; j < matrix->n; j ++) {
            const void* cell = matrix_get(matrix, i, j);
            matrix->field->print(cell);
            if (j + 1 < matrix->n) {
                printf(" , ");
            }
        }
        printf(" ]\n");
    }
}
