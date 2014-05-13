
#include <linux/errno.h>
#include <linux/sched.h>
#include <linux/mm.h>
#include <linux/fs.h>
#include <linux/smp.h>
#include <linux/sem.h>
#include <linux/msg.h>
#include <linux/shm.h>
#include <linux/stat.h>
#include <linux/syscalls.h>
#include <linux/mman.h>
#include <linux/file.h>
#include <linux/utsname.h>
#include <linux/ipc.h>

#include <linux/uaccess.h>
#include <linux/unistd.h>

#include <asm/syscalls.h>

int kernel_execve(const char *filename,
		  const char *const argv[],
		  const char *const envp[])
{
	long __res;
	asm volatile ("int $0x80"
	: "=a" (__res)
	: "0" (__NR_execve), "b" (filename), "c" (argv), "d" (envp) : "memory");
	return __res;
}
