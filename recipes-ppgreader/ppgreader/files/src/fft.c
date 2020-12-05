#include <math.h>
#include "fft.h"

void pds(complex *v, int n, float *abs) {
    int k;
    
    for(k = 0; k < n; k++) {
        abs[k] = (50.0/2048)*((v[k].Re*v[k].Re)+(v[k].Im*v[k].Im)); 
    }
}

void fft(complex *v, int n, complex *tmp) {
    if (n > 1) {			/* otherwise, do nothing and return */
        int k,m;    
        complex z, w, *vo, *ve;
    
        ve = tmp; vo = tmp+n/2;
    
        for(k = 0; k <n/2; k++) {
            ve[k] = v[2*k];
            vo[k] = v[2*k+1];
        }
        
        fft( ve, n/2, v );		/* FFT on even-indexed elements of v[] */
        fft( vo, n/2, v );		/* FFT on odd-indexed elements of v[] */
        
        for(m = 0; m < n/2; m++) {
            w.Re = cos(2*PI*m/(double)n);
            w.Im = -sin(2*PI*m/(double)n);
            
            z.Re = w.Re*vo[m].Re - w.Im*vo[m].Im;	/* Re(w*vo[m]) */
            z.Im = w.Re*vo[m].Im + w.Im*vo[m].Re;	/* Im(w*vo[m]) */
            
            v[  m  ].Re = ve[m].Re + z.Re;
            v[  m  ].Im = ve[m].Im + z.Im;
            v[m+n/2].Re = ve[m].Re - z.Re;
            v[m+n/2].Im = ve[m].Im - z.Im;
        }
    }
}