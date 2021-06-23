#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <fstream>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <atomic>
#include <thread>
#include <chrono>
#include <list>
#include "SynchQueue.hpp"
#include "SynchSet.hpp"
#include "Node.hpp"
#include "MyQueue.hpp"
#include "Pool.hpp"
 
using namespace std;
using namespace std::literals::chrono_literals;
using usecs = std::chrono::microseconds;
using msecs = std::chrono::milliseconds;

class Graph
{

    private:
        int number_of_vertices;
        Node* nodes;
    public:

        Graph(int num_nodes, Node* n){
            number_of_vertices = num_nodes;
            nodes = n;
        }

        void addEdge(int source_id, int dest_id){
            if(nodes[source_id].get_id()==-1){
                nodes[source_id].set_id(source_id);
                nodes[source_id].set_value(rand() % static_cast<int>(3));
                nodes[source_id].add_adj_node(dest_id);
            } else {
                nodes[source_id].add_adj_node(dest_id);
            }

            if(nodes[dest_id].get_id() == -1){
                nodes[dest_id].set_id(dest_id);
                nodes[dest_id].set_value(rand() % static_cast<int>(3));
            }
        }

        Node get_node_at(int index){
            return nodes[index];
        }

        int size(){
            return number_of_vertices;
        }

        int BFS(int x, int s);

        int anotherBFS(int x, int s);
};

void processNode(SynchSet<int>& visited, int index, Node n, SynchQueue<Node>& q){
    if (!visited.contains(index)){
        visited.add(index);
        q.push(n);
    }
}

void check_value(SynchSet<int> &visited, int value, atomic_int &occ, Node n, MyQueue<int> & q){
    cout << "Cheching node " << n.get_id() << " \n";

    if(!visited.contains(n.get_id())){
    
        if (n.get_value() == value){
            occ++;
        }
        
        visited.add(n.get_id());
        //cout << "Pushing "<< n.get_adj_list().size() <<" elements\n";
        for(int index : n.get_adj_list()){
            q.push(index);
        }
    }
}


int Graph::anotherBFS(int value, int source_id){
    Pool thread_pool;
    SynchSet<int> visited; // Stores the ids of already visited nodes
    atomic_int res = 0;
    MyQueue<int> next_level;    

    next_level.push(source_id);

    vector<thread> vecOfThreads;
    
    while (!next_level.empty()){
        Node n = this->get_node_at(next_level.get());

        //thread_pool.add_job(check_value(ref(visited), value, ref(res), n, ref(next_level)));

        thread_pool.add_job(std::bind(ref(check_value),  ref(visited), value, ref(res), n, ref(next_level)));

        //vecOfThreads.push_back(thread(check_value, ref(visited), value, ref(res), n, ref(next_level)));

        /*for(thread &th : vecOfThreads){
            if (th.joinable()){
                th.join();
            }
        }*/
        //thread_pool.start();
        //thread_pool.shutdown();       
    }
    thread_pool.start();
    thread_pool.shutdown();

    return res;        
}
 
int Graph::BFS(int x, int s){
    int len = this->number_of_vertices;

    SynchSet<int> visited; //stores the ids of visited nodes 
    int res = 0;
 
    // Create a queue for BFS
    SynchQueue<Node> q;
 
    visited.add(s);
    q.push(this->get_node_at(s));


    while (!q.empty()){
        // Dequeue a vertex from queue and print it
        Node n = q.get();

        //cout << "Node id: " << n.get_id() << ", Node value: " << n.get_value() << ";\t";
        if (n.get_value() == x)
            res++;

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it

        static const int adj_size = n.get_adj_list().size();

        //cout << "Starting " << adj_size <<" threads for "<< n.get_id() <<" adjacency list.\n";

        vector<thread> vecOfThreads;

        for(int id : n.get_adj_list()){
            if(!visited.contains(id)){
                //cout << "Spawning thread for node: " << id << "\n";

            Node node = this->get_node_at(id);

            vecOfThreads.push_back(std::thread(processNode, ref(visited), id, node, ref(q)));

            //cout << "Node " << id << " processed!\n";
            }
        }

        for(thread &th : vecOfThreads){
            if (th.joinable()){
                th.join();
            }
        }

        /*cout << "Queue:\n";
        for (size_t i = 0; i < q.size(); i++){
            cout << q.at(i).get_id() << " ";
        }
        cout <<"\n";*/
        
    }
    return res;
}

int main(int argc, char *argv[])
{
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] 
                << " inputfile.txt number_of_vertices\n"<< std::endl;
        return 1;
    }

    //no. vertices
    int nv = atoi(argv[2]);

    // input file
    string filename = argv[1];
    ifstream inFile;
    inFile.open(filename);

    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

    // check that the file has been opened
    if (!inFile) {
    cerr << "Unable to open file " << filename << "\n";
    exit(1);   // call system to stop
    }


    // Create a graph given in the above diagram
    //vector<thread> threads;
    Node nodes[nv] = {Node()};
    
    Graph g(nv, nodes);

    int a, b;
    while (inFile >> a >> b){
        g.addEdge(a, b);
    }
 
    /*cout << "Following is Breadth First Traversal "
         << "(starting from vertex 0) \n";
         
    start = std::chrono::system_clock::now();
    int occ = g.BFS(1, 0);
    stop = std::chrono::system_clock::now();
    cout << "\n";

    cout << "#occorrenze del VALORE 1 a partire dal nodo con ID=0: " << occ << "\n";
    std::chrono::duration<double> elapsed = stop - start;
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Par1 computed in " << musec << " microseconds\n";*/


    cout << "Try another BFS\n";
    start = std::chrono::system_clock::now();
    int occ = g.anotherBFS(1,0);
    stop = std::chrono::system_clock::now();

    cout << "#occorrenze del VALORE 1 a partire dal nodo con ID=0 usando l'altro metodo: " << occ << "\n";

    std::chrono::duration<double> elapsed = stop - start;
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Thread pool version computed in " << musec << " microseconds\n";
 
    inFile.close();
    return 0;
}