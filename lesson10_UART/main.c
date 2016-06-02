int gboot_main()
{
#ifdef MMU_ON
	mmu_start()
#endif
	unsigned char buf[2048];
	
	led_init();
	key_init();
	init_irq();
	
	uart_init();
	putc('H');
	
	while(1)
		getc();
		
	return 0;
}