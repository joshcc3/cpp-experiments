//
// Created by Joshua on 14/08/2019.
//

#include <iostream>
#include <algorithm>
#include <ctime>
#include <cmath>
#include <chrono>
#include <thread>
#include <utility>
#include <functional>
#include <sstream>
#include <string.h>
#include <vector>
#include <fstream>
#include <queue>
#include <map>
#include <list>
#include <assert.h>

using namespace std;

template<class A>
pair<int, A> time_it(const string& e, function<A()> f) {
    auto start = chrono::high_resolution_clock::now();
    A result = f();

    auto end = chrono::high_resolution_clock::now();
    int count = (chrono::duration_cast<chrono::microseconds>(end - start)).count();
    cout << "Experiment " << e << " took: " << count / 1000.0 << "ms" << endl;

    return pair<int, A>(count, result);
}

ostream& operator<<(ostream& out, const map<int, int> &c) {
    out << "{";
    auto last_elem = --c.end();
    auto it = c.begin();
    for(; it != last_elem; it++) {
        out << "(" << it->first << ", " << it->second << "), ";
    }
    if(!c.empty()) {
        out << "(" << it->first << ", " << it->second << ")";
    }
    out << "}";
    out << endl;
    this_thread::sleep_for(2s);
    return out;
}

ostream& operator<< (ostream &out, const list<int> &c) {
    out << "list: ";
    out << "[";
    auto lastElem = --c.end();
    for(auto it = c.begin(); it != lastElem; it++) {
        out << *it << ", ";
    }
    if(!c.empty()) {
        out << *lastElem;
    }
    out << "]";
    return out;
}


template <class V>
class KV {
public:
    virtual void get(int k, V& v)=0;
    virtual bool put(int k, const V& v)=0;
    virtual int size()=0;
};

template <class V>
class FastKV : public KV<V> {
    int _size;
    V* store;
public:
    FastKV(int s) : KV<V>(), _size(s) {
        store = new V[_size];
    }

    ~FastKV() {
        delete store;
    }

    void get(int k, V& v) {
        if(k >= _size || k < 0) {
            v = 0;
        } else {
            v = *(store + k);
        }
    }

    bool put(int k, const V& v) {
        if(k >= _size || k < 0) {
            return false;
        }
        store[k] = v;
        return true;
    }

    int size() {
        return _size;
    }
};

template <class K, class V>
class LRU {
    int capacity;
    int _size;
public:
    list<pair<K, V>> seen;
    map<K, typename list<pair<K, V>>::iterator> ptrTable;
    LRU(int s) : capacity(s) {
        _size = 0;
    }

    void add(const K& k, const V& v) {
        if(ptrTable.count(k) > 0) {
            seen.erase(ptrTable[k]);
        } else {
            if (_size == capacity) {
                auto firstElem = seen.begin();
                ptrTable.erase(ptrTable.find(firstElem->first));
                seen.erase(firstElem);
            } else {
                _size++;
            }
        }
        ptrTable[k] = seen.insert(seen.end(), pair<K, V>(k, v));
    }

    V& get(K& k) {
        if(ptrTable.count(k) > 0) {
            return ptrTable[k]->second;
        }
        throw 0;
    }
    V& get(K&& k) {
        if(ptrTable.count(k) > 0) {
            return ptrTable[k]->second;
        }
        throw 0;
    }

    int size() {
        return _size;
    }
};


