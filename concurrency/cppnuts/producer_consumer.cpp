/*

The producer-consumer, or bounded-buffer problem

*/

#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <deque>

bool stopped = false;
std::mutex g_lock;
std::condition_variable cond_var;
std::deque<int> buffer;
const unsigned int maxBufferSize = 5;

void producer(int val){
    while(val){
        std::unique_lock<std::mutex> lockguard(g_lock);
        cond_var.wait(lockguard, [](){ return buffer.size() < maxBufferSize; });
        buffer.push_back(val);
        std::cout << "Produced: " << val << std::endl;
        val--;
        cond_var.notify_one();
    }
    stopped = true;
}

void consumer(){
    while(true){
        std::unique_lock<std::mutex> lockguard(g_lock);
        cond_var.wait(lockguard, [](){ return stopped || buffer.size() > 0; });
        if(stopped && buffer.size() == 0) break;
        int val = buffer.back();
        buffer.pop_back();
        std::cout << "Consumed: " << val << std::endl;
        cond_var.notify_one();
    }
}

int main(){
    std::thread t1(producer, 20);
    std::thread t2(consumer);

    t1.join();
    t2.join();

    return 0;
}