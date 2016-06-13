#pragma once

#include "../stm32f1/feature.hxx"
#include "../stm32f1/metacpp.hxx"

#define ALLOCATE_STACK(BytesCount) \
extern "C" { \
  __attribute__((section("STACK"),zero_init,aligned(4),used)) uint8_t __stack[BytesCount]; \
  __attribute__((section("STACK"),zero_init,aligned(4),used)) uint8_t __initial_sp; \
}

#define ALLOCATE_HEAP(BytesCount) \
extern "C" { \
  __attribute__((section("HEAP"),zero_init,aligned(4),used)) uint8_t __heap_base[BytesCount]; \
  __attribute__((section("HEAP"),zero_init,aligned(4),used)) uint8_t __heap_limit; \
}

extern "C" {
	int __main();
	extern uint8_t __initial_sp;
}

namespace stm32f1
{
  template < int crystal_Mhz > struct HSE_MHZ { constexpr static int mhz = crystal_Mhz; };

  /* supported ISRs */
	constexpr uint64_t IRQ_FLAG_BIT		  = 1;
  constexpr uint64_t IRQ_NONE         = 0;
  constexpr uint64_t IRQ_ADC          = IRQ_FLAG_BIT << 0;
  constexpr uint64_t IRQ_RTC          = IRQ_FLAG_BIT << 1;
  constexpr uint64_t IRQ_SPI_1        = IRQ_FLAG_BIT << 2;
  constexpr uint64_t IRQ_SPI_2        = IRQ_FLAG_BIT << 3;
  constexpr uint64_t IRQ_USB_FS       = IRQ_FLAG_BIT << 4;
  constexpr uint64_t IRQ_I2C_1        = IRQ_FLAG_BIT << 5;
  constexpr uint64_t IRQ_I2C_2        = IRQ_FLAG_BIT << 6;
  constexpr uint64_t IRQ_FLASH        = IRQ_FLAG_BIT << 7;
  constexpr uint64_t IRQ_WATCHDOG     = IRQ_FLAG_BIT << 8;
  constexpr uint64_t IRQ_RCC          = IRQ_FLAG_BIT << 9;
  constexpr uint64_t IRQ_TIMER_1      = IRQ_FLAG_BIT << 10;
  constexpr uint64_t IRQ_TIMER_2      = IRQ_FLAG_BIT << 11;
  constexpr uint64_t IRQ_TIMER_3      = IRQ_FLAG_BIT << 12;
  constexpr uint64_t IRQ_TIMER_4      = IRQ_FLAG_BIT << 13;
  constexpr uint64_t IRQ_DMA_1        = IRQ_FLAG_BIT << 14;
  constexpr uint64_t IRQ_DMA_2        = IRQ_FLAG_BIT << 15;
  constexpr uint64_t IRQ_DMA_3        = IRQ_FLAG_BIT << 16;
  constexpr uint64_t IRQ_DMA_4        = IRQ_FLAG_BIT << 17;
  constexpr uint64_t IRQ_DMA_5        = IRQ_FLAG_BIT << 18;
  constexpr uint64_t IRQ_DMA_6        = IRQ_FLAG_BIT << 19;
  constexpr uint64_t IRQ_DMA_7        = IRQ_FLAG_BIT << 20;
  constexpr uint64_t IRQ_USART_1      = IRQ_FLAG_BIT << 21;
  constexpr uint64_t IRQ_USART_2      = IRQ_FLAG_BIT << 22;
  constexpr uint64_t IRQ_USART_3      = IRQ_FLAG_BIT << 23;
  constexpr uint64_t IRQ_EXTI_0       = IRQ_FLAG_BIT << 24;
  constexpr uint64_t IRQ_EXTI_1       = IRQ_FLAG_BIT << 25;
  constexpr uint64_t IRQ_EXTI_2       = IRQ_FLAG_BIT << 26;
  constexpr uint64_t IRQ_EXTI_3       = IRQ_FLAG_BIT << 27;
  constexpr uint64_t IRQ_EXTI_4       = IRQ_FLAG_BIT << 28;
  constexpr uint64_t IRQ_EXTI_5_9     = IRQ_FLAG_BIT << 39;
  constexpr uint64_t IRQ_EXTI_10_15   = IRQ_FLAG_BIT << 30;

