#lec 3 SPOC Discussion

## 3.1 BIOS

> 1. 比较UEFI和BIOS的区别

BIOS启动流程

- 系统开机 - 上电自检（Power On Self Test 或 POST）。
- POST过后初始化用于启动的硬件（磁盘、键盘控制器等）。
- BIOS会运行BIOS磁盘启动顺序中第一个磁盘的首440bytes（MBR启动代码区域）内的代码。
- 启动引导代码从BIOS获得控制权，然后引导启动下一阶段的代码（如果有的话）（一般是系统的启动引导代码）。
- 再次被启动的代码（二阶段代码）（即启动引导）会查阅支持和配置文件。根据配置文件中的信息，启动引导程序会将内核和initramfs文件载入系统的RAM中，然后开始启动内核。 

UEFI启动流程

- 系统开机 - 上电自检（Power On Self Test 或 POST）。
- UEFI 固件被加载，并由它初始化启动要用的硬件。
- 固件读取其引导管理器以确定从何处（比如，从哪个硬盘及分区）加载哪个 UEFI 应用。
- 固件按照引导管理器中的启动项目，加载UEFI 应用。已启动的 UEFI 应用还可以启动其他应用（对应于 UEFI shell 或 rEFInd 之类的引导管理器的情况）或者启动内核及initramfs（对应于GRUB之类引导器的情况），这取决于 UEFI 应用的配置。


> 2. 描述PXE的大致启动流程

- 客户端电脑开机后，IOS设置从网络启动
- 网卡中的PXE Boot ROM自测，获得控制权，然后发送一个动态获得IP地址的广播包。
- DHCP服务器在收到该广播包后，发送给客户端分配IP地址的DHCP回应包。内容包括客户端的IP地址，TFTP服务器的IP地址（DHCP服务器的066选项），预设通讯通道，及开机启动文件（DHCP服务器的067选项）。该文件应该是一种由PXE启动规范规定的固定格式的可执行文件，类似于Windows XP正常启动时显示多重启动菜单之前的启动代码。 
- 客户面收到DHCP回应后，则会响应一个FRAME，以请求传送启动文件。之后，服务端将和客户机再进行一系列应答，决定启动参数。
- 客户端通过TFTP通讯协议从服务器下载开机启动文件。启动文件接收完成后，将控制权转交给启动块，完成PXE启动。客户端通过这个开机影像文件开机，这个开机文件可以只是单纯的开机程式也可以是操作系统。如果是用3Com Boot Image Editor编辑的启动文件，系统将根据PXE文件中的代码决定是显示选择菜单还是直接下载预设的镜像文件启动电脑。

3.2 系统启动流程

> 1. 了解 NTLDR 的启动流程

- 访问启动盘的文件系统 (FAT 或者 NTFS)
- 如果 Windows 处于冬眠模式，将 `hiberfil.sys` 载入内存，系统恢复至冬眠前的状况
- 否则读取 `boot.ini`，显示启动菜单提示用户
- 如果用户选择了非 `NT-based` 的系统，NTLDR 加载 `boot.ini` 内列出的文件，交出控制权
- 如果选择 `NT-based` 的系统，NTLDR 运行 ｀ntdetect.com｀，获取电脑硬件信息
- 运行 `Ntoskrnl.exe`，把上面获得的信息提交给它

> 2. 了解 GRUB 的启动流程

- BIOS加载MBR里面的GRUB，由于只有GRUB只占用446字节所以不能实现太多的功能，所以就有此阶段里面的文件来加载下一阶段的文件（/boot/grub下的文件）

- 加载识别文件系统的程序，来识别文件系统，不加载就无法识别文件系统，进而就找不到boot目录，由于GRUB是无法识别LVM，所以不能把/boot分区设置为LVM，所以必须要把/boot单独分区

- 开始寻找内核，启动内核

> 3. 比较NTLDR和GRUB的功能有差异

- ntldr功能很少，只能引导win，只能装在硬盘；
- grub是第三方操作系统引导器，从多种设备引导，也可以引导多种操作系统

> 4. 了解u-boot的功能

U-Boot 是一个主要用于嵌入式系统的引导程序，可以支持多种不同的计算机系统结构，包括 PPC、ARM、AVR32、MIPS、x86、68k、Nios 与 MicroBlaz。U-Boot不仅仅支持嵌入式Linux系统的引导，它还支持NetBSD, VxWorks, QNX, RTEMS, ARTOS, LynxOS, android嵌入式操作系统。

拥有 高度灵活的功能设置，适合U-Boot调试、操作系统不同引导要求、产品发布等。有丰富的设备驱动源码，如串口、以太网、SDRAM、FLASH、LCD、NVRAM、EEPROM、RTC、键盘等。还有较为丰富的开发调试文档与强大的网络技术支持。

## 3.3 中断、异常和系统调用比较
> 举例说明Linux中有哪些中断，哪些异常？
> Linux的系统调用有哪些？大致的功能分类有哪些？  (w2l1)

Linux的系统调用大概有250个左右，其继承了UNIX系统调用中最基本和最有用的部分，按照功能逻辑大致可以分为如下几类：

