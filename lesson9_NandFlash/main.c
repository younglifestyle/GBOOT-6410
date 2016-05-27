int gboot_main()
{
#ifdef MMU_ON
	mmu_start()
#endif

	led_init();
	key_init();
	init_irq();
	
	led_on();
	while(1);
		
	return 0;
}