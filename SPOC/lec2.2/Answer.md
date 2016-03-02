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

---

> 比较不同特权级的中断切换时的堆栈变化差别；(challenge)写出一些简单的小程序（c or asm）来显示出不同特权级的的中断切换的堆栈变化情况。


在用户态触发中断时，用户态切换到内核态，ESP栈指针从用户态指向了内核态中断服务程序的栈空间中，接着依次压栈SS/ESP/EFLAGS/CS/EIP/Error Code，完成栈的建立，接下来进入正常的中断处理程序。

当在内核态触发中断时，ESP指针没有发生跳转，没有切换堆栈，仅仅压入EFLAGS/CS/EIP/Error Code。

Challenge做法如下：

利用 `lab1_switch_test()` 转换用户态和内核态，同时添加打印栈的代码，输出结果如下

用户态 -> 核心态:

	// 触发中断前：用户态
	ebp:0x0010fcd8 eip:0x00100a47 args:0x00000000 0x00000000 0x00000000 0x00000000 
	   kern/debug/kdebug.c:305: print_stackframe+21
	ebp:0x0010fd08 eip:0x00101e56 args:0x0010fd34 0x00000000 0x00000000 0x00000000 
	   kern/trap/trap.c:201: trap_dispatch+493
	ebp:0x0010fd28 eip:0x00101eb2 args:0x00007b74 0x00000000 0x00010094 0x00007bb8 
           kern/trap/trap.c:225: trap+16
	ebp:0x00007bb8 eip:0x0010293e args:0x0010e560 0x00000100 0x00007be8 0x0010006e 
           kern/trap/trapentry.S:24: <unknown>+0

	// 触发中断后：核心态
	ebp:0x00007bb8 eip:0x00100a47 args:0x0010e560 0x00000100 0x00007be8 0x0010006e 
	    kern/debug/kdebug.c:305: print_stackframe+21
	ebp:0x00007bc8 eip:0x0010020d args:0x00000000 0x00000000 0x00000000 0x00103560 
	    kern/init/init.c:119: lab1_switch_test+78
	ebp:0x00007be8 eip:0x0010006e args:0x00000000 0x00000000 0x00000000 0x00007c4f 
	    kern/init/init.c:40: kern_init+109
	ebp:0x00007bf8 eip:0x00007d72 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
	    <unknow>: -- 0x00007d71 --

	
	
核心态 -> 核心态:

	// 触发中断前
	ebp:0x00007bb8 eip:0x00100a47 args:0x0010e560 0x00000100 0x00007be8 0x0010006e 
	    kern/debug/kdebug.c:305: print_stackframe+21
	ebp:0x00007bc8 eip:0x001001d9 args:0x00000000 0x00000000 0x00000000 0x00103560 
	    kern/init/init.c:111: lab1_switch_test+26
	ebp:0x00007be8 eip:0x0010006e args:0x00000000 0x00000000 0x00000000 0x00007c4f 
	    kern/init/init.c:40: kern_init+109
	ebp:0x00007bf8 eip:0x00007d72 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
	    <unknow>: -- 0x00007d71 --

	// 触发中断后
	ebp:0x00007b10 eip:0x00100a47 args:0x00007b48 0x00100255 0x00100210 0x00007b3c 
	    kern/debug/kdebug.c:305: print_stackframe+21
	ebp:0x00007b40 eip:0x00101dbe args:0x00007b6c 0x00100255 0x00100210 0x00007b6c 
	    kern/trap/trap.c:182: trap_dispatch+341
	ebp:0x00007b60 eip:0x00101ead args:0x00007b6c 0x00000000 0x00010094 0x00007bb8 
	    kern/trap/trap.c:224: trap+16
	ebp:0x00007bb8 eip:0x00102939 args:0x0010e560 0x00000100 0x00007be8 0x0010006e 
	    kern/trap/trapentry.S:24: <unknown>+0
	ebp:0x00007bc8 eip:0x001001ee args:0x00000000 0x00000000 0x00000000 0x00103560 
	    kern/init/init.c:113: lab1_switch_test+47
	ebp:0x00007be8 eip:0x0010006e args:0x00000000 0x00000000 0x00000000 0x00007c4f 
	    kern/init/init.c:40: kern_init+109
	ebp:0x00007bf8 eip:0x00007d72 args:0xc031fcfa 0xc08ed88e 0x64e4d08e 0xfa7502a8 
	    <unknow>: -- 0x00007d71 --
