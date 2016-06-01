int gboot_main()
{
#ifdef MMU_ON
	mmu_start()
#endif

	int num;
	unsigned char buff[2048];
	
	led_init();
	key_init();
	init_irq();
	
	uart_init();
	
	NF_Page_Erase(2048*135+1);
	
	buff[0] = 100;
	NF_Page_Write(2048*135+1, buff, 2048);
	
	buff[0] = 10;
	NF_Page_Read(2048*135+1, buff, 2048);
	printf("%d\n", buff[0]);
	
	led_on();

	while(1)
	{
				printf("\n\r#######################################\n\r");
	    	printf("\n\r################TINY6410###############\n\r");
	    	printf("1:Download Linux Kernel from TFTP Server!\n\r");
	    	printf("2:Boot Linux from RAM!\n\r");
	    	printf("3:Boor Linux from Nand Flash!\n\r");
	    	printf("\n Plese Select:");
	    	
	    	scanf("%d",&num);
	    
	        switch (num)
	        {
	            case 1:
	            //tftp_load();
	            break;
	            
	            case 2:
	            //boot_linux_ram();
	            break;
	            
	            case 3:
	            //boot_linux_nand();
	            break;
	            
	            default:
	                printf("Error: wrong selection!\n\r");
	            break;	
					}
	}
		
	return 0;
}