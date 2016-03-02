>描述符特权级DPL、当前特权级CPL和请求特权级RPL的含义是什么？在哪些寄存器中这些字段？对应的访问条件是什么？ (challenge)写出一些简单的小程序（c or asm）来体现这些特权级的区别和联系。

- DPL：存储在段描述符中，规定访问该段的权限级别(Descriptor Privilege Level)，存在每个段的DPL固定两位。
- CPL：是当前正在执行的代码所在的段的特权级，存在于cs寄存器的低两位。
- RPL：说明的是进程对段访问的请求权限，存在于DS,ES,SS,FS,GS寄存器的最后两文，

访问一个段的条件为
	
	max {CPL, RPL} ≤ DPL

Challenge做法如下：

- 增加用户程序 printPL.c，输出用户态的CPL

```c
#include <ulib.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

int get_cpl() {
	int ret;
	asm volatile ("movl %%cs, %0"
			: "=r" (ret));
}

int main(void) {
	int cpl;
	cpl = get_cpl() & 3;
	cprintf("In user mode, CPL = %d\n", cpl);
}

```

- 在 syscall.c 中添加代码输出核心态的CPL

```c

int get_cpl() {
	int ret;
	asm volatile ("movl %%cs, %0"
			: "=r" (ret));
}

void
syscall() {
    static int firstinto = 1;
    if (firstinto) {
    	firstinto = 0;
       cprintf("In kernel mode, cpl = %d\n", get_cpl() & 3);
    ...
    }
}

```

> 比较不同特权级的中断切换时的堆栈变化差别；(challenge)写出一些简单的小程序（c or asm）来显示出不同特权级的的中断切换的堆栈变化情况。


在用户态触发中断时，用户态切换到内核态，ESP栈指针从用户态指向了内核态中断服务程序的栈空间中，接着依次压栈SS/ESP/EFLAGS/CS/EIP/Error Code，完成栈的建立，接下来进入正常的中断处理程序。

当在内核态触发中断时，ESP指针没有发生跳转，没有切换堆栈，仅仅压入EFLAGS/CS/EIP/Error Code。

