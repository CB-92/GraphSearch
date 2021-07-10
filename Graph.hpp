#include "Node.hpp"
#include <unordered_map>
#include <iostream>
#include <fstream>
#include <sstream>

using namespace std;

class Graph
{

    private:
        int number_of_vertices;
        unordered_map<int, NodePtr> nodes;
        bool is_concurrent;
    public:

        Graph(int num_nodes, bool concurrent, ifstream& inFile) : is_concurrent(concurrent){
            number_of_vertices = num_nodes;
            nodes.reserve(number_of_vertices);

            // create graph
            int a, b;
            bool hashtag_found = false;
            string line;
            
            while(getline(inFile, line)){
                //cout<<"Checking line: "<< line<<"\n";
                if(line == "#"){
                    hashtag_found = true;
                } else {
                    istringstream iss(line);
                    //cout << "A: " << a << ", B: " << b << "\n";
                    if(!hashtag_found){
                        // Adding node
                        iss >> a >> b;
                        this->addNode(a, b);
                    } else {
                        // Adding edge
                        string adj_nodes;
                        iss >> a >> adj_nodes;
                        //cout << "A: " << a << ", AN: " << adj_nodes << "\n";

                        std::string delimiter = ",";

                        size_t pos = 0;
                        std::string token;
                        while ((pos = adj_nodes.find(delimiter)) != std::string::npos) {
                            token = adj_nodes.substr(0, pos);
                            this->addEdge(a, stoi(token));
                            adj_nodes.erase(0, pos + delimiter.length());
                        }
                    }
                }
            }

        }

        void addNode (int id, int val) {
            // TODO Check existence
            nodes.insert(make_pair<> (id, make_shared<Node> (id, val, is_concurrent)));
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
                cout << "Node id: " << p.first<<", value: " << p.second->get_value()<< ", #neighbours: "<< p.second->get_adj_list().size() <<"\n";
                for (auto node : p.second->get_adj_list()){
                    cout << node->get_id() << " ";
                }

                cout << "\n";
            }
        }

        int BFS(int x, int s, int th_num=1);
};
