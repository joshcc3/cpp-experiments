//
// Created by Joshua on 01/08/2019.
//

#include <iostream>
#include <algorithm>
#include <vector>
#include <cmath>

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

int find(int val, vector<int>& v, int s, int e) {
    if(s == e) {
        return -1;
    } else if(e - s == 1) {
        if(v[s] > v) {
            return -1;
        } else {
            return s;
        }
    } else if(val > v[s] && val < v[e]) {
        int mid = (s + e)/2;
        if(val > v[mid]) {

        } else if(val < v[mid]) {

        } else {
            return mid;
        }
    } else if(val == v[s]) {
        return s;
    } else if(val < v[s]) {
        return -1;
    }
}

int nthElem(vector<int>& n1, int s1, int e1, vector<int>& n2, int s2, int e2, int e) {
    if(e == 0) {
        return n1[0] < n2[0] ? n1[0] : n2[0];
    } else {

        int m1 = (e1 + s1)/2;
        int m2 = find(n1[m1], n2, s2, e2);
        if(m1 + m2 < e) {
            return nthElem(n1, m1 + 1, e1, n2, m2 + 1, e2, e);
        } else if(m1 + m2 == e) {
            return n1[m1] > n2[m2] ? n[m1] : n2[m2];
        } else {
            return nthElem(n1, s1, m1, n2, s2, m2, e);
        }
    }

}

double findMedianSortedArrays(vector<int>& nums1, vector<int>& nums2) {
        int n1 = nums1.size();
        int n2 = nums2.size();

        if((n1 + n2) % 2 == 0) {
            return ((float)(nthElem(nums1, num2, (n1 + n2)/2)) +  (float)(nthElem(nums1, num2, (n1 + n2)/2)))/2.0;
        } else {
            return nthElem(nums1, num2, 0, n1, 0, n2, (n1 + n2)/2.0);
        }
}




int main() {
}
