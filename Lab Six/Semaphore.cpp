#include "Semaphore.h"

void Semaphore::Wait()
{
      std::unique_lock< std::mutex > lock(m_mutex);
      m_condition.wait(lock,[this]()->bool{ return m_uiCount>0; });
      --m_uiCount;
}

void Semaphore::Signal()
{
      std::unique_lock< std::mutex > lock(m_mutex);
      ++m_uiCount;
      m_condition.notify_one();
}
