// Include necessary headers
#include <iostream>        // For console output
#include <vector>         // For storing pointers in tests
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
        // Currently returns nullptr because allocate() is not implemented
        std::cout << "\nTesting allocation:" << std::endl;
        void* ptr1 = allocator.allocate();
        std::cout << "First allocation returned: " << ptr1 << std::endl;
        // Expected: valid memory address (e.g., 0x7f8b2c000000)
        // Current: 0x0 (nullptr) until you implement allocate()
        
        void* ptr2 = allocator.allocate();
        std::cout << "Second allocation returned: " << ptr2 << std::endl;
        // Expected: different valid memory address
        // Current: 0x0 (nullptr) until you implement allocate()
        
        // Test deallocation - should return true for valid pointers
        std::cout << "\nTesting deallocation:" << std::endl;
        bool success = allocator.deallocate(ptr1);
        std::cout << "Deallocation success: " << (success ? "true" : "false") << std::endl;
        // Expected: true if ptr1 was valid, false if ptr1 was nullptr
        // Current: false because deallocate() returns false by default
        
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
 * Main entry point for testing the FixedAllocator
 * 
 * This program serves as a development test harness for the fixed allocator.
 * It will help you verify that your implementation works correctly as you
 * build the allocate() and deallocate() methods.
 * 
 * Current status:
 * - ✅ Constructor and destructor work
 * - ✅ Memory pool allocation works  
 * - ✅ Statistics and validation methods work
 * - ❌ allocate() method returns nullptr (TODO)
 * - ❌ deallocate() method returns false (TODO)
 * 
 * Expected output progression:
 * 1. Initially: allocations return nullptr, deallocations return false
 * 2. After implementing allocate(): allocations return valid addresses
 * 3. After implementing deallocate(): deallocations return true, stats update
 * 4. Final: all tests pass, pool management works correctly
 */
int main() {
    std::cout << "Memory Allocator Project - Development Test Suite" << std::endl;
    std::cout << "=================================================" << std::endl;
    std::cout << "This program tests your FixedAllocator implementation" << std::endl;
    std::cout << "as you develop the allocate() and deallocate() methods.\n" << std::endl;
    
    // Run test suite
    // These tests will show current status and guide your implementation
    test_basic_allocator();     // Test core functionality
    test_allocator_limits();    // Test edge cases and limits
    
    // Development guidance
    std::cout << "\n" << std::string(50, '=') << std::endl;
    std::cout << "🚀 NEXT IMPLEMENTATION STEPS:" << std::endl;
    std::cout << "1. Open src/allocator/fixAlloc.cpp" << std::endl;
    std::cout << "2. Implement the allocate() method:" << std::endl;
    std::cout << "   - Use find_free_block() to find available block" << std::endl;
    std::cout << "   - Mark block as used with mark_block_used()" << std::endl;
    std::cout << "   - Return pointer using block_index_to_ptr()" << std::endl;
    std::cout << "3. Implement the deallocate() method:" << std::endl;
    std::cout << "   - Validate pointer with is_valid_pointer()" << std::endl;
    std::cout << "   - Convert to block index with ptr_to_block_index()" << std::endl;
    std::cout << "   - Mark block as free with mark_block_free()" << std::endl;
    std::cout << "4. Re-run this test to see your progress!" << std::endl;
    std::cout << "\n💡 TIP: Implement allocate() first, then deallocate()" << std::endl;
    std::cout << "💡 TIP: Add debug prints to see what's happening" << std::endl;
    
    return 0;
}