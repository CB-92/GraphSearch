#include <mutex>
#include <condition_variable>

using namespace std;

template<typename T>
class SynchArray
{
private:
    T* my_array;
    int len;
    mutex a_mutex;
    condition_variable is_updatable;
    int read_count;
    int write_count;
    bool update_flag;
public:

    SynchArray(T* a){
        read_count = 0;
        write_count = 0;
        my_array = a;
    }


    T get(int index){
        a_mutex.lock();
        read_count++;
        a_mutex.unlock();
        T el = my_array[index];
        a_mutex.lock();
        read_count--;
        if(read_count == 0)
            is_updatable.notify_all();
        a_mutex.unlock();

        return el;
    }

    void set(int index, T value){
        unique_lock<mutex> writer_lock(a_mutex);
        write_count++;
        while (read_count != 0){
            is_updatable.wait(writer_lock);
        }
        my_array[index] = value;

        if(write_count > 0)
            is_updatable.notify_all(); 
    }
    
};