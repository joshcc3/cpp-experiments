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
    static vector<string>* data_buffer;
    static map<string, string>* header_buffer;
    static char* error_buffer;
    static size_t data_callback(char *ptr, size_t size, size_t nmemb, void *userdata);
    static size_t header_callback(char *ptr, size_t size, size_t nmemb, void *userdata);

    static CURL* initialize_handle() {
        handle = curl_easy_init();
        if(!handle) {
            cerr << "Failed to init curl" << endl;
        }
        curl_easy_setopt(handle, CURLOPT_TCP_KEEPALIVE, 1);
        curl_easy_setopt(handle, CURLOPT_NOPROGRESS, 1);
        curl_easy_setopt(handle, CURLOPT_NOSIGNAL, 1);
        curl_easy_setopt(handle, CURLOPT_WRITEFUNCTION, data_callback);
        curl_easy_setopt(handle, CURLOPT_WRITEDATA, (void*)data_buffer);
        curl_easy_setopt(handle, CURLOPT_HEADERFUNCTION, header_callback);
        curl_easy_setopt(handle, CURLOPT_HEADERDATA, (void*)header_buffer);
        curl_easy_setopt(handle, CURLOPT_ERRORBUFFER, error_buffer);
    }
public:
    struct response {
        int status_code;
        map<string, string> headers;
        stringstream body;
    };

    static void get(const string& url, response& result, const map<string, string>& headers=map<string, string>()) {
        const char* c_url = url.c_str();
        char* escaped_url = curl_easy_escape(handle, url.c_str(), url.length());

        curl_easy_setopt(handle, CURLOPT_URL, escaped_url);
        curl_easy_setopt(handle, CURLOPT_HTTPGET, 1);

        auto res = curl_easy_perform(handle);
        if(res != CURLE_OK) {
            cerr << error_buffer << " " << res;
            throw res;
        }

        curl_easy_getinfo(handle, CURLINFO_RESPONSE_CODE, &result.status_code);
        result.headers = move(*header_buffer);
        result.body = stringstream();
        for(auto it = data_buffer->begin(); it != data_buffer->end(); it++) {
            result.body << *it;
        }
        curl_free(escaped_url);

    }
};

CURL* http::handle = http::initialize_handle();
vector<string>* http::data_buffer = new vector<string>();
map<string, string>* http::header_buffer = new map<string, string>();
char* http::error_buffer = new char[1000];

size_t http::data_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    vector<string>* buf = (vector<string>*)userdata;
    buf->push_back(ptr);
    return nmemb;
}

size_t http::header_callback(char *ptr, size_t size, size_t nmemb, void *userdata) {
    map<string, string>* hdrs = (map<string, string>*)userdata;
    char *ix = ptr;
    while(*ix != ':' && ptr - ix < nmemb) ix++;
    *ix = 0;
    hdrs->emplace(string(ptr), string(ix + 1));
    return nmemb;
}

int main() {
    http::response resp;
    http::get("https://jsonplaceholder.typicode.com/todos/1", resp);
    cout << resp.body.str() << endl;
}
