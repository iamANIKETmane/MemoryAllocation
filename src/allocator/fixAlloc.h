#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>

class FixedAllocator {
public:
    FixedAllocator(size_t block_size, size_t num_blocks);
    ~FixedAllocator();
    
    // Delete copy constructor and assignment operator
    FixedAllocator(const FixedAllocator&) = delete;
    FixedAllocator& operator=(const FixedAllocator&) = delete;
    
    void* allocate();
    bool deallocate(void* ptr);
    bool is_valid_pointer(void* ptr) const;
    
    // Statistics methods
    size_t get_block_size() const { return block_size_; }
    size_t get_total_blocks() const { return num_blocks_; }
    size_t get_free_blocks() const;
    size_t get_used_blocks() const;
    bool is_full() const;
    bool is_empty() const;

private:
    // Helper methods
    size_t ptr_to_block_index(void* ptr) const;
    void* block_index_to_ptr(size_t index) const;
    size_t find_free_block() const;
    void mark_block_used(size_t index);
    void mark_block_free(size_t index);
    bool is_block_free(size_t index) const;
    
    // Member variables
    size_t block_size_;
    size_t num_blocks_;
    size_t alignment_;
    size_t free_blocks_count_;
    uint8_t* memory_pool_;
    std::vector<bool> block_bitmap_;  // This is the bitmap for tracking free/used blocks
};
// #pragma once

// #include <cstddef>
// #include <cstdint>
// #include <vector>

// class FixedAllocator {
// public:
//     FixedAllocator(size_t block_size, size_t num_blocks);
//     ~FixedAllocator();
    
//     // Delete copy constructor and assignment operator
//     FixedAllocator(const FixedAllocator&) = delete;
//     FixedAllocator& operator=(const FixedAllocator&) = delete;
    
//     void* allocate();
//     bool deallocate(void* ptr);
//     bool is_valid_pointer(void* ptr) const;
    
//     // Statistics methods
//     size_t get_block_size() const { return block_size_; }
//     size_t get_total_blocks() const { return num_blocks_; }
//     size_t get_free_blocks() const;
//     size_t get_used_blocks() const;
//     bool is_full() const;
//     bool is_empty() const;

// private:
//     // Helper methods
//     size_t ptr_to_block_index(void* ptr) const;
//     void* block_index_to_ptr(size_t index) const;
//     size_t find_free_block() const;
//     void mark_block_used(size_t index);
//     void mark_block_free(size_t index);
//     bool is_block_free(size_t index) const;
    
//     // Member variables
//     uint8_t* memory_pool_;
//     size_t block_size_;
//     size_t num_blocks_;
//     size_t alignment_;
//     std::vector<bool> block_bitmap_;
//     mutable size_t free_blocks_count_;
// };
