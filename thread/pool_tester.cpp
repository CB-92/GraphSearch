#include "Pool.hpp"
#include <iostream>

using namespace std;

int main(int argc, char *argv[]){
    Node nodes[5] = {Node()};
    
    Graph g(5, nodes);
    g.addEdge(0,1);
    g.addEdge(0,2);
    g.addEdge(1,2);
    g.addEdge(2,3);
    g.addEdge(2,4);

    g.insertValue(0,1);
    g.insertValue(1,0);
    g.insertValue(2,1);
    g.insertValue(3,2);
    g.insertValue(4,1);
    
    Pool pool(4, 1, g);

    future<int> f = pool.add_source(g.get_node_at(0));
    f.wait();
    int res = f.get();
    cout << "Res is: "<< res << "\n";
}