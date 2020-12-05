#ifndef _FFT_H
#define _FFT_H

#ifndef PI
#define PI	3.14159265358979323846264338327950288
#endif

typedef float real;
typedef struct {
    real Re; 
    real Im;
} complex;

void fft(complex *v, int n, complex *tmp);
void pds(complex *v, int n, float *abs);

#endif
