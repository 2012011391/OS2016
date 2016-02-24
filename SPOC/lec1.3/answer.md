
分析和实验funcall.c，需要完成的内容包括： 
-[X]

 - 修改代码，可正常显示小组两位同学的学号（用字符串） 
 - 生成funcall.c的汇编码，理解其实现并给汇编码写注释

```c
root/usr/funcall.c  2: int ret;
root/usr/funcall.c  3: out(port, val)
root/usr/funcall.c  4: {
root/usr/funcall.c  5:   asm(LL,8);   // load register a with port
00000000  0000080e  LL    0x8 (D 8)
root/usr/funcall.c  6:   asm(LBL,16); // load register b with val
00000004  00001026  LBL   0x10 (D 16)
root/usr/funcall.c  7:   asm(BOUT);   // output byte to console
00000008  0000009a  BOUT
root/usr/funcall.c  8: }
root/usr/funcall.c  9: 
root/usr/funcall.c  10: int write(int f, char *s, int n)
0000000c  00000002  LEV   0x0 (D 0)
root/usr/funcall.c  11: {
root/usr/funcall.c  12:   int i;
root/usr/funcall.c  13:   ret = 1;
00000010  fffff801  ENT   0xfffffff8 (D -8)                 // SP-8
00000014  00000123  LI    0x1 (D 1)
00000018  00000045  SG    0x0 (D 0)                         // Ret地址在PC+0x84
root/usr/funcall.c  14:   i=n;
0000001c  0000200e  LL    0x20 (D 32)                       // SP + 0x20是n的地址
00000020  00000440  SL    0x4 (D 4)                         // SP+4是i的地址
root/usr/funcall.c  15:   while (i--)
00000024  00000003  JMP   <fwd>
root/usr/funcall.c  16:     out(f, *s++);
00000028  0000180e  LL    0x18 (D 24)                       // 字符串首地址
0000002c  ffffff57  SUBI  0xffffffff (D -1)                 // 字符串+1
00000030  00001840  SL    0x18 (D 24)                       // 写回
00000034  ffffff1f  LXC   0xffffffff (D -1)                 // 读出一个char
00000038  0000009d  PSHA                                    // 传入参数
0000003c  0000180e  LL    0x18 (D 24)                       // 读出port
00000040  0000009d  PSHA                                    // 传入参数
00000044  ffffb805  JSR   0xffffffb8 (TO 0x0)               // 调用函数
00000048  00001001  ENT   0x10 (D 16)                       // SP += 16
root/usr/funcall.c  17:   return i;
0000004c  0000040e  LL    0x4 (D 4)
00000050  00000157  SUBI  0x1 (D 1)
00000054  00000440  SL    0x4 (D 4)
00000058  00000154  ADDI  0x1 (D 1)
0000005c  00000086  BNZ   <fwd>
00000060  0000040e  LL    0x4 (D 4)
00000064  00000802  LEV   0x8 (D 8)
root/usr/funcall.c  18: }  
root/usr/funcall.c  19: 
root/usr/funcall.c  20: main()
00000068  00000802  LEV   0x8 (D 8)
root/usr/funcall.c  21: {
root/usr/funcall.c  22: 
root/usr/funcall.c  23:   //Change S1/S2 ID to your student ID, and change 12 to new str length
root/usr/funcall.c  24:   ret = write(1, "S1_ID S2_ID",12);
0000006c  00000c9e  PSHI  0xc (D 12)                    // 将12推入栈中
00000070  00000008  LEAG  0x0 (D 0)                     // 将字符串地址赋值A寄存器
00000074  0000009d  PSHA                                // 将A推入栈中
00000078  0000019e  PSHI  0x1 (D 1)                     // 将1推入栈中
0000007c  ffff9005  JSR   0xffffff90 (TO 0x10)          // 函数调用
00000080  00001801  ENT   0x18 (D 24)                   
00000084  00000045  SG    0x0 (D 0)
root/usr/funcall.c  25:   asm(HALT);                    // 停机
00000088  00000000  HALT
root/usr/funcall.c  26: }
root/usr/funcall.c  27: 
0000008c  00000002  LEV   0x0 (D 0)
```


 - 尝试用xem的简单调试功能单步调试代码
 - 回答如下问题：
   - funcall中的堆栈有多大？是内核态堆栈还是用户态堆栈
        - 124M
        - 内核态

   - funcall中的全局变量ret放在内存中何处？如何对它寻址？
        - 0xa0
        - pc + 0x84

   - funcall中的字符串放在内存中何处？如何对它寻址？
        - 0x90
        - pc + 0x1c

   - 局部变量i在内存中的何处？如何对它寻址？
        - 0x07BFFFF8
        - sp + 0x04

   - 当前系统是处于中断使能状态吗？
        - 不能, iena 为 0

   - funcall中的函数参数是如何传递的？函数返回值是如何传递的？
        - 参数从右向左依次压栈
        - SP+4

   - 分析并说明funcall执行文件的格式和内容
        - HDR + TEXT + DATA
