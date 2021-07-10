#include <set>
#include <vector>
#include <algorithm>
#include <mutex>
#include <condition_variable>
#include <utility>
#include <thread>

#ifndef NODE_HPP
#define NODE_HPP

using namespace std;
using namespace std::chrono;

class Node;

using NodePtr   = shared_ptr<Node>;

class Node{
    private:
        int node_id;
        int value;
        bool _visited;
        bool _enqued;
        bool is_concurrent;
        vector<NodePtr> adj;
        mutex mux;
        condition_variable cv;

    public:
        using visit_result = pair<int, vector<NodePtr>>;

        Node(int id, int val, bool concurrent): node_id(id), value(val), _visited(false), _enqued(false), is_concurrent(concurrent){
            set<Node*> adj = {};
            // Concurrent set to false in sequential execution
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
            unique_lock<mutex> lock;

            if(is_concurrent)
                lock =  unique_lock<mutex>(mux);

            return this->_visited;
        }

        bool check_and_enqueue(){
            bool res = false;

            unique_lock<mutex> lock;

            if(is_concurrent)
                lock =  unique_lock<mutex>(mux);


            if (!this->_enqued && !this->_visited){
                _enqued = true;
                res = true;
            }
             return res;
        }


        visit_result check_and_visit(int value){
            visit_result res;
            unique_lock<mutex> lock;
            int check = 0;

            if(is_concurrent)
                lock =  unique_lock<mutex>(mux);
            
            if(!this->_visited){
                this->_visited = true;
                // check value
                if(this->value == value){
                    check=1;
                }

                // fill neighbours
                for(auto n : adj){
                    if(n->check_and_enqueue()){
                        res.second.push_back(n);
                    }
                }
            }

            res.first = check;

            return res;
        }
        
};



#endif