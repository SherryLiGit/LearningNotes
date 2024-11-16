/*

g++ 05_async_n_future.cpp -std=c++17 -o prog 

----------- Async and Future -----------

* an abstraction of creating and join threads.
  std::async(func, args) performs like creating a thread,
  whereas std::future<int>::get() performs like a join.

*/

#include <iostream>
#include <future>
#include <chrono>

bool background_loader(){
    for(int i = 0; i < 4; i++){
        std::cout << "Loading in the background...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(300));
    }
    return true;
}

int main(){
    std::future<bool> backgound_thread = std::async(std::launch::async, background_loader);

    while(true){
        std::cout << "Main thread running...\n";
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
        std::future_status status = backgound_thread.wait_for(std::chrono::milliseconds(1));
        if(status == std::future_status::ready){
            std::cout << "Data is ready. Press any key to end.";
            std::cin.get();
            break;
        }
    }
}