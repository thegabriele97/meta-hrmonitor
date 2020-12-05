#include <stdio.h>
#include "unistd.h"
#include "fcntl.h"

#include "threads.h"
#include "global.h"
#include "fft.h"

void waitfor(double ms) {
    clock_t begin;
    
    ms /= 1000;
    begin = clock();
    while((double)(clock() - begin) / (CLOCKS_PER_SEC) < ms);
}

void *_th_acquire(void *fd_wr) {
    int fd, i;
    int buf;
    clock_t begin, end;

    if ((fd = open(ACQUIRE_DEVICE, O_RDONLY)) < 0) {
        fprintf(stderr, "Unable to open %s\n", ACQUIRE_DEVICE);
        pthread_exit(NULL);
    }

    do {
        for (i = 0; i < MAX_DATA; i++) {
            waitfor(SAMPLE_PERIOD_MS);

            read(fd, &buf, sizeof buf);
            write(*(int *)fd_wr, &buf, sizeof buf);
        }
    } while(1);

    close(fd);
    pthread_exit(NULL);
}

void *_th_compute(void *fd_rd) {
    complex v[MAX_DATA], scratch[MAX_DATA];
    float abs[MAX_DATA];
    int minIdx, maxIdx;
    int i, m, buf;

    do {
        
        for (i = 0; i < MAX_DATA; i++) {
            read(*(int *)fd_rd, &buf, sizeof buf);
            v[i].Re = buf;
            v[i].Im = 0;
        }

        // fft
        fft(v, MAX_DATA, scratch);

        // power-spectrum density
        pds(v, MAX_DATA, abs);

        minIdx = (0.5*MAX_DATA)/50;   // position in the PSD of the spectral line corresponding to 30 bpm
        maxIdx = 3*MAX_DATA/50;       // position in the PSD of the spectral line corresponding to 180 bpm

        // Find the peak in the PSD from 30 bpm to 180 bpm
        for(i = m = minIdx; i < maxIdx; i++) {
            if (abs[i] > abs[m]) {
                m = i;
            }
        }

        // Print the heart beat in bpm
        printf("\n\n\n%d bpm\n\n\n", (m)*60*50/MAX_DATA);
    } while(1);

    pthread_exit(NULL);
}
