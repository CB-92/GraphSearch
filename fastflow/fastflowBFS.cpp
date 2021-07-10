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
#include <ff/pipeline.hpp>
#include <ff/farm.hpp>

using namespace ff;
using namespace std;
using namespace std::chrono;
using WorkerResult = pair<int, list<NodePtr>>;

int searched_val;
int source_id;
Graph g;

struct emitterCollector: ff_node_t<WorkerResult>{

    WorkerResult* svc(WorkerResult* wr){
        if( wr == nullptr){
            //firts element
            cout << "Starting..\n";
            ff_send_out(g.get_node_at(source_id).get());
            return GO_ON;
        }
        cout << "Count: "<< wr->first << ", Lenght: " << wr->second.size() << "\n";

        return GO_ON; //FIXME!
    }

    void svc_end () {
        // TODO write me!
        cout << "Addio!\n";
    }
};


struct nodeAnalyzer: ff_node_t<Node, WorkerResult>{
    WorkerResult* svc(Node* node){
        WorkerResult* res = new pair<int, list<NodePtr>> (0, {});
        cout << "Sono qui dentro!\n";
        Node::visit_result n_res = node->check_and_visit(searched_val);
        res->first = n_res.first;
        for (auto& n : n_res.second)
            res->second.push_back (n);

        return res;
    }
};


int Graph::BFS(int value, int source_id, int th_num){
    int res = 0;

    emitterCollector ec;
    vector<unique_ptr<ff_node>> workers;
    for (size_t i = 0; i < th_num; i++){
        workers.push_back(make_unique<nodeAnalyzer>());
    }
    ff_Farm<WorkerResult> farm(move(workers), ec);
    farm.remove_collector();
    farm.wrap_around();

    ffTime(START_TIME);
    if (farm.run_and_wait_end()<0) {
        error("running farm");
        return -1;
    }
    ffTime(STOP_TIME);
    std::cout << "Time: " << ffTime(GET_TIME) << "\n";

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
    searched_val = atoi(argv[3]);
    source_id = atoi(argv[4]);
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
    
    start = high_resolution_clock::now();
    g = Graph(nv, false, inFile);
    stop = high_resolution_clock::now();


    elapsed = stop - start;
    cout << "Graph init in " << elapsed.count() << " milliseconds\n";

    start = high_resolution_clock::now();
    int occ = g.BFS(searched_val, source_id, th_num);
    stop = high_resolution_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< searched_val <<" starting from node with ID="<< source_id <<": " << occ << "\n";
    
    elapsed = stop - start;

    cout << "FastFlow BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();

    return 0;
}