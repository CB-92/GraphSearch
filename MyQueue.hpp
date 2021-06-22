#include <mutex>
#include <condition_variable>
#include <queue>
#include <set>

using namespace std;

template<typename T>
class MyQueue{
    private:
        const unsigned int len = 10000;
        set<T> q;
        mutex mq;
        condition_variable is_not_empty, is_not_full;

    public:
        int size(){
            return q.size();
        }

        bool empty(){
            return q.empty();
        }

        bool contains(T el){
            return q.find(el) != q.end();
        }

        void push(T el){
            unique_lock<mutex> mlock(mq);

            while (q.size() == len){
                cout << "Waiting because the queue is full\n";
                is_not_full.wait(mlock);
            }
            q.insert(el);
            mlock.unlock();
            
            is_not_empty.notify_all();
        }

        T get(){
            unique_lock<mutex> mlock(mq);

            while (q.empty()){
                is_not_empty.wait(mlock);
            }

            //T el = q.front();
            //q.pop_front();

            auto it = q.begin();


            T el = *it;
            q.erase(q.begin());

            mlock.unlock();
            is_not_full.notify_all();

            return el;            
        }

};