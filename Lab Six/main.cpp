/*
 * Filename: main.cpp
 * Description: Producers and consumers sharing one SafeBuffer.
 * Author: Jia Xinyu
 * Module: Concurrent and Distributed Systems
 * Licence: GNU GPL v3 (see LICENSE)
 */

#include "SafeBuffer.h"
#include "Event.h"
#include <iostream>
#include <thread>
#include <vector>
#include <atomic>
#include <mutex>
#include <string>
#include <chrono>
#include <cstdlib>
#include <ctime>

const int BUFFER_SIZE        = 5;
const int PRODUCER_COUNT     = 3;
const int CONSUMER_COUNT     = 2;
const int ITEMS_PER_PRODUCER = 4;
const int ITEMS_PER_CONSUMER = 6;

SafeBuffer buffer(BUFFER_SIZE);
Event startEvent;

std::mutex printMutex;
std::atomic<int> sumProduced(0);
std::atomic<int> sumConsumed(0);

void print(const std::string& strMessage)
{
    std::lock_guard< std::mutex > lock(printMutex);
    std::cout << strMessage << std::endl;
}

void producer(int iID)
{
    startEvent.Wait();

    for (int i = 0; i < ITEMS_PER_PRODUCER; i++) {
        int iItem = iID * 100 + i;

        // the producers work faster than the consumers, so the buffer fills up
        std::this_thread::sleep_for(std::chrono::milliseconds(20 + rand() % 30));

        buffer.Add(iItem);
        sumProduced += iItem;

        print("Producer " + std::to_string(iID) + " added   " + std::to_string(iItem)
              + "   (buffer " + std::to_string(buffer.Count()) + "/"
              + std::to_string(buffer.Capacity()) + ")");
    }
}

void consumer(int iID)
{
    startEvent.Wait();

    for (int i = 0; i < ITEMS_PER_CONSUMER; i++) {
        int iItem = buffer.Remove();
        sumConsumed += iItem;

        print("Consumer " + std::to_string(iID) + " removed " + std::to_string(iItem)
              + "   (buffer " + std::to_string(buffer.Count()) + "/"
              + std::to_string(buffer.Capacity()) + ")");

        std::this_thread::sleep_for(std::chrono::milliseconds(80 + rand() % 80));
    }
}

int main()
{
    srand(time(NULL));

    std::cout << "Buffer size " << buffer.Capacity() << ", "
              << PRODUCER_COUNT << " producers and "
              << CONSUMER_COUNT << " consumers" << std::endl;

    std::vector<std::thread> threads;

    for (int i = 1; i <= PRODUCER_COUNT; i++) {
        threads.push_back(std::thread(producer, i));
    }
    for (int i = 1; i <= CONSUMER_COUNT; i++) {
        threads.push_back(std::thread(consumer, i));
    }

    // all the threads are waiting on the event, release them together
    std::cout << "Releasing the start event" << std::endl;
    startEvent.Signal();

    for (unsigned int i = 0; i < threads.size(); i++) {
        threads[i].join();
    }

    std::cout << "-----------------------------------" << std::endl;
    std::cout << "Sum produced: " << sumProduced << std::endl;
    std::cout << "Sum consumed: " << sumConsumed << std::endl;

    if (sumProduced == sumConsumed && buffer.Count() == 0) {
        std::cout << "PASS" << std::endl;
    } else {
        std::cout << "FAIL" << std::endl;
    }

    return 0;
}
