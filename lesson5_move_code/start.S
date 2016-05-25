.text
.global _start
_start:
		b	reset						
		ldr	pc, _undefined_instruction	
		ldr	pc, _software_interrupt		
		ldr	pc, _prefetch_abort			
		ldr	pc, _data_abort				
		ldr	pc, _not_used				
		ldr	pc, _irq					
		ldr	pc, _fiq

_undefined_instruction:	.word undefined_instruction
_software_interrupt:	.word software_interrupt
_prefetch_abort:	.word prefetch_abort
_data_abort:	.word data_abort
_not_used:	.word not_used
_irq:		.word irq
_fiq:		.word fiq

undefined_instruction:
		nop
	
software_interrupt:
		nop

prefetch_abort:
		nop

data_abort:
		nop

not_used:
		nop

irq:
		nop

fiq:
		nop
	
reset:
		bl set_svc
		bl set_peri_port
		bl disable_watchdog
		bl disable_interrupt
		bl disable_mmu
		bl clock_init
		bl mem_ctrl_asm_init
		bl copy_to_ram
		bl light_led
		
set_svc:
		mrs r0, cpsr
		bic r0, r0, #0x1f
		orr r0, r0, #0xd3
		msr cpsr, r0
		mov pc, lr

set_peri_port:
	ldr r0, =0x70000000
	orr r0, r0, #0x13
	mcr p15,0,r0,c15,c2,4
	mov pc, lr

#define pWTCON 0x7e004000
disable_watchdog:
		ldr r0, =pWTCON
		mov r1, #0x0
		str r1, [r0]
		mov pc, lr
		
disable_interrupt:
		ldr r0, =0x71200014
		mvn r1, #0x0
		str r1, [r0]
		
		ldr r0, =0x71300014
		str r1, [r0]
		mov pc, lr

disable_mmu:
	mcr p15,0,r0,c7,c7,0
	mrc p15,0,r0,c1,c0,0
	bic r0, r0, #0x00000007
	mcr p15,0,r0,c1,c0,0
	mov pc, lr

#define CLK_DIV0 0x7E00F020
#define OTHERS   0x7E00F900
#define APLL_CON 0x7E00F00C
#define MPLL_CON 0x7E00F010
#define CLK_SRC  0x7E00F01C

#define CLK_DIV_VAL	((3<<12) | (2<<9) | (1<<8) | (1<<4) | 0)
#define PLL_VAL	    ((1<<31) | (266<<16) | (3<<8) | (1<<0))
clock_init:
	ldr r0, =CLK_DIV0        @ 从6410—uboot中得到，分频系数
	ldr r1, =CLK_DIV_VAL
	str r1, [r0]
	
	ldr r0, =OTHERS          @ 设置CPU工作在异步模式以及选择后续时钟的产生源
	ldr r1, [r0]
	bic r1, r1, #0xc0
	str r1, [r0]
	
	ldr r0, =APLL_CON        @ 设置MPLL和APLL的时钟，both of 533MHZ
	ldr r1, =MPLL_CON
	ldr r2, =PLL_VAL
	str r2, [r0]
	str r2, [r1]
	
	ldr r0, =CLK_SRC         @ CLK_SRC一个选择器（从时钟初始化流程图得）
	mov r1, #0x03
	str r1, [r0]
	mov pc, lr

copy_to_ram:
	ldr r0, =0x0C000000
	ldr r1, =0x50008000
	add r3, r0, #1024*8
	
copy_loop:
	ldr r2, [r0], #4
	str r2, [r1], #4
	cmp r0, r3
	bne copy_loop
	
	mov pc, lr

	
@  GPK4:1 GPK5:2 GPK6:3 GPK7:4 
#define GPKCON0 0x7F008800
#define GPKDAT  0x7F008808
light_led:
	ldr r0, =GPKCON0
	ldr r1, =0x11110000
	str r1, [r0]
	
	ldr r0, =GPKDAT
	mov r1, #0x0
	str r1, [r0]
	mov pc, lr
