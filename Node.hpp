#include <set>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <utility>

#ifndef NODE_HPP
#define NODE_HPP

using namespace std;

class Node;

using NodePtr   = shared_ptr<Node>;

class Node{
    private:
        int node_id;
        int value;
        bool _visited;
        vector<NodePtr> adj;
        mutex mux;
        condition_variable cv;

    public:
        using visit_result = pair<int, vector<NodePtr>>;

        Node(int id, int val): node_id(id), value(val), _visited(false){
            set<Node*> adj = {};
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

        vector<NodePtr>& get_adj_list(){
            return adj;
        }

        void add_adj_node(NodePtr n){
            adj.push_back(n);
        }

        bool visited(){
            unique_lock<mutex> lock(mux);
            return this->_visited;
        }


        visit_result check_and_visit(int value){
            visit_result res;
            int check = 0;

            unique_lock<mutex> lock(mux);
            
            if(!this->_visited){
                // check value
                if(this->value == value){
                    check=1;
                }

                // fill neighbours
                for(auto n : adj){
                    if(!n->visited()){
                        res.second.push_back(n);
                    }
                }
            }

            res.first = check;

            return res;
        }
        
};



#endif