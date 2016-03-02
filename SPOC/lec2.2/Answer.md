>描述符特权级DPL、当前特权级CPL和请求特权级RPL的含义是什么？在哪些寄存器中这些字段？对应的访问条件是什么？ (challenge)写出一些简单的小程序（c or asm）来体现这些特权级的区别和联系。

- DPL：存储在段描述符中，规定访问该段的权限级别(Descriptor Privilege Level)，存在每个段的DPL固定两位。
- CPL：是当前正在执行的代码所在的段的特权级，存在于cs寄存器的低两位。
- RPL：说明的是进程对段访问的请求权限，存在于DS,ES,SS,FS,GS寄存器的最后两文，

访问一个段的条件为
	
	max {CPL, RPL} ≤ DPL

