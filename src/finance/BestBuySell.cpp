//
// Created by Joshua on 28/07/2019.
//

#include <iostream>
#include <string>
#include <sstream>
#include <algorithm>
#include <vector>
#include <cmath>

using namespace std;

/*
 * A <typ> <id> <price> <volume>
 * R <id> <volume>
 */
struct Add {
    char typ;
    int id;
    float price;
    int volume;
};

struct Remove {
    int id;
    int volume;
};

struct Entry {
    enum Tag{ADD, REMOVE} tag;
    union {
        Add a;
        Remove r;
    };
};

class OrderBook {
public:
    vector<Add> entries;
    void processEntry(Entry e);
    float bestBuy();
    float bestSell();
};

float OrderBook::bestBuy() {
    float res = NAN;
    for(auto it = entries.begin(); it != entries.end(); it++) {
        if(it->typ == 'B' && it->volume > 0) {
            if(isnan(res) || it->price > res) {
                res = it->price;
            }
        }
    }
    return res;
}

float OrderBook::bestSell() {
    float res = NAN;
    for(auto it = entries.begin(); it != entries.end(); it++) {
        if(it->typ == 'S' && it->volume > 0) {
            if(isnan(res) || it->price < res) {
                res = it->price;
            }
        }
    }
    return res;
}

void OrderBook::processEntry(Entry e) {
    if(e.tag == Entry::ADD) {
        entries.push_back(e.a);
    } else if(e.tag == Entry::REMOVE) {
        for(auto it = entries.begin(); it != entries.end(); it++) {
            if(it->id == e.r.id) {
                it->volume -= e.r.volume;
            }
        }
    } else {
        cerr << "Was asked to process an entry that had tag " << e.tag;
        throw 0;
    }
}

Entry parseEntry(const string &entry) {
    char typ;
    stringstream stream(entry);
    stream >> typ;
    if(typ == 'A') {
        int id, volume;
        float price;
        char typ;
        stream >> typ >> id >> price >> volume;
        Entry result;
        result.tag = Entry::ADD;
        result.a = {typ, id, price, volume};
        return result;
    } else if(typ == 'R') {
        int id, volume;
        stream >> id >> volume;
        Entry result;
        result.tag = Entry::REMOVE;
        result.r = {id, volume};
        return result;
    } else {
        stringstream message;
        cerr << "Received unexpected entry with type: " << typ << endl;
        throw 0;
    }
}

int main() {

    OrderBook *ob = new OrderBook();

    string entry;
    getline(cin, entry);
    while (!cin.eof()) {
        Entry e = parseEntry(entry);
        ob->processEntry(e);
        cout << "(" << ob->bestBuy() << "), (" << ob->bestSell() << ");" << endl;
        getline(cin, entry);
    }
}

void testEOF() {
    string entry;

    getline(cin, entry);
    cout << "Entry1: " << entry << "; EOF: " << cin.eof() << endl;

    getline(cin, entry);
    cout << "Entry2: " << entry << "; EOF: " << cin.eof() << endl;

    getline(cin, entry);
    cout << "Entry3: " << entry << "; EOF: " << cin.eof() << endl;

}