#include "complex_number.h"
#include "field_info.h"
#include "matrix.h"

#include <stdio.h>

static void fill_matrix_manual(Matrix* matrix, const FieldInfo* field, const char* name) {
    size_t i = 1;
    size_t j = 1;

    printf("Fill matrix %s\n", name);
    for (i = 1; i <= matrix->n; i ++) {
        for (j = 1; j <= matrix->n; j ++) {
            if (field == get_int_field_info()) {
                int value = 0;
                printf("  %s[%zu][%zu] = ", name, i, j);
                scanf("%d", &value);
                matrix_set(matrix, i - 1, j - 1, &value);
            }
            else {
                double re = 0.0;
                double im = 0.0;
                Complex value;

                printf("  %s[%zu][%zu] (re im) = ", name, i, j);
                scanf("%lf %lf", &re, &im);

                value = complex_make(re, im);
                matrix_set(matrix, i - 1, j - 1, &value);
                complex_destroy_value(&value);
            }
        }
    }
}

int main(void) {
    const FieldInfo* field = NULL;
    Matrix* a = NULL;
    Matrix* b = NULL;
    Matrix* sum = NULL;
    Matrix* product = NULL;
    Matrix* scaled = NULL;

    int type = 1;
    int n = 2;
    int scalar_int = 0;
    Complex scalar_complex;
    void* scalar_ptr = NULL;

    printf("Type: 1 - int, 2 - complex\n> ");
    scanf("%d", &type);
    field = (type == 2) ? get_complex_field_info() : get_int_field_info();

    printf("Matrix size n\n> ");
    scanf("%d", &n);
    if (n < 1) {
        n = 1;
    }

    a = matrix_create((size_t)n, field);
    b = matrix_create((size_t)n, field);
    if (a == NULL || b == NULL) {
        printf("Error.\n");
        matrix_destroy(a);
        matrix_destroy(b);
        return 1;
    }

    fill_matrix_manual(a, field, "A");
    fill_matrix_manual(b, field, "B");

    printf("\nA =\n");
    matrix_print(a);
    printf("B =\n");
    matrix_print(b);

    sum = matrix_add(a, b);
    if (sum != NULL) {
        printf("\nA + B =\n");
        matrix_print(sum);
    }

    product = matrix_mul(a, b);
    if (product != NULL) {
        printf("\nA * B =\n");
        matrix_print(product);
    }

    if (field == get_int_field_info()) {
        printf("\nScalar (int)\n> ");
        scanf("%d", &scalar_int);
        scalar_ptr = &scalar_int;
    } 
    else {
        double re = 0.0;
        double im = 0.0;
        printf("\nScalar (re im)\n> ");
        scanf("%lf %lf", &re, &im);
        scalar_complex = complex_make(re, im);
        scalar_ptr = &scalar_complex;
    }

    scaled = matrix_scalar_mul(a, scalar_ptr);
    if (field == get_complex_field_info()) {
        complex_destroy_value(&scalar_complex);
    }
    if (scaled != NULL) {
        printf("\nA * scalar =\n");
        matrix_print(scaled);
    }

    matrix_destroy(sum);
    matrix_destroy(product);
    matrix_destroy(scaled);
    matrix_destroy(a);
    matrix_destroy(b);
    return 0;
}
