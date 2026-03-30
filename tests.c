#include "tests.h"

#include "field_info.h"
#include "matrix.h"

#include <assert.h>
#include <stdio.h>

void test_add_int_1x1(void) {
    int a_value = 2;
    int b_value = 5;
    Matrix* a = matrix_create(1, get_int_field_info());
    Matrix* b = matrix_create(1, get_int_field_info());
    Matrix* sum = NULL;
    const int* result = NULL;

    assert(a != NULL);
    assert(b != NULL);
    assert(matrix_set(a, 0, 0, &a_value) == 1);
    assert(matrix_set(b, 0, 0, &b_value) == 1);

    sum = matrix_add(a, b);
    assert(sum != NULL);

    result = (const int*)matrix_get(sum, 0, 0);
    assert(result != NULL);
    assert(*result == 7);

    matrix_destroy(a);
    matrix_destroy(b);
    matrix_destroy(sum);
}

void test_mul_int_1x1(void) {
    int a_value = 3;
    int b_value = 4;
    Matrix* a = matrix_create(1, get_int_field_info());
    Matrix* b = matrix_create(1, get_int_field_info());
    Matrix* product = NULL;
    const int* result = NULL;

    assert(a != NULL);
    assert(b != NULL);
    assert(matrix_set(a, 0, 0, &a_value) == 1);
    assert(matrix_set(b, 0, 0, &b_value) == 1);

    product = matrix_mul(a, b);
    assert(product != NULL);

    result = (const int*)matrix_get(product, 0, 0);
    assert(result != NULL);
    assert(*result == 12);

    matrix_destroy(a);
    matrix_destroy(b);
    matrix_destroy(product);
}

void test_scalar_mul_int_1x1(void) {
    int value = 4;
    int scalar = 3;
    Matrix* a = matrix_create(1, get_int_field_info());
    Matrix* scaled = NULL;
    const int* result = NULL;

    assert(a != NULL);
    assert(matrix_set(a, 0, 0, &value) == 1);

    scaled = matrix_scalar_mul(a, &scalar);
    assert(scaled != NULL);

    result = (const int*)matrix_get(scaled, 0, 0);
    assert(result != NULL);
    assert(*result == 12);

    matrix_destroy(a);
    matrix_destroy(scaled);
}

void test_add_different_types_returns_null(void) {
    Matrix* int_matrix = matrix_create(2, get_int_field_info());
    Matrix* complex_matrix = matrix_create(2, get_complex_field_info());

    assert(int_matrix != NULL);
    assert(complex_matrix != NULL);

    assert(matrix_add(int_matrix, complex_matrix) == NULL);
    assert(matrix_add(complex_matrix, int_matrix) == NULL);

    matrix_destroy(int_matrix);
    matrix_destroy(complex_matrix);
}

void test_mul_different_types_returns_null(void) {
    Matrix* int_matrix = matrix_create(2, get_int_field_info());
    Matrix* complex_matrix = matrix_create(2, get_complex_field_info());

    assert(int_matrix != NULL);
    assert(complex_matrix != NULL);

    assert(matrix_mul(int_matrix, complex_matrix) == NULL);
    assert(matrix_mul(complex_matrix, int_matrix) == NULL);

    matrix_destroy(int_matrix);
    matrix_destroy(complex_matrix);
}

void test_different_sizes_return_null(void) {
    Matrix* small = matrix_create(2, get_int_field_info());
    Matrix* large = matrix_create(3, get_int_field_info());

    assert(small != NULL);
    assert(large != NULL);

    assert(matrix_add(small, large) == NULL);
    assert(matrix_mul(small, large) == NULL);

    matrix_destroy(small);
    matrix_destroy(large);
}

void run_all_tests(void) {
    printf("Running all the tests...\n");

    test_add_int_1x1();
    printf("test_add_int_1x1: OK\n");

    test_mul_int_1x1();
    printf("test_mul_int_1x1: OK\n");

    test_scalar_mul_int_1x1();
    printf("test_scalar_mul_int_1x1: OK\n");

    test_add_different_types_returns_null();
    printf("test_add_different_types_returns_null: OK\n");

    test_mul_different_types_returns_null();
    printf("test_mul_different_types_returns_null: OK\n");

    test_different_sizes_return_null();
    printf("test_different_sizes_return_null: OK\n");

    printf("All the tests are passed.\n");
}
