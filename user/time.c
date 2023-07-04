#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"

int
main(int argc, char *argv[])
{
  int year = 10;
  printf("year p is %p\n", &year);
  time(&year);
  printf("year is %d", year);
  exit(0);
}
