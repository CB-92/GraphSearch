#include <iostream>
#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "../Node.hpp"
#include "ff/map.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include "../Graph.hpp"

using namespace std;
using namespace std::chrono;

int Graph::BFS(int value, int source_id, int th_num){
    atomic_int res = 0;
   
    return res;        
}

int main(int argc, char *argv[])
{
     if(argc != 6) {
        std::cerr << "Usage: " << argv[0] 
                << " inputfile.txt number_of_vertices value source_node thread_num\n"<< std::endl;
        return 1;
    }

    //no. vertices
    int nv = atoi(argv[2]);

    int value = atoi(argv[3]);
    int node_id = atoi(argv[4]);
    int th_num = atoi(argv[5]);

    // input file
    string filename = argv[1];
    ifstream inFile;
    inFile.open(filename);

    // check that the file has been opened
    if (!inFile) {
    cerr << "Unable to open file " << filename << "\n";
    exit(1);   // call system to stop
    }

    high_resolution_clock::time_point start, stop;
    duration<double, std::milli> elapsed;

    // Create the graph
    
    Graph g(nv, false);

    int a, b;
    start = high_resolution_clock::now();
    bool hashtag_found = false;
    string line;
    
    while(getline(inFile, line)){
        if(line == "#"){
            hashtag_found = true;
        } else {
            istringstream iss(line);
            iss >> a >> b;
            if(!hashtag_found){
                // Adding node
                g.addNode(a, b);
            } else {
                // Adding edge
                g.addEdge(a, b);
            }
        }
    }

    stop = high_resolution_clock::now();


    elapsed = stop - start;
    cout << "Graph init in " << elapsed.count() << " milliseconds\n";




    start = high_resolution_clock::now();;
    int occ = g.BFS(value, node_id, th_num);
    stop = high_resolution_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";
    
    elapsed = stop - start;

    cout << "FastFlow BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();

    return 0;
}