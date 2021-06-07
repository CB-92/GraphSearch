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
 
using namespace std;

class Node{
    public:
        Node(int id, int v, set<int> a) : node_id(id), value(v), adj(a){}
        int node_id;
        int value;
        set<int> adj;
};

class Graph
{
    public:
        public:
        Graph(vector<Node> v, int nv) : nodes(v), number_of_vertices(nv) {}
    // prints BFS traversal from a given source s
        void addEdge(int source_id, int dest_id);
        //int contains(int node_id);
        int BFS(int x, int s);
        int number_of_vertices;
        vector<Node> nodes;
};

void Graph::addEdge(int source, int dest){
    if (this->nodes.at(source).node_id == -1){
        // Source node to be initialized
        this->nodes.at(source).node_id = source;
        this->nodes.at(source).value = rand() % static_cast<int>(3);
        this->nodes.at(source).adj.insert(dest);
    } else {
        // Add dest to source's adj list
        this->nodes.at(source).adj.insert(dest);
    }

    if (this->nodes.at(dest).node_id == -1){
        // Dest node to be initialized
        this->nodes.at(dest).node_id = dest;
        this->nodes.at(dest).value = rand() % static_cast<int>(3);
    }
    
}


 
int Graph::BFS(int x, int s){
    int len = this->number_of_vertices;
    int visited[len] = {0};
    int res = 0;
 
    // Create a queue for BFS
    queue<Node> q;
 
    visited[s] = 1;
    q.push(this->nodes.at(s));

    while (!q.empty()){
        // Dequeue a vertex from queue and print it
        Node n = q.front();
        //cout << n.node_id << " ";
        cout << "Node id: " << n.node_id << ", Node value: " << n.value << ";\t";
        if (n.value == x)
            res++;
        q.pop();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for(int id : n.adj){
         
            if (!visited[id]){
                visited[id] = 1;
                q.push(this->nodes.at(id));
            }            
        }
    }
    return res;
}


int main(int argc, char *argv[])
{
    if(argc != 3) {
        std::cerr << "Usage: " << argv[0] 
                << " inputfile.txt number_of_vertices\n"<< std::endl;
        return 1;
    }

    //no. vertices
    int nv = atoi(argv[2]);

    // input file
    string filename = argv[1];
    ifstream inFile;
    inFile.open(filename);

    // check that the file has been opened
    if (!inFile) {
    cerr << "Unable to open file " << filename << "\n";
    exit(1);   // call system to stop
    }


    // Create a graph given in the above diagram
    Node init(-1, -1, {});
    vector<Node> nodes;
    for (size_t i = 0; i < nv; i++){
        nodes.push_back(init);
    }
    
    Graph g(nodes, nv);

    int a, b;
    while (inFile >> a >> b){
        g.addEdge(a, b);
    }
 
    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 0) \n";
    int occ = g.BFS(1, 0);
    cout << "\n";

    cout << "#occorrenze del VALORE 1 a partire dal nodo con ID=0: " << occ << "\n";
 
    inFile.close();
    return 0;
}