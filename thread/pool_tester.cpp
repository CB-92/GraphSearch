#include "Pool.hpp"
#include <iostream>
#include<vector>

using namespace std;

int main(int argc, char *argv[]){
    /*Node nodes[5] = {Node()};
    
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
    cout << "Res is: "<< res << "\n";*/
    Pool pool([](int i){
        cout << "Consuming " << i << "\n";
        //this_thread::sleep_for (chrono::milliseconds(1));
        return i*i;
    }, 4);

    for (int i=0;i<10000; i++) {
        pool.add_item(i);
    }

    /*future<int> f = pool.start();
    f.wait();
    cout<<"Res: "<< f.get() <<"\n";*/
    /*cout << "Altro giro, altra corsa!\n";

    pool.add_item(60);
    pool.add_item(70);
    pool.add_item(80);
    pool.add_item(90);
    pool.add_item(100);

    pool.start().wait();//*/

    //pool.close();
    /*future<int> result;
    int occ =0;


    ThreadPool pool(4);
    for(int i =0; i<3;i++){
        pool.enqueue([i, occ](int* o){
            cout << "Consuming " << i << "\n";
            if(i%2==0){
                o++;
            }
            return o;

        }, &occ);
    }

    // get result from future
    cout << result.get() << std::endl;*/

    return 0;

}