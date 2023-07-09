#include "kernel/types.h"
#include "kernel/stat.h"
#include "user/user.h"
#include "user/dateconv.c"

int
main(int argc, char *argv[])
{
  uint64 t = time();
  printf("current unix time is %l\n",t);
  uint32 year;uint8 month; uint8 day; uint8 hour; uint8 minute; uint8 second; uint8 weekday;
  ConvertUnixTimeToTokyoTime(t, &year, &month, &day, &hour, &minute, &second, &weekday);
  printf("current datetime is %d/%d/%d %d:%d:%d\n",year,month,day,hour,minute,second);
  exit(0);
}
