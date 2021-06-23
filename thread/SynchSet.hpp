#include <mutex>
#include <condition_variable>
#include <set>

#ifndef SYNCHSET_HPP
#define SYNCHSET_HPP

using namespace std;

template<typename T>
class SynchSet
{
private:
    set<T> my_set;
    mutex set_mutex;
    condition_variable is_updatable;
    int read_count;
    int write_count;
    bool update_flag;
public:

    SynchSet(){
        read_count = 0;
        write_count = 0;
    }

    bool contains(T el){
        set_mutex.lock();
        read_count++;
        set_mutex.unlock();
        bool res = my_set.find(el) != my_set.end();
        set_mutex.lock();
        read_count--;
        if(read_count == 0)
            is_updatable.notify_all();
        set_mutex.unlock();

        return res;
    }

    int size(){
        return my_set.size();
    }

    void add(T el){
        unique_lock<mutex> writer_lock(set_mutex);
        write_count++;
        while (read_count != 0){
            is_updatable.wait(writer_lock);
        }
        
        my_set.insert(el);
        write_count--;

        if(write_count > 0)
            is_updatable.notify_all();        
    }
};

#endif
