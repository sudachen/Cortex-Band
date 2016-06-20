
#pragma once

namespace stm32f1
{

  template < typename mCu >
  struct usb_fs
  {
    template < typename... Args >
    __noinline static void setup(Args... args)
    {
    }

    static void irq_usb_low()
    {
    }

    static void irq_usb_high()
    {
    }

    static void enable()
    {
    }

    static void disable()
    {
    }
  };


}
