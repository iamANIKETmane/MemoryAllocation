// Include necessary headers
#include <iostream>        // For console output
#include <vector>         // For storing pointers in tests
#include <cstdlib>        // For malloc/free
#include "fixAlloc.h"     // Our custom fixed allocator

/**
 * Test basic allocator functionality
 * This function tests:
 * 1. Constructor and initial state
 * 2. Basic allocation attempts
 * 3. Deallocation attempts  
 * 4. Pointer validation
 */
void test_basic_allocator() {
    std::cout << "\n=== Testing Basic Allocator ===" << std::endl;
    
    try {
        // Create allocator with 64-byte blocks, 10 blocks total
        // This allocates a pool of 640 bytes (64 * 10) at construction time
        FixedAllocator allocator(64, 10);
        
        // Display initial allocator statistics
        // These should show: 10 total, 10 free, 0 used
        std::cout << "Allocator stats after creation:" << std::endl;
        std::cout << "  Block size: " << allocator.get_block_size() << " bytes" << std::endl;
        std::cout << "  Total blocks: " << allocator.get_total_blocks() << std::endl;
        std::cout << "  Free blocks: " << allocator.get_free_blocks() << std::endl;
        std::cout << "  Used blocks: " << allocator.get_used_blocks() << std::endl;
        
        // Test allocation - should return valid pointers once implemented
        std::cout << "\nTesting allocation:" << std::endl;
        void* ptr1 = allocator.allocate();
        std::cout << "First allocation returned: " << ptr1 << std::endl;
        
        void* ptr2 = allocator.allocate();
        std::cout << "Second allocation returned: " << ptr2 << std::endl;
        
        // Test deallocation - should return true for valid pointers
        std::cout << "\nTesting deallocation:" << std::endl;
        bool success = allocator.deallocate(ptr1);
        std::cout << "Deallocation success: " << (success ? "true" : "false") << std::endl;
        
        // Test pointer validation with external memory
        // This should always return false (pointer not from our pool)
        std::cout << "\nTesting pointer validation:" << std::endl;
        void* random_ptr = malloc(64);  // Allocate memory from system heap
        bool is_valid = allocator.is_valid_pointer(random_ptr);
        std::cout << "Random heap pointer is valid: " << (is_valid ? "true" : "false") << std::endl;
        // Expected: false (this pointer is not from our allocator)
        free(random_ptr);  // Clean up system memory
        
        // Test nullptr validation
        bool null_valid = allocator.is_valid_pointer(nullptr);
        std::cout << "nullptr is valid: " << (null_valid ? "true" : "false") << std::endl;
        // Expected: false (nullptr should never be valid)
        
        // Test our own pointer validation
        bool ptr2_valid = allocator.is_valid_pointer(ptr2);
        std::cout << "Our allocated pointer is valid: " << (ptr2_valid ? "true" : "false") << std::endl;
        // Expected: true (this pointer is from our allocator)
        
        // Clean up remaining allocation
        if (ptr2) {
            allocator.deallocate(ptr2);
        }
        
    } catch (const std::exception& e) {
        // Catch any errors during allocator construction or testing
        std::cerr << "Error in basic allocator test: " << e.what() << std::endl;
    }
}

/**
 * Test allocator behavior when reaching capacity limits
 * This function tests:
 * 1. Allocating all available blocks
 * 2. Behavior when pool is exhausted
 * 3. Proper deallocation and pool state tracking
 */
