#include <mutex>
#include <condition_variable>
#include <queue>

using namespace std;

template<typename T>
class SynchQueue{
    private:
        const unsigned int len = 50;
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

        void push(T el){
            unique_lock<mutex> mlock(mq);

            while (q.size() == len){
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