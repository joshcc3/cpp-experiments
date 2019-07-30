//
// Created by Joshua on 28/07/2019.
//

#include <map>
#include <algorithm>
#include <iostream>
#include <string>
#include <sstream>
#include<vector>

using namespace std;

#include <curl/curl.h>


class http {
    static CURL *handle;
    static vector<string> data_buffer;
    static map<string, string> header_buffer;
    static char error_buffer[];
    static size_t data_callback(void *, size_t, size_t, vector<string>*);
    static size_t header_callback(void *, size_t, size_t, map<string, string>*);

    static CURL* initialize_handle();
public:
    struct response {
        int status_code;
        map<string, string> headers;
        stringstream body;
    };

    static void get(const string& url, response& result, const map<string, string>& headers=map<string, string>()) {
        curl_easy_setopt(handle, CURLOPT_URL, url.c_str());
        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &result.status_code);

        struct curl_slist *hdrs = NULL;
        if(!headers.empty()) {
            for (auto it = headers.begin(); it != headers.end(); it++) {
                hdrs = curl_slist_append(hdrs, string(it->first + ": " + it->second).c_str());
            }
            curl_easy_setopt(handle, CURLOPT_HTTPHEADER, hdrs);
        }

        curl_easy_perform(handle);
        if(!headers.empty()) {
            curl_slist_free_all(hdrs);
        }
        for(auto it = data_buffer.begin(); it != data_buffer.end(); it++) {
            result.body << *it;
        }
    }
};

CURL* http::handle = http::initialize_handle();
vector<string> http::data_buffer;
map<string, string> http::header_buffer;
char http::error_buffer[CURL_ERROR_SIZE];


size_t http::data_callback(void *ptr, size_t size, size_t nmemb, vector<string>* data) {
    data->push_back((char*) ptr);
    return size * nmemb;
}

size_t http::header_callback(void *ptr, size_t s, size_t n, map<string, string>* hdrs) {
    return s * n;
}

CURL* http::initialize_handle() {
    handle = curl_easy_init();
    if(!handle) {
        cerr << "Failed to init curl" << endl;
    }
    curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1);
    curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
    curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, data_callback);
    curl_easy_setopt(handle, CURLOPT_WRITEDATA, &data_buffer);
    curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);
    curl_easy_setopt(handle, CURLOPT_HEADERDATA, &header_buffer);
    curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, &error_buffer);
    return handle;
}

int main() {
    http::response resp;
    http::get("https://jsonplaceholder.typicode.com/todos/1", resp);
    cout << resp.body.str() << endl;
}
