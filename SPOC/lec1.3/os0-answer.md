分析和实验os0.c，需要完成的内容包括： 

 - 生成os0.c的汇编码，理解其实现并给汇编码写注释
 
 ```asm
root/usr/os/os0.c  1: // os0.c -- simple timer isr test
root/usr/os/os0.c  2: 
root/usr/os/os0.c  3: #include <u.h>
root/usr/os/os0.c  4: 
root/usr/os/os0.c  5: int current;
root/usr/os/os0.c  6: 
root/usr/os/os0.c  7: out(port, val)  { asm(LL,8); asm(LBL,16); asm(BOUT); }
00000000  0000080e  LL    0x8 (D 8)
00000004  00001026  LBL   0x10 (D 16)
00000008  0000009a  BOUT
root/usr/os/os0.c  8: ivec(void *isr) { asm(LL,8); asm(IVEC); }
0000000c  00000002  LEV   0x0 (D 0)
00000010  0000080e  LL    0x8 (D 8)
00000014  000000a4  IVEC
root/usr/os/os0.c  9: stmr(int val)   { asm(LL,8); asm(TIME); }
00000018  00000002  LEV   0x0 (D 0)							
0000001c  0000080e  LL    0x8 (D 8)							// 设置寄存器A为val
00000020  000000a7  TIME									// 设置时钟中断阈值
root/usr/os/os0.c  10: halt(val)       { asm(LL,8); asm(HALT); }
00000024  00000002  LEV   0x0 (D 0)							// 返回
00000028  0000080e  LL    0x8 (D 8)							// 设置寄存器a为val
0000002c  00000000  HALT
root/usr/os/os0.c  11: 
root/usr/os/os0.c  12: alltraps()
00000030  00000002  LEV   0x0 (D 0)
root/usr/os/os0.c  13: {
root/usr/os/os0.c  14:   asm(PSHA);
00000034  0000009d  PSHA									// a寄存器压栈
root/usr/os/os0.c  15:   asm(PSHB);
00000038  000000a0  PSHB									// b寄存器压栈
root/usr/os/os0.c  16: 
root/usr/os/os0.c  17:   current++;
0000003c  00000015  LG    0x0 (D 0)							// 取出全局变量current到寄存器a
00000040  ffffff57  SUBI  0xffffffff (D -1)					// current++
00000044  00000045  SG    0x0 (D 0)							// 写回current
root/usr/os/os0.c  18: 
root/usr/os/os0.c  19:   asm(POPB);
00000048  000000a1  POPB									// b寄存器弹出
root/usr/os/os0.c  20:   asm(POPA);
0000004c  000000a3  POPA									// a寄存器弹出
root/usr/os/os0.c  21:   asm(RTI);
00000050  00000098  RTI 									// 中断处理返回
root/usr/os/os0.c  22: }
root/usr/os/os0.c  23: 
root/usr/os/os0.c  24: main()
00000054  00000002  LEV   0x0 (D 0)
root/usr/os/os0.c  25: {
root/usr/os/os0.c  26:   current = 0;
00000058  00000023  LI    0x0 (D 0)							// 设置寄存器a为0
0000005c  00000045  SG    0x0 (D 0)							// 设置全局变量current为0
root/usr/os/os0.c  27: 
root/usr/os/os0.c  28:   stmr(1000);
00000060  0003e89e  PSHI  0x3e8 (D 1000)					// 参数1000压栈
00000064  ffffb405  JSR   0xffffffb4 (TO 0x1c)				// 函数调用
00000068  00000801  ENT   0x8 (D 8)							// 恢复栈
root/usr/os/os0.c  29:   ivec(alltraps);
0000006c  ffffc408  LEAG  0xffffffc4 (D -60)				// 设置寄存器A为alltraps地址
00000070  0000009d  PSHA									// 参数alltraps压栈
00000074  ffff9805  JSR   0xffffff98 (TO 0x10)				// 函数调用
00000078  00000801  ENT   0x8 (D 8)							// 恢复栈
root/usr/os/os0.c  30:   
root/usr/os/os0.c  31:   asm(STI);
0000007c  00000097  STI 									// 打开中断使能
root/usr/os/os0.c  32:   
root/usr/os/os0.c  33:   while (current < 10) {
00000080  00000003  JMP   <fwd>
root/usr/os/os0.c  34:     if (current & 1) out(1, '1'); else out(1, '0');
00000084  00000015  LG    0x0 (D 0)							// 加载current到寄存器a
00000088  00000169  ANDI  0x1 (D 1)							// current & 1
0000008c  00000084  BZ    <fwd>
00000090  0000319e  PSHI  0x31 (D 49)						// 参数'1'压栈
00000094  0000019e  PSHI  0x1 (D 1)							// 参数1压栈
00000098  ffff6405  JSR   0xffffff64 (TO 0x0)				// 调用out函数
0000009c  00001001  ENT   0x10 (D 16)						// 恢复栈
000000a0  00000003  JMP   <fwd>
000000a4  0000309e  PSHI  0x30 (D 48)						// 参数'0'压栈
000000a8  0000019e  PSHI  0x1 (D 1)							// 参数1压栈
000000ac  ffff5005  JSR   0xffffff50 (TO 0x0)				// 调用out函数
000000b0  00001001  ENT   0x10 (D 16)						// 恢复栈
root/usr/os/os0.c  35:   }
root/usr/os/os0.c  36: 
root/usr/os/os0.c  37:   halt(0);
000000b4  00000015  LG    0x0 (D 0)
000000b8  00000a3b  LBI   0xa (D 10)						// 加载10到b寄存器
000000bc  0000008c  BLT   <fwd>								// 条件跳转
000000c0  0000009e  PSHI  0x0 (D 0)							// 参数0压栈
000000c4  ffff6005  JSR   0xffffff60 (TO 0x28)				// 调用Halt函数
000000c8  00000801  ENT   0x8 (D 8)
root/usr/os/os0.c  38: }
root/usr/os/os0.c  39: 
000000cc  00000002  LEV   0x0 (D 0)
 ```
 
 - 回答如下问题：
   - 何处设置的中断使能？   
   - 系统何时处于中断屏蔽状态？
   - 如果系统处于中断屏蔽状态，如何让其中断使能？
   - 系统产生中断后，CPU会做哪些事情？（在没有软件帮助的情况下）
   - CPU执行RTI指令的具体完成工作是哪些？
