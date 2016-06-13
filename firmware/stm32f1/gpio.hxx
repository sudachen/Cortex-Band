#pragma once

#include "stm32f1.hxx"
#include "metacpp.hxx"

namespace stm32f1
{

  enum open_drain_t
  {
    open_drain
  };
  enum pull_up_t
  {
    pull_up
  };
  enum pull_down_t
  {
    pull_down
  };
  enum input_t
  {
    input
  };
  enum output_t
  {
    output
  };
  enum low_speed_t
  {
    low_speed
  };
  enum medium_speed_t
  {
    medium_speed
  };
  enum high_speed_t
  {
    high_speed
  };

  template < typename leg >
  struct gpio
  {

    template < typename ... Opts >
    __noinline static void _setup()
    {
      using opts_list = mp::list<Opts...>;
      using leg_info = typename leg::info;
      constexpr auto leg_ctl = _gpio_ctl<leg_info::gpio_port>();

      static_assert(mp::in<output_t,opts_list>::count_once 
									+ mp::in<input_t,opts_list>::count_once < 2,
							"leg should configured as output or input");
      static_assert(mp::in<output_t,opts_list>::count_once 
									+ mp::in<pull_up_t,opts_list>::count_once < 2,
							"output could not be pulled up");
      static_assert(mp::in<output_t,opts_list>::count_once 
									+ mp::in<pull_down_t,opts_list>::count_once < 2,
							"output could not be pulled down");
			static_assert(mp::in<input_t,opts_list>::count_once  
				          + mp::in<open_drain_t,opts_list>::count_once < 2,
							"input can't be open drain");
      static_assert(mp::in<pull_down_t,opts_list>::count_once
									+ mp::in<pull_up_t,opts_list>::count_once
									+ mp::in<open_drain_t,opts_list>::count_once < 2,
							"leg could not be configured as pull-up/down and as open-drain at one time");
      
			constexpr bool read_only = 
					mp::in<input_t, opts_list>::exists 
					|| mp::in<pull_up_t,opts_list>::exists
					|| mp::in<pull_down_t,opts_list>::exists
					;
			
      RCC->APB2ENR |= _gpio_ahbenr<leg_info::gpio_port>();
			
			int cnf = 0;
			int mode = 0;
						
      if ( !read_only && mp::in<open_drain_t, opts_list>::exists )
				// open drain output
        cnf = 1;
      else if ( !read_only )
				// push-pull output
        (void)0; // none, cnf = 0
			// input case
			else if ( mp::in<pull_up_t, opts_list>::exists )
				cnf = 10;
			else if ( mp::in<pull_down_t, opts_list>::exists )
				cnf = 10;
			else /* floating input */
				cnf = 1;
			
			if ( !read_only ) /* otherwise mode == 0, it means leg configured for input */
			{
				if ( !mp::in<medium_speed_t, opts_list>::exists
						&& !mp::in<low_speed_t, opts_list>::exists )
					/* high speed (50Mhz) */
					mode = 3;
				else if ( mp::in<medium_speed_t, opts_list>::exists )
					/* medium speed (10Mhz) */
					mode = 1; 
				else /* low speed (2Mhz)*/
					mode = 2;				
			}

			const uint32_t cr_value = ( (cnf << 2) | mode) << ( (leg_info::gpio_channel & 7 ) * 4 );
			const uint32_t cr_mask  = ~( uint32_t(0x0f) << ( (leg_info::gpio_channel & 7) * 4 ));
			if ( leg_info::gpio_channel > 7 )
				leg_ctl->CRH = (leg_ctl->CRH & cr_mask) | cr_value;
			else
				leg_ctl->CRL = (leg_ctl->CRL & cr_mask) | cr_value;
    }

    template < typename ... Opts >
    __forceinline void setup(Opts ... opts) const
    {
      _setup<Opts...>();
    }

    __noinline
    static void _set_high()
    {
      using info = typename leg::info;
      constexpr auto ctl = _gpio_ctl<info::gpio_port>();
      ctl->ODR |= (uint16_t(1) << info::gpio_channel);
    }

    __forceinline void set_high() const
    {
      _set_high();
    }

    __noinline
    static void _set_low()
    {
      using info = typename leg::info;
      constexpr auto ctl = _gpio_ctl<info::gpio_port>();
      ctl->ODR &= ~(uint16_t(1) << info::gpio_channel);
    }

    __forceinline void set_low() const
    {
      _set_low();
    }

    __noinline
    static bool _get()
    {
      using info = typename leg::info;
      constexpr auto ctl = _gpio_ctl<info::gpio_port>();
      return (ctl->IDR & (1 << info::gpio_channel) == 0) ? false : true;;
    }

    __forceinline bool get() const
    {
      return _get();
    }

    static constexpr bool high = true;
    static constexpr bool low = false;
  };
}
