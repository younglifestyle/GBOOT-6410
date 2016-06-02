int gboot_main()
{
#ifdef MMU_ON
	mmu_start()
#endif
	void (*theKernel)(int zero, int arch, uint params);
	
	led_init();
	key_init();
	init_irq();
	
	uart_init();
	
	led_on();
	
	/* ¸´ÖÆÄÚºË */
	NF_Page_Read();
	
	
	theKernel = (void (*)(int, int, unsigned int))0x51000000;
	
	
		
	return 0;
}