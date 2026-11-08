# Concurrent-Development-1

Coursework repository for the Concurrent and Distributed Systems module.

## Contents

| Folder | Topic | Language |
|--------|-------|----------|
| `CD Lab1` | Barrier, mutex, semaphore, rendezvous, signalling and atomic operations | Go |
| `Lab Four` | Barrier implementation | Go |
| `Lab Five` | Dining Philosophers problem, solved with a hand-written semaphore and with channels | C++ and Go |

## Lab Five in detail

`Lab Five` contains two implementations of the same problem:

- **C++** (`main.cpp` + `Semaphore.h` + `Semaphore.cpp`) uses a hand-written
  `Semaphore` class built on `std::mutex` and `std::condition_variable`.
  Each fork is a binary semaphore.
- **Go** (`dinPhil.go`) uses one buffered channel of capacity 1 per fork,
  which behaves as a binary semaphore.

Both versions prevent the classic deadlock with the odd-even strategy: odd
philosophers pick up the left fork first, even philosophers pick up the right
fork first, which removes the circular-wait condition.

Build and run instructions, required libraries, known issues and the licence
for each lab are documented in that lab's own `README` file.

## Licence

The repository's own material is released under the MIT License (see
`LICENSE`). Each lab folder also carries the licence of its own source code;
the Lab Five implementation is a derivative of a GPL-licensed template and
therefore remains under the GNU General Public License v3.0.
