#include <iostream>
#include <set>
#include <vector>
#include <string>
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
        Node(int id, std::set<int> a) : node_id(id), adj(a) {}
        int node_id;
        std::set<int> adj;
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
        void sequentialBFS(int s);
        vector<Node> nodes; 
};
 
 
void Graph::addEdge(int source_id, int dest_id){
    vector<Node> g = this->nodes;
    vector<Node>::iterator it = std::find_if(g.begin(), g.end(), find_by_id(source_id));

    if (it != g.end()){
        /*if (g.at(it - g.begin()).node_id == source_id){
            cout << "OK!\n";
        } else {
            cout << "NOT OK!!!\n";
        }*/
         
        //cout << "Nodo già presente! Devo inserire " << dest_id << " nella adj list di " << source_id << ".\n";
        g.at(it - g.begin()).adj.insert(source_id);

    } else{
        //cout << "Inserted node " << source_id << " with " << dest_id << " in his adj list.\n";
        Node n(source_id, {dest_id});
        this->nodes.push_back(n);
    }
   
}
 
void Graph::sequentialBFS(int s)
{/*
    // Mark all the vertices as not visited
    bool *visited = new bool[V];
    for(int i = 0; i < V; i++)
        visited[i] = false;
 
    // Create a queue for BFS
    list<int> queue;
 
    // Mark the current node as visited and enqueue it
    visited[s] = true;
    queue.push_back(s);
 
    // 'i' will be used to get all adjacent
    // vertices of a vertex
    list<int>::iterator i;
 
    while(!queue.empty())
    {
        // Dequeue a vertex from queue and print it
        s = queue.front();
        cout << s << " ";
        queue.pop_front();
 
        // Get all adjacent vertices of the dequeued
        // vertex s. If a adjacent has not been visited,
        // then mark it visited and enqueue it
        for (i = adj[s].begin(); i != adj[s].end(); ++i)
        {
            if (!visited[*i])
            {
                visited[*i] = true;
                queue.push_back(*i);
            }
        }
    }*/
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
    cerr << "Unable to open file datafile.txt";
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
    //g.sequentialBFS(2);
    cout << "\n";
 
    inFile.close();
    return 0;
}