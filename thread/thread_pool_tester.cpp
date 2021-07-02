#include <iostream>
#include <vector>
#include <mutex>
#include <condition_variable>
#include <atomic>
#include "ThreadPool.hpp"
#include "SynchSet.hpp"
#include "SynchQueue.hpp"
#include "../Node.hpp"
#include "../Graph.hpp"

using namespace std;

mutex global_mux;
condition_variable global_cv;
atomic_int running_threads;

int count_even(int el){
    cout << "Consuming " << el << "\n";
    int res = 0;
    if (el%2==0){
        res++;
    }
    return res;   
}

int check_value(SynchSet<int> &visited, int value, Node* n, SynchQueue<int> & q){
    // I'm starting the execution --> increment # running threads
    running_threads++;

    // Acquire lock on Node n in order to visit it once per time
    auto lock = n->acquire();
    cout << "Node " << n->get_id() << " has value " << n->get_value() << "\n";
    int my_occ = 0;

    if(!visited.contains(n->get_id())){
        cout << "Node " << n->get_id() << " is not already visited!\n";
    
        if (n->get_value() == value){
            my_occ++;
        }
        
        visited.add(n->get_id());
        //cout << "Pushing "<< n.get_adj_list().size() <<" elements\n";
        for(int index : n->get_adj_list()){
            cout << "Node " << index << " inserted? " << q.push(index) << "\n";            
        }
    } else{
        cout << "Node " << n->get_id() << " VISITED!\n";
    }

    // Finish of the task --> decrement # running threads
    running_threads--;

    // I finished my job, so I notify the threads waiting for new tasks
    global_cv.notify_all();

    return my_occ;
}

int main(int argc, char *argv[]){
    // Init # running threads to 0
    running_threads = 0;
    ThreadPool pool(4);
    std::vector< std::future<int> > results;


    Node nodes[10] = {Node()};
    
    Graph g(10, nodes);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,2);
    g.addEdge(1,7);
    g.addEdge(2,3);
    g.addEdge(2,4);
    g.addEdge(3,5);
    g.addEdge(3,6);
    g.addEdge(4,5);
    g.addEdge(5,6);
    g.addEdge(6,8);
    g.addEdge(7,9);
    g.addEdge(8,9);

    g.insertValue(0,3);
    g.insertValue(1,4);
    g.insertValue(2,2);
    g.insertValue(3,0);
    g.insertValue(4,1);
    g.insertValue(5,1);
    g.insertValue(6,2);
    g.insertValue(7,4);
    g.insertValue(8,4);
    g.insertValue(9,0);

    SynchSet<int> visited; // Stores the ids of already visited nodes
    SynchQueue<int> next_level; 
    next_level.push(1);
    auto s = g.get_node_at(0)->get_adj_list().size();
    cout << "Source has " << s << "adj nodes\n";
    cout << "Results size: " << results.size() << "\n";


    while (!next_level.empty() || running_threads>0){
        unique_lock<mutex> lock(global_mux);

        // Wait on global_cv while(next_level is empty && running_threads>0)
        global_cv.wait(lock, [&next_level]{return (!next_level.empty()) || (running_threads==0);});

        if(next_level.empty() && running_threads==0){
            // Exit from the while because all the tasks have been completed
            break;
        }
        

        Node* n = g.get_node_at(next_level.get());
        results.emplace_back(
            pool.enqueue(check_value, ref(visited), 4, n, ref(next_level))
        );
        cout << "Queue size: " << next_level.size() << "\n";
    }

    
    int occurrencies=0;

    for(auto && result: results){
        occurrencies += result.get();
    }
    
    cout << "# occurrencies is: " << occurrencies << "\n";

    return 0;
}