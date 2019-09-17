//
// Created by Joshua on 08/09/2019.
//

#include <iostream>
#include <cstdlib>
#include <iostream>
#include <exception>
#include <string>
#include <stdexcept>
#include <vector>

using namespace std;

class B {
    int x;
    virtual void f()=0;
};

class A : public B {

};

class C : public B {
    void f() {

    }
};

int main() {

    A *a;
    B *b = dynamic_cast<B*>(a);
//    B *b = new C();
    C *c = dynamic_cast<C*>(a);

    //
//    try {
//        void* mem = new int[1024*1024*1024][1024*1024];
//    } catch(bad_alloc e) {
//        cout << "Bad Alloc" << endl;
//    }
}