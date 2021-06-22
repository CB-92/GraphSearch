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
#include "../Node.hpp"
#include "MyQueue.hpp"
 
using namespace std;
using namespace std::chrono;

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

void check_value(SynchSet<int> &visited, int value, atomic_int &occ, Node n, MyQueue<int> & q){

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


int Graph::BFS(int value, int source_id){
    SynchSet<int> visited; // Stores the ids of already visited nodes
    atomic_int res = 0;
    MyQueue<int> next_level;    

    next_level.push(source_id);

    vector<thread> vecOfThreads;
    
    while (!next_level.empty()){
        Node n = this->get_node_at(next_level.get());
        //cout << "Node id: " << n.get_id() << ", Node value: " << n.get_value() << ";\n";

        vecOfThreads.push_back(thread(check_value, ref(visited), value, ref(res), n, ref(next_level)));
        
        for(thread &th : vecOfThreads){
            if (th.joinable()){
                th.join();
            }
        }
        
    }

    return res;        
}

int main(int argc, char *argv[])
{
    if(argc != 5) {
        std::cerr << "Usage: " << argv[0] 
                << " inputfile.txt number_of_vertices value source_node\n"<< std::endl;
        return 1;
    }

    //no. vertices
    int nv = atoi(argv[2]);

    int value = atoi(argv[3]);
    int node_id = atoi(argv[4]);

    // input file
    string filename = argv[1];
    ifstream inFile;
    inFile.open(filename);

    high_resolution_clock::time_point start, stop;
    duration<double, std::milli> elapsed;

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
    start = high_resolution_clock::now();
    while (inFile >> a >> b){
        g.addEdge(a, b);
    }
    stop = high_resolution_clock::now();

    elapsed = stop - start;
    cout << "Graph init in " << elapsed.count() << " milliseconds\n";
 
    start = high_resolution_clock::now();
    int occ = g.BFS(value,node_id);
    stop = high_resolution_clock::now();

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";

    elapsed = stop - start;
    cout << "Thread BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();
    return 0;
}