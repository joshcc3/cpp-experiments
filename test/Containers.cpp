//
// Created by Joshua on 27/07/2019.
//

#include <algorithm>
#include <sstream>
#include <iostream>

using namespace std;

template <class T>
class ArrayList {
public:
    T* arr;
    int size;
    int length;
    ArrayList(int s) : size(s) {
        cout << "Base List Con" <<endl;
        arr = new T[s];
        size = s;
        length = 0;
    }
    virtual ~ArrayList() {
        cout << "Base List Destr" <<endl;
        delete arr;
    }
    virtual string toString() {
        stringstream s;
        for(int i = 0; i < length; i++) {
            s << operator[](i) << " ";
        }
        return s.str();
    }

    virtual T& operator[](const int i)=0;
    virtual void append(T t)=0;
};

template<class T>
class CircularBuffer : public ArrayList<T> {
    using ArrayList<T>::length;
    using ArrayList<T>::size;
    using ArrayList<T>::arr;
    int start;
    int end;
public:
    CircularBuffer(int s) : ArrayList<T>(s) {
        cout << "CircBuf Con" << endl;
        start = 0;
        end = 0;
    }

    ~CircularBuffer() {
        cout << "Circ Buf  Destr" << endl;
    }

    int len() {
        return length;
    }

    T& operator[](const int i) override {
        return arr[(start + i)%size];
    }

    void append(T t) override {
        length = min(length + 1, size);
        arr[end] = t;
        if(end == start && length == size) {
            start = (start + 1)%size;
        }
        end = (end + 1)%size;
    }
};

int main() {

    CircularBuffer<int> buf(10);
    cout << buf.toString() << endl;
    buf.append(1);
    cout << buf.len() << "  "<< buf.toString() << endl;

    cout << buf.toString() << endl;

    for(int i = 7; i < 22; i++) {
        buf.append(i);
        cout << buf.len() << " " << buf.toString() << endl;
    }
    cout << buf.toString() << endl;
    cout << buf[9] << endl;
}


