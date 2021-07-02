#include <vector>
#include <iostream>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <stdexcept>
#include "../Node.hpp"

#ifndef POOL_HPP
#define POOL_HPP

#define LOG(msg){ \
    cout << "[" << th_id << "] " << msg << "\n";\
}

using namespace std;

class Pool {
    private:
        queue<int> items;
        mutex items_mux;
        condition_variable is_not_ready;
        function<int (int)> task;   
        promise<int> prom;
        vector<thread> workers;
        bool stop;
        function<void (int)> thread_fun = [&](int th_id){
            for(;;)
                {
                    int el;

                    {
                        unique_lock<std::mutex> lock(items_mux);
                        is_not_ready.wait(lock,
                            [&]{ return stop || items.empty();}
                        );

                        if(stop && items.empty())
                            return;

                        el = move(items.front());
                        items.pop();
                    }

                    task(el);
                }
        };
    public:
         Pool(function<int (int)> task, int thread_num);
         ~Pool();
         void add_item(int item);
};

//The constructor spawns thread_num threads
inline Pool::Pool(function<int (int)> wf, int thread_num) : stop(false), task(wf){
    for (int i = 0; i < thread_num; i++){
        workers.emplace_back(thread(thread_fun, i));
    }
}

// the destructor joins all threads
inline Pool::~Pool(){
     {
        unique_lock<mutex> lock(items_mux);
        stop = true;
    }
    is_not_ready.notify_all();
    for(thread &worker: workers)
        worker.join();
}

inline void Pool::add_item(int item){
    {
        unique_lock<mutex> lock(items_mux);

        // don't allow enqueueing after stopping the pool
        if(stop)
            throw std::runtime_error("add_items on stopped Pool");
        
        items.emplace(item);
    }
    is_not_ready.notify_one();    
}

#endif