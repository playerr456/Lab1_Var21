#ifndef COMPLEX_NUMBER_H
#define COMPLEX_NUMBER_H

typedef struct Complex {
    void* re;
    void* im;
} Complex;

Complex complex_make(double re, double im);
int complex_init(void* value);
void complex_destroy_value(void* value);
int complex_copy(const void* src, void* dst);

void complex_add(const void* left, const void* right, void* out);
void complex_mul(const void* left, const void* right, void* out);
void complex_print(const void* value);
int  complex_equals(const void* left, const void* right);

#endif
