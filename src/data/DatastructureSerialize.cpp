//
// Created by Joshua on 19/08/2019.
//

#include <chrono>
#include <utility>
#include <cmath>
#include "DatastructureSerialize.h"
#include <string>
#include <cstring>
#include <deque>
#include <stack>
#include <functional>
#include <sstream>
#include <map>
#include <iostream>
#include <vector>


/**
 * Definition for a binary tree node.
 **/

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



struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};

const unsigned char _masks[4] = { (1 << 7), (3 << 6), (7 << 5), (15 << 4)};
const unsigned char masks[4] = { ~_masks[0], ~_masks[1], ~_masks[2], ~_masks[3] };


int read_varint(const unsigned char* rep) {

  int result = 0;
  unsigned char* ref = (unsigned char*) &result;
  int ix = 0;
  const unsigned char *it = rep;
  int size = 0;
  for(; (_masks[0] & *it) == 0; size++, it++);
  size++;
  int res = 0;
  if(size == 1) {
    ref[0] = masks[0] & rep[0];
  } else {
    ref[0] = rep[0] << 1;
    if(size == 2) {
      ref[0] |= (rep[1] >> 6) & 1;
      ref[1] = rep[1] & masks[1];
    } else {

      ref[0] |= (rep[1] >> 6) & 1;
      ref[1] = (rep[1] & masks[1]) << 2;
      if(size == 3) {
	ref[1] |= (rep[2] >> 5) & 3;
	ref[2] = rep[2] & masks[2];
      } else {
	ref[1] |= (rep[2] >> 5) & 3;
	ref[2] = rep[2] << 3;
	if(size == 4) {
	  ref[2] |= (rep[3] >> 4) & 7;
	  ref[3] = rep[3] & masks[3];
	} else {
	  ref[2] |= (rep[3] >> 4) & 7;
	  ref[3] = ((rep[3] & masks[3]) << 4) | ((rep[4] >> 3) & 15);
	}
      }
    }
  }
  return result;
}

unsigned char msb = 1 << 7;

int trivial_encode(unsigned char *rep, int _v) {
  *((int*)rep) = _v;
  return 4;
}

int varint(unsigned char *rep, int _v) {
  uint v = (uint)_v;
  unsigned long long *result = (unsigned long long*)(rep);
  unsigned char* vc = (unsigned char*)(&v);
  if(v < 128) {
    *result = msb | vc[0];
    return 1;
  }
  else if(v < (1 << 14)) {
    *result = (vc[0] >> 1) | ((uint)msb << 8) | ((uint)(vc[0] & 1) << 14) | ((uint)vc[1] << 8);
    return 2;
  }
  else if(v < (1 << 21)) {
    *result = (vc[0] >> 1) | ((uint)(((vc[0] & 1) << 6) | (vc[1] >> 2)) << 8) | ((uint)(msb | ((vc[1] & 3) << 5) | vc[2]) << 16);
    return 3;
  }
  else if(v < (1 << 28)) {
    *result = (vc[0] >> 1) | ((uint)(((vc[0] & 1) << 6) | (vc[1] >> 2)) << 8) | ((uint)(((vc[1] & 3) << 5) | (vc[2] >> 3)) << 16) | ((uint)(msb | ((vc[2] & 7) << 4) | vc[3]) << 24);
    return 4;
  } else {
    *result = (vc[0] >> 1) | ((unsigned long long)((vc[0] & 1) << 14)) | ((unsigned long long)vc[1] << 6) | ((unsigned long long)(vc[1] & 3) << 21) | ((unsigned long long)vc[2] << 13) | ((unsigned long long)(vc[2] & 7) << 28) | ((unsigned long long)(vc[3]) << 20) | ((unsigned long long)msb << 35)  | ((unsigned long long)(vc[3] & 15) << 35);
    return 5;
  }
}
struct VarInt {
  unsigned char* data;
  int size;
  VarInt(unsigned char* dat, int s): data(dat), size(s) {}
};


ostream& operator<<(ostream& os, const VarInt& vi ) {
  os << "[";
  for(int i = 0; i < vi.size - 1; i++) {
    os << "0x" << hex << (uint)(vi.data[i]) << ", ";
  }
  os << "0x" << hex << (uint)(vi.data[vi.size - 1]) << "]";
  return os;
}


int test_varint() {
  for(int i = 0; i < 10; i++) {
    unsigned char dat[5];
    cout << "VarInt for " + to_string(i) + ": " << VarInt(dat, varint(dat, i)) << endl;
  }
  vector<int> testcases = {(1 << 7), (1 << 7) + 2, (3 << 6), (1 << 15), (1 << 15) + (1 << 7) + 1, (1 << 23) + (1 << 15) + (1 << 7) + 1, (1 << 31) + (1 << 23) + (1 << 15) + (1 << 7) + 1};

  for(auto it = testcases.begin(); it != testcases.end(); it++) {
    unsigned char dat[5];
    cout << "VarInt for 0x" << hex << *it << ": " << VarInt(dat, varint(dat, *it)) << endl;
  }
}

