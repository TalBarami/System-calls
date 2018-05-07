#include "types.h"
#include "x86.h"
#include "defs.h"
#include "date.h"
#include "param.h"
#include "memlayout.h"
#include "mmu.h"
#include "proc.h"

int sys_setVariable(void){
	char *key, *val;
	if(argstr(0, &key) < 0 || argstr(1, &val) < 0){
	  return -2;
	}
	return setVariable(key, val);
}

int sys_getVariable(void){
	char *key, *dst;

	if(argstr(0, &key) < 0 || argstr(1, &dst) < 0){
	  return -1;
	}

	return getVariable(key, dst);
}

int sys_remVariable(void){
	char *key;

	if(argstr(0, &key) < 0){
	  return -1;
	}

	return remVariable(key);
}

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

// Task 2:
int sys_wait2(void){
  int pid;
  int *wtime, *rtime, *iotime;

  if(argint(0, &pid) < 0 || 
	 argptr(1,(void*)&wtime, sizeof(wtime)) < 0 ||
	 argptr(2,(void*)&rtime, sizeof(rtime)) < 0 ||
	 argptr(3,(void*)&iotime, sizeof(iotime)) < 0){
	  return -1;
  }
	  
  return wait2(pid, wtime, rtime, iotime);
}

// Task 3:
int sys_set_priority(void){
  int priority;

  if(argint(0, &priority) < 0 || priority < 1 || priority > 3){
	  return -1;
  }
	  
  return set_priority(priority);
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
