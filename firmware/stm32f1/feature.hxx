
#pragma once

#include <stdint.h>

#include "../stm32f1/metacpp.hxx"

namespace feature
{
  struct frequency
  {
    struct _Hz  {};
    struct _kHz {};
    struct _mHz {};
    uint32_t value;
    explicit frequency(uint32_t val) : value(val) {}
    frequency() : value(0) {};
  };

  inline frequency operator*(uint32_t value,frequency::_Hz)
  {
    return frequency(value);
  }

  inline frequency operator*(uint32_t value,frequency::_kHz)
  {
    return frequency(value*1000);
  }

  inline frequency operator*(uint32_t value,frequency::_mHz)
  {
    return frequency(value*1000*1000);
  }

  struct duty
  {
    struct _Dty {};
    uint8_t value;
    explicit duty(uint8_t val): value(val) {}
    duty() : value(0) {}
  };

  inline duty operator*(uint8_t value,duty::_Dty)
  {
    return duty(value);
  }
	
  struct timeout
  {
    struct _Mks {};
    struct _Ms  {};
    struct _Sec {};
    uint32_t value; // in micro seconds ~ 4000sec max
    explicit timeout(uint32_t val) : value(val) {}
    timeout() : value(0) {}
  };

  inline timeout operator*(uint32_t value,timeout::_Mks)
  {
    return timeout(value);
  }

  inline timeout operator*(uint32_t value,timeout::_Ms)
  {
    return timeout(value*1000);
  }

  inline timeout operator*(uint32_t value,timeout::_Sec)
  {
    return timeout(value*1000*1000);
  }

  struct prime
  {
    static constexpr int PRIMES_8bit_COUNT = 54;
    
    __forceinline static uint8_t get_8bit(const unsigned no) 
    {
      static const uint8_t numbers[PRIMES_8bit_COUNT] = {       
        2,      3,      5,      7,     11,     13,     17,     19,     23,     29, 
       31,     37,     41,     43,     47,     53,     59,     61,     67,     71, 
       73,     79,     83,     89,     97,    101,    103,    107,    109,    113, 
      127,    131,    137,    139,    149,    151,    157,    163,    167,    173, 
      179,    181,    191,    193,    197,    199,    211,    223,    227,    229, 
      233,    239,    241,    251 
      };

      return no < PRIMES_8bit_COUNT ? numbers[no] : 1;
    }
  };
}

static constexpr feature::duty::_Dty       _dty_{};
static constexpr feature::frequency::_Hz   _hz_{};
static constexpr feature::frequency::_kHz  _khz_{};
static constexpr feature::frequency::_mHz  _mhz_{};
static constexpr feature::timeout::_Mks    _mks_{};
static constexpr feature::timeout::_Ms     _ms_{};
static constexpr feature::timeout::_Sec    _sec_{};
	
