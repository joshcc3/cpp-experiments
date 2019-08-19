//
// Created by Joshua on 01/08/2019.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>
#include <assert.h>

using namespace std;

class X {
public:
    int a;
    int *p;
    X(int x): a(x) {}
    X(): a(0) {}
};

class Y : public X {
    int b;
public:
    Y(int x): b(x),X(x + 1)  {}
    Y(): b(0), X() {}
};

class Base {
public:
    int z;
    int x;
    int y;

    int *p;
    Base() : z(0) {}
};

class A: public Base {
public:
    int z;
    A(): z(1) {}
};

class B : public Base {
public:
    int z;
    B() : z(2) {}
};

class C : public A, public B {
public:
    int z;
    C() : z(3) {}
};


/**
 * What do the access specifiers mean
 * Can a subclass access all properties of its parent class?
 * Do the access specifiers only control its access from out side a class
 * Prove that the size of a base class increases with diamond inheritance
 * What happens when the same variable defined in parent classes has different access specifiers
 * How does virtual interact with classes?
 */
//int main() {
//
//    C c;
//    cout << sizeof(Base) << endl;
//    cout << sizeof(A) << endl;
//    cout << sizeof(B) << endl;
//    cout << sizeof(C) << endl;
////    cout << c.A::Base::z << endl;
//
//}


class Buffer {
    int *buffer;
    int size;

public:
    Buffer(int n): size(n) {
        buffer = new int[size];
    }
    ~Buffer() {
        delete buffer;
    }

    void init(int s, int l) {
        for(auto it = buffer; it != buffer + size; it++) {
            *it = rand() * (l - s) + s;
        }
    }


    int& operator[](size_t idx) {
        if(idx < size && idx >= 0)
            return *(buffer + idx);
        else
            throw 1;
    }

};

void mostProfit(vector<int>& v, vector<int>& returnVal) {
    int s = v[0];
    int l = v[0];
    int result = l - s;

    for(auto it = v.begin(); it != v.end(); it++) {
        int v = *it;
        if(v < s) {
            s = v;
            l = v;
        } else if(v > l) {
            l = v;
        }
        result = max(result, l - s);
        returnVal.push_back(result);
    }
}

void mostProfitReverse(vector<int> &v, vector<int>& returnVal) {
    int s = v[v.size() - 1];
    int l = s;
    int result = l - s;
    for(auto it = v.rbegin(); it != v.rend(); it++) {
        int v = *it;
        if(v > l) {
            s = v;
            l = v;
        } else if(v < s) {
            s = v;
        }
        result = max(result, l - s);
        returnVal.push_back(result);
    }
    reverse(returnVal.begin(), returnVal.end());
}

int mostProfit1Trade(vector<int> &v) {

    vector<int> prefixes;
    mostProfit(v, prefixes);
    return prefixes[v.size() - 1];
}

int mostProfit2Trades(vector<int> &v) {

    vector<int> prefixes;
    vector<int> suffixes;
    mostProfit(v, prefixes);
    mostProfitReverse(v, suffixes);
    int result = suffixes[0];
    for(int i = 0; i < v.size() - 1; i++) {
        result = max(result, prefixes[i] + suffixes[i+1]);
    }
    return result;
}

int main() {
    vector<int> v;
    v.push_back(4);
    v.push_back(1);
    v.push_back(2);
    v.push_back(3);
    v.push_back(8);
    v.push_back(7);
    v.push_back(7);
    v.push_back(8);
    v.push_back(7);
    v.push_back(9);
    cout << mostProfit2Trades(v) << endl;
    cout << mostProfit1Trade(v) << endl;
}

int testSingleTradeProfit() {
    vector<int> v;
    for(int i = 0; i < 10; i++) {
        v.push_back(i);
    }
//    cout << mostProfit(v) << endl;
//    assert(mostProfit(v) == 9);
}