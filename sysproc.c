#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"


int sys_yield(void)
{
  yield(); 
  return 0;
}

int
sys_fork(void)
{
  return fork();
}

int
sys_exit(void)
{
  exit();
  return 0;  // not reached
}

int
sys_wait(void)
{
  return wait();
}

int
sys_kill(void)
{
  int pid;

  if(argint(0, &pid) < 0)
    return -1;
  return kill(pid);
}

int
sys_getpid(void)
{
  return myproc()->pid;
}

int
sys_sbrk(void)
{
  int addr;
  int n;

  if(argint(0, &n) < 0)
    return -1;
  addr = myproc()->sz;
  if(growproc(n) < 0)
    return -1;
  return addr;
}

int
sys_sleep(void)
{
  int n;
  uint ticks0;

  if(argint(0, &n) < 0)
    return -1;
  acquire(&tickslock);
  ticks0 = ticks;
  while(ticks - ticks0 < n){
    if(myproc()->killed){
      release(&tickslock);
      return -1;
    }
    sleep(&ticks, &tickslock);
  }
  release(&tickslock);
  return 0;
}

// return how many clock tick interrupts have occurred
// since start.
int
sys_uptime(void)
{
  uint xticks;

  acquire(&tickslock);
  xticks = ticks;
  release(&tickslock);
  return xticks;
}

int sys_setVariable(void){
    char* variable;
    char* value;

    if (argptr(0, &variable, 32) < 0 || argptr(1, &value, 128) < 0)
        return -1;

    return setVariable(variable, value);
}

int sys_getVariable(void){
    char* variable;
    char* value;

    if (argptr(0, &variable, 32) < 0 || argptr(1, &value, 128) < 0)
        return -1;

    return getVariable(variable, value);
}

int sys_remVariable(void){
    char* variable;

    if (argptr(0, &variable, 32) < 0 )
        return -1;

    return remVariable(variable);
}

int sys_wait2(void){
  int pid;
  int *wtime;
  int *rtime;
  int *iotime;

  if (argint(0, &pid)<0 || argptr(1, (void *)&wtime, sizeof(int*))<0 || argptr(2, (void *)&rtime, sizeof(int*))<0 || argptr(3, (void *)&iotime, sizeof(int*))<0)
    return -1;
  return wait2(pid, wtime, rtime, iotime);
}