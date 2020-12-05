#include <stdio.h>
#include <stdlib.h>
#include "unistd.h"
#include "fcntl.h"
#include "pthread.h"

#include "src/global.h"
#include "src/threads.h"

int main() {
    pthread_t threads[2];
    int pipe_fd[2];
    int retval1, retval2 = 0;

    if (retval1 = pipe(pipe_fd) < 0) {
        fputs(stderr, "Error: unable to create the pipe\n");
        return retval1;
    } 

    retval1 = pthread_create(&threads[0], NULL, _th_acquire, &pipe_fd[1]);
    retval2 = pthread_create(&threads[1], NULL, _th_compute, &pipe_fd[0]);

    if (retval1 || retval2) {
        fprintf(stderr, "Error: return code of pthread_create is %d\n", retval1 ? retval1 : retval2);
        return (retval1 ? retval1 : retval2);
    }

    pthread_join(threads[0], NULL);

    close(pipe_fd[0]);
    close(pipe_fd[1]);
    return EXIT_SUCCESS;
}