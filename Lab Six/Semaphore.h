#ifndef SEMAPHORE_H
#define SEMAPHORE_H 
#include <mutex>
#include <condition_variable>
#include <chrono>
/*! \class Semaphore
    \brief A Semaphore Implementation
   Uses C++11 features such as mutex and condition variables to implement Semaphore
*/
class Semaphore
{
private:
    unsigned int m_uiCount; /*!< Holds the Semaphore count */
    std::mutex m_mutex;
    std::condition_variable m_condition;
public:
    Semaphore(unsigned int uiCount=0)
          : m_uiCount(uiCount) { };
    void Wait();
    /* template member functions have to be defined in the header */
    template< typename R,typename P >
    bool Wait(const std::chrono::duration<R,P>& crRelTime)
    {
        std::unique_lock< std::mutex > lock(m_mutex);
        if (!m_condition.wait_for(lock, crRelTime, [this]()->bool { return m_uiCount>0; })) {
            return false;
        }
        --m_uiCount;
        return true;
    }
    void Signal();
};
#endif
