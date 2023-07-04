#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"
#include "dateconv.c"

//RTC macro
#define VIRTRTC0 0x101000
#define VIRTRTC0_IRQ 1
#define R(r) ((volatile uint32*)(VIRTRTC0 + (r)))

#define TIME_LOW 0x00
#define TIME_HIGH 0x04
#define ALARM_LOW 0x08
#define ALARM_HIGH 0x0c
#define CLEAR_INTERRUPT 0x10

uint64
sys_exit(void)
{
  int n;
  argint(0, &n);
  exit(n);
  return 0;  // not reached
}

uint64
sys_getpid(void)
{
  return myproc()->pid;
}

uint64
sys_fork(void)
{
  return fork();
}

uint64
sys_wait(void)
{
  uint64 p;
  argaddr(0, &p);
  return wait(p);
}

uint64
sys_sbrk(void)
{
  uint64 addr;
  int n;

  argint(0, &n);
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

uint64
sys_sleep(void)
{
  int n;
  uint ticks0;

  argint(0, &n);
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(killed(myproc())){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

uint64
sys_kill(void)
{
  int pid;

  argint(0, &pid);
  return kill(pid);
}

// return how many clock tick interrupts have occurred
// since start.
uint64
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

//return epoch time
uint64
sys_time(void) {
  uint64 testarg;
  argaddr(0,&testarg);
  uint64 time_low32 = 0;
  uint64 time_high32 = 0;
  time_low32 = *R(TIME_LOW);
  time_high32 = *R(TIME_HIGH);
  uint64 time = (time_high32<<32) | time_low32;
  time = time / 1000000000;
  uint32 year;uint8 month; uint8 day; uint8 hour; uint8 minute; uint8 second; uint8 weekday;
  ConvertUnixTimeToTokyoTime(time, &year, &month, &day, &hour, &minute, &second, &weekday);
  // printf("time is %p \n", time);
  printf("%d/%d/%d %d:%d:%d(%d)\n",year,month,day,hour,minute,second,weekday);
  uint64 test;
  fetchaddr(testarg, &test);
  printf("num is %d\n", test);
  printf("first p is %d, sec p is %p\n", testarg, &test);
  return 0;
}