#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>

void sigHandler (int);

int main()
{
    signal(SIGINT, sigHandler);
    printf("waiting...\n");
    pause();
    return 0;
}

void sigHandler (int sigNum)
{
    printf(" received an interrupt signal through Ctrl-C.\n");
    printf("Going to sleep.\n Notice the process is not exiting immediately\n");
    // this is where shutdown code would be inserted
    sleep (1);
    printf ("Out of here.\n");
    printf("Now the process will terminate with the exit system call \n");
    exit(0);
}
