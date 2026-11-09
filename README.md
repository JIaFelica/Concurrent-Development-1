# Concurrent-Development-1

Coursework repository for the Concurrent and Distributed Systems module.

## Contents

| Folder | Topic | Language |
|--------|-------|----------|
| `CD Lab1` | Barrier, mutex, semaphore, rendezvous, signalling and atomic operations | Go |
| `Lab Four` | Barrier implementation | Go |
| `Lab Five` | Dining Philosophers problem, solved with a hand-written semaphore and with channels | C++ and Go |
| `Lab Six` | Bounded buffer (`SafeBuffer`) and event (`Event`), used by producers and consumers | C++ |

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

## Lab Six in detail

`Lab Six` implements two synchronisation classes from empty templates:

- `Event` (`Event.h`) - a manually-reset event: `Wait()` blocks while the
  flag is clear, `Signal()` sets it and wakes every waiting thread, and
  `Reset()` clears it again. It is used in `main.cpp` as a start gate that
  releases all worker threads at the same moment.
- `SafeBuffer` (`SafeBuffer.h`) - a bounded buffer of `int` protected by
  three semaphores: one for free slots, one for stored items and one binary
  semaphore for mutual exclusion. `Add()` blocks when the buffer is full and
  `Remove()` blocks when it is empty.

`main.cpp` starts three producers and two consumers on one `SafeBuffer`. The
producers are deliberately faster than the consumers, so the buffer fills
up and the producers are forced to wait; at the end the consumers drain it
back to empty. Every item carries its producer id, so the program compares
the sum of everything produced with the sum of everything consumed and
prints `PASS` or `FAIL`.

## Licence

The repository's own material is released under the MIT License (see
`LICENSE`). Each lab folder also carries the licence of its own source code;
the Lab Five and Lab Six implementations are derivatives of GPL-licensed
templates and therefore remain under the GNU General Public License v3.0.