void lruTestCorrectness() {
    {
        LRU<int, int> l(1);
        l.add(1, 1);
        int k1 = 1;
        assert(l.get(1) == 1);
        l.add(2, 1);
        assert(l.size() == 1);
    }
    {
        LRU<int, int> l(5);
        for(int i = 1; i < 5; i++) {
            l.add(i, i*i);
        }
        for(int i = 1; i < 5; i++) {
            assert(l.get(i) == i * i);
        }
        assert(l.size() == 4);
        for(int i = 4; i >= 1; i--) {
            l.add(i, i*i);
            assert(l.size() == 4);
        }
        for(int i = 1; i < 5; i++) {
            assert(l.get(i) == i * i);
        }

        l.add(5, 25);
        assert(l.size() == 5);
        l.add(6, 36);
        assert(l.size() == 5);
        try {
            l.get(4);
            assert(false);
        } catch(int e) {}
        assert(l.get(1) == 1);
        assert(l.get(2) == 4);
        assert(l.get(3) == 9);
        assert(l.get(5) == 25);
        assert(l.get(6) == 36);
        l.add(7, 49);
        assert(l.get(1) == 1);
        assert(l.get(2) == 4);
        assert(l.get(5) == 25);
        assert(l.get(6) == 36);
        assert(l.get(7) == 49);
    }
    cout << "All tests passed" << endl;

}

void lruTestPerformance() {

}

template <class V>
class PersistentKV : public KV<V> {
    int _size;
    fstream store;
    int fileSize;
public:
    PersistentKV(int s) : KV<V>(), _size(s) {
        {
            ofstream f;
            f.open("contents.kv", fstream::out | fstream::binary | fstream::ate | fstream::app);
            if(!f.is_open()) {
                throw "bad";
            }
            fileSize = f.tellp();
            cout << "File size is " << fileSize << endl;
        }

        store.open("contents.kv", fstream::in | fstream::out | fstream::binary);
        int requiredSize = sizeof(int) * s;
        if(fileSize < requiredSize) {
            cout << "Expanding file by " << requiredSize - fileSize << endl;
            char buf[requiredSize - fileSize];
            memset(buf, 0, requiredSize - fileSize);
            store.write(buf, requiredSize - fileSize);
        }
    }

    ~PersistentKV() {
        store.flush();
        store.close();
    }

    int size() {
        return _size;
    }

    void get(int k, V& res) {
        if(!(k >= _size || k < 0)) {
            cout << "Seeking to " << k * sizeof(int) << endl;
            store.seekg(k * sizeof(int));
            store.read((char*)&res, sizeof(V));

            cout << "Result set to " << res << endl;
        } else {
            throw "bad";
        }
    }

    bool put(int k, const V& v) {
        if(k >= _size || k < 0) {
            return false;
        }
        store.seekg(k * sizeof(int));
        store.write((char*)&v, sizeof(V));
        return true;
    }

    void close() {
        store.flush();
    }
};

template <class V>
void measureKV(KV<V>& v) {
    int i;
    auto f = [&v, &i] (){
        int j = 0;
        for(; j < pow(2, i); j++) {
            v.put(j%10, j);
        }
        cout << "Wrote to " << j << endl;
        return 2;
    };

    for(i = 10; i < 22; i++) {
        time_it<int>("KV write[0]", f);
    }
}

void testPersistentKV() {
    PersistentKV<int> v(10);
    v.put(1, 10);
    v.put(2, 11);
    int res = 1;
    v.get(1, res);
    cout << res << endl;
    v.get(2, res);
    cout << res << endl;
}

void initKV(PersistentKV<int>& kv) {
    for(int i = 0; i < 10; i++) {
        kv.put(i, i * i * i);
    }
}

int testPersistence() {
//    testPersistentKV();
//    FastKV kv(10);
//    measureKV(kv);

    PersistentKV<int> kv(10);
//    initKV(kv);

    list<int> l(10);
    int i = 0;
    for(auto it = l.begin(); it != l.end(); it++, i++) {
        kv.get(i, *it);
    }
    cout << l << endl;
}


int main() {
//    measureKV(kv);
//
//    {
//        PersistentKV<int> kv(10);
//        kv.put(1, 1);
//        kv.put(2, 10);
//        kv.put(3, 100);
//        kv.put(4, 1000);
//        kv.close();
//    }

    PersistentKV<int> kv(10);
    int r = 1;
    kv.get(4, r);
    cout << r << endl;
//    kv.close();
//    testPersistence();
}