#include <unistd.h>
#include <stddef.h>

typedef block_header* p_block;

struct block_header{
    size_t size; // size_t 作为无符号整数，表示内存块大小
    bool free;
};

// FIX: proper initialization should be applied
p_block memory_start;
void* memory_end;

void* malloc(size_t num_bytes){
    // 1. Find if available block exists
    p_block current_block = memory_start;
    p_block availble_block = nullptr;
    while((void*)current_block != memory_end){
        if(current_block->free && current_block->size >= num_bytes){
            availble_block = current_block;
            current_block->free = false;
            break;
        }
        current_block = current_block + sizeof(block_header) + current_block->size;
    }
    // 2. If not, lend new block from system
    if(availble_block == nullptr){
        if(sbrk(num_bytes + sizeof(block_header)) == (void*)(-1)){
            return nullptr;
        }
        availble_block = (p_block)memory_end;
        memory_end = memory_end + sizeof(block_header) + num_bytes;
        availble_block->free = false;
        availble_block->size = num_bytes;
    }
    return availble_block + sizeof(block_header);
}

void free(void* address){
    p_block block = (p_block)(address - sizeof(block_header));
    block->free = true;
}

int main(){
    return 0;
}
