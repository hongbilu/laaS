#include <Prestera_Driver/RWLock.hpp>

const std::thread::id RWLock::NULL_THREAD;

RWLock::RWLock(bool writeFirst):
    WRITE_FIRST(writeFirst),
    m_write_thread_id(),
    m_lockCount(0),
    m_writeWaitCount(0)
{
}
int RWLock::readLock() {
    // it's unique write status when equal, no need to lock
    if (std::this_thread::get_id() != this->m_write_thread_id)
    {
        int count;
        if (WRITE_FIRST)//for write first mode, need to check if threads number which are waiting for read is 0(m_writeWaitCount==0)
            do {
                while ((count = m_lockCount) == WRITE_LOCK_STATUS || m_writeWaitCount > 0);
            } while (!m_lockCount.compare_exchange_weak(count, count + 1));
        else
            do {
                while ((count = m_lockCount) == WRITE_LOCK_STATUS);
            } while (!m_lockCount.compare_exchange_weak(count, count + 1));
    }
    return m_lockCount;
}
int RWLock::readUnlock() {
    // it's unique write status when equal, no need to lock
    if (std::this_thread::get_id() != this->m_write_thread_id)
        --m_lockCount;
    return m_lockCount;
}
int RWLock::writeLock(){
    // it's unique write status when equal, avoid lock again
    if (std::this_thread::get_id() != this->m_write_thread_id){
        ++m_writeWaitCount;
        for(int zero=FREE_STATUS;!this->m_lockCount.compare_exchange_weak(zero,WRITE_LOCK_STATUS);zero=FREE_STATUS);
        --m_writeWaitCount;
        m_write_thread_id = std::this_thread::get_id();
    }
    return m_lockCount;
}
int RWLock::writeUnlock(){
    if(std::this_thread::get_id() != this->m_write_thread_id){
        throw std::runtime_error("writeLock/Unlock mismatch");
    }
    assert(WRITE_LOCK_STATUS == m_lockCount);
    m_write_thread_id = NULL_THREAD;
    m_lockCount.store(FREE_STATUS);
    return m_lockCount;
}



