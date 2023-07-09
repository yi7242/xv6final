#include "kernel/types.h"
#include "kernel/stat.h"
#include "kernel/fcntl.h"
#include "kernel/spinlock.h"
#include "user/user.h"

int main(int argc, char *argv[]) {
  int pipes[2];
  pipe(pipes);
  int status;
  int pid = fork();
  if (pid == 0) {
    write(pipes[1], "test", 5);
  } else {
    wait(&status);
    char buf[100];
    read(pipes[0], buf, sizeof(buf));
    printf("%s\n",buf);
  }
  exit(0);
}
