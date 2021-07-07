#include "../Graph.hpp"
#include "ThreadPool.hpp"
#include <iostream>
#include <string>
#include <sstream>
#include <queue>
#include <fstream>

using namespace std;
using namespace std::chrono;


ThreadPool::NodeAnalyzer node_analyzer = [](NodePtr node, int searched_value) -> ThreadPool::WorkerResult {
    ThreadPool::WorkerResult res = pair<int, list<NodePtr>> (0, {});

    Node::visit_result n_res = node->check_and_visit (searched_value);
    res.first = n_res.first;
    for (auto& n : n_res.second)
        res.second.push_back (n);

    return res;
};


int Graph::BFS(int x, int s, int th_num){
    ThreadPool thread_pool(th_num, node_analyzer, x);
    ThreadPool::WorkerResult tmp_res = pair<int, list<NodePtr>> (0, {});
    int occurrencies = 0;

    //Enqueue the first node
    tmp_res.second.push_back ({this->get_node_at(s)});

    do {
        // Preparing argument
        // Enqueueing next level nodes
        thread_pool.enqueue (tmp_res.second);
        // Waiting
        tmp_res = thread_pool.wait();
        // Parsing result and preparing global_res for the next iteration
        occurrencies += tmp_res.first;
    } while (tmp_res.second.size() != 0);

    return occurrencies;
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

    cout << "Thread BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();

    return 0;
}