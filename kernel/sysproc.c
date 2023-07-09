#include "types.h"
#include "riscv.h"
#include "defs.h"
#include "param.h"
#include "memlayout.h"
#include "spinlock.h"
#include "proc.h"

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
  int* p;
  argaddr(0,(uint64 *)&p);
  uint64 time_low32 = 0;
  uint64 time_high32 = 0;
  time_low32 = *R(TIME_LOW);
  time_high32 = *R(TIME_HIGH);
  uint64 time = (time_high32<<32) | time_low32;
  time = time / 1000000000;
  return time;
}

//return datetime
uint64
sys_datetime(void) {
  int* p;
  argaddr(0,(uint64 *)&p);
  uint64 time_low32 = 0;
  uint64 time_high32 = 0;
  time_low32 = *R(TIME_LOW);
  time_high32 = *R(TIME_HIGH);
  uint64 time = (time_high32<<32) | time_low32;
  time = time / 1000000000;
  
  
  // uint64 datetime =  second + 100*minute + 10000*hour + 1000000 * day + 100000000*month + 10000000000*1;
  uint64 datetime =  4300000000;
  return datetime;
}