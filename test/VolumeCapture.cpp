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
    Command(const string &input) {
        stringstream stream(input);
        char typ;
        stream >> typ;
        if(typ == 'P') {
            int timestamp;
            tag = PRINT_VOL;
            stream >> print.timestamp;
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
        os << "PRINT " << obj.print.timestamp;
    } else if(obj.tag == Command::CMPY_TRADE) {
        os << "CMP_TRADE " << obj.cmpyTrade.timestamp << " " << obj.cmpyTrade.price << " " << obj.cmpyTrade.volume;
    } else if(obj.tag == Command::MARKET_TRADE) {
        os << "MKT_TRADE " << obj.mktTrade.timestamp << " " << obj.mktTrade.price << " " << obj.mktTrade.volume;
    }
    return os;
}

class FSM {
public:
    void process(Command cmd);
    void handlePrint(Print p);
    void handleMktTrade(MarketTrade trade);
    void handleCmpyTrade(CmpyTrade trade);
};

void FSM::handlePrint(Print p) {

}

void FSM::handleMktTrade(MarketTrade trade){

}

void FSM::handleCmpyTrade(CmpyTrade trade) {

}

void FSM::process(Command cmd) {
    cout << "Received the command: " << cmd << endl;
    if(cmd.tag == Command::PRINT_VOL) handlePrint(cmd.print);
    else if(cmd.tag == Command::MARKET_TRADE) handleMktTrade(cmd.mktTrade);
    else if(cmd.tag == Command::CMPY_TRADE) handleCmpyTrade(cmd.cmpyTrade);
}

int main() {
    FSM fsm;
    string input;
    getline(cin, input);
    while(!cin.eof()) {
        fsm.process(Command(input));
        getline(cin, input);
    }
}