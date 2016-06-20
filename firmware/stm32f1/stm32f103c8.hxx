
#pragma once

#ifndef STM32F103
#define STM32F103
#endif
#ifndef STM32F10X_MD
#define STM32F10X_MD
#endif

#include "st/stm32f103xb.h"
#include "stm32f1.hxx"

namespace stm32f1
{

  template < typename exact_mCu >
  struct stm32f103c8_base : stm32f1xx<exact_mCu>
  {
    template < typename... Args >
    static void setup(Args... args)
    {
      stm32f1xx<exact_mCu>::setup(args...);
    }

    template <int leg_number> struct leg
    {
      using mCu  = exact_mCu;
      using info = typename mp::find_number<leg_number,typename mCu::legs>::value;
      static_assert(mp::find_number<leg_number,typename mCu::legs>::exists,"specified leg does not exist on the device");
    };

    using PA0 = leg<10>;
    using PA1 = leg<11>;
    using PA2 = leg<12>;
    using PA3 = leg<13>;
    using PA4 = leg<14>;
    using PA5 = leg<15>;
    using PA6 = leg<16>;
    using PA7 = leg<17>;
    using PA9 = leg<30>;
    using PA10= leg<31>;
    using PA11= leg<32>;
    using PA12= leg<33>;
    using PA13= leg<34>;
    using PA14= leg<37>;
    using PA15= leg<38>;
    using PB0 = leg<18>;
    using PB1 = leg<19>;
    using PB2 = leg<20>;
    using PB3 = leg<39>;
    using PB4 = leg<40>;
    using PB5 = leg<41>;
    using PB6 = leg<42>;
    using PB7 = leg<43>;
    using PB8 = leg<45>;
    using PB9 = leg<46>;
    using PB10= leg<21>;
    using PB11= leg<22>;
    using PB12= leg<25>;
    using PB13= leg<36>;
    using PB14= leg<27>;
    using PB15= leg<28>;
    using PC13= leg<2>;
    using PC14= leg<3>;
    using PC15= leg<4>;
    using PD0 = leg<5>;
    using PD1 = leg<6>;

    using legs = mp::list<
      leg_def<2, 'C',13 >,
      leg_def<3, 'C',14 >,
      leg_def<4, 'C',15 >,
      leg_def<5, 'D',1  >,
      leg_def<10,'A',0  >,
      leg_def<11,'A',1  >,
      leg_def<12,'A',2  >,
      leg_def<13,'A',3  >,
      leg_def<14,'A',4  >,
      leg_def<15,'A',5  >,
      leg_def<16,'A',6  >,
      leg_def<17,'A',7  >,
      leg_def<18,'B',0  >,
      leg_def<19,'B',1  >,
      leg_def<20,'B',2  >, /*BOOT1 on reset*/
      leg_def<21,'B',10 >,
      leg_def<22,'B',11 >,
      leg_def<25,'B',12 >,
      leg_def<26,'B',13 >,
      leg_def<27,'B',14 >,
      leg_def<28,'B',15 >,
      leg_def<29,'A',8  >,
      leg_def<30,'A',9  >,
      leg_def<31,'A',10 >,
      leg_def<32,'A',11 >,
      leg_def<33,'A',12 >,
      leg_def<34,'A',13 >,
      leg_def<37,'A',14 >,
      leg_def<38,'A',15 >,
      leg_def<39,'B',3  >,
      leg_def<40,'B',4  >,
      leg_def<41,'B',5  >,
      leg_def<42,'B',6  >,
      leg_def<43,'B',7  >,
      leg_def<45,'B',8  >,
      leg_def<46,'B',9  >
    >;

    using af_swdio = mp::list<leg_af_remap<34,0>>;
    using af_swclk = mp::list<leg_af_remap<37,0>>;
    using af_usbdm = mp::list<leg_af_remap<32,0>>;
    using af_usbdp = mp::list<leg_af_remap<33,0>>;

  };

  struct stm32f103c8 : stm32f103c8_base<stm32f103c8>
  {
    static constexpr auto SUPPORTED_IRQS = IRQ_NONE;
  };

}

