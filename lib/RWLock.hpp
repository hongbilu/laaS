#pragma once
#include <cstdlib>
#include <cassert>
#include <atomic>
#include <thread>
#include <Prestera_Driver/raii.hpp>
/*
 * atomic to implent read/write lock, unique write, share read, forbidden copy constructor and operator '='
 * when WRITE_FIRST is true, if there is thread waiting for read(m_writeWaitCount>0), writing thread will get the lock first
 * support nest lock
 * readLock/Unlock to implement shared read lock/unlock, no limit for thread number
 * writeLock/Unlock to implement unique write lock/unlock, only allow one thread to write in
 * when there is thread reading, writing thread suspends, when writing threading running, all the reading thread will be suspended
 */
class RWLock {
#define WRITE_LOCK_STATUS -1
#define FREE_STATUS 0
private:
    static const std::thread::id NULL_THREAD;
    const bool WRITE_FIRST;
    std::thread::id m_write_thread_id;
    std::atomic_int m_lockCount;
    std::atomic_uint m_writeWaitCount;
public:
    RWLock(const RWLock&) = delete;
    RWLock& operator=(const RWLock&) = delete;
    //RWLock& operator=(const RWLock&) volatile = delete;
    RWLock(bool writeFirst=true);
    virtual ~RWLock()=default;
    int readLock();
    int readUnlock();
    int writeLock();
    int writeUnlock();
    raii read_guard()const noexcept{
        return make_raii(*this, &RWLock::readUnlock, &RWLock::readLock);
    }
    raii write_guard()noexcept{
        return make_raii(*this, &RWLock::writeUnlock, &RWLock::writeLock);
    }
};


