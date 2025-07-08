#pragma once

#include <cstddef>
#include <string>
#include <memory>

/**
 * Abstract base class for all memory allocators
 * Provides common interface and statistics tracking
 */
class IAllocator {
public:
    virtual ~IAllocator() = default;
    
    // Core allocation interface
    virtual void* allocate(size_t size) = 0;
    virtual void* allocate(size_t size, size_t alignment) = 0;
    virtual bool deallocate(void* ptr) = 0;
    virtual bool deallocate(void* ptr, size_t size) = 0;  // Size hint for optimization
    
    // Memory information
    virtual bool owns(void* ptr) const = 0;
    virtual size_t get_allocation_size(void* ptr) const = 0;
    virtual size_t get_usable_size(void* ptr) const = 0;
    
    // Statistics and monitoring
    virtual size_t get_total_allocated() const = 0;
    virtual size_t get_total_free() const = 0;
    virtual size_t get_peak_usage() const = 0;
    virtual size_t get_allocation_count() const = 0;
    virtual size_t get_deallocation_count() const = 0;
    
    // Allocator metadata
    virtual std::string get_name() const = 0;
    virtual std::string get_stats_string() const = 0;
    
    // Advanced features
    virtual bool supports_alignment() const { return false; }
    virtual bool supports_realloc() const { return false; }
    virtual bool is_thread_safe() const { return false; }
    
    // Optional: realloc-like functionality
    virtual void* reallocate(void* ptr, size_t old_size, size_t new_size) {
        (void)ptr; (void)old_size; (void)new_size;
        return nullptr;  // Not supported by default
    }
    
    // Memory validation and debugging
    virtual bool validate_heap() const { return true; }
    virtual void print_debug_info() const {}
};

/**
 * RAII wrapper for automatic deallocation
 */
template<typename T>
class AllocatorPtr {
public:
    AllocatorPtr(T* ptr, IAllocator* allocator) 
        : ptr_(ptr), allocator_(allocator) {}
    
    ~AllocatorPtr() {
        if (ptr_ && allocator_) {
            allocator_->deallocate(ptr_);
        }
    }
    
    // Move semantics
    AllocatorPtr(AllocatorPtr&& other) noexcept 
        : ptr_(other.ptr_), allocator_(other.allocator_) {
        other.ptr_ = nullptr;
        other.allocator_ = nullptr;
    }
    
    AllocatorPtr& operator=(AllocatorPtr&& other) noexcept {
        if (this != &other) {
            if (ptr_ && allocator_) {
                allocator_->deallocate(ptr_);
            }
            ptr_ = other.ptr_;
            allocator_ = other.allocator_;
            other.ptr_ = nullptr;
            other.allocator_ = nullptr;
        }
        return *this;
    }
    
    // Delete copy operations
    AllocatorPtr(const AllocatorPtr&) = delete;
    AllocatorPtr& operator=(const AllocatorPtr&) = delete;
    
    T* get() const { return ptr_; }
    T* operator->() const { return ptr_; }
    T& operator*() const { return *ptr_; }
    explicit operator bool() const { return ptr_ != nullptr; }
    
private:
    T* ptr_;
    IAllocator* allocator_;
};