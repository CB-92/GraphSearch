#include <thread>
#include <vector>
#include <functional>
#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

class Pool{
    private:
        int num_threads;
        vector<thread> pool;
        mutex queue_mutex, pool_mutex;
        queue<function<void()>> q;
        condition_variable condition;
        bool terminate_pool;

    public:
        Pool(){
            num_threads = thread::hardware_concurrency();
            terminate_pool = false;            
        }


        void fetch(){

            while(true){
                function<void()> job;
                
                unique_lock<mutex> lock(queue_mutex);

                while (q.empty() && !terminate_pool){
                        condition.wait(lock);
                }
                job = q.front();
                q.pop();
                job();                
            }

        }

        void start(){
            for (size_t i = 0; i < num_threads; i++){
                pool.push_back(thread(&Pool::fetch, this));
            }
        }

        void add_job(function<void()> new_job){
            unique_lock<mutex> lock(queue_mutex);
            q.push(new_job);
            condition.notify_one();
        }

        void shutdown(){
            {
                unique_lock<mutex> lock(pool_mutex);
                terminate_pool = true;
            }

            condition.notify_all();

            for( thread &th : pool){
                th.join();
            }

            pool.clear();
        }
};