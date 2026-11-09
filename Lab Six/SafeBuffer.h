/*
 * Filename: SafeBuffer.h
 * Description: A bounded buffer of ints that several threads can use safely.
 * Author: Jia Xinyu
 * Module: Concurrent and Distributed Systems
 * Licence: GNU GPL v3 (see LICENSE)
 */
#ifndef SAFEBUFFER_H
#define SAFEBUFFER_H

#include "Semaphore.h"
#include <vector>

class SafeBuffer
{
private:
    std::vector<int> m_buffer;
    unsigned int m_uiSize;
    unsigned int m_uiIn;
    unsigned int m_uiOut;
    unsigned int m_uiCount;

    Semaphore m_spaces;    // how many free slots are left
    Semaphore m_items;     // how many items are stored
    Semaphore m_mutex;     // only one thread at a time inside the buffer

public:
    SafeBuffer(unsigned int uiSize = 10)
          : m_buffer(uiSize), m_uiSize(uiSize), m_uiIn(0), m_uiOut(0), m_uiCount(0),
            m_spaces(uiSize), m_items(0), m_mutex(1) { }

    // adds an item, blocks while the buffer is full
    void Add(int iItem)
    {
        m_spaces.Wait();
        m_mutex.Wait();
        m_buffer[m_uiIn] = iItem;
        m_uiIn = (m_uiIn + 1) % m_uiSize;
        ++m_uiCount;
        m_mutex.Signal();
        m_items.Signal();
    }

    // removes an item, blocks while the buffer is empty
    int Remove()
    {
        m_items.Wait();
        m_mutex.Wait();
        int iItem = m_buffer[m_uiOut];
        m_uiOut = (m_uiOut + 1) % m_uiSize;
        --m_uiCount;
        m_mutex.Signal();
        m_spaces.Signal();
        return iItem;
    }

    // how many items are in the buffer right now
    unsigned int Count()
    {
        m_mutex.Wait();
        unsigned int uiCount = m_uiCount;
        m_mutex.Signal();
        return uiCount;
    }

    unsigned int Capacity()
    {
        return m_uiSize;
    }
};

#endif
