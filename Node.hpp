#include <set>
#include <algorithm>

#ifndef NODE_HPP
#define NODE_HPP

using namespace std;

class Node{
    private:
        int node_id;
        int value;
        set<int> adj;

    public:
        Node(){
            node_id = -1;
            value = -1;
            set<int> adj = {};
        }

        int get_id(){
            return node_id;
        }

        int get_value(){
            return value;
        }

        set<int> get_adj_list(){
            return adj;
        }

        void set_id(int id){
            node_id = id;
        }

        void set_value(int v){
            value = v;
        }

        void add_adj_node(int id){
            adj.insert(id);
        }        

        bool operator<(const Node &node) const{
            return node_id < node.node_id;
        }

        bool operator==(const Node &node) const {
            return node_id == node.node_id;
        }
};

#endif