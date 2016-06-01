.text
.global mem_ctrl_asm_init

#define ELFIN_DMC1_BASE		0x7e001000
#define ELFIN_MEM_SYS_CFG	0x7e00f120

#define INDEX_DMC_MEMC_STATUS   (0x00)
#define INDEX_DMC_MEMC_CMD      (0x04)
#define INDEX_DMC_DIRECT_CMD    (0x08)
#define INDEX_DMC_MEMORY_CFG    (0x0C)
#define INDEX_DMC_REFRESH_PRD   (0x10)
#define INDEX_DMC_CAS_LATENCY   (0x14)
#define INDEX_DMC_T_DQSS        (0x18)
#define INDEX_DMC_T_MRD         (0x1C)
#define INDEX_DMC_T_RAS         (0x20)
#define INDEX_DMC_T_RC          (0x24)
#define INDEX_DMC_T_RCD         (0x28)
#define INDEX_DMC_T_RFC         (0x2C)
#define INDEX_DMC_T_RP          (0x30)
#define INDEX_DMC_T_RRD         (0x34)
#define INDEX_DMC_T_WR          (0x38)
#define INDEX_DMC_T_WTR         (0x3C)
#define INDEX_DMC_T_XP          (0x40)
#define INDEX_DMC_T_XSR         (0x44)
#define INDEX_DMC_T_ESR         (0x48)
#define INDEX_DMC_MEMORY_CFG2	(0x4C)
#define INDEX_DMC_CHIP_0_CFG    (0x200)
#define INDEX_DMC_CHIP_1_CFG    (0x204)
#define INDEX_DMC_CHIP_2_CFG    (0x208)
#define INDEX_DMC_CHIP_3_CFG    (0x20C)
#define INDEX_DMC_USER_STATUS	(0x300)
#define INDEX_DMC_USER_CONFIG	(0x304)

#define DMC1_MEM_CFG		0x00010012	/* Supports one CKE control, Chip1, Burst4, Row/Column bit */
#define DMC1_MEM_CFG2		0xB45
#define DMC1_CHIP0_CFG		0x150F8
#define DMC_DDR_32_CFG		0x0 		/* 32bit, DDR */

#define DMC_NOP0 			0x0c0000
#define DMC_PA0 			0x000000	//Precharge all
#define DMC_AR0 			0x040000	//Autorefresh
#define DMC_mDDR_MR0			0x080032	//CAS 3, Burst Length 4
#define DMC_mDDR_EMR0			0x0a0000	// DS:Full, PASR:Full Array


mem_ctrl_asm_init:
	ldr	r0, =ELFIN_MEM_SYS_CFG			@ Memory sussystem address 0x7e00f120
	mov	r1, #0xd				            @ Xm0CSn2 = NFCON CS0, Xm0CSn3 = NFCON CS1
	str	r1, [r0]                    @ 这里面还设置了引脚功能

	ldr	r0, =ELFIN_DMC1_BASE			@DMC1 base address 0x7e001000

	ldr	r1, =0x04
	str	r1, [r0, #INDEX_DMC_MEMC_CMD]

	ldr r1, =( ( 7800 / ( 1000000000/133000000 ) + 1 ) )  @设置刷新时间
	str	r1, [r0, #INDEX_DMC_REFRESH_PRD]

	mov	r1, #(3 << 1)
	str	r1, [r0, #INDEX_DMC_CAS_LATENCY]

	mov	r1, #1
	str	r1, [r0, #INDEX_DMC_T_DQSS]

	mov	r1, #2
	str	r1, [r0, #INDEX_DMC_T_MRD]

	ldr	r1, =( ( 45 / ( 1000000000 / 133000000 ) + 1 ) ) @t_RAS寄存器
	str	r1, [r0, #INDEX_DMC_T_RAS]

	ldr	r1, =( ( 68 / ( 1000000000 / 133000000 ) + 1 ) ) @t_RC寄存器
	str	r1, [r0, #INDEX_DMC_T_RC]

	ldr	r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_RCD]

	ldr	r1, =( ( 80 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_RFC]

	ldr	r1, =( ( 23 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_RP]

	ldr	r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_RRD]

	ldr	r1, =( ( 15 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_WR]

	mov r1, #0x07
	str	r1, [r0, #INDEX_DMC_T_WTR]

	mov r1, #0x02
	str	r1, [r0, #INDEX_DMC_T_XP]

	ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_XSR]

	ldr r1, =( ( 120 / ( 1000000000 / 133000000 ) + 1 ) )
	str	r1, [r0, #INDEX_DMC_T_ESR]

	ldr	r1, =DMC1_MEM_CFG
	str	r1, [r0, #INDEX_DMC_MEMORY_CFG]

	ldr	r1, =DMC1_MEM_CFG2
	str	r1, [r0, #INDEX_DMC_MEMORY_CFG2]

	ldr	r1, =DMC1_CHIP0_CFG
	str	r1, [r0, #INDEX_DMC_CHIP_0_CFG]

	ldr	r1, =DMC_DDR_32_CFG
	str	r1, [r0, #INDEX_DMC_USER_CONFIG]

	@ 开始初始化DDR
	@DMC0 DDR Chip 0 configuration direct command reg
	ldr	r1, =DMC_NOP0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Precharge All
	ldr	r1, =DMC_PA0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Auto Refresh	2 time
	ldr	r1, =DMC_AR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@MRS
	ldr	r1, =DMC_mDDR_EMR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Mode Reg
	ldr	r1, =DMC_mDDR_MR0
	str	r1, [r0, #INDEX_DMC_DIRECT_CMD]

	@Enable DMC1
	mov	r1, #0x0
	str	r1, [r0, #INDEX_DMC_MEMC_CMD]

check_dmc1_ready:
	ldr	r1, [r0, #INDEX_DMC_MEMC_STATUS]
	mov	r2, #0x3
	and	r1, r1, r2
	cmp	r1, #0x1
	bne	check_dmc1_ready
	nop
	mov	pc, lr