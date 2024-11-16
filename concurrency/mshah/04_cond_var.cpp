/*

----------- Condition Variable Introduction -----------

std::condition_variable is a synchronization primitive used with a std::mutex 
to block one or more threads until another thread 
both modifies a shared variable (the condition) and notifies the std::condition_variable.

The thread that intends to modify the shared variable must:
    Acquire a std::mutex (typically via std::lock_guard).
    Modify the shared variable while the lock is owned.
    Call notify_one or notify_all on the std::condition_variable (can be done after releasing the lock). 

Any thread that intends to wait on a std::condition_variable must:
    Acquire a std::unique_lock<std::mutex> on the mutex used to protect the shared variable.
    Do one of the following: 
        Check the condition, in case it was already updated and notified.
        Call wait, wait_for, or wait_until on the std::condition_variable (atomically releases the mutex and suspends thread execution until the condition variable is notified, a timeout expires, or a spurious wakeup occurs, then atomically acquires the mutex before returning).
        Check the condition and resume waiting if not satisfied. 

----------- Classes and Functions -----------

class unique_lock; // can use with condition variables

class condition_variable;
    wait();
    wait_for();
    notify_one();

*/

#include <iostream>
#include <string>

#include <thread>
#include <mutex>
#include <condition_variable>

// mutex
std::mutex g_lock; 

// condition & shared variable
bool ready = false;
int result = 0;

// condition variable
std::condition_variable cond_var;

int main(){

    std::thread worker([&](){
        // 1. require lock
        std::unique_lock<std::mutex> lock(g_lock);
        // 2. change data and condition
        result = 123 + 456 + 789;
        ready = true;
        std::this_thread::sleep_for(std::chrono::seconds(5));
        std::cout << "Work Complete\n";
        // 3. notify
        cond_var.notify_one();
    });

    std::thread reporter([&](){
        // 1. require lock
        std::unique_lock<std::mutex> lock(g_lock);
        
        // 2. check cond and wait for lock
        // or otherwise, cond_var.wait(lock, []{ return ready; });
        while(!ready){
            cond_var.wait(lock);
        }

        // 3. use the data
        std::cout << "Reporter, result = " << result << std::endl;
    });

    reporter.join();
    worker.join();

    return 0;
}