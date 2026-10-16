/*
Author: Xinyu Jia（Felica）
Help received from:Eileen
Help given to:Eileen
License:MIT
*/
//Barrier.go Template Code
//Copyright (C) 2024 Dr. Joseph Kehoe

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
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

//--------------------------------------------
// Author: Joseph Kehoe (Joseph.Kehoe@setu.ie)
// Created on 30/9/2024
// Modified by:Xinyu Jia
// Description:
// A simple barrier implemented using mutex and unbuffered channel
// Issues:
// None I hope
//1. Change mutex to atomic variable
//2. Make it a reusable barrier
//--------------------------------------------

package main

import (
	"fmt"
	"sync"
	"sync/atomic"
	"time"
)

// Place a barrier in this function --use ATOMIC and unbuffered channel
func doStuff(goNum int, arrived *int32, max int32, wg *sync.WaitGroup, theChan chan bool) bool {
	time.Sleep(time.Second)
	fmt.Println("Part A", goNum)

	atomic.AddInt32(arrived, 1)

	if atomic.LoadInt32(arrived) == max {
		//last goroutine to arrive - start chain signal
		theChan <- true
		<-theChan
	} else {
		//wait for chain signal
		<-theChan
		theChan <- true //pass signal to next waiting goroutine
	}

	atomic.AddInt32(arrived, -1)

	fmt.Println("PartB", goNum)
	wg.Done()
	return true
}

func main() {
	totalRoutines := int32(10)
	var arrived int32 = 0 //atomic 用int32

	var wg sync.WaitGroup
	wg.Add(int(totalRoutines))

	theChan := make(chan bool) //unbuffered channel for signalling

	for i := range totalRoutines {
		go doStuff(i, &arrived, totalRoutines, &wg, theChan)
	}
	wg.Wait()
}
