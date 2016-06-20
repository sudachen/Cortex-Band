
#include "../stm32f1/stm32f103c8.hxx"
#include "../stm32f1/gpio.hxx"
#include "../stm32f1/usb_fs.hxx"
#include "../stm32f1/sched.hxx"

ALLOCATE_STACK(0x400);
ALLOCATE_HEAP(0x400);

struct mcu : stm32f1::stm32f103c8_base<mcu>
{
  using mcu_base = stm32f1::stm32f103c8_base<mcu>;
  static constexpr auto SUPPORTED_IRQS = stm32f1::IRQ_USB_FS|stm32f1::IRQ_SYS_TICK;

  using led_green = stm32f1::gpio<leg<2>>;
  using led_red   = stm32f1::gpio<leg<38>>;
  using usb       = stm32f1::usb_fs<mcu>;
  using sched     = stm32f1::sched<mcu>;

  static void setup()
  {
    mcu_base::setup(HSE8MHZ);
    led_green::setup();
    led_red::setup();

    /* setting up usb */
    usb::setup();
    usb::enable();

    sched::setup();
    sched::repeat(2*_hz_,[]{led_green::toggle();});
  }

  static void irq_sys_tick()
  {
    sched::irq_sys_tick();
  }

  static void irq_usb_low()
  {
    usb::irq_usb_low();
  }

  static void irq_usb_high()
  {
    usb::irq_usb_high();
  }

	static void irq_usb_wakeup()
	{
	}

	static void main_loop()
	{
	  for(;;)
	  {
	    while ( sched::has_next_action() ) sched::execute();
	    __wfi();
	  }
	}

};

int main()
{
  mcu::setup();
  mcu::main_loop();
}
