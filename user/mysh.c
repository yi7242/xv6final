#include "kernel/types.h"
#include "kernel/fcntl.h"
#include "kernel/stat.h"
#include "user/user.h"

#define NULL 0

// strtok functions
unsigned int is_delim(char c, char *delim) {
  while (*delim != '\0') {
    if (c == *delim)
      return 1;
    delim++;
  }
  return 0;
}
char *my_strtok(char *srcString, char *delim) {
  static char *backup_string; // start of the next search
  if (!srcString) {
    srcString = backup_string;
  }
  if (!srcString) {
    // user is bad user
    return NULL;
  }
  // handle beginning of the string containing delims
  while (1) {
    if (is_delim(*srcString, delim)) {
      srcString++;
      continue;
    }
    if (*srcString == '\0') {
      // we've reached the end of the string
      return NULL;
    }
    break;
  }
  char *ret = srcString;
  while (1) {
    if (*srcString == '\0') {
      /*end of the input string and
      next exec will return NULL*/
      backup_string = srcString;
      return ret;
    }
    if (is_delim(*srcString, delim)) {
      *srcString = '\0';
      backup_string = srcString + 1;
      return ret;
    }
    srcString++;
  }
}

// shell function
int main(void) {
  while (1) {
    printf("myshell: ");

    int pid = fork();
    if (pid < 0) {
      fprintf(2, "fork fail\n");
    } else if (pid == 0) {
      char buf[512];
      char pipebuf[512];
      int flag = 0;
      int pipeflag = 0;
      memset(buf, 0, 512);
      memset(pipebuf, 0, 512);
      read(0, buf, 512);
      int count = 0;
      char *str = my_strtok(buf, " \n");
      char *args[100];
      while (str) {
        args[count] = str;
        str = my_strtok(NULL, " \n");
        count++;

        // redirect
        if (strcmp(str, ">") == 0) {
          flag = 1;
          int p[2];
          pipe(p);
          args[count] = 0;
          int pid2 = fork();
          if (pid2 == 0) {
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            exec(args[0], args);
            exit(0);
          } else {
            wait(0);
            char buf2[512];
            memset(buf2, 0, 512);
            read(p[0], buf2, sizeof(buf2));
            str = my_strtok(NULL, " \n");
            int df = open(str, O_RDWR | O_CREATE);
            write(df, buf2, sizeof(buf2));
            close(p[0]);
            close(p[1]);
          }

        // pipe
        } else if (strcmp(str, "|") == 0) {
          pipeflag = 1;
          int p[2];
          pipe(p);
          args[count] = 0;
          int pid2 = fork();
          if (pid2 == 0) {
            close(1);
            dup(p[1]);
            close(p[0]);
            close(p[1]);
            exec(args[0], args);
            exit(0);
          } else {
            wait(0);
            read(p[0], pipebuf, sizeof(pipebuf));
            str = my_strtok(NULL, " \n");
            count = 0;
            close(p[0]);
            close(p[1]);
          }
        }
      }
      if (pipeflag) {
        args[count] = 0;
        int pid2 = fork();
        if (pid2 == 0) {
          write(0, pipebuf, sizeof(pipebuf));
          exec(args[0], args);
          exit(0);
        } else {
          wait(0);
        }
        exit(0);
      }

      else if (!flag) {
        args[count] = 0;
        exec(args[0], args);
        fprintf(2, "exec error: %s¥n", args[0]);
      }
    } else {
      wait(0);
    }
  }

  return 0;
}