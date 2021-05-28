#include <iostream>
#include <set>
#include <vector>
#include <string>
#include <queue>
#include <fstream>
#include <algorithm>
 
using namespace std;

/*struct Node{
    Node(int id, std::set<int> a) : node_id(id), adj(a) {}
    int node_id;
    std::set<int> adj;
};*/

class Node{
    public:
        Node(int id, vector<int> a) : node_id(id), adj(a) {
            visited = false;
        }
        int node_id;
        bool visited;
        vector<int> adj;
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
        void sequentialBFS(int s);
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
        this->nodes.at(pos).adj.push_back(dest_id);
    } else{
        Node n(source_id, {dest_id});
        this->nodes.push_back(n);
    }

    if(this->contains(dest_id) == -1){
        this->nodes.push_back(Node(dest_id, {}));
    }
   
}
 
void Graph::sequentialBFS(int s)
{
 
    // Create a queue for BFS
    queue<Node> q;
 
    // Mark the current node as visited and enqueue it
    int pos = this->contains(s);
    if(pos == -1){
        cerr << "Node id " << s << " not found!\n";
        return;
    }
    this->nodes.at(pos).visited = true;
    q.push(this->nodes.at(pos));

    while (!q.empty()){
        // Dequeue a vertex from queue and print it
        Node n = q.front();
        cout << n.node_id << " ";
        q.pop();

        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for(int id : n.adj){
            int p = this->contains(id);

            if (p == -1 ){
                cerr << "Node id " << id << " not found!\n";
                return;
            }
         
            if (!this->nodes.at(p).visited){
                this->nodes.at(p).visited = true;
                q.push(this->nodes.at(p));
            }            
        }
    }
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
         << "(starting from vertex 2) \n";
    g.sequentialBFS(2);
    cout << "\n";

    int pos = g.contains(2);
    if (pos == -1){
        cout << "Node id 2 not found!!!";
    }

    cout << "Lista di adiacenza del nodo 2:\n";

    for(int i : g.nodes.at(pos).adj){
        cout << i <<", ";
    }
    cout << "\n";
 
    inFile.close();
    return 0;
}