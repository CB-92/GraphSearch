#include <mutex>
#include <condition_variable>
#include <queue>
#include <set>

using namespace std;

template<typename T>
class SynchQueue{
    private:
        const unsigned int len = 10000;
        deque<T> q;
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
            bool found = false;
            int i = 0;
            while (i<q.size() && !found){
                if(q[i] == el){
                    found = true;
                } else {
                    i++;
                }
            }
            return found;
        }

        void push(T el){
            unique_lock<mutex> mlock(mq);

            while (q.size() == len){
                cout << "Waiting because the queue is full\n";
                is_not_full.wait(mlock);
            }
            q.push_back(el);
            mlock.unlock();
            
            is_not_empty.notify_all();
        }

        T get(){
            unique_lock<mutex> mlock(mq);

            while (q.empty()){
                is_not_empty.wait(mlock);
            }

            T el = q.front();
            q.pop_front();

            mlock.unlock();
            is_not_full.notify_all();

            return el;            
        }

        T at(int index){
            return q[index];
        }

};