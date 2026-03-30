#include "complex_number.h"

#include <math.h>
#include <stdio.h>
#include <stdlib.h>

static double* complex_re_ptr(const Complex* value) {
    return (double*)value->re;
}

static double* complex_im_ptr(const Complex* value) {
    return (double*)value->im;
}

static int complex_alloc_parts(Complex* value) {
    if (value == NULL) {
        return 0;
    }
    value->re = malloc(sizeof(double));
    value->im = malloc(sizeof(double));
    if (value->re == NULL || value->im == NULL) {
        free(value->re);
        free(value->im);
        value->re = NULL;
        value->im = NULL;
        return 0;
    }
    return 1;
}

Complex complex_make(double re, double im) {
    Complex value;
    value.re = NULL;
    value.im = NULL;
    if (complex_alloc_parts(&value)) {
        *complex_re_ptr(&value) = re;
        *complex_im_ptr(&value) = im;
    }
    return value;
}

int complex_init(void* value) {
    Complex* c = (Complex*)value;
    if (!complex_alloc_parts(c)) {
        return 0;
    }
    *complex_re_ptr(c) = 0.0;
    *complex_im_ptr(c) = 0.0;
    return 1;
}

void complex_destroy_value(void* value) {
    Complex* c = (Complex*)value;
    if (c == NULL) {
        return;
    }
    free(c->re);
    free(c->im);
    c->re = NULL;
    c->im = NULL;
}

int complex_copy(const void* src, void* dst) {
    const Complex* s = (const Complex*)src;
    Complex* d = (Complex*)dst;
    if (s == NULL || d == NULL || s->re == NULL || s->im == NULL || d->re == NULL || d->im == NULL) {
        return 0;
    }
    *complex_re_ptr(d) = *complex_re_ptr(s);
    *complex_im_ptr(d) = *complex_im_ptr(s);
    return 1;
}

void complex_add(const void* left, const void* right, void* out) {
    const Complex* a = (const Complex*)left;
    const Complex* b = (const Complex*)right;
    Complex* result = (Complex*)out;

    if (a == NULL || b == NULL || result == NULL || a->re == NULL || a->im == NULL || b->re == NULL || b->im == NULL ||
        result->re == NULL || result->im == NULL) {
        return;
    }

    *complex_re_ptr(result) = *complex_re_ptr(a) + *complex_re_ptr(b);
    *complex_im_ptr(result) = *complex_im_ptr(a) + *complex_im_ptr(b);
}

void complex_mul(const void* left, const void* right, void* out) {
    const Complex* a = (const Complex*)left;
    const Complex* b = (const Complex*)right;
    Complex* result = (Complex*)out;

    if (a == NULL || b == NULL || result == NULL || a->re == NULL || a->im == NULL || b->re == NULL || b->im == NULL ||
        result->re == NULL || result->im == NULL) {
        return;
    }

    *complex_re_ptr(result) =
        (*complex_re_ptr(a)) * (*complex_re_ptr(b)) - (*complex_im_ptr(a)) * (*complex_im_ptr(b));
    *complex_im_ptr(result) =
        (*complex_re_ptr(a)) * (*complex_im_ptr(b)) + (*complex_im_ptr(a)) * (*complex_re_ptr(b));
}

void complex_print(const void* value) {
    const Complex* c = (const Complex*)value;
    if (c == NULL || c->re == NULL || c->im == NULL) {
        printf("null");
        return;
    }
    if (*complex_im_ptr(c) < 0.0) {
        printf("%.3f - %.3fi", *complex_re_ptr(c), fabs(*complex_im_ptr(c)));
    } 
    else {
        printf("%.3f + %.3fi", *complex_re_ptr(c), *complex_im_ptr(c));
    }
}

int complex_equals(const void* left, const void* right) {
    const Complex* a = (const Complex*)left;
    const Complex* b = (const Complex*)right;
    const double eps = 1e-9;

    if (a == NULL || b == NULL || a->re == NULL || a->im == NULL || b->re == NULL || b->im == NULL) {
        return 0;
    }

    return fabs(*complex_re_ptr(a) - *complex_re_ptr(b)) < eps &&
           fabs(*complex_im_ptr(a) - *complex_im_ptr(b)) < eps;
}