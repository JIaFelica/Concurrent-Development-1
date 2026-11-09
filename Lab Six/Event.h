/*
 * Filename: Event.h
 * Description: A simple event, used to block threads until something happens.
 * Author: Jia Xinyu
 * Module: Concurrent and Distributed Systems
 * Licence: GNU GPL v3 (see LICENSE)
 */
#ifndef EVENT_H
#define EVENT_H

#include <mutex>
#include <condition_variable>

class Event
{
private:
    bool m_bSignal;
    std::mutex m_mutex;
    std::condition_variable m_condition;

public:
    Event(bool bInitial = false)
          : m_bSignal(bInitial) { }

    // blocks while the event is not signalled
    void Wait()
    {
        std::unique_lock< std::mutex > lock(m_mutex);
        m_condition.wait(lock, [this]()->bool { return m_bSignal; });
    }

    // sets the event and wakes up every thread waiting on it
    void Signal()
    {
        std::unique_lock< std::mutex > lock(m_mutex);
        m_bSignal = true;
        m_condition.notify_all();
    }

    // clears the event so the next Wait() blocks again
    void Reset()
    {
        std::unique_lock< std::mutex > lock(m_mutex);
        m_bSignal = false;
    }
};

#endif
