#pragma once
#include <assert.h>
#include <type_traits>
#include <functional>

template<typename T>
struct no_const {
    using type=typename std::conditional<std::is_const<T>::value,typename std::remove_const<T>::type,T>::type;
};
/*
 * when creating object, run acquire(apply source) action (can be empty[]{})
 * when destruct object, run release(free source) action
 * forbidden object copy and assignment
 */
class raii{
public:
    using fun_type = std::function<void()>;
    /* release: the function when destructor run
     * acquire: the function when constructor run
     * default_com:m_commit, default value, use commit() to re-configure it
     */
    explicit raii(fun_type release, fun_type acquire = [] {}, bool default_com = true) noexcept:
            m_commit(default_com), m_release(release)
    {
        acquire();
    }
    ~raii() noexcept
    {
        if (m_commit) m_release();
    }
    /* move destructor, allow right assignment */
    raii(raii && rv)noexcept:m_commit(rv.m_commit),m_release(rv.m_release){
        rv.m_commit=false;
    };
    raii(const raii&) = delete;
    raii& operator=(const raii&) = delete;
    raii& commit(bool c = true)noexcept { m_commit = c; return *this; };
private:
    bool m_commit;
protected:  
    std::function<void()> m_release;
};


template<typename T>
class raii_var
{
public:
    using    _Self      = raii_var<T>;
    using   acq_type    =std::function<T()>;
    using   rel_type    =std::function<void(T &)>;
    explicit raii_var(acq_type acquire , rel_type release) noexcept:
            m_resource(acquire()),m_release(release)
    {
    }
    raii_var(raii_var&& rv):
            m_resource(std::move(rv.m_resource)),
            m_release(std::move(rv.m_release))
    {
        rv.m_commit=false;
    }

    ~raii_var() noexcept{
        if (m_commit) m_release(m_resource);
    }
    _Self& commit(bool c = true)noexcept { m_commit = c; return *this; };
    T& get() noexcept{return m_resource;}
    T& operator*() noexcept    { return get();}

    template<typename _T=T>
    typename std::enable_if<std::is_pointer<_T>::value,_T>::type operator->() const noexcept
    { return m_resource;}
    template<typename _T=T>
    typename std::enable_if<std::is_class<_T>::value,_T*>::type operator->() const noexcept
    { return std::addressof(m_resource);}

private:
    bool m_commit=true;
    T m_resource;
    rel_type m_release;
};
/* make raii object,
 * RES      source type
 * M_REL    member function address for releaseing source
 * M_ACQ    member function address for applying source
 */
template<typename RES, typename M_REL, typename M_ACQ>
raii make_raii(RES & res, M_REL rel, M_ACQ acq, bool default_com = true)noexcept {
    // checking argement type when compiling
    // detials about type_traits, plze see:http://www.cplusplus.com/reference/type_traits/
    static_assert(std::is_class<RES>::value, "RES is not a class or struct type.");
    static_assert(std::is_member_function_pointer<M_REL>::value, "M_REL is not a member function.");
    static_assert(std::is_member_function_pointer<M_ACQ>::value,"M_ACQ is not a member function.");
    assert(nullptr!=rel&&nullptr!=acq);
    auto p_res=std::addressof(const_cast<typename no_const<RES>::type&>(res));
    return raii(std::bind(rel, p_res), std::bind(acq, p_res), default_com);
}

template<typename RES, typename M_REL>
raii make_raii(RES & res, M_REL rel, bool default_com = true)noexcept {
    static_assert(std::is_class<RES>::value, "RES is not a class or struct type.");
    static_assert(std::is_member_function_pointer<M_REL>::value, "M_REL is not a member function.");
    assert(nullptr != rel);
    auto p_res=std::addressof(const_cast<typename no_const<RES>::type&>(res));
    return raii(std::bind(rel, p_res), []{}, default_com);
}


