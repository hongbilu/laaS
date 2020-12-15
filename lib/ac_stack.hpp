#pragma once
#include <atomic>
#include <iostream>

namespace abstract_container
{

template <class abstractor>
class stack : protected abstractor
{
    public:
        typedef typename abstractor::T_IdxType T_IndexType;

        // Handle C++ gotcha with names from base class that is template-
        // parameter-dependent.
        using abstractor::get_next;
        using abstractor::set_next;
        using abstractor::null;

        std::atomic<T_IndexType> frontNode_m;

    public:

    // Initialize stack to empty state.
    stack(void) : frontNode_m(null()) { }

    // Make stack empty.
    void purge(void) { frontNode_m = null(); }

    // Returns true if stack is empty.
    bool empty(void) { return(frontNode_m == null()); }

    // Return handle of element at the front of the stack, or null() if
    // the stack is empty.
    T_IndexType front(void) const { return(frontNode_m); }

    // Push the element whose handle is h onto the front of the stack.
    void push_front(T_IndexType h)
    {
        auto fn = frontNode_m.load(std::memory_order_acquire);
        do {
            set_next(h, fn);
        }while(!frontNode_m.compare_exchange_weak(fn, h));
    }
    
    // Must not call this if stack is empty.
    /*
    void pop_front(void)
    {
        auto fn = frontNode_m.load(std::memory_order_acquire);
        auto next = get_next(fn);
        while(!frontNode_m.compare_exchange_weak(fn, next))
        {
            next = get_next(fn);
        }
    }
    */
    T_IndexType alloc()
    {
        auto fn = frontNode_m.load(std::memory_order_acquire);
        auto next = get_next(fn);
        while(!frontNode_m.compare_exchange_weak(fn, next))
        {
            next = get_next(fn);
        }
        return fn;
    }
    /*
    // Does nothing if stack is empty.
    void safe_pop_front(void)
    {
        if (frontNode_m != null())
            pop_front();
    }

    // Insert the element whose handle is i in the stack after the element
    // (which must already be in the stack) whose handle is h.
    void insert_after(T_IndexType h, T_IndexType i)
    {
        set_next(i, get_next(h));
        set_next(h, i);
    }

    // Remove element after element whose handle is h.  Must not call this
    // if the element whose handle is h is the last element in the stack.
    void rm_next(T_IndexType h)
    {
        T_IndexType n = get_next(h);
        set_next(h, get_next(n));
    }

    // Remove element after element whose handle is h.  Does nothing
    // if the element whose handle is h is the last element in the stack.
    void safe_rm_next(T_IndexType h)
    {
        auto n = get_next(h);
        if (n != null())
            set_next(h, get_next(n));
    }
    */
  };

} // end namespace abstract_container
