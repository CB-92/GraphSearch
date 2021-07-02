#include "Node.hpp"

using namespace std;

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

        Node* get_node_at(int index){
            return &(nodes[index]);
        }

        int size(){
            return number_of_vertices;
        }

        int BFS(int x, int s);
};
