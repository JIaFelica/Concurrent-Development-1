/*
Author: Xinyu Jia（Felica）
Help received from:Adam Eileen
Help given to:Eileen Adam
License:MIT
*/
package main

import (
	"fmt"
	"sync"
	"time"
)

const totalRoutines = 10

var count int
var mu sync.Mutex
var cond = sync.NewCond(&mu)

func doStuff(goNum int, wg *sync.WaitGroup) bool {
	// Part A
	time.Sleep(1 * time.Second)
	fmt.Printf("Part A %d\n", goNum)

	mu.Lock()
	count++
	if count == totalRoutines {
		cond.Broadcast() 
	} else {
		cond.Wait() 
	mu.Unlock()

	// Part B
	fmt.Printf("Part B %d\n", goNum)
	wg.Done()
	return true
}

func main() {
	var wg sync.WaitGroup
	wg.Add(totalRoutines)
	for i := 0; i < totalRoutines; i++ {
		go doStuff(i, &wg)
	}
	wg.Wait()
}
