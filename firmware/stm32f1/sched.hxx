
#pragma once

#include "metacpp.hxx"

namespace stm32f1
{

  constexpr uint64_t RESOURCE_SCHEDULER_QUEUE = resource_id<'S','C','H','E','D','Q','U','E'>::value;

  template < typename mCu, int QUEUE_LENGTH = 8 >
  struct sched
  {

    static_assert(QUEUE_LENGTH < 0x1000, "queue is to large");
    static_assert(mCu::SUPPORTED_IRQS&stm32f1::IRQ_SYS_TICK, "scheduler requires SYSTICK IRQ");
    static constexpr unsigned COUNTER_WINDOW = 0x4000;

    struct Action
    {
      volatile void (*f)(); // nullptr if record is free
      uint32_t execute_on_counter;
      uint32_t counter_step; // 0 if no repeat
    };

    struct State
    {
      Action   queue[QUEUE_LENGTH];

      volatile uint32_t counter;
      volatile int      last_index;

      int      action_index;
      void     (*next_action)();
    };

    static State st;

    static void __noinline setup()
    {
      memset(&st,0,sizeof(st));
    }

    static void irq_sys_tick()
    {
      ++st.counter;

      // to restat actions enumeration set lat_index before first element
      st.last_index = -1;

      // since enumeration can increment last_index right after irq call,
      //   lst_index sets to -1, which can be correctly incremented to zero.
    }

    static bool __noinline has_next_action()
    {
      int index;

      if ( st.next_action )
        return true;

      while ( (index = st.last_index) < QUEUE_LENGTH )
      {
        uint32_t counter = st.counter;
        ++st.last_index;
        if ( index < 0 ) // yes, it's possible and OK.
          continue;

        Action& act = st.queue[index];
        if (   act.f != nullptr
            && act.execute_on_counter < counter
            && act.execute_on_counter+COUNTER_WINDOW > counter )
        {
          st.next_action = act.f;

          if ( act.counter_step )
          {
            act.execute_on_counter += act.counter_step;
            st.action_index = index;
          }
          else
          {
            // placing nullptr to act.f ireleases record,
            //   so it can be modified by placing delayed action in some interrupt handle
            st.action_index = QUEUE_LENGTH;
            act.f = nullptr;
          }

          return true;
        }

      }

      return false;
    }

    static void execute()
    {
      if ( st.next_action )
      {
        st.next_action();
        st.next_action = nullptr;
        st.action_index = QUEUE_LENGTH;
      }
    }

    static void _allocate_action(void(*action)(), uint32_t execute_on, uint32_t step)
    {
      for ( int i = 0; i < QUEUE_LENGTH; ++i )
      {
        if ( st.queue[i].f == nullptr )
        {
          bool was_enabled = stm32f1::disable_irq();
          if ( st.queue[i].f == nullptr )
          {
            st.queue[i].f = action;
            st.queue[i].execute_on_counter = execute_on;
            st.queue[i].counter_step = step;
            if ( was_enabled ) stm32f1::enable_irq();
            return;
          }
          if ( was_enabled ) stm32f1::enable_irq();
        }
      }
      mCu::no_enough(stm32f1::RESOURCE_SCHEDULER_QUEUE);
      /* no return here */
    }

    static uint32_t _calculate_step(feature::timeout mks)
    {
      uint32_t ns_per_tick = 1000000000u/mCu::systick_frequency;
      uint32_t counter_delay = uint32_t((uint64_t(mks.value)*1000/ns_per_tick+500)/1000);
      assert(counter_delay > 0);
      return counter_delay;
    }

    static uint32_t _calculate_step(feature::frequency hz)
    {
      uint32_t ns_per_tick = 1000000000u/mCu::systick_frequency;
      uint64_t ns_per_delay = 1000000000u/hz.value;
      uint32_t counter_delay = uint32_t((ns_per_delay/ns_per_tick+500)/1000);
      assert(counter_delay > 0);
      return counter_delay;
    }

    static void __noinline delay(feature::timeout mks,void(*action)())
    {
      _allocate_action(action,
                        _calculate_step(mks) + st.counter, // start after delay
                        0); // do not repeat
    }

    /*
     repeat specified action with desired frequency
     */
    static void __noinline repeat(feature::frequency hz,void(*action)())
    {
      _allocate_action(action,
                        1 + st.counter, // start on next cycle
                        _calculate_step(hz));
    }

    /*
     repeat specified action with desired interval
     */
    static void __noinline repeat(feature::timeout mks,void(*action)())
    {
      _allocate_action(action,
                        1 + st.counter, // start on next cycle
                        _calculate_step(mks));
    }

    /*
     terminate repeating for currentelly executing action
     */
    static void terminate()
    {
      if ( st.action_index < QUEUE_LENGTH )
      {
        st.queue[st.action_index].f = nullptr;
      }
    }

  };

  template < typename mCu, int QUEUE_LENGTH >  __noinit
  typename sched<mCu,QUEUE_LENGTH>::State sched<mCu,QUEUE_LENGTH>::st;

}

