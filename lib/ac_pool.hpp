#pragma once

//#include <new>
//#include <limits.h>
#include "ac_stack.hpp"

namespace abstract_container
{

namespace impl
{

template <unsigned num_bytes, bool dup>
struct align_ { };

//template <>
//struct align_<sizeof(long long), false>
//{
//    typedef long long type;
//};

#undef X
#define X(T, N) \
template <> \
struct align_<sizeof(T), (sizeof(T) == sizeof(N))> \
  { typedef T type; };

X(char, short)
X(short, int)
X(int, long)
X(long, long long)

#undef X

// align<N>::type is the integral type (if any) which is aligned on
// addresses that are multiples of N bytes.
template <unsigned num_bytes>
struct align : public align_<num_bytes, false> { };

//class dummy;

// footprint<T> is a "unionable" type with the same alignment requirements
// as type T as well as the same size.
template <typename any_type>
class footprint
{
    private:
        // To avoid inaccessability warnings.
        //friend class impl::dummy;
        struct padded { char c; any_type t; };
        static const unsigned align_factor = sizeof(padded) - sizeof(any_type);
        union uu;
        friend union uu;

        union uu
        {
            char space[sizeof(any_type)];
            typename align<align_factor>::type a;
        };

        uu u;
};

    template <class user_entry, typename index>
    struct entry
    {
        union
        {
            index link;
            footprint<user_entry> ue;
        };
    };
    // Array for 'pool' template (entries internal to pool instance).
    template<class user_entry, typename index, index num_entries>
    struct pool_arr
    {
        typedef entry<user_entry, index> type[num_entries];
    };

    // Array for 'external_pool' template (entries external to pool instance).
    template<class user_entry, typename index>
    struct external_pool_arr
    {
        typedef entry<user_entry, index> *type;
    };
}
// 'base_pool' is an (abstract) pool of fixed-size entries of the same
// type.
// 'user_entry' is the type of the entries in the pool.
// An instance 'arr_type' must implicitly convert to 'index', and
// 'arr_type' must have a parameterless constructor.
// 'index' is some integral type, which can hold any value in the
// range from 0 to the number of entries in the pool.
//
template <class user_entry, typename arr_type, typename T_IndexType = unsigned>
class base_pool
{
    public:

    // If, for an instance of base_pool, the array of pool entries must
    // be defined by the user code external to the base_pool instance,
    // 'entry' is the type the array elements must have.
        typedef impl::entry<user_entry, T_IndexType> entry;
        static T_IndexType end(void)     { return(static_cast<T_IndexType>(~ static_cast<T_IndexType>(0))); }

    public:
        class stack_abs
        {
            public:
                arr_type arr;
                typedef T_IndexType handle;
                void set_next(handle h, handle lh) { arr[h].link = lh; }
                handle get_next(handle h) { return(arr[h].link); }
                static handle null(void) { return(end()); }
        };

        class avail_type : public stack<stack_abs>
        {
            public:
                entry & operator [] (T_IndexType idx) { return(arr[idx]); }
                using stack<stack_abs>::arr;
        };

        void free_all(T_IndexType num_entries)
        {
            while (num_entries--)
                avail_m.push_front(num_entries);
        }

        void init(T_IndexType num_entries)
        {
            avail_m.purge();
            free_all(num_entries);
        }
    public:
        avail_type avail_m;

    public:
      // Returns false if the next call to alloc() will return end().
      bool can_alloc(void) { return(!avail_m.empty()); }

      // Allocates an entry and returns its index, or returns end()
      // if there are no avail_mable entries.
      T_IndexType alloc(void)
      {
          auto i = avail_m.front();
          if (i != end())
          {
              avail_m.pop_front();

          // Use placement new to execute constructor for allocated entry.
          new (&(avail_m[i].ue)) user_entry;
        }
      return(i);
    }

      // Make avail_mable a previously-allocated entry.
      void free(T_IndexType i)
      {
          // Explicitly call destructor for freed entry.
          reinterpret_cast<user_entry *>(&(avail_m[i].ue))->~user_entry();
          avail_m.push_front(i);
      }

      // Returns reference to entry whose index is i.
      user_entry & operator [] (T_IndexType i)
      { return(*reinterpret_cast<user_entry *>(&(avail_m[i].ue))); }

      // For debugging purposes -- returns number of avail_mable entries.
      T_IndexType num_avail(void)
      {
          T_IndexType cnt = 0;
          T_IndexType i = avail_m.front();
          while (i != end())
          {
              cnt++;
              i = avail_m[i].link;
          }
          return(cnt);
      }
  };



// 'pool' is a pool of fixed-size entries of the same type.  The entries
// are stored in an array that is internal to the pool instance.
// 'user_entry' is the type of the entries in the pool.
// 'index' is some integral type, which can hold any value in the
// range from 0 to the number of entries in the pool.
// 'num_entries' is the number of entries in the pool.
//
template <class user_entry, typename T_IndexType, T_IndexType num_entries>
class pool :  public base_pool<user_entry, typename impl::pool_arr<user_entry, T_IndexType, num_entries>::type, T_IndexType>
{
    protected:

        typedef base_pool<user_entry, typename impl::pool_arr<user_entry, T_IndexType, num_entries>::type, T_IndexType> super;

    public:
        pool(void)  { super::free_all(num_entries); }
        //pool(no_purge_class np) { }

    void init(void)
      { super::init(num_entries); }
};


} // end namespace abstract_container
