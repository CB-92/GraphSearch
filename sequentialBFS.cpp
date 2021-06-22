#include <iostream>
#include <set>
#include <vector>
#include <list>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>
#include <stdio.h>      
#include <stdlib.h>     /* srand, rand */
#include <time.h> 
#include <chrono>
#include "Node.hpp"
 
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
};
 
int Graph::BFS(int x, int s){
    int len = this->number_of_vertices;
    int visited[len] = {0};
    int res = 0;
 
    // Create a queue for BFS
    queue<Node> q;
 
    visited[s] = 1;
    q.push(this->get_node_at(s));

    while (!q.empty()){
        // Dequeue a vertex from queue and print it
        Node n = q.front();
        //cout << n.node_id << " ";
        //cout << "Node id: " << n.get_id() << ", Node value: " << n.get_value() << ";\t";
        if (n.get_value() == x)
            res++;
        q.pop();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for(int id : n.get_adj_list()){
         
            if (!visited[id]){
                visited[id] = 1;
                q.push(this->get_node_at(id));
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

    // check that the file has been opened
    if (!inFile) {
    cerr << "Unable to open file " << filename << "\n";
    exit(1);   // call system to stop
    }
    
    std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;

    // Create a graph given in the above diagram
    Node nodes[nv] = {Node()};
    
    Graph g(nv, nodes);

    int a, b;
    start = std::chrono::system_clock::now();
    while (inFile >> a >> b){
        g.addEdge(a, b);
    }
    stop = std::chrono::system_clock::now();

    std::chrono::duration<double> elapsed = stop - start;
    auto musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();
    cout << "Graph init in " << musec << " microseconds\n";
 
    start = std::chrono::system_clock::now();
    int occ = g.BFS(value, node_id);
    stop = std::chrono::system_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";
    
    elapsed = stop - start;
    musec = std::chrono::duration_cast<std::chrono::microseconds>(elapsed).count();

    cout << "Sequential BFS computed in " << musec << " microseconds\n";
 
    inFile.close();
    return 0;
}