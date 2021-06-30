#include <mutex>

using namespace std;

template<typename T>
class AtomicType
{
private:
public:
    AtomicType(){}
    ~AtomicType(){}

    T value;
    mutex mux;

    unique_lock<mutex> acquire(){
        return unique_lock<mutex>(mux);
    }
};
