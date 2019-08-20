#include <iostream>
#include <math.h>

using namespace std;

template<class A>
struct array {
    A * const arr;
    const int size;
};

bool isPrime(int);

template <class A, class B>
array<B> map(B (*f)(A), array<A> a) {
    B* const storage = (B*)malloc(sizeof(B) * a.size);
    array<B> res = { .arr = storage, .size = a.size };
    for(int i = 0; i < a.size; i++) {
        storage[i] = f(a.arr[i]);
    }
    return res;
}

template<class A, class B>
array<B> foldl(B (*f)(A, B), B seed, array<A> l) {
    for(int i = 0; i < l.size; i++) {
        seed = f(l.arr[i], seed);
    }
    return seed;
}


int square(int x) {
    return x * x;
}

int dbl(int x) {
    return 2 * x;
}

template<class A>
void printArray(array<A> a) {
    for(int i = 0; i < a.size; i++) {
        cout << a.arr[i] << " ";
    }
    cout << endl;
}

template <class A, class B>
using fn_t = B(A);

template<class A, class B, class C>
C (*compose(fn_t<B, C> f, fn_t<A, B> g)) (A) {
    return [f1=move(f), g1= move(g)](A a) {
      return f1(g1(a));
    };
    //    return f(x);
    //    return f(g(a));
}

void mapExample() {
    int size;
    cin >> size;
    int internal[size];
    for(int i = 0; i < size; i++) {
        cin >> internal[i];
    }
    array<int> a = { .arr = internal, .size = size};
    array<int> b = map<int, int>(square, a);
    printArray<int>(b);
}

int main() {
    composeExample();
}

bool isPrime(int n) {
    for(int i = 2; i <= pow(n, 0.5); i++) {
        if(n % i == 0) {
            return false;
        }
    }
    return true;
}