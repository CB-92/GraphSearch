#include <iostream>
#include <stdio.h>      
#include <stdlib.h>
#include <time.h>
#include <chrono>
#include "../Node.hpp"
#include "ff/map.hpp"
#include <string>
#include <sstream>

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
                nodes[source_id].add_adj_node(dest_id);
            } else {
                nodes[source_id].add_adj_node(dest_id);
            }

            if(nodes[dest_id].get_id() == -1){
                nodes[dest_id].set_id(dest_id);
            }
        }

        void insertValue(int node_id, int value){
            nodes[node_id].set_value(value);
        }

        Node get_node_at(int index){
            return nodes[index];
        }

        int size(){
            return number_of_vertices;
        }

        int BFS(int x, int s);
};




int Graph::BFS(int value, int source_id){
    int len = this->number_of_vertices;
    int visited[len] = {0};
    atomic_int res = 0;
   
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
 
    /*start = high_resolution_clock::now();
    int occ = g.BFS(value,node_id);
    stop = high_resolution_clock::now();

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";

    elapsed = stop - start;
    cout << "Thread BFS computed in " << elapsed.count() << " milliseconds\n";*/
 
    inFile.close();
    return 0;
}