#include <vector>
#include <iostream>
#include <thread>
#include <utility>
#include <chrono>


using namespace std;


class X {
public:
  int v;
  X(): v(123) {
    cout << "Default constructor has been called" << endl;
  }

  X(const X& x) {
    cout << "Copy constructor has been called" << endl;
    v = x.v;
  }

  X(X&& val) {
    cout << "The move constructor has been called" << endl;
    v = val.v;
  }
  
  
  ~X() {
    cout << "Destructor has been called" << endl;
    v = 321;
  }
};

X genX() {
  return X();
}

int main() {
  cout << "Main: Default constructor call" << endl;
  X x;

  cout << "Main: Copy constructor should be called" << endl;
  X y = x;
  cout << "Main: Copy constructor should be called" << endl;
  X z(x);
  cout << "Main: Destructor should be called" << endl;
  x.~X();
  cout << "Main: 1 Move constructor should be called" << endl;
  X m = move(y);
  
  cout << "Main: 2 Move constructor should be called" << endl;
  X n = move(genX());
  

  X o[3];
  cout << "Main: 3 Copy constructor was called here." << endl;
  X p(*(o + 2));
  


  cout << x.v << endl;
}