  constexpr uint64_t IRQ_SYS_TICK     = IRQ_FLAG_BIT << 50;
  constexpr uint64_t IRQ_NMI          = IRQ_FLAG_BIT << 51;
  constexpr uint64_t IRQ_HARD_FAULT   = IRQ_FLAG_BIT << 53;
  constexpr uint64_t IRQ_MEM_MANAGE   = IRQ_FLAG_BIT << 54;
  constexpr uint64_t IRQ_BUS_FAULT    = IRQ_FLAG_BIT << 55;
  constexpr uint64_t IRQ_USAGE_FAULT  = IRQ_FLAG_BIT << 56;
  constexpr uint64_t IRQ_SVC          = IRQ_FLAG_BIT << 57;
  constexpr uint64_t IRQ_DEBUG_MON    = IRQ_FLAG_BIT << 58;
  constexpr uint64_t IRQ_PEND_SV      = IRQ_FLAG_BIT << 59;

  template < int leg_number, int port, int channel >
  struct leg_def
  {
    using index = mp::number<leg_number>;
    static constexpr int gpio_port = port;
    static constexpr int gpio_channel = channel;
    static constexpr int no = leg_number;
  };

  template<int leg_number, int af_remap>
  struct leg_af_remap
  {
    using index = mp::number<leg_number>;
    static constexpr int no = leg_number;
    static constexpr int remap = af_remap;
  };

  template<int port>   decltype(GPIOA) _gpio_ctl(); // yep, GPIOA.
  template<> constexpr decltype(GPIOA) _gpio_ctl<'A'>() { return GPIOA; }
  template<> constexpr decltype(GPIOB) _gpio_ctl<'B'>() { return GPIOB; }
  template<> constexpr decltype(GPIOC) _gpio_ctl<'C'>() { return GPIOC; }
  template<> constexpr decltype(GPIOD) _gpio_ctl<'D'>() { return GPIOD; }
  template<> constexpr decltype(GPIOE) _gpio_ctl<'E'>() { return GPIOE; }

  template<int port>   uint32_t _gpio_ahbenr();
  template<> constexpr uint32_t _gpio_ahbenr<'A'>() { return RCC_APB2ENR_IOPAEN; }
  template<> constexpr uint32_t _gpio_ahbenr<'B'>() { return RCC_APB2ENR_IOPBEN; }
  template<> constexpr uint32_t _gpio_ahbenr<'C'>() { return RCC_APB2ENR_IOPCEN; }
  template<> constexpr uint32_t _gpio_ahbenr<'D'>() { return RCC_APB2ENR_IOPDEN; }
  template<> constexpr uint32_t _gpio_ahbenr<'E'>() { return RCC_APB2ENR_IOPEEN; }

	using irq_handler = void(*)();
	
  template < typename mCu >
  struct irq_table
  {
    static const irq_handler value[59];
  };