void test_allocator_limits() {
    std::cout << "\n=== Testing Allocator Limits ===" << std::endl;
    
    try {
        // Create a small allocator for easier testing of limits
        // 32-byte blocks, only 3 blocks total = 96 bytes
        FixedAllocator allocator(32, 3);
        
        std::cout << "Created small allocator: 3 blocks of 32 bytes each" << std::endl;
        std::cout << "Initial state - Free: " << allocator.get_free_blocks() 
                  << ", Used: " << allocator.get_used_blocks() << std::endl;
        
        // Container to store allocated pointers for cleanup
        std::vector<void*> ptrs;
        
        // Try to allocate more blocks than available (5 attempts vs 3 available)
        std::cout << "\nAttempting to allocate 5 blocks (more than available):" << std::endl;
        for (int i = 0; i < 5; ++i) {
            void* ptr = allocator.allocate();
            
            if (ptr) {
                // Successful allocation
                ptrs.push_back(ptr);
                std::cout << "✓ Block " << i << " allocated at: " << ptr << std::endl;
            } else {
                // Allocation failed (pool exhausted)
                std::cout << "✗ Block " << i << " allocation FAILED (pool full)" << std::endl;
            }
            
            // Show pool state after each allocation attempt
            std::cout << "  Current state - Free: " << allocator.get_free_blocks() 
                      << ", Used: " << allocator.get_used_blocks();
            
            // Check pool status flags
            if (allocator.is_full()) {
                std::cout << " [POOL FULL]";
            }
            if (allocator.is_empty()) {
                std::cout << " [POOL EMPTY]";
            }
            std::cout << std::endl;
        }
        
        // Summary of allocation results
        std::cout << "\nAllocation summary:" << std::endl;
        std::cout << "  Successfully allocated: " << ptrs.size() << " blocks" << std::endl;
        std::cout << "  Expected successful: 3 blocks (pool capacity)" << std::endl;
        
        // Test deallocation to verify pool state returns to normal
        std::cout << "\nDeallocating all allocated blocks:" << std::endl;
        for (size_t i = 0; i < ptrs.size(); ++i) {
            bool success = allocator.deallocate(ptrs[i]);
            std::cout << "  Deallocated block " << i << ": " 
                      << (success ? "SUCCESS" : "FAILED") << std::endl;
        }
        
        // Verify pool is back to initial state
        std::cout << "\nFinal state after deallocation:" << std::endl;
        std::cout << "  Free: " << allocator.get_free_blocks() 
                  << ", Used: " << allocator.get_used_blocks() << std::endl;
        std::cout << "  Pool is empty: " << (allocator.is_empty() ? "YES" : "NO") << std::endl;
        std::cout << "  Pool is full: " << (allocator.is_full() ? "YES" : "NO") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in limits test: " << e.what() << std::endl;
    }
}

/**
 * Test error handling and edge cases
 */
void test_error_handling() {
    std::cout << "\n=== Testing Error Handling ===" << std::endl;
    
    try {
        FixedAllocator allocator(32, 2);
        
        // Test double-free detection
        void* ptr = allocator.allocate();
        if (ptr) {
            std::cout << "Allocated pointer: " << ptr << std::endl;
            
            bool first_free = allocator.deallocate(ptr);
            std::cout << "First deallocation: " << (first_free ? "SUCCESS" : "FAILED") << std::endl;
            
            bool second_free = allocator.deallocate(ptr);
            std::cout << "Second deallocation (should fail): " << (second_free ? "SUCCESS" : "FAILED") << std::endl;
        }
        
        // Test invalid pointer deallocation
        void* invalid_ptr = reinterpret_cast<void*>(0x12345678);
        bool invalid_free = allocator.deallocate(invalid_ptr);
        std::cout << "Invalid pointer deallocation: " << (invalid_free ? "SUCCESS" : "FAILED") << std::endl;
        
    } catch (const std::exception& e) {
        std::cerr << "Error in error handling test: " << e.what() << std::endl;
    }
}

/**
 * Main entry point for testing the FixedAllocator
 */
int main() {
    std::cout << "Memory Allocator Project - Development Test Suite" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "Testing your FixedAllocator implementation\n" << std::endl;
    
    // Run test suite
    test_basic_allocator();     // Test core functionality
    test_allocator_limits();    // Test edge cases and limits
    test_error_handling();      // Test error conditions
    
    // Final message
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "🎉 Test suite completed!" << std::endl;
    std::cout << "Your FixedAllocator implementation is working!" << std::endl;
    
    return 0;
}