1. 进程控制：创建、终止、挂起进程等基本操作以及进程信息的查询
2. 文件系统控制：打开、关闭、读写文件等基本操作以及更改文件属性等系统操作
3. 系统控制：系统内核相关信息的查询以及一些系统级别的控制操作
4. 内存管理：分配、释放内存、地址映射、缓冲写回等内存相关的操作
5. 网络管理：域名的查询和设置以及主机名称、标识号的查询与设置
6. socket控制：建立连接、发送消息、断开连接等一系列套接字操作
7. 用户管理：用户和组相关信息的设置和查询
8. 进程间通信：进程间的消息、信号、管道、共享内存等通信操作

> 以ucore lab8的answer为例，uCore的系统调用有哪些？大致的功能分类有哪些？(w2l1)

ucore lab8的answer中共有22个系统调用，大致分为如下几类

1. 进程管理：包括 fork/exit/wait/exec/yield/kill/getpid/sleep
2. 文件操作：包括 open/close/read/write/seek/fstat/fsync/getcwd/getdirentry/dup
3. 内存管理：pgdir命令
5. 外设输出：putc命令
 
## 3.4 linux系统调用分析

> 通过分析[lab1_ex0](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex0.md)了解Linux应用的系统调用编写和含义。(w2l1)
  
objdump的作用是将一个文件反汇编得到其各个程序段的汇编码，以该文件为例，使用-S参数后得到代码段.text的全部反汇编内容；

nm的作用是显示特定文件的符号信息，该程序的符号信息输出包括 `_start` 的地址 `0x080482f0` ， `SYS_write` 的值 `0x04` 等等
  
file的作用是显示指定文件的格式，该文件输出结果为：ELF格式32位小端序可执行文件，Intel 80386架构，动态链接等
  
 
 > 通过调试[lab1_ex1](https://github.com/chyyuu/ucore_lab/blob/master/related_info/lab1/lab1-ex1.md)了解Linux应用的系统调用执行过程。(w2l1)
 
Linux系统调用主要是通过 `int 0x80` 实现，通过寄存器 `eax` , `ebx` , `ecx` , `edx` 传入参数来，其中 `eax` 代表系统调用号，而 `ebx` 往后均为系统调用函数的参数

strace可以跟踪进程执行时的系统调用和所接收的信号，加上-c参数后还可以知道每个调用所花费的时间。

 
## 3.5 ucore系统调用分析
> 1. ucore的系统调用中参数传递代码分析。

在 ucore 中，执行系统调用前，需要将系统调用的参数出存在寄存器中。eax 表示参数个数，参数依次存在 edx, ecx, ebx, edi, esi 中。

```c
	...
    int num = tf->tf_regs.reg_eax;
    ... 
    arg[0] = tf->tf_regs.reg_edx; 
    arg[1] = tf->tf_regs.reg_ecx;
    arg[2] = tf->tf_regs.reg_ebx;
    arg[3] = tf->tf_regs.reg_edi;
    arg[4] = tf->tf_regs.reg_esi;
    ...
```
	
	
> 2. ucore的系统调用中返回结果的传递代码分析。

系统调用返回结果存在eax中。

```c
	tf->tf_regs.reg_eax = syscalls[num](arg);
```

> 3. 以ucore lab8的answer为例，分析 ucore 应用的系统调用编写和含义。



> 4. 以ucore lab8的answer为例，尝试修改并运行ucore OS kernel代码，使其具有类似Linux应用工具`strace`的功能，即能够显示出应用程序发出的系统调用，从而可以分析ucore应用的系统调用执行过程。
 

 
 
## 3.6 请分析函数调用和系统调用的区别
 1. 请从代码编写和执行过程来说明。
   1. 说明`int`、`iret`、`call`和`ret`的指令准确功能


### v9-cpu系统调用实现
> v9-cpu中os4.c的系统调用中参数传递代码分析。

在os4中，用户态程序通过 `write()` 这个API函数来实现输出的操作，而这个函数是通过系统调用来实现的。

```c
  asm(LL,8); asm(LBL,16); asm(LCL,24); asm(TRAP,S_write);
```
     
`write` 函数将寄存器a,b,c分别设置为三个传入参数，然后触发系统调用，在 `alltraps` 函数中，做好堆栈的处理，进入中断向量 `trap` 函数，其中将这三个参数传递给 `sys_write` 这个内核函数，在内核态完成输出，最终返回用户态。
    
> v9-cpu中os4.c的系统调用中返回结果的传递代码分析。

 由
 
 ```c
   case S_write: a = sys_write(a, b, c); break;
 ```
 可知 `sys_write`函数的返回值存放在了 `a` 这一个变量所在的位置，而这个变量在 `alltraps` 中通过倒数第二句 `POPA` 将寄存器a设置为a变量的值，即函数返回值是通过寄存器a实现的。
    
> 理解v9-cpu中os4.c的系统调用编写和含义。

在os4.c中，用户态 `task0` 执行的时候，需要使用操作系统提供的输出接口，而这首先是通过一个用户态API函数调用 `write()` ；接着该函数进行系统调用，并传入必要的参数，进入内核态；在内核态中的中断处理程序过程中，系统得到 `write` 函数传入的参数，再通过内核态的函数 `sys_write` 来进行输出。
    
总之，用户程序通过API函数调用、再到系统调用、再到内核态函数调用，完成了用户程序需要的输出功能。
