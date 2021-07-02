#include "Pool.hpp"
#include <iostream>
#include<vector>

using namespace std;

int count_even(int el){
    cout << "Consuming " << el << "\n";
    int res = 0;
    if (el%2==0){
        res++;
    }
    return res;   
}

int main(int argc, char *argv[]){


    return 0;

}