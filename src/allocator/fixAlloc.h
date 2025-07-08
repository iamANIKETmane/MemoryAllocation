// Enhanced fixAlloc.h
#pragma once

#include <cstddef>
#include <cstdint>
#include <vector>
#include <atomic>
#include <mutex>
#include <chrono>

/**
 * Production-ready Fixed-size memory allocator
 * Features:
 * - O(1) allocation/deallocation using free list
 * - Thread safety with lock-free operations
 * - Memory debugging and leak detection
 * - Performance monitoring and statistics
 */
class FixedAllocator {
public:
    // Construction with enhanced options
    struct Config {
        size_t block_size = 64;
        size_t num_blocks = 1000;
        bool enable_debugging = true;
        bool enable_thread_safety = true;
        bool zero_on_alloc = false;      // Zero memory on allocation
        bool poison_on_free = true;      // Fill with pattern on free
        size_t alignment = sizeof(void*);
    };
    
    explicit FixedAllocator(const Config& config);
    FixedAllocator(size_t block_size, size_t num_blocks); // Legacy constructor
    ~FixedAllocator();
    
    // Core allocation interface (optimized)
    void* allocate();
    bool deallocate(void* ptr);
    
    // Memory information and validation
    bool is_valid_pointer(void* ptr) const;
    size_t get_block_size() const { return block_size_; }
    size_t get_total_blocks() const { return num_blocks_; }
    
    // Thread-safe statistics
    size_t get_free_blocks() const;
    size_t get_used_blocks() const;
    bool is_full() const;
    bool is_empty() const;
    
    // Performance monitoring
    struct Statistics {
        std::atomic<uint64_t> total_allocations{0};
        std::atomic<uint64_t> total_deallocations{0};
        std::atomic<uint64_t> allocation_failures{0};
        std::atomic<uint64_t> double_free_attempts{0};
        std::atomic<uint64_t> invalid_free_attempts{0};
        std::atomic<size_t> peak_usage{0};
        std::atomic<size_t> current_usage{0};
        
        // Timing statistics (nanoseconds)
        std::atomic<uint64_t> total_alloc_time{0};
        std::atomic<uint64_t> total_free_time{0};
        std::atomic<uint64_t> max_alloc_time{0};
        std::atomic<uint64_t> max_free_time{0};
    };
    
    const Statistics& get_statistics() const { return stats_; }
    void reset_statistics();
    void print_performance_report() const;
    
    // Memory debugging features
    bool validate_heap() const;
    std::vector<void*> find_leaks() const;
    size_t detect_corruption() const;
    void enable_debugging(bool enable) { debug_enabled_ = enable; }
    
    // Thread safety
    bool is_thread_safe() const { return thread_safe_enabled_; }
    void set_thread_safe(bool enable);

private:
    // Optimized free block management using free list
    struct FreeBlock {
        FreeBlock* next;
    };
    
    // Configuration
    size_t block_size_;
    size_t num_blocks_;
    size_t alignment_;
    bool debug_enabled_;
    bool thread_safe_enabled_;
    bool zero_on_alloc_;
    bool poison_on_free_;
    
    // Memory management
    uint8_t* memory_pool_;
    std::atomic<FreeBlock*> free_list_head_;  // Lock-free free list
    std::atomic<size_t> free_blocks_count_;
    
    // Thread safety
    mutable std::mutex debug_mutex_;  // Only for debugging operations
    
    // Performance monitoring
    mutable Statistics stats_;
    
    // Memory debugging
    static constexpr uint32_t ALLOC_MAGIC = 0xDEADBEEF;
    static constexpr uint32_t FREE_MAGIC = 0xFEEDFACE;
    static constexpr uint8_t POISON_BYTE = 0xDD;
    
    struct DebugHeader {
        uint32_t magic;
        size_t block_index;
        std::chrono::steady_clock::time_point alloc_time;
        uint32_t checksum;
    };
    
    // Helper methods
    void initialize_free_list();
    FreeBlock* pop_free_block();
    void push_free_block(FreeBlock* block);
    
    size_t ptr_to_block_index(void* ptr) const;
    void* block_index_to_ptr(size_t index) const;
    
    // Debugging helpers
    DebugHeader* get_debug_header(void* user_ptr) const;
    void* get_user_ptr(DebugHeader* header) const;
    uint32_t calculate_checksum(const DebugHeader* header) const;
    void poison_memory(void* ptr, size_t size) const;
    bool is_poisoned(void* ptr, size_t size) const;
    
    // Performance helpers
    class ScopedTimer {
    public:
        ScopedTimer(std::atomic<uint64_t>& total, std::atomic<uint64_t>& max_time);
        ~ScopedTimer();
    private:
        std::chrono::steady_clock::time_point start_;
        std::atomic<uint64_t>& total_time_;
        std::atomic<uint64_t>& max_time_;
    };
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
