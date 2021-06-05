#include <iostream>
#include <set>
#include <vector>
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

struct find_by_id{
            find_by_id(const int & id) : id(id) {}
            bool operator()(const Node & node) {
                return node.node_id == id;
            }
            private:
            int id;
        };

class Graph
{
    public:
        Graph(vector<Node> v) : nodes(v) {}
    // prints BFS traversal from a given source s
        void addEdge(int source_id, int dest_id);
        int contains(int node_id);
        int sequentialBFS(int x, int s);
        vector<Node> nodes; 
};

int Graph::contains(int node_id){
    // Returns the position of node_id in this->nodes if present, -1 otherwise.
    vector<Node>::iterator it = std::find_if(this->nodes.begin(), this->nodes.end(), find_by_id(node_id));
    if (it != this->nodes.end()){
        // Found element with node id equal to node_id
        return (it - this->nodes.begin());
    }

    return -1;    
}
 
void Graph::addEdge(int source_id, int dest_id){

    int pos = this->contains(source_id);

    if (pos > -1){
        this->nodes.at(pos).adj.insert(dest_id);
    } else{
        Node n(source_id, (rand() % static_cast<int>(3)), {dest_id});
        this->nodes.push_back(n);
    }

    if(this->contains(dest_id) == -1){
        this->nodes.push_back(Node(dest_id, (rand() % static_cast<int>(3)), {}));
    }
   
}
 
int Graph::sequentialBFS(int x, int s){
    int visited[this->nodes.size()] = {0};
    int res = 0;
 
    // Create a queue for BFS
    queue<Node> q;
 
    // Mark the current node as visited and enqueue it
    int pos = this->contains(s);
    if(pos == -1){
        cerr << "Node id " << s << " not found!\n";
        return -1;
    }
    visited[s] = 1;
    q.push(this->nodes.at(pos));

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
            int p = this->contains(id);

            if (p == -1 ){
                cerr << "Node id " << id << " not found!\n";
                return -1;
            }
         
            if (!visited[id]){
                visited[id] = 1;
                q.push(this->nodes.at(p));
            }            
        }
    }
    return res;
}


int main(int argc, char *argv[])
{
    if(argc != 2) {
        std::cerr << "Usage: " << argv[0] 
                << " inputfile.txt" << std::endl;
        return 1;
    }

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
    Graph g({});

    int a, b;
    while (inFile >> a >> b){
        g.addEdge(a, b);
    }
 
    cout << "Following is Breadth First Traversal "
         << "(starting from vertex 0) \n";
    int occ = g.sequentialBFS(1, 0);
    cout << "\n";

    int pos = g.contains(0);
    if (pos == -1){
        cout << "Node id 0 not found!!!";
    }

    cout << "#occorrenze del VALORE 1 a partire dal nodo con ID=0: " << occ << "\n";
 
    inFile.close();
    return 0;
}