//
// Created by Joshua on 28/07/2019.
//

#ifndef JJVM_HTTP_H
#define JJVM_HTTP_H

#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>

using namespace std;

class http {
    static void get(const string& url, const string& result, const map<string, string>& headers);
};

#endif //JJVM_HTTP_H