  template < typename mCu >
  __attribute__((section("RESET")))
  const irq_handler irq_table<mCu>::value[59] = {
      (irq_handler)&__initial_sp,
      &mCu::on_reset,
			&mCu::irq_nmi,
			&mCu::irq_hard_fault,
			&mCu::irq_mem_manage,
			&mCu::irq_bus_fault,
			&mCu::irq_usage_fault,
		
      //(mCu::SUPPORTED_IRQS&IRQ_NMI?&mCu::irq_nmi:&mCu::irq_dummy),
      //(mCu::SUPPORTED_IRQS&IRQ_HARD_FAULT?&mCu::irq_hard_fault:&mCu::irq_dummy),
      //(mCu::SUPPORTED_IRQS&IRQ_MEM_MANAGE?&mCu::irq_mem_manage:&mCu::irq_dummy),
      //(mCu::SUPPORTED_IRQS&IRQ_BUS_FAULT?&mCu::irq_bus_fault:&mCu::irq_dummy),
      //(mCu::SUPPORTED_IRQS&IRQ_USAGE_FAULT?&mCu::irq_usage_fault:&mCu::irq_dummy),
      0,0,0,0,
      (mCu::SUPPORTED_IRQS&IRQ_SVC?&mCu::irq_svc:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DEBUG_MON?&mCu::irq_debug_mon:&mCu::irq_dummy),
      0,0,
      (mCu::SUPPORTED_IRQS&IRQ_PEND_SV?&mCu::irq_pen_sv:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_SYS_TICK?&mCu::irq_sys_tick:&mCu::irq_dummy),

      (mCu::SUPPORTED_IRQS&IRQ_WATCHDOG?&mCu::irq_watchdog:&mCu::irq_dummy),
      &mCu::irq_dummy, /*pvd*/
      &mCu::irq_dummy, /*tamper*/
      (mCu::SUPPORTED_IRQS&IRQ_RTC?&mCu::irq_rtc:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_FLASH?&mCu::irq_flash:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_RCC?&mCu::irq_rcc:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_0?&mCu::irq_exti_0:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_1?&mCu::irq_exti_1:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_2?&mCu::irq_exti_2:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_3?&mCu::irq_exti_3:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_4?&mCu::irq_exti_4:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_1?&mCu::irq_dma_1:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_2?&mCu::irq_dma_2:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_3?&mCu::irq_dma_3:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_4?&mCu::irq_dma_4:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_5?&mCu::irq_dma_5:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_6?&mCu::irq_dma_6:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_DMA_7?&mCu::irq_dma_7:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_ADC?&mCu::irq_adc:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USB_FS?&mCu::irq_usb_high:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USB_FS?&mCu::irq_usb_low:&mCu::irq_dummy),
      &mCu::irq_dummy, /*CAN*/
      &mCu::irq_dummy, /*CAN*/
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_5_9?&mCu::irq_exti_5_9:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_1?&mCu::irq_timer_1_brk:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_1?&mCu::irq_timer_1_up:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_1?&mCu::irq_timer_1_trg_com:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_1?&mCu::irq_timer_1_cc:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_2?&mCu::irq_timer_2:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_3?&mCu::irq_timer_3:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_TIMER_4?&mCu::irq_timer_4:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_I2C_1?&mCu::irq_i2c_1_event:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_I2C_1?&mCu::irq_i2c_1_error:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_I2C_2?&mCu::irq_i2c_2_event:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_I2C_2?&mCu::irq_i2c_2_error:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USART_1?&mCu::irq_usart_1:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USART_2?&mCu::irq_usart_2:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USART_3?&mCu::irq_usart_3:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_EXTI_10_15?&mCu::irq_exti_10_15:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_RTC?&mCu::irq_rtc_alarm:&mCu::irq_dummy),
      (mCu::SUPPORTED_IRQS&IRQ_USB_FS?&mCu::irq_usb_wakeup:&mCu::irq_dummy),
  };

  template < typename exact_mCu >
  struct stm32f1xx
  {
    static constexpr HSE_MHZ<8>  HSE8MHZ {};
			
		static uint32_t sys_clock;
			
    template <int leg_number> struct leg
    {
      using mCu  = exact_mCu; /* workaround */
      using info = typename mp::find_number<leg_number,typename exact_mCu::legs>::value;
      static_assert(mp::find_number<leg_number,typename exact_mCu::legs>::exists,"specified leg does not exist on the device");
    };

    template < typename... Args >
    __noinline static void* setup(Args... args)
    {
			using opts_list = mp::list<Args...>;

			// force direct HSI 8Mhz without PLL
			RCC->CR = (RCC->CR&~(RCC_CR_PLLON|RCC_CR_HSEON|RCC_CR_CSSON|RCC_CR_HSIRDY))|RCC_CR_HSION;
			while ( !(RCC->CR & RCC_CR_HSIRDY) ) __nop(); 			
			RCC->CFGR |= RCC_CFGR_SW_HSI;
			while ( (RCC->CFGR&RCC_CFGR_SWS) != RCC_CFGR_SWS_HSI ) __nop();
			
			if ( mp::in<HSE_MHZ<8>,opts_list>::exists )
			{
				/* using external crystal oscillator 8Mhz */
				/* 72Mhz sysclock, PLL x9, USB /1.5, APB1 /2, APB2 /1, ADC /2 */

				// enable HSE
				RCC->CR = (RCC->CR&~RCC_CR_HSERDY)|RCC_CR_HSEON;
				while ( !(RCC->CR&RCC_CR_HSERDY) ) __nop();
				
				FLASH->ACR |= FLASH_ACR_PRFTBE;
				FLASH->ACR &= (uint32_t)~FLASH_ACR_LATENCY;
				FLASH->ACR |= (uint32_t)FLASH_ACR_LATENCY_0;
				
				// configure clocking
				RCC->CFGR = 0x001d0400;
				
				// enable PLL
				RCC->CR &= ~(RCC_CR_HSION|RCC_CR_PLLRDY|RCC_CR_HSERDY);
				RCC->CR |= RCC_CR_PLLON;
				while ( !(RCC->CR&RCC_CR_PLLRDY) ) __nop();

				// setting pll as clock source
				RCC->CFGR |= 2; 
				while ( (RCC->CFGR&0x0c) != 0x08 ) __nop();
				
				sys_clock = 72000000;
			}
			else 
			{
				/* using HSI 8Mhz*/
				/* 48Mhz sysclock, PLL x12, USB x1, APB1 /2, APB2 /1, ADC /2 */
				
				RCC->CFGR = 0x00690402;
				while ( (RCC->CFGR&0x0c) != 0x08 ) __nop();
				
				RCC->CR |= RCC_CR_PLLON;
				while ( !(RCC->CR&RCC_CR_PLLRDY) ) __nop();				
				
				sys_clock = 48000000;
			}
			
      return (void*)&irq_table<exact_mCu>::value;
    }

    static void irq_dummy()
    {
    }

    static void on_reset()
    {
      RCC->CIR = 0; // disable interrupts
			__main();
    }

    static void irq_nmi() {}
    static void irq_hard_fault() {}
    static void irq_mem_manage() {}
    static void irq_bus_fault() {}
    static void irq_usage_fault() {}
    static void irq_svc();
		static void irq_watchdog();
    static void irq_debug_mon();
    static void irq_pen_sv();
    static void irq_sys_tick();
    static void irq_rtc();
    static void irq_flash();
    static void irq_rcc();
    static void irq_rtc_alarm();
    static void irq_exti_0();
    static void irq_exti_1();
    static void irq_exti_2();
    static void irq_exti_3();
    static void irq_exti_4();
    static void irq_exti_5_9();
    static void irq_exti_10_15();
    static void irq_dma_1();
    static void irq_dma_2();
    static void irq_dma_3();
    static void irq_dma_4();
    static void irq_dma_5();
    static void irq_dma_6();
    static void irq_dma_7();
    static void irq_adc();
    static void irq_usb_high();
    static void irq_usb_low();
		static void irq_usb_wakeup();
		static void irq_timer_1_brk();
		static void irq_timer_1_up();
		static void irq_timer_1_trg_com();
		static void irq_timer_1_cc();
		static void irq_timer_2();
		static void irq_timer_3();
		static void irq_timer_4();
		static void irq_i2c_1_event();
		static void irq_i2c_1_error();
		static void irq_i2c_2_event();
		static void irq_i2c_2_error();
		static void irq_usart_1();
		static void irq_usart_2();
		static void irq_usart_3();

  };

	template < typename exact_mCu > uint32_t stm32f1xx<exact_mCu>::sys_clock;
}
