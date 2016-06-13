
#include "../stm32f1/stm32f103c8.hxx"
#include "../stm32f1/gpio.hxx"
#include "../stm32f1/usb_fs.hxx"

ALLOCATE_STACK(0x400);
ALLOCATE_HEAP(0x400);

struct mcu : stm32f1::stm32f103c8_base<mcu>
{
  using mcu_base = stm32f1::stm32f103c8_base<mcu>;
  static constexpr auto SUPPORTED_IRQS = stm32f1::IRQ_USB_FS|stm32f1::IRQ_SYS_TICK;

  static const stm32f1::gpio<leg<2>> led_green;
  //static const stm32f1::gpio<leg<15>> led_green;
  static const stm32f1::gpio<leg<38>> led_red;
  static const stm32f1::usb_fs<mcu> usb;

  static void setup()
  {
    mcu_base::setup(HSE8MHZ);
    led_green.setup();
    led_red.setup();

    /* setting up usb */
    usb.setup();
    usb.enable();
  }

  static void irq_sys_tick()
  {
  }

  static void irq_usb_low()
  {
    usb.irq_usb_low();
  }

  static void irq_usb_high()
  {
    usb.irq_usb_high();
  }

	static void irq_usb_wakeup()
	{
	}

};

int main()
{
  mcu::setup();
  mcu::led_green.set_low();
	for(;;) __nop();
}
