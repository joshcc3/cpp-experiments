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
void measureKV(KV<V>& v, string name) {
    int i;
    auto f = [&v, &i] (){
        int j = 0;
        for(; j < pow(2, i); j++) {
            v.put(j%10, j);
        }
        return 2;
    };

    for(i = 10; i < 22; i++) {
        stringstream s;
        s << name << ": " << pow(2, i);
        time_it<int>(s.str(), f);
    }
}

int randInt(int l, int u) {
    return (int)(rand() * (u - l)) + l;
}

void measureMap() {
    /*
     * write only
     * write followed by a number of reads
     * even number of writes and reads
     * 10% writes
     * 10% reads
     */
    int size = 1024;
    map<int, int> data;
    int readFraction = 50;

    ofstream dataFile("mapBenchmarks.csv", ios::app|ios::out);
    if(!(dataFile.is_open())) {
        throw "bad";
    }

    function<int()> writeOnly = [&data, &size]() {
        for(int i = 0; i < size; i++) {
            data[i] = i * i;
        }
        return 1;
    };
    function<int()> readOnly = [&data, &size]() {
        int sum = 0;
        for(int i = 0; i < size; i++) {
            sum += data[i];
        }
        return sum;
    };
    function<int()> readWrites = [&data, &size, &readFraction]() {
        int sum = 0;
        for(int i = 0; i < size; i++) {
            int read = randInt(0, 100);
            int ix = randInt(0, size);
            if(read < readFraction) {
                sum += data[ix];
            } else {
                data[ix] = sum;
            }
        }
        return sum;
    };
    /*
     * write only
     * write followed by a number of reads
     * even number of writes and reads
     * 10% writes
     * 10% reads
     */
    for(; size < pow(2, 20); size *= 2) {

        pair<int, int> resWriteOnly = time_it("WriteOnly" + to_string(size), writeOnly);
        dataFile << "WriteOnly," << size << "," << resWriteOnly.first << endl;
        pair<int, int> resReadOnly = time_it("ReadOnly" + to_string(size), readOnly);
        dataFile << "ReadOnly," << size << "," << resReadOnly.first << endl;
        readFraction = 50;
        pair<int, int> resEvenReadsWrites = time_it("EvenReadsWrites" + to_string(size), readWrites);
        dataFile << "EvenReadsWrites," << size << "," << resEvenReadsWrites.first << endl;
        readFraction = 10;
        pair<int, int> res10pctReads = time_it("10pctReads" + to_string(size), readWrites);
        dataFile << "10pctReads," << size << "," << res10pctReads.first << endl;
        readFraction = 90;
        pair<int, int> res10pctWrites = time_it("10pctWrites" + to_string(size), readWrites);
        dataFile << "10pctWrites," << size << "," << res10pctWrites.first << endl;
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

//
//int main() {
//    PersistentKV<int> kv(10);
//    measureKV(kv, "PersistentKV");
//}

int measureKVEntryPoint() {
    FastKV<int> kv(10);
    measureKV(kv, "FastKV");
    cout << "#####" << endl;
    PersistentKV<int> kv2(10);
    measureKV(kv2, "PersistentKV");
}

int main() {
    measureMap();
}