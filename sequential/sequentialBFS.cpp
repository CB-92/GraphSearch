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

        void print_graph(){
            for (int i = 0; i < number_of_vertices; i++)
            {
                cout<<"Node id: "<< nodes[i].get_id() << ", value: "<<nodes[i].get_value() << ", adj list:";
                for(auto &n : nodes[i].get_adj_list()){
                    cout << " " << n;
                }
                cout<<"\n";
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

    high_resolution_clock::time_point start, stop;
    duration<double, std::milli> elapsed;
    
    /*std::chrono::system_clock::time_point start;
    std::chrono::system_clock::time_point stop;*/

    // Create a graph given in the above diagram
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
 
    start = high_resolution_clock::now();;
    int occ = g.BFS(value, node_id);
    stop = high_resolution_clock::now();
    cout << "\n";

    cout << "Number of occurrencies of VALUE "<< value <<" starting from node with ID="<< node_id <<": " << occ << "\n";
    
    elapsed = stop - start;

    cout << "Sequential BFS computed in " << elapsed.count() << " milliseconds\n";
 
    inFile.close();
    return 0;
}