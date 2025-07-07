#include "fixAlloc.h"
#include <stdexcept>
#include <cstdlib>
#include <cassert>
#include <cstring>
#include <iostream>

FixedAllocator::FixedAllocator(size_t block_size, size_t num_blocks)
    : block_size_(block_size)
    , num_blocks_(num_blocks)
    , alignment_(sizeof(void*))  // Align to pointer size
    , free_blocks_count_(num_blocks)
{
    // Input validation
    if (block_size == 0 || num_blocks == 0) {
        throw std::invalid_argument("Block size and number of blocks must be > 0");
    }
    
    // Align block size to alignment boundary
    block_size_ = (block_size + alignment_ - 1) & ~(alignment_ - 1);
    
    // Allocate memory pool
    size_t total_size = block_size_ * num_blocks_;
    void* ptr = nullptr;
    if (posix_memalign(&ptr, alignment_, total_size) != 0) {
        throw std::bad_alloc();
    }
    memory_pool_ = static_cast<uint8_t*>(ptr);
    
    // Initialize memory (optional, for debugging)
    std::memset(memory_pool_, 0, total_size);
    
    // Initialize bitmap - all blocks start as free
    block_bitmap_.resize(num_blocks_, false);  // false = free
    
    std::cout << "FixedAllocator created: " << num_blocks_ 
              << " blocks of " << block_size_ 
              << " bytes each (total: " << total_size << " bytes)" << std::endl;
}

FixedAllocator::~FixedAllocator() {
    if (memory_pool_) {
        std::free(memory_pool_);
        memory_pool_ = nullptr;
    }
    
    std::cout << "FixedAllocator destroyed" << std::endl;
}

void* FixedAllocator::allocate() {
    // Find a free block
    size_t free_index = find_free_block();
    
    // Check if we found a free block
    if (free_index >= num_blocks_) {
        // No free blocks available
        return nullptr;
    }
    
    // Mark the block as used
    mark_block_used(free_index);
    
    // Return pointer to the block
    return block_index_to_ptr(free_index);
}

bool FixedAllocator::deallocate(void* ptr) {
    // TODO: Implement this
    // 1. Validate pointer using is_valid_pointer()
    // 2. Convert pointer to block index
    // 3. Mark block as free
    // 4. Return success/failure
    
    std::cout << "deallocate() called with ptr=" << ptr << " - TODO: implement" << std::endl;
    return false;
}

bool FixedAllocator::is_valid_pointer(void* ptr) const {
    if (!ptr || !memory_pool_) {
        return false;
    }
    
    // Check if pointer is within our memory pool
    uint8_t* byte_ptr = static_cast<uint8_t*>(ptr);
    if (byte_ptr < memory_pool_ || byte_ptr >= memory_pool_ + (block_size_ * num_blocks_)) {
        return false;
    }
    
    // Check if pointer is aligned to block boundary
    size_t offset = byte_ptr - memory_pool_;
    return (offset % block_size_) == 0;
}

size_t FixedAllocator::get_free_blocks() const {
    return free_blocks_count_;
}

size_t FixedAllocator::get_used_blocks() const {
    return num_blocks_ - free_blocks_count_;
}

bool FixedAllocator::is_full() const {
    return free_blocks_count_ == 0;
}

bool FixedAllocator::is_empty() const {
    return free_blocks_count_ == num_blocks_;
}

// TODO: Implement these private helper methods
size_t FixedAllocator::ptr_to_block_index(void* ptr) const {
    uint8_t* byte_ptr = static_cast<uint8_t*>(ptr);
    return (byte_ptr - memory_pool_) / block_size_;
}

void* FixedAllocator::block_index_to_ptr(size_t index) const {
    return memory_pool_ + (index * block_size_);
}

size_t FixedAllocator::find_free_block() const {
    // TODO: Implement efficient free block finding
    // Linear search for now (you can optimize later)
    for (size_t i = 0; i < num_blocks_; ++i) {
        if (!block_bitmap_[i]) {  // false = free
            return i;
        }
    }
    return num_blocks_;  // No free block found
}

void FixedAllocator::mark_block_used(size_t index) {
    // TODO: Add bounds checking
    block_bitmap_[index] = true;
    --free_blocks_count_;
}

void FixedAllocator::mark_block_free(size_t index) {
    // TODO: Add bounds checking and double-free detection
    block_bitmap_[index] = false;
    ++free_blocks_count_;
}

bool FixedAllocator::is_block_free(size_t index) const {
    return !block_bitmap_[index];
}
