// #include <unistd.h>
// #include <stdio.h>
// #include <stdlib.h>
// #include <sys/wait.h>
// #include <fcntl.h>
#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "kernel/fcntl.h"


int main(int argc, char *argv[])
{
  int status;
  while (1) {
    char buf[512];
    char buf2[512];
    read(0, buf, 512);
    for (int i = 0; i < 512; i++) {
      buf2[i] = buf[i];
    }
    fprintf(1, "%s\n", buf);

    // pid_t pid;
    int pid = fork();
    if (pid < 0) {
      fprintf(2, "fork fail\n");
    }
    else if (pid==0) {
      fprintf(1, "test\n");
      char* cmdArgv[6];
      int startFlag = 1;
      for (int i = 0; i < 512; i++) {
        if (startFlag) {
          startFlag = 0;
          
        }
      }

      exec();
      printf("%s\n", buf2);
      fprintf(2, "exec error: %s¥n", argv[0]);
    }
    else {
      wait(&status);
      fprintf(1, "finish\n");
    }

  }
  exit(0);
}
