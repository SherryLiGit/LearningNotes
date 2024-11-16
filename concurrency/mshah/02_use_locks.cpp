/*

----------- Mutex and LockGuards -----------

class mutex;
    lock() // usually not accessed directly
    try_lock()
    unlock()

class lock_guard; // RAII idea for mutex, lock when construct, unlock when destruct

*/

#include <thread>
#include <mutex>
#include <iostream>
#include <vector>

static int shared_value = 0;
std::mutex g_lock;

int main(){

    auto shared_increment = [](){
        std::lock_guard<std::mutex> lock_guard(g_lock);
        shared_value = shared_value + 1;
    };

    std::vector<std::thread> threads;
    for(int i = 0; i < 100; i++){
        threads.push_back(std::thread(shared_increment));
    }
    for(int i = 0; i < 100; i++){
        threads[i].join();
    }

    std::cout << "Shared value = " << shared_value << std::endl;

    return 0;
}
