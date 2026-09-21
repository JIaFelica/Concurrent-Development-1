/*
Author: Xinyu Jia（Felica）
Help received from:Adam Eileen
Help given to:Eileen Adam
License:MIT
*/
package main

import (
	"fmt"
	"math/rand/v2"
	"sync"
	"time"
)

var mu sync.Mutex
var cond = sync.NewCond(&mu)
var count int

const threadCount = 5

func WorkWithRendezvous(wg *sync.WaitGroup, Num int) bool {

	X := time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second)
	fmt.Println("Part A", Num)

	mu.Lock()
	count++
	if count == threadCount {

		cond.Broadcast()
	} else {

		cond.Wait()
	}
	mu.Unlock()

	fmt.Println("Part B", Num)
	wg.Done()
	return true
}

func main() {
	var wg sync.WaitGroup
	wg.Add(threadCount)

	for N := 0; N < threadCount; N++ {
		go WorkWithRendezvous(&wg, N)
	}
	wg.Wait()
	fmt.Println("All threads finished")
}
