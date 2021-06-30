#include <iostream>
#include <future>
#include <queue>
#include <vector>
#include <thread>
#include <mutex>
#include <condition_variable>
#include "AtomicType.hpp"

#define LOG(msg){ \
    cout << "[" << th_id << "] " << msg << "\n";\
}

using namespace std;

template<typename IN_T, typename OUT_T>
class Pool{
    public:
    using WorkerFunction = function<void (IN_T)>;


        Pool(WorkerFunction wf, int thread_num){
            /*close_pool.value = false;
            ready.value = false;
            work_fun = wf;
            active_threads.value = 0;*/

            for(int i=0; i<thread_num; i++){
                threads.push_back(thread(thread_fun, i));
            }
        }

        ~Pool(){}


        void add_item(IN_T item){
            items.push(item);
        }

        future<OUT_T> start(){
            auto at_lock = active_threads.acquire();
            active_threads.value = threads.size();
            cout << "at val (start) " << active_threads.value << "\n";
            at_lock.unlock();

            auto r_lock = ready.acquire();
            ready.value = true;
            r_lock.unlock();

            prom = promise<OUT_T>();
            is_not_empty.notify_all();

            return prom.get_future();
        }


        void close(){
            auto lock = close_pool.acquire();
            close_pool.value = true;
            lock.unlock();

            is_not_empty.notify_all();

            for(thread &th : threads){
                if(th.joinable())
                    th.join();
            }
        }

    private:
        queue<IN_T> items;
        mutex items_mux;
        condition_variable is_not_empty;
        WorkerFunction work_fun;   
        AtomicType<int> active_threads;
        promise<OUT_T> prom;

        vector<thread> threads;
        AtomicType<bool> close_pool, ready;

        function<void (int)> thread_fun = [&](int th_id){
            auto cp_lock = close_pool.acquire();
            while(!close_pool.value){
                cp_lock.unlock();
                LOG("at val (i) " << active_threads.value);

                unique_lock<mutex> lock(items_mux);

                {
                    auto at_lock = active_threads.acquire();
                    LOG("decreasing " << active_threads.value);
                    active_threads.value--;
                    LOG("at val (la) " << active_threads.value);
                }
    
                is_not_empty.wait(lock, [&]{
                    LOG("Waking up..");
                    auto r_lock = ready.acquire();
                    auto close_lock = close_pool.acquire();
                    LOG("Testing..");
                    LOG("Queue size: "<<items.size());
                    LOG("Ready? " << ready.value);
                    LOG("Close the pool? " << close_pool.value);

                    return ((!items.empty() && ready.value) || close_pool.value);
                });

                if (close_pool.value) {
                    return ;
                }

                {
                    auto at_lock = active_threads.acquire();
                    LOG("increasing " << active_threads.value);
                    active_threads.value++;
                    LOG("at val (la) " << active_threads.value);
                }

                while(!items.empty()){
                    // Get items from the queue
                    IN_T el = items.front();
                    items.pop();

                    lock.unlock();

                    //is_not_empty.notify_all();

                    work_fun(el);

                    lock.lock();
                }
                lock.unlock();

                auto at_lock = active_threads.acquire();
                LOG("decreasing " << active_threads.value);
                active_threads.value--;
                LOG("at val (la) " << active_threads.value);

                is_not_empty.notify_all();

                if (items.empty()) {
                    auto r_lock = ready.acquire();
                    ready.value = false;
                }

                // Check if there are no other active threads
                if(active_threads.value==0){
                    LOG("i'M THE LAST");
                    prom.set_value(0); // TODO: mettere valore giusto
                }

                LOG ("A: " << active_threads.value)
                at_lock.unlock();

                cp_lock.lock();
            }

        };
};