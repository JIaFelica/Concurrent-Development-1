/*
Author: Xinyu Jia（Felica）
Help received from:Adam Eileen
Help given to:Eileen Adam
License:MIT
*/

package main

import (
	"context"
	"fmt"
	"log"
	"runtime"
	"sync"

	"golang.org/x/sync/semaphore"
)

func main() {
	ctx := context.Background()
	var (
		maxWorkers = runtime.GOMAXPROCS(0)
		sem        = semaphore.NewWeighted(int64(maxWorkers))
		out        = make([]int, 64)
		wg         sync.WaitGroup
	)

	for i := range out {
		if err := sem.Acquire(ctx, 1); err != nil {
			log.Printf("Failed to acquire semaphore: %v", err)
			break
		}
		wg.Add(1)
		go func(i int) {
			defer sem.Release(1)
			defer wg.Done()
			defer func() {
				if r := recover(); r != nil {
					log.Printf("task %d panic: %v", i, r)
				}
			}()
			out[i] = collatzSteps(i + 1)
		}(i)
	}
	wg.Wait()
	fmt.Println(out)
}

func collatzSteps(n int) (steps int) {
	if n <= 0 {
		panic("nonpositive input")
	}
	for ; n > 1; steps++ {
		if steps < 0 {
			panic("too many steps")
		}
		if n%2 == 0 {
			n /= 2
			continue
		}
		const maxInt = int(^uint(0) >> 1)
		if n > (maxInt-1)/3 {
			panic("overflow")
		}
		n = 3*n + 1
	}
	return steps
}
