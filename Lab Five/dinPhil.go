// dinPhil.go --- Dining Philosophers in Go
//
// Module:     Concurrent and Distributed Systems - Lab Five
// Author:     Xinyu Jia
// Template:   Dr. Joseph Kehoe (original template code, 21/10/24)
// Created:    08/11/2026
// Licence:    GNU General Public License v3.0 (see LICENSE file)
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU General Public License as published by
// the Free Software Foundation, either version 3 of the License, or
// (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU General Public License for more details.
//
// You should have received a copy of the GNU General Public License
// along with this program.  If not, see <https://www.gnu.org/licenses/>.
//
// Notes on the completed version:
//   1. A README is included and documents build, run and known issues.
//   2. Full licence information is included (header above + LICENSE file).
//   3. All functions are commented.
//   4. The original version could deadlock. It is fixed here with the
//      odd-even fork acquisition strategy (see getForks).
//
// Design:
//   Each fork is a buffered channel of capacity 1, so it behaves exactly like
//   a binary semaphore: sending acquires the fork, receiving releases it.
//   A send blocks while the channel is full, which gives the waiting behaviour.

package main

import (
	"fmt"
	"math/rand/v2"
	"time"
)

const philCount = 5

// think simulates a philosopher thinking for a random time.
func think(index int) {
	var X time.Duration
	X = time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second) // wait random time amount
	fmt.Println("Phil: ", index, "was thinking")
}

// eat simulates a philosopher eating for a random time.
func eat(index int) {
	var X time.Duration
	X = time.Duration(rand.IntN(5))
	time.Sleep(X * time.Second) // wait random time amount
	fmt.Println("Phil: ", index, "was eating")
}

// getForks acquires both forks needed by philosopher index.
//
// Odd-numbered philosophers take the left fork first, even-numbered
// philosophers take the right fork first. This breaks the circular wait
// condition: if every philosopher grabbed their left fork first, all five
// could hold one fork and then wait forever for the second one (deadlock).
// With this asymmetric order at least one philosopher can always complete.
func getForks(index int, forks map[int]chan bool) {
	if index%2 == 1 {
		forks[index] <- true
		forks[(index+1)%philCount] <- true
	} else {
		forks[(index+1)%philCount] <- true
		forks[index] <- true
	}
}

// putForks releases both forks held by philosopher index.
func putForks(index int, forks map[int]chan bool) {
	<-forks[index]
	<-forks[(index+1)%philCount]
}

// doPhilStuff is the endless life loop of one philosopher:
// think, pick up both forks, eat, put both forks down.
func doPhilStuff(index int, forks map[int]chan bool) {
	for {
		think(index)
		getForks(index, forks)
		eat(index)
		putForks(index, forks)
	}
}

func main() {
	// One buffered channel per fork, capacity 1 = binary semaphore.
	forks := make(map[int]chan bool)
	for k := range philCount {
		forks[k] = make(chan bool, 1)
	}

	// Start one goroutine per philosopher.
	for N := range philCount {
		go doPhilStuff(N, forks)
	}

	// Block forever so main does not exit while philosophers are running.
	// The program is stopped with Ctrl+C.
	select {}
}
