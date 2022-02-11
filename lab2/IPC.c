#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

#define TRUE 1
#define FALSE 0

void sigHandler(int);

int main()
{

  srand(time(0));

  int pid = fork(); /* call fork() from parent process*/
  if (pid == 0)
  {
    /* fork returned 0. This part will be executed by child process*/
    /*  getpid() will give child process id here */

    /*use getppid() here to get the parent pid*/
    while(TRUE) {
      int sleepTime = rand() % 3 + 1;
      sleep(sleepTime);

      int whichSignal = rand() % 2;
      if(whichSignal == 0) kill(getppid(), SIGUSR1);
      if(whichSignal == 1) kill(getppid(), SIGUSR2);
    }
  }
  else
  {
    /* fork returned child pid which is non zero. This part will be executed by
    parent process*/
    /*  getpid() will give parent process id here */
    while(TRUE) {
      sleep(1);

      if(signal(SIGUSR1, sigHandler) == SIG_ERR)
        printf("\ncan't catch SIGUSR1\n");
      if(signal(SIGUSR2, sigHandler) == SIG_ERR)
        printf("\ncan't catch SIGUSR2\n");
      if(signal(SIGINT, sigHandler) == SIG_ERR)
        printf("\ncan't catch SIGINT\n");

    }
  }
}

void sigHandler(int sigNum)
{
  if(sigNum == SIGINT) {
    printf("Received SIGINT\n");
    exit(0);
  }

  if(sigNum == SIGUSR1) {
    printf("Caught signal SIGUSR1\n");
  }

  if(sigNum == SIGUSR2) {
    printf("Caught signal SIGUSR2\n");
  }
}
