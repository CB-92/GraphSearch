#include "Node.hpp"
#include <unordered_map>

using namespace std;

class Graph
{

    private:
        int number_of_vertices;
        unordered_map<int, NodePtr> nodes;
    public:

        Graph(int num_nodes){
            number_of_vertices = num_nodes;
            nodes.reserve(number_of_vertices);
        }

        void addNode (int id, int val) {
            // TODO Check existence
            nodes.emplace(make_pair<> (id, make_shared<Node> (id, val)));
        }

        void addEdge(int source_id, int dest_id){
            NodePtr dNode = nodes[dest_id];
            nodes[source_id]->add_adj_node (dNode);
        }

        NodePtr get_node_at(int index){
            // TODO Check existence
            return nodes[index];
        }

        int size(){
            return number_of_vertices;
        }

        void print(){

            for(const auto &p : nodes){
                cout << "Node id: " << p.first<<", value: " << p.second->get_value()<< "\t";
                for (auto node : p.second->get_adj_list()){
                    cout << node->get_id() << " ";
                }

                cout << "\n";
            }

           /* auto it = nodes.begin();

            while(it!=nodes.end()){
                cout << "Node id: " << it->first<<", value: " << it->second->get_value()<< "\t";
                for(auto n : it->second->get_adj_list()){
                    cout << n->get_id() << " ";
                }
                cout << "\n";

                it++;
            }*/
        }

        int BFS(int x, int s, int th_num=1);
};
