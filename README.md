# Memory Allocation Project

A fixed-size memory allocator with lock-free message queue implementation.

## Project Structure

```
MemoryAllocation/
├── src/
│   ├── allocator/          # Fixed-size allocator implementation
│   │   ├── fixAlloc.h
│   │   └── fixAlloc.cpp
│   ├── queue/              # Message queue implementations (TODO)
│   └── benchmarks/         # Performance benchmarks (TODO)
├── tests/                  # Unit tests (TODO)
├── docs/                   # Documentation
├── main.cpp               # Test/demo application
├── CMakeLists.txt         # Build configuration
└── README.md             # This file
```

## Recent Progress

### Latest Updates
- **🎉 allocate() method working!** Returns real memory addresses
- **✅ Complete build system** setup and working
- **✅ Comprehensive test suite** with detailed progress tracking
- **✅ Memory pool management** with proper alignment
- **✅ Bitmap-based block tracking** fully functional

### Current Test Results
```bash
./allocator_test
# Shows:
# - Memory pool creation: ✅ 640 bytes allocated
# - Block management: ✅ 10 blocks of 64 bytes each  
# - allocate() calls: ✅ Returns valid memory addresses
# - Statistics tracking: ✅ Free/used counts accurate
# - Pointer validation: ✅ Rejects invalid pointers
```

### Performance Characteristics (Current)
- **Memory Pool**: Pre-allocated at construction (no runtime malloc)
- **Allocation Time**: O(n) linear search for free blocks
- **Deallocation Time**: O(1) once implemented
- **Memory Overhead**: 1 bit per block + alignment padding
- **Thread Safety**: Single-threaded (atomic operations planned)

## Build Instructions

### Prerequisites (macOS)

```bash
# Install development tools
xcode-select --install

# Install CMake via Homebrew
brew install cmake
```

### Building

```bash
# Create build directory
mkdir build && cd build

# Configure with CMake
cmake ..

# Build the project
make

# Run the test application
./allocator_test
```

### Development with VS Code

1. Install required extensions:
   - C/C++ (Microsoft)
   - CMake Tools (Microsoft)

2. Open the project folder in VS Code
3. Use Cmd+Shift+P → "CMake: Configure"
4. Use Cmd+Shift+P → "CMake: Build" or click the build button

## Current Status

### ✅ Completed
- ✅ Project structure and build system (CMake + macOS + VS Code)
- ✅ Basic allocator class design with complete API
- ✅ Memory pool allocation and initialization
- ✅ Pointer validation logic (`is_valid_pointer()`)
- ✅ Statistics tracking (free/used blocks, pool state)
- ✅ Comprehensive test suite with detailed output
- ✅ Helper methods (bitmap management, index conversion)
- ✅ `allocate()` method implementation
- ✅ Working build system (compiles and runs successfully)

### 🚧 In Progress
- `deallocate()` method implementation
- Error handling and bounds checking
- Double-free detection

### 📋 TODO
- Complete deallocate() method
- Add thread safety (atomic operations)
- Implement lock-free message queue
- Write comprehensive unit tests (GTest integration)
- Performance benchmarking vs std::allocator
- Memory usage profiling
- Documentation and usage examples

## Usage Example

```cpp
#include "fixAlloc.h"

int main() {
    // Create allocator with 64-byte blocks, 1000 blocks total
    FixedAllocator allocator(64, 1000);
    
    // Check initial state
    std::cout << "Free blocks: " << allocator.get_free_blocks() << std::endl;  // 1000
    std::cout << "Block size: " << allocator.get_block_size() << std::endl;   // 64 (or aligned size)
    
    // Allocate some memory
    void* ptr1 = allocator.allocate();  // Returns valid address
    void* ptr2 = allocator.allocate();  // Returns different valid address
    
    if (ptr1 && ptr2) {
        std::cout << "Allocated successfully!" << std::endl;
        std::cout << "Used blocks: " << allocator.get_used_blocks() << std::endl;  // 2
        
        // Use the memory...
        memset(ptr1, 0x42, 64);  // Safe to write 64 bytes
        
        // Deallocate when done (TODO: implement deallocate first!)
        // allocator.deallocate(ptr1);
        // allocator.deallocate(ptr2);
    }
    
    return 0;
}
```

### Running the Test Suite

```bash
# Build and run the comprehensive test
mkdir build && cd build
cmake ..
make
./allocator_test

# Expected output shows:
# - Memory pool creation and statistics
# - Successful allocations with real addresses  
# - Pointer validation working correctly
# - Pool state tracking (empty/full detection)
# - Clear guidance on next implementation steps
```

## Design Goals

1. **O(1) Performance**: Constant time allocation/deallocation
2. **No Dynamic Allocation**: All memory pre-allocated at construction
3. **Real-Time Safe**: Suitable for real-time applications
4. **Thread Safe**: Lock-free operations where possible
5. **Memory Efficient**: Minimal metadata overhead

## Next Steps

### 🎯 Immediate Priority (This Week)
1. **Complete `deallocate()` method**
   - Validate input pointers
   - Update bitmap and statistics
   - Add double-free detection

2. **Add Error Handling**
   - Bounds checking in helper methods
   - Better error messages and debugging
   - Handle edge cases gracefully

3. **Optimize Performance**
   - Replace linear search with bit manipulation for O(1) allocation
   - Consider free-list approach for faster allocation

### 🚀 Medium Term (Next 2 Weeks)
1. **Thread Safety Implementation**
   - Convert bitmap to atomic operations
   - Lock-free allocation/deallocation
   - Multi-threaded stress testing

2. **Message Queue Implementation**
   - Lock-free circular buffer design
   - Integration with fixed allocator
   - Producer-consumer testing

### 📊 Long Term (Next Month)
1. **Performance Analysis**
   - Benchmark against std::allocator, malloc
   - Memory fragmentation analysis
   - Latency profiling under load

2. **Production Features**
   - Comprehensive unit test suite (GTest)
   - Memory debugging tools
   - Documentation and examples

## Contributing

This is a learning project. Feel free to:
- Suggest improvements
- Report bugs
- Contribute optimizations

## License

MIT License - see LICENSE file for details