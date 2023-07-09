#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/spinlock.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pipes[2];
  char buf[100];
  pipe(pipes);
  int status;
  int pid = fork();
  if (pid == 0) {
      read(pipes[0], buf, sizeof(buf));
      printf("fds is %s\n", buf);
      
    
  } else {
    fprintf(pipes[1], "aaaaa");
    wait(&status);
    printf("im parent\n");
  }
  exit(0);
}
