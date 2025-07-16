# Fixed Memory Allocator

A simple fixed-size memory allocator implementation in C++ for learning memory management concepts.

## What it does

This allocator pre-allocates a large memory pool and divides it into fixed-size blocks. It uses a bitmap to track which blocks are free or used.

**Key features:**
- Fixed-size block allocation (no fragmentation)
- Bitmap-based free block tracking
- Pointer validation 
- Double-free detection
- Basic statistics (free/used blocks)

## Files

- `fixAlloc.h` - Header file with class declaration
- `fixAlloc.cpp` - Implementation of the allocator
- `main.cpp` - Test program demonstrating usage

## How to build

```bash
g++ -std=c++11 fixAlloc.cpp main.cpp -o test
./test
```

## Basic usage

```cpp
#include "fixAlloc.h"

// Create allocator: 64-byte blocks, 10 total blocks
FixedAllocator allocator(64, 10);

// Allocate memory
void* ptr1 = allocator.allocate();
void* ptr2 = allocator.allocate();

// Check how many blocks are used
std::cout << "Used blocks: " << allocator.get_used_blocks() << std::endl;

// Free memory
allocator.deallocate(ptr1);
allocator.deallocate(ptr2);
```

## How it works

1. Constructor allocates one large memory pool using `posix_memalign`
2. Pool is divided into equal-sized blocks
3. A `std::vector<bool>` bitmap tracks free (false) vs used (true) blocks
4. `allocate()` finds first free block and marks it used
5. `deallocate()` validates pointer and marks block as free

## Limitations

- All allocations must be ≤ block size
- Linear search for free blocks (O(n) allocation time)
- Single-threaded only
- POSIX systems only (Linux/macOS)

## Learning objectives

This project demonstrates:
- Custom memory management
- Pointer arithmetic and validation
- Bitmap data structures
- C++ RAII principles
- Error handling in systems code

## License

MIT License - use for educational purposes.