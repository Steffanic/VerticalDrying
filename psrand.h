#include <iostream>
using namespace std;

struct psRand {
    int64_t a, m, seed, rando;

    psRand(const int64_t x): a(10014146), m(549755813881), seed(x), rando(x){ 
        if(!(x >= 1 && x<= m-1)){
            cout<<"incorrect seed. Setting to 42";
            seed = 42;
            rando = seed;
        }
    }

    void updateRando(){
        rando = a*(rando)%m;
        return;
    }

    int64_t getRando(){
        updateRando();
        return rando;
    }
};