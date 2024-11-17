/*

g++ -std=c++17 -o prog thread_pool.cpp  
time ./prog 

*/

#include <iostream>
#include <sstream>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>

class ThreadPool{
public:
    ThreadPool(size_t numThreads) : stop(false) {
        for (size_t i = 0; i < numThreads; i++) {
            workers.emplace_back([this] {
                while(true) {
                    std::unique_lock<std::mutex> lock(queueMutex);
                    condition.wait(lock, [this] { return stop || !tasks.empty(); });
                    if(stop && tasks.empty()) return;
                    
                    auto task = std::move(tasks.front());
                    tasks.pop();
                    lock.unlock(); // is a must to let other 'workers' to use the queue
                    task();
                }
            });
        }
    }

    template<class F>
    void enqueue(F&& task) {
        std::unique_lock<std::mutex> lock(queueMutex);
        tasks.emplace(std::forward<F>(task));
        lock.unlock(); //maybe not that important because it will be unlocked after notify
        condition.notify_one(); // It is OK that more notifies are given before cv.wait, they won't wait if a notify is given before that
    }

    ~ThreadPool() {
        std::unique_lock<std::mutex> lock(queueMutex);
        stop = true;
        lock.unlock(); // a must, otherwise it waits for workers to end, but workers wait for lock
        condition.notify_all(); // add up some more notifications to let each thread go another round and return
        std::cout << "Start destructing the thread pool" << std::endl;
        for (std::thread& worker : workers)
            worker.join();
    }

private:
    bool stop;
    std::mutex queueMutex;
    std::condition_variable condition;
    std::vector<std::thread> workers;
    std::queue<std::function<void()>> tasks;
};

std::string get_thread_id(){
    auto myid = std::this_thread::get_id();
    std::stringstream ss;
    ss << myid;
    return ss.str();
}

int main(){

    ThreadPool pool(3);
    std::cout << "Thread Pool Created.\n";

    std::cout << "Enqueue some tasks...\n";
    for (int i = 0; i < 8; i++) {
        pool.enqueue([i]{
            printf("Task %d %s executed by thread.\n", i, get_thread_id().c_str());
            std::this_thread::sleep_for(std::chrono::seconds(5));
        });
    }

    std::cout << "Start doing other things in main thread.\n";
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return 0;
}