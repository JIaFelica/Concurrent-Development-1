/*
Author: Xinyu Jia（Felica）
Help received from:Eileen
Help given to:Eileen
License:MIT
*/
package main

import (
	"fmt"
	"math/rand/v2"
	"sync"
	"time"
)

type barrier struct {
	theChan chan struct{}
	theLock sync.Mutex
	total   int
	count   int
}

func createBarrier(N int) barrier {
	theBarrier := barrier{
		theChan: make(chan struct{}),
		total:   N,
		count:   0,
	}
	return theBarrier
}

func (b *barrier) wait() {
	b.theLock.Lock()
	b.count++
	if b.count == b.total {

		fmt.Println("here")
		b.theLock.Unlock()
		for i := 0; i < b.total-1; i++ {
			b.theChan <- struct{}{}
		}
	} else {
		currentCount := b.count
		fmt.Println(currentCount)
		b.theLock.Unlock()
		<-b.theChan
	}
}

func WorkWithRendezvous(wg *sync.WaitGroup, Num int, theBarrier *barrier) bool {
	X := time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second)
	fmt.Println("Part A", Num)
	theBarrier.wait()
	fmt.Println("PartB", Num)
	wg.Done()
	return true
}

func main() {
	var wg sync.WaitGroup
	barrier := createBarrier(5)
	threadCount := 5
	wg.Add(threadCount)
	for N := range threadCount {
		go WorkWithRendezvous(&wg, N, &barrier)
	}
	wg.Wait()
}
