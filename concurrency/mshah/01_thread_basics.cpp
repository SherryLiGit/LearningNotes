/*

----------- Thread Basics -----------

* why concurrency: 
Gives the "potential" for more tasks to happen at once,
but the "order" matters and you might need to "share resources"
(e.g. we can do some calculation while network request is awaiting data)

* thread introduction:
Share the same code, data and kernel context;
Have its own logical control flow(registers including stack pointer and program counter)

* thread libraries:
Standard C++ library, Boost, pthread etc...

----------- Focused Properties -----------

class thread; // no copy (as no two objects may represent the same control flow)
    properties:
        id
    functions:
        (constructor) // passes the starting function and its arguments
        join // let the program wait until thread finishes
        get_id

class jthread; // automatically calls join in deconstructor

namespace std::this_thread:
    get_id();
    sleep_for();
        std::this_thread::sleep_for(std::chrono::microsecond(1));
        using namespace std::chrono_literals;
        std::this_thread::sleep_for(2000ms);

*/

#include <iostream>
#include <thread>
#include <vector>


void test(int x){
    std::cout << "Hello from thread " << std::this_thread::get_id() << " with argument x = " << x << std::endl;
}

int main(){

    std::vector<std::thread> myThreads;
    for(int i = 0; i < 10; i++){
        myThreads.push_back(std::thread(test, i));
    }
    
    std::cout << "Hello from main thread\n";
    for(int i = 0; i < 10; i++){
        myThreads[i].join();
    }

    return 0;
}

