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
#include <string>
#include <sstream>
#include "SynchQueue.hpp"
#include "SynchSet.hpp"
#include "../Node.hpp"
#include "../Graph.hpp"
#include "MyQueue.hpp"
 
using namespace std;
using namespace std::chrono;

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
        Node* n = this->get_node_at(next_level.get());
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
    bool hashtag_found = false;
    string line;
    
    while(getline(inFile, line)){
        //cout<<"Checking line: "<< line<<"\n";
        if(line == "#"){
            hashtag_found = true;
        } else {
            istringstream iss(line);
            iss >> a >> b;
            //cout << "A: " << a << ", B: " << b << "\n";
            if(!hashtag_found){
                g.addEdge(a, b);
            } else {
                g.insertValue(a, b);
            }
        }
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