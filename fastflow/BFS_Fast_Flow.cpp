#include <iostream>
#include <stdio.h>      
#include <stdlib.h>
#include <list>
#include <vector>
#include <time.h>
#include <chrono>
#include "../Node.hpp"
#include <string>
#include <sstream>
#include <fstream>
#include "../Graph.hpp"
#include <ff/ff.hpp>
#include <ff/parallel_for.hpp>

using namespace ff;
using namespace std;
using namespace std::chrono;
using WorkerResult = pair<int, list<NodePtr>>;
using WorkerResultPtr = shared_ptr<WorkerResult>;

WorkerResult node_analyzer(NodePtr node, int searched_value){
    WorkerResult res = pair<int, list<NodePtr>> (0, {});

    Node::visit_result n_res = node->check_and_visit (searched_value);
    res.first = n_res.first;
    for (auto& n : n_res.second)
        res.second.push_back (n);

    return res;
};

int Graph::BFS(int value, int source_id, int th_num){
    int res = 0;
    list<NodePtr> frontier;


    ParallelForReduce<WorkerResult>* pf;

    if (th_num>1) {
        pf = new ParallelForReduce<WorkerResult>(th_num);
    }

    frontier.push_back(this->get_node_at(source_id));
    
    if (th_num>1) {
        pf->parallel_reduce(res, 0,
                            0,frontier.size(), 
                           [&frontier, &value](WorkerResult tmp){
                               NodePtr n = frontier.front();
                               frontier.pop_front();
                               tmp = node_analyzer(n, value);
                           },
                           [&frontier](int& res, WorkerResultPtr tmp_res) {
                               res+=tmp_res->first;
                               frontier.splice(tmp_res->second);
                           });       
    }












   
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

    /*start = high_resolution_clock::now();
    int occ = g.BFS(value, node_id, th_num);
    stop = high_resolution_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";
    
    elapsed = stop - start;

    cout << "FastFlow BFS computed in " << elapsed.count() << " milliseconds\n";*/
 
    inFile.close();

    return 0;
}