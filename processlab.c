#include  <stdio.h>
#include <stdlib.h>
#include  <string.h>
#include <time.h>
#include  <sys/types.h>

#define BUF_SIZE 100

void ChildProcess(void);
void ParentProcess(void);
void WaitForBothChildren(void);

void main(void) {
  // seed random with current time
  time_t t;
  srandom((unsigned) time( & t));
  
  // make first child
  pid_t pid;

  pid = fork();
  if (pid == 0)
    ChildProcess();
  else
    ParentProcess();
}

void ParentProcess(void) {
  // make second child
  pid_t pid = fork();
  if (pid == 0)
    ChildProcess();
  else
    WaitForBothChildren();
}

void WaitForBothChildren(void) {
  int status;
  pid_t pid;
  int num_processes = 2;
  char buf[BUF_SIZE];
  while (num_processes > 0) {
    pid = wait( & status);
    sprintf(buf, "Child Pid: %d has completed\n", pid);
    write(1, buf, strlen(buf));
    num_processes--;
  }
}

void ChildProcess(void) {
  unsigned int random_iter = (random() % 29) + 1; // prevent 0 by adding 1
  unsigned int i;
  char buf[BUF_SIZE];
  for (i = 1; i <= random_iter; i++) {
    sprintf(buf, "Child Pid: %d is going to sleep!\n", getpid());
    write(1, buf, strlen(buf));
    sleep((random() % 9) + 1);
    sprintf(buf, "Child Pid: %d is awake!\nWhere is my Parent: %d?\n", getpid(), getppid());
    write(1, buf, strlen(buf));
  }
}