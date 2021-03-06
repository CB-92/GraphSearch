#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <chrono>
#include "../Node.hpp"
#include "../Graph.hpp"
 
using namespace std;
using namespace std::chrono;

int Graph::BFS(int x, int s, int th_num){
    int occurrencies=0;
    queue<NodePtr> q;

    q.push(this->get_node_at(s));

    while (!q.empty()){
        // Dequeue a node from the queue
        NodePtr node = q.front();
        q.pop();

        Node::visit_result res = node->check_and_visit(x);

        occurrencies += res.first;

        // Enqueue the nodes in s'adj list that are not already visited
        for (auto n : res.second){
            q.push(n);
        }
    }

    return occurrencies;
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

    // log file
    fstream logFile;
    logFile.open("seq_log.csv", std::fstream::in | std::fstream::out | std::fstream::app);

    // check that the file has been opened
    if (!inFile) {
    cerr << "Unable to open file " << filename << "\n";
    exit(1);   // call system to stop
    }

    high_resolution_clock::time_point start, stop;
    duration<double, std::milli> elapsed;

    // Create the graph

    start = high_resolution_clock::now();
    Graph g(nv, false, inFile);
    stop = high_resolution_clock::now();


    elapsed = stop - start;
    cout << "Graph init in " << elapsed.count() << " milliseconds\n";
 
    start = high_resolution_clock::now();;
    int occ = g.BFS(value, node_id);
    stop = high_resolution_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";
    
    elapsed = stop - start;

    cout << "Sequential BFS computed in " << elapsed.count() << " milliseconds\n";
    logFile << "" << nv << ", 1, " << elapsed.count() << "\n";
 
    inFile.close();
    logFile.close();
    return 0;
}