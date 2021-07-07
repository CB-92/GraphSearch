#ifndef THREAD_POOL_HPP
#define THREAD_POOL_HPP

#include <vector>
#include <queue>
#include <memory>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <future>
#include <functional>
#include <atomic>
#include <stdexcept>
#include <list>
#include <future>
#include "../Node.hpp"

class ThreadPool {
public:
    using WorkerResult = pair<int, list<NodePtr>>;
    using WorkerResultPtr = shared_ptr<WorkerResult>;
    using NodeAnalyzer = function <WorkerResult (NodePtr, int)>;

    ThreadPool(size_t, NodeAnalyzer, int);
    void enqueue(list<NodePtr>);
    void terminate();
    int get_working_threads();
    bool finished();
    WorkerResult wait();
    ~ThreadPool();

private:
    // need to keep track of threads so we can join them
    std::vector<pair<WorkerResultPtr, std::thread>> workers;
    // the nodes queue
    list<NodePtr> _nodes;
    
    // synchronization
    std::mutex queue_mutex, exit_mutex;
    std::condition_variable queue_condition, finish_condition;
    atomic_bool stop;
    atomic_int working_threads;
    NodeAnalyzer _node_function;
    int _searched_value;
};
 
// the constructor just launches some amount of workers
inline ThreadPool::ThreadPool(size_t threads, NodeAnalyzer node_function, int searched_value)
    :   stop(false), working_threads(0), _node_function (node_function), _searched_value (searched_value) {
    for(size_t i = 0;i<threads;++i) {
        WorkerResultPtr w_result = shared_ptr<WorkerResult> (new WorkerResult(0, {}));
        auto task = packaged_task<void ()> ([this, w_result] (){
            bool stopMe = false;
            while (!stopMe) {
                NodePtr currNode = nullptr;

                {
                    std::unique_lock<std::mutex> lock(this->queue_mutex);
                    this->queue_condition.wait(lock,
                        [this]{return this->stop || !this->_nodes.empty();});
                    if(this->stop && this->_nodes.empty())
                        stopMe = true;
                    else {
                        working_threads++;
                        currNode = std::move(this->_nodes.front());
                        this->_nodes.pop_front();
                    }
                }
                if (!stopMe) {
                    auto tmp_result = _node_function (currNode, _searched_value);
                    working_threads--;
                    w_result->first += tmp_result.first;
                    w_result->second.splice (w_result->second.end(), tmp_result.second);
                    //cout << "res: " << w_result->first << "\tlen: " << w_result->second.size() << endl;
                }

                // Check if lock is needed
                finish_condition.notify_all();
            }
        });
        auto item = pair<WorkerResultPtr, thread> (w_result, thread(move(task)));
        workers.push_back (move(item));
    }
}

int ThreadPool::get_working_threads(){
    return working_threads;
}

bool ThreadPool::finished(){
    return ((working_threads==0) && _nodes.empty());
}

ThreadPool::WorkerResult ThreadPool::wait(){
    WorkerResult res;

    unique_lock<mutex> lock(exit_mutex);
    finish_condition.wait(lock, [&]{return finished();});

    // Collecting results
    for (auto &w : workers) {
        auto& w_res = w.first;
        res.first += w_res->first;
        res.second.splice (res.second.end(), w_res->second);
        w_res->first = 0;
        w_res->second = {};
    }

    return res;
}

// add new work item to the pool
void ThreadPool::enqueue(list<NodePtr> next_nodes) {
    _nodes.splice (_nodes.end(), next_nodes);
    queue_condition.notify_all();
}

void ThreadPool::terminate() {
    {
        std::unique_lock<std::mutex> lock(queue_mutex);
        stop = true;
    }
    queue_condition.notify_all();
    for(auto &worker: workers){
        if(worker.second.joinable()){
            worker.second.join();
        }
    }
        
}

// the destructor joins all threads
inline ThreadPool::~ThreadPool() {
    terminate();
}

#endif