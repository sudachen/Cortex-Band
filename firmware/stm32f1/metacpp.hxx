
#pragma once

#if defined __GNUC__
#define __forceinline __attribute__((always_inline))
#define __noinline    __attribute__((noinline))
#else
#define __noinline    __declspec(noinline)
#endif

#include <stdint.h>

namespace mp
{
  template<bool B, class T = void>
  struct enable_if { typedef T type; };
   
  template<class T>
  struct enable_if<false, T> {};
    
  template<typename... List> 
  struct length_of {};
  
  template<> 
  struct length_of<> 
  { 
    enum { value = 0 }; 
  };
  
  template<typename Head, typename... Tail> 
  struct length_of<Head,Tail...> 
  { 
    enum {value = length_of<Tail...>::value+1}; 
  };

  template<typename Head, typename Tail> 
  struct link
  {
    using head = Head;
    using tail = Tail;
    static constexpr int length = tail::length + 1;
  };  
    
  struct E 
  {
    using head = E;
    using tail = link<E,E>;
    static constexpr int length = 0;
  };
    
  template<uint64_t Value>
  struct number
  {
    static constexpr uint64_t value = Value;
  };
    
  template<typename... Types> struct concat_types;
  
  template<typename Head,typename... Tail> 
  struct concat_types<Head,Tail...>
  {
    using result = link<Head,typename concat_types<Tail...>::result>;
  };
  
  template<typename Head> 
  struct concat_types<Head>
  {
    using result = link<Head,link<E,E>>;
  };
  
  template<> 
  struct concat_types<>
  {
    using result = link<E,E>;
  };
  
  template<int... numbers> struct concat_numbers;
  
  template<int head,int... tail> 
  struct concat_numbers<head,tail...>
  {
    using result = link<number<head>,typename concat_numbers<tail...>::result>;
  };
  
  template<int head> 
  struct concat_numbers<head>
  {
    using result = link<number<head>,link<E,E>>;
  };
  
  template<>
  struct concat_numbers<>
  {
    using result = link<E,E>;
  };
  
  template<typename... Types> using list = typename concat_types<Types...>::result;  
  template<int... numbers> using number_list = typename concat_numbers<numbers...>::result;  
  using empty_list = link<E,E>;
      
  template<typename A, typename B>
  struct the_same
  {
    static constexpr bool is_true = false;
  };
    
  template<typename A>
  struct the_same<A,A>
  {
    static constexpr bool is_true = true;
  };

  template<typename A, typename B>
  struct the_same_index
  {
    static constexpr bool is_true = the_same<A,typename B::index>::is_true;
  };
    
  template<typename A>
  struct the_same_index<A,E>
  {
    static constexpr bool is_true = false;
  };

  template<bool, template<typename...> class Pred, typename What, typename Head, typename Tail>
  struct recursive_compare
  {
    using value = typename recursive_compare<Pred<What,typename Tail::head>::is_true,Pred,What,typename Tail::head,typename Tail::tail>::value;
    static constexpr bool exists = recursive_compare<Pred<What,typename Tail::head>::is_true,Pred,What,typename Tail::head,typename Tail::tail>::exists;
    static constexpr int order = recursive_compare<Pred<What,typename Tail::head>::is_true,Pred,What,typename Tail::head,typename Tail::tail>::order + 1;
  };
  
  template<template<typename...> class Pred, typename What, typename Head, typename Tail>
  struct recursive_compare<true,Pred,What,Head,Tail>
  {
    using value = Head;
    static constexpr bool exists = true;
    static constexpr int order = 0;
  };
    
  template<template<typename...> class Pred, typename What>
  struct recursive_compare<false,Pred,What,E,empty_list>
  {
    using value = E;
    static constexpr bool exists = false;
    static constexpr int order = 0;
  };

  template<typename What, template<typename...> class Pred, typename List>
  struct list_find_p
  {
    using value = typename recursive_compare<Pred<What,typename List::head>::is_true,Pred,What,typename List::head,typename List::tail>::value;
    static constexpr bool exists = recursive_compare<Pred<What,typename List::head>::is_true,Pred,What,typename List::head,typename List::tail>::exists;
  };
  
  template<typename What, typename List>
  struct in
  {
    static constexpr bool exists = recursive_compare<the_same<What,typename List::head>::is_true,the_same,What,typename List::head,typename List::tail>::exists;
    static constexpr int count_once = exists?1:0;
    static constexpr int order = recursive_compare<the_same<What,typename List::head>::is_true,the_same,What,typename List::head,typename List::tail>::order;
  };
  
  template<typename Index, typename List>
  struct find
  {
    using value = typename recursive_compare<the_same_index<Index,typename List::head>::is_true,the_same_index,Index,typename List::head,typename List::tail>::value;
    static constexpr bool exists = recursive_compare<the_same_index<Index,typename List::head>::is_true,the_same_index,Index,typename List::head,typename List::tail>::exists;
    static constexpr int order = recursive_compare<the_same_index<Index,typename List::head>::is_true,the_same_index,Index,typename List::head,typename List::tail>::order;
  };
  
  template<uint64_t index, typename List>
  struct find_number
  {
    using value = typename find<number<index>,List>::value;
    static constexpr bool exists = find<number<index>,List>::exists;
    static constexpr int order = find<number<index>,List>::order;
  };
  
  template<typename List,template<typename...> class Operator,typename... Arg>
  struct for_each
  {
    template<typename... Opts>
    static void apply(Opts... opts) 
    { 
      Operator<typename List::head,Arg...>::apply(opts...);
      for_each<typename List::tail,Operator,Arg...>::apply(opts...);
    }
  };
  
  template<template<typename...> class Operator,typename... Arg>
  struct for_each<empty_list,Operator,Arg...>
  {
    template<typename... Opts>
    static void apply(Opts... opts) {}
  };

  template<typename What>
  struct arg_fetch
  {
    template<typename First,typename... Next>
    __forceinline static 
    typename enable_if<the_same<First,What>::is_true,What>::type value(First first, Next...)
    {
      return first;
    }
    
    template<typename First,typename... Next>
    __forceinline static
    typename enable_if<!the_same<First,What>::is_true,What>::type value(First, Next... next)
    {
      return arg_fetch<What>::value(next...);
    }
    
    __forceinline static What value()
    {
      return What{};
    }
  };

  template<typename Operator,typename... Args>
  struct for_each_arg
  {
    template<typename First,typename... Next>
    __forceinline static
    void apply(Args... args, First first, Next... next)
    {
      Operator::apply(first,args...);
      return for_each_arg<Operator>::apply(next...);
    }
    
    __forceinline static void apply(Args... args)
    {
      return;
    }
  };
  
  template <typename List, typename T>
  struct get
  {
    static constexpr const T at(unsigned index) { return index == 0 ? T(List::head::value) : get<typename List::tail,T>::at(index-1); }
  };
  template <typename T> 
  struct get<empty_list,T> 
  { 
    static constexpr const T at(unsigned index) { return T{}; } 
  };
    
}
