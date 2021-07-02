#include <set>
#include <algorithm>
#include <mutex>
//TODO: vedere se incrementare l'efficienza con una cv

#ifndef NODE_HPP
#define NODE_HPP

using namespace std;

class Node{
    private:
        int node_id;
        int value;
        set<int> adj;
        mutex mux;

    public:
        Node(){
            node_id = -1;
            value = -1;
            set<int> adj = {};
        }

        auto acquire(){
            return unique_lock<mutex>(mux);
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