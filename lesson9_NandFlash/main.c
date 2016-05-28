int gboot_main()
{
#ifdef MMU_ON
	mmu_start()
#endif
	unsigned char buf[2048];
	
	led_init();
	key_init();
	init_irq();

	NF_Page_Erase(5*128+1);
	
	buf[0] = 100;
	NF_Page_Write(5*128+1, buf, 2048);
	
	buf[0] = 10;
	
	NF_Page_Read(5*128+1, buf, 2048);
	if(buf[0] == 100)
		led_on();
	else if(buf[0] == 10)
		led_off();
	else
		;
	
	while(1);
		
	return 0;
}