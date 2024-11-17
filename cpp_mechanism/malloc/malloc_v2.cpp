/*

UNDONE!!! (esp. extend_heap struct)

*/

#include <unistd.h>
#include <stddef.h>

typedef block_header* p_block;
#define BLOCK_SIZE 24

struct block_header{
    size_t size; // 8
    p_block p_next_block; // 8
    int free; // 4
    int padding; //4
    char data[1];
};

// FIX: proper initialization should be applied
void* memory_begin = nullptr;
void* memory_end = nullptr;

size_t align_8(size_t size){
    if(size & 0x7 == 0) return size;
    return ((size >> 3) + 1) << 3;
}

p_block find_block(size_t num_bytes){
    p_block current_block = memory_begin;
    while(current_block != nullptr){
        if(current_block->free && current_block->size >= num_bytes){
            return current_block;
        }
        current_block = current_block->p_next_block;
    }
    return nullptr;
}

p_block extend_heap(){
    if(sbrk(num_bytes + sizeof(block_header)) == (void*)(-1)){
        return nullptr;
    }
    available_block = (p_block)memory_end;
    memory_end = memory_end + sizeof(block_header) + num_bytes;
    availble_block->free = false;
    availble_block->size = num_bytes;
}

void* malloc(size_t num_bytes) {
    // 1. Find if available block exists
    num_bytes = align_8(num_bytes);
    p_block available_block = find_block(memory_begin);
    if (available_block != nullptr) {
        // 2. If so, check whether we can break it into small blocks
        
    } else {
        // 3. If not, lend new block from system

    }
    return available_block + sizeof(block_header);
}

void free(void* address){
    p_block block = (p_block)(address - sizeof(block_header));
    block->free = true;
}

int main(){
    return 0;
}
