//
// Created by Joshua on 28/07/2019.
//

/*
 * Given a set of trades on the market and given a set of
 * trades made by us, print the percentage of volume
 * captured by us in the last 'n' milliseconds
 */

#include <iostream>
#include <algorithm>
#include <sstream>
#include <cmath>
#include <vector>
#include <string>

using namespace std;

struct Print {
    int timestamp;
    int interval;
};


struct MarketTrade {
    float price;
    int volume;
    int timestamp;
};

struct CmpyTrade {
    float price;
    int volume;
    int timestamp;
};
struct Command {
    enum Tag{PRINT_VOL, MARKET_TRADE, CMPY_TRADE} tag;
    union {
        Print print;
        MarketTrade mktTrade;
        CmpyTrade cmpyTrade;
    };
    Command() {}
    Command(const string &input) {
        stringstream stream(input);
        char typ;
        stream >> typ;
        if(typ == 'P') {
            int timestamp;
            tag = PRINT_VOL;
            stream >> print.timestamp >> print.interval;
        } else if(typ == 'M') {
            tag = MARKET_TRADE;
            stream >> cmpyTrade.price >> cmpyTrade.volume >> cmpyTrade.timestamp;
        } else if(typ == 'C') {
            tag = CMPY_TRADE;
            stream >> mktTrade.price >> mktTrade.volume >> mktTrade.timestamp;
        } else {
            cerr << "Encountered an unexpected command " << typ << endl;
            throw 1;
        }
    }
};

ostream& operator<<(ostream& os, const Command& obj) {
    if(obj.tag == Command::PRINT_VOL) {
        os << "PRINT " << obj.print.timestamp << " " << obj.print.interval << "ms";
    } else if(obj.tag == Command::CMPY_TRADE) {
        os << "CMP_TRADE " << obj.cmpyTrade.timestamp << " " << obj.cmpyTrade.price << " " << obj.cmpyTrade.volume;
    } else if(obj.tag == Command::MARKET_TRADE) {
        os << "MKT_TRADE " << obj.mktTrade.timestamp << " " << obj.mktTrade.price << " " << obj.mktTrade.volume;
    }
    return os;
}


template<class T>
class CircularBuffer {
    int start;
    int end;
    T* arr;
    int size;
    int length;
public:
    CircularBuffer(int s) : start(0), end(0), size(s), length(0) {
        arr = new T[s];
    }
    ~CircularBuffer() {
        delete[] arr;
    }
    T& operator[](size_t idx);
    void append(T t);
    void deque();
    int len();
};

template<class T>
int CircularBuffer<T>::len() {
    return CircularBuffer<T>::length;
}

template<class T>
T& CircularBuffer<T>::operator[](size_t idx) {
    return arr[(CircularBuffer<T>::start + idx) % CircularBuffer<T>::size];
}

template<class T>
void CircularBuffer<T>::append(T t) {
    int end = CircularBuffer<T>::end;
    int start = CircularBuffer<T>::start;
    int size = CircularBuffer<T>::size;
    int length = CircularBuffer<T>::length;
    arr[end] = t;
    CircularBuffer<T>::end = (end + 1)%size;
    if(CircularBuffer<T>::end == start) {
        CircularBuffer<T>::start = (start + 1)%size;
    }
    CircularBuffer<T>::length = min(size, length + 1);
}

template<class T>
void CircularBuffer<T>::deque() {
    int start = CircularBuffer<T>::start;
    int size = CircularBuffer<T>::size;
    CircularBuffer<T>::start = (start+1)%size;
    CircularBuffer<T>::length--;
}

class FSM {
    CircularBuffer<MarketTrade> mktTrades;
    CircularBuffer<CmpyTrade> cmpTrades;
    int size;
public:
    FSM(int s) : size(s), mktTrades(s), cmpTrades(s) {}
    void process(Command cmd);
    void handlePrint(Print p);
    void handleMktTrade(MarketTrade trade);
    void handleCmpyTrade(CmpyTrade trade);
};


void FSM::handlePrint(Print p) {
    int cutOff = p.timestamp - p.interval;
    int cmpStart = 0;
    int mktStart = 0;
    while(cmpStart < cmpTrades.len() && cmpTrades[cmpStart].timestamp < cutOff) {
        cmpStart++;
    }
    while(mktStart < mktTrades.len() && mktTrades[mktStart].timestamp < cutOff) {
        mktStart++;
    }
    if(cmpStart >= cmpTrades.len()) {
        cout << "Nan" << endl;
        return;
    }
    int mktVol = 0;
    int cmpVol = 0;
    int mktIx = mktStart;
    for(int i = cmpStart; i < cmpTrades.len() && p.timestamp >= cmpTrades[i].timestamp; i++) {
        auto cmpTrade = cmpTrades[i];
//        Command cmd;
//        cmd.tag = Command::CMPY_TRADE;
//        cmd.cmpyTrade = cmpTrade;
//        cout << "DEBUG: " << cmd << endl;
        while(mktTrades[mktIx].timestamp < cmpTrade.timestamp) {
            mktIx++;
        }
        int mktTmpVol = 0;
        for(int m2 = mktIx; mktTrades[m2].timestamp == mktTrades[mktIx].timestamp; m2++) {
            if(mktTrades[m2].price == cmpTrade.price) {
                mktTmpVol += mktTrades[m2].volume;
            }
        }
        mktVol += cmpTrade.price * mktTmpVol;
        cmpVol += cmpTrade.price * cmpTrade.volume;
    }
    cout << cmpVol/float(mktVol) << endl;
}

void FSM::handleMktTrade(MarketTrade trade){
    mktTrades.append(trade);
}

void FSM::handleCmpyTrade(CmpyTrade trade) {
    cmpTrades.append(trade);
}

void FSM::process(Command cmd) {
    cout << "Received the command: " << cmd << endl;
    if(cmd.tag == Command::PRINT_VOL) handlePrint(cmd.print);
    else if(cmd.tag == Command::MARKET_TRADE) handleMktTrade(cmd.mktTrade);
    else if(cmd.tag == Command::CMPY_TRADE) handleCmpyTrade(cmd.cmpyTrade);
}

int main() {
    int interval;
    cin >> interval;
    string input;
    getline(cin, input);

    FSM fsm(interval);

    getline(cin, input);
    while(!cin.eof()) {
        fsm.process(Command(input));
        getline(cin, input);
    }
}
// Start: 17:21
/*    t   p    v        t   p    v
 * c: 1  10  100 | m:   1  10  200
 *    4  11  100 |      4   8 1000
 *    6  12  100 |      4  11  200
 *               |      4  11  200
 *               |      6  13 1000
 *               |      6  12  600
 *               |     10  13 1000
 *
 */