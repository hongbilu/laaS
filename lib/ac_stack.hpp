#pragma once

namespace abstract_container
{

template <class abstractor>
class stack : protected abstractor
{
    public:
    	typedef typename abstractor::handle handle;

    	// Handle C++ gotcha with names from base class that is template-
    	// parameter-dependent.
    	using abstractor::get_next;
    	using abstractor::set_next;
    	using abstractor::null;

    	handle frontNode_m;

  public:

    // Initialize stack to empty state.
    stack(void) : frontNode_m(null()) { }

    // Make stack empty.
    void purge(void) { frontNode_m = null(); }

    // Returns true if stack is empty.
    bool empty(void) { return(frontNode_m == null()); }

    // Return handle of element at the front of the stack, or null() if
    // the stack is empty.
    handle front(void) const { return(frontNode_m); }

    // Push the element whose handle is h onto the front of the stack.
    void push_front(handle h)
    {
    	set_next(h, frontNode_m);
    	frontNode_m = h;
    }

    // Must not call this if stack is empty.
    void pop_front(void) { frontNode_m = get_next(frontNode_m); }

    // Does nothing if stack is empty.
    void safe_pop_front(void)
      {
	if (frontNode_m != null())
	  pop_front();
      }

    // Insert the element whose handle is i in the stack after the element
    // (which must already be in the stack) whose handle is h.
    void insert_after(handle h, handle i)
      {
	set_next(i, get_next(h));
	set_next(h, i);
      }

    // Remove element after element whose handle is h.  Must not call this
    // if the element whose handle is h is the last element in the stack.
    void rm_next(handle h)
      {
	handle n = get_next(h);
	set_next(h, get_next(n));
      }

    // Remove element after element whose handle is h.  Does nothing
    // if the element whose handle is h is the last element in the stack.
    void safe_rm_next(handle h)
      {
	handle n = get_next(h);
	if (n != null())
	  set_next(h, get_next(n));
      }
  };

} // end namespace abstract_container
