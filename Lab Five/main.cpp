/* main.cpp --- 
 * 
 * Filename: main.cpp
 * Description: Dining Philosophers problem solved with a hand-written
 *              Semaphore class (C++11 mutex + condition_variable).
 *              Deadlock is prevented with the odd-even fork strategy.
 * Author: Joseph (original template)
 * Student: Xinyu Jia (implementation and deadlock fix)
 * Maintainer: Xinyu Jia
 * Created: Wed Oct 11 09:28:12 2023 (+0100)
 * Last-Updated: Sun Nov 08 2026
 *           By:Xinyu Jia 
 *     Update #: 14
 * 
 */

/* Commentary: 
 * 
 * 
 * 
 */

/* This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or (at
 * your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 */

/* Code: */

#include "Semaphore.h"
#include <iostream>
#include <thread>
#include <vector>
#include <deque>
#include <chrono>
#include <stdlib.h>     /* srand, rand */
#include <time.h>       /* time */

const int COUNT = 5;
const int THINKTIME=3;
const int EATTIME=5;
std::deque<Semaphore> forks;

// Protects std::cout: a chain of << is not atomic, so concurrent threads
// would otherwise interleave characters on the console.
std::mutex print_mutex;

void think(int myID){
  int seconds=rand() % THINKTIME + 1;
  {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << myID << " is thinking! "<<std::endl;
  }
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// Pick up both forks. Odd phil picks left first, even phil picks right first.
// This breaks the circular wait and prevents deadlock.
void get_forks(int philID){
  if (philID % 2 == 1) {
    forks[philID].Wait();
    forks[(philID+1)%COUNT].Wait();
  } else {
    forks[(philID+1)%COUNT].Wait();
    forks[philID].Wait();
  }
}

void put_forks(int philID){
  forks[philID].Signal();
  forks[(philID+1)%COUNT].Signal();  
}

void eat(int myID){
  int seconds=rand() % EATTIME + 1;
  {
    std::lock_guard<std::mutex> lock(print_mutex);
    std::cout << myID << " is chomping! "<<std::endl;
  }
  std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

// One philosopher: think, pick up both forks, eat, put both forks down.
void philosopher(int id){
  while(true){
    think(id);
    get_forks(id);
    eat(id);
    put_forks(id);
  }//while  
}//philosopher



int main(void){
  srand (time(NULL)); // initialize random seed: 

  // Initialise each fork semaphore to 1 (fork is available)
  for (int i = 0; i < COUNT; i++) {
    forks.emplace_back(1);
  }

  std::vector<std::thread> vt(COUNT);
  int id=0;
  for(std::thread& t: vt){
    t=std::thread(philosopher,id++);
  }
  /**< Join the philosopher threads with the main thread */
  for (auto& v :vt){
      v.join();
  }
  return 0;
}

/* main.cpp ends here */
