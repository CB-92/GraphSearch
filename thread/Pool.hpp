#include <thread>
#include <vector>
#include <condition_variable>
#include <mutex>
#include <queue>
#include <functional>
#include <future>
#include <atomic>
#include "../Node.hpp"
#include "SynchSet.hpp"
#include "pthreadBFS.cpp"

#ifndef POOL_HPP
#define POOL_HPP

using namespace std;

typedef struct {
    queue<int> level;
    condition_variable cv;
    mutex mux;
} critical_region;

class Pool{
    private:
        int thread_num;
        vector<thread> vec_of_threads;
        shared_ptr<critical_region> curr, next;
        promise<int> prom;
        atomic_int occurrences;
        SynchSet<int> visited_nodes;
        Graph& graph;
        int target_value;
        atomic_int active_threads;
        atomic_bool change_level;
        atomic_bool visit_completed;

        std::function<void(Node& n, queue<int>& target_level)> check_item = [&](Node& n, queue<int>& target_level){
            cout << "Consuming node " << n.get_id() <<"\n";

            if(!visited_nodes.contains(n.get_id())){
    
                if (n.get_value() == target_value){
                    occurrences++;
                }
                
                visited_nodes.add(n.get_id());
                //cout << "Pushing "<< n.get_adj_list().size() <<" elements\n";
                for(int index : n.get_adj_list()){
                    target_level.push(index);
                }
            }      
        };
        
        //Threads must take items to work with from the queue
        std::function<void()> task = [&](){
            // This thread waits for the first element
            while(!visit_completed){
                // Stating new level

                unique_lock<mutex> curr_lock(curr->mux);
                active_threads--;
                curr->cv.wait(curr_lock, [&]{return !curr->level.empty();}); // Wait until the queue contains at least one item

                active_threads++;
                
                while(!curr->level.empty()){
                    int node_id = curr->level.front();
                    curr->level.pop();

                    //Release lock
                    curr_lock.unlock();
                    curr->cv.notify_all();
                    Node n = graph.get_node_at(node_id);

                    check_item(n, next->level);
                    curr_lock.lock();
                }
                curr_lock.unlock();
                
                change_level = false;
                unique_lock<mutex> next_lock(next->mux);
                if(active_threads>1){
                    auto change_lev_ptr = &change_level;
                    active_threads--;
                    next->cv.wait(next_lock, [change_lev_ptr]{return change_lev_ptr;});
                    active_threads++;
                } else {
                    // Check if the visit is completed
                    if (curr->level.empty() && next->level.empty() && active_threads <= 1){
                        visit_completed = true;
                        change_level = true;

                        //Notify on next
                        next->cv.notify_all();
                    } else{
                        // Swap next with curr
                        swap(curr, next);

                        change_level = true;

                        // Notify on curr cv
                        curr->cv.notify_all();
                    }
                }
            }
            //Check if all the other threads are waiting
            if(active_threads<=1){
                //Set promise value
                prom.set_value(occurrences);
            }
            
            active_threads--;

            cout << "Exit!\n";
        };


    public:
        Pool(int capacity, int value, Graph& g) : graph(g), target_value(value){
            curr = make_shared<critical_region>();
            next = make_shared<critical_region>();
            occurrences = 0;
            active_threads = 0;
            for(int i=0; i<capacity; i++){
                vec_of_threads.push_back(thread(task)); //TODO: missing parameters
                active_threads++;
            }
        }

        // used by main to add source node
        future<int> add_source(Node n){
            unique_lock<mutex> curr_lock(curr->mux);
            check_item(n, ref(curr->level));
            curr_lock.unlock();
            curr->cv.notify_all();

            return prom.get_future();
        }

        ~Pool(){
            for(thread &th : vec_of_threads){
            if (th.joinable()){
                th.join();
            }
        }
        }
};

#endif