int test_read_varint() {
  for(int i = 0; i < 10; i++) {
    unsigned char dat[5];
    varint(dat, i);
    int result = read_varint(dat);
    cout << "Round-tripping " + to_string(i) + ": " << result << endl;
  }
  vector<int> testcases = {(1 << 7), (1 << 7) + 2, (3 << 6), (1 << 15), (1 << 15) + (1 << 7) + 1, (1 << 23) + (1 << 15) + (1 << 7) + 1, (1 << 31) + (1 << 23) + (1 << 15) + (1 << 7) + 1};
 
  for(auto it = testcases.begin(); it != testcases.end(); it++) {
    unsigned char dat[5];
    VarInt vi(dat, varint(dat, *it));
    int result = read_varint(dat);
    cout << "Round-tripping 0x" << hex << *it  << " -> 0x" << vi << " -> " << "0x" << hex << result << endl;
  }
}


int performance_test_varint() {
  // test_varint();
  // test_read_varint();
  unsigned char dat[5];
  int size;
  //function<int(unsigned char*, int)> testFun = trivial_encode;
  function<int(unsigned char*, int)> testFun = varint;
  function<int()> f = [&] {
    for(int i = 0; i < size; i++) {
      testFun(dat, 1<<31);
    }
    return 0;
  };

  for(int i = 10; i < 30; i++) {
    size = pow(2, i);
    time_it("Serialize " + to_string(size) + " ints: ", f);
  }
}


int main() {
  //test_varint();
  //test_read_varint();
  performance_test_varint();
}



class StringCodec {

  int _writeHelper(const TreeNode *input, char* result) {
    
    memcpy(*result, &(input->val), 4);
    *(result + 4) = "{";
    int rsize = _writeHelper(input, result + 5);
    *(result + 5 + rsize) = "}";
    *(result + 6 + rsize) = "{";
    int lsize = _writeHelper(input, result + 7 + rsize);
    *(result + 7 + rsize + lsize) = "}";
    return 8 + rsize  + lsize;
  }

public: 
  void write(const TreeNode *input, char** result) {
    int size = estimateSize(input);
    *result = new char[size];
    _writeHelper(input, *result);
  }
  void read(char* input, TreeNode* result)=0;
};

class StringCodec: virtual BinTreeCodec {
public:
  StringCodec() {}
  BinTree
};


class Codec {
public:
    void write(stringstream& result, const int v, map<int, int>& mp, int& ix, int& size) {
        if(mp.count(v) == 0) {
            unsigned char intRep[5];
            int size = varint(intRep, v);
            mp[v] = ix;
            ix += size;
        }
        int offset = mp[v];
        char *bits = (char*)&offset;
        result.write(bits, 4);
        size += 4;
    }

    // Encodes a tree to a single string.
    string serialize(TreeNode* root) {
        if(root == NULL) {
            return "\0";
        }
        map<int, int> mp;
        int ix = 1;

        int size1 = 0;
        stringstream col1;
        stack<TreeNode*> st;
        st.push(root);
        while(!st.empty()) {
            TreeNode* top = st.top();
	    st.pop();
            if(top == NULL) {
                col1.write("\0", 1);
                size1++;
            } else {
                st.push(top->left);
                st.push(top->right);
                write(col1, top->val, mp, ix, size1);
            }
        }

        int size2 = 0;
        unsigned char charBuf[5];

        stringstream col2;
        for(auto it = mp.begin(); it != mp.end(); it++) {
	  col2.write((const char*)charBuf, size2 += varint(charBuf, it->first));
        }

        char *result = new char[4 + 4 + size1 + size2];
        memcpy(result, (char*)(&size1), 4);
        memcpy(result + 4, col1.str().c_str(), size1);
        memcpy(result, (char*)(&size2), 4);
        memcpy(result + 8 + size1, col2.str().c_str(), size2);

        return result;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {

    }
};


int test_serialization() {
    TreeNode t(1);
    TreeNode t2(2);
    TreeNode t3(3);
    t3.left = &t;
    TreeNode t4(4);
    t4.right = &t2;
    TreeNode t5(5);
    t5.left = &t3;
    t5.right = &t4;
    TreeNode t6(6);
    t6.left = &t5;
    t6.right = &t5;
    Codec codec;
//    codec.deserialize(codec.serialize(root));
}
