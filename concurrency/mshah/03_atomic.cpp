/*

----------- Atomic Operation -----------

implemented by machine-level mechanisms like CAS

*/

#include <thread>
#include <iostream>
#include <vector>

static std::atomic<int> shared_value = 0;

int main(){

    auto shared_increment = [](){
        shared_value++; // should use operations supported by atomic
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
