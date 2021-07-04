#include "../Graph.hpp"
#include "ThreadPool.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>

using namespace std;
using namespace std::chrono;

atomic_int occurencies;

int check_node(NodePtr node, int value, ThreadPool* p){
    Node::visit_result res = node->check_and_visit(value);
    occurencies += res.first;

    // For each node in res.second I create a task and I add it to the task queue of the pool
    //unique_lock<mutex> res_lock(*mux);
    for(auto n : res.second){
         p->enqueue(
                check_node, n, value, p
            );
    }

    return res.first;
}

int Graph::BFS(int x, int s, int th_num){
    ThreadPool pool(th_num);

    //Enqueue the first node
    
    pool.enqueue(
        check_node, this->get_node_at(s), x, &pool
    );

    pool.wait();

    return occurencies;
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
    
    Graph g(nv, true);

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

    cout << "Sequential BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();

    return 0;
}