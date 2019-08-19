//
// Created by Joshua on 19/08/2019.
//

#include "DatastructureSerialize.h"
#include <string>
#include <cstring>
#include <deque>
#include <stack>
#include <stringstream>

/**
 * Definition for a binary tree node.
 **/

using namespace std;

struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(int x) : val(x), left(NULL), right(NULL) {}
};


class Codec {
public:
    void write(sstream& result, const int v, map<int, int>& mp, int& ix, int& size) {
        if(mp.count(v) == 0) {
            char intRep[5];
            int size = varint(intRep, v);
            mp[v] = ix;
            ix += size;
        }
        int offset = mp[v];
        char *bits = static_cast<char*>&offset;
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
        sstream col1;
        stack<int> st;
        st.push(root);
        while(!st.empty()) {
            TreeNode* top = st.pop();
            if(top == NULL) {
                col1.write("\0", 1);
                size++;
            } else {
                st.push(top->left);
                st.push(top->right);
                write(col1, top->val, mp, ix, size1);
            }
        }
        int size2 = 0;
        char charBuf[5];

        sstream col2;
        for(auto it = mp.begin(); it != mp.end(); it++) {
            col2.write(charBuf, size2 += varint(charBuf, *it));
        }

        char *result = new char[4 + 4 + size1 + size2];
        char*bs1 = static_cast<char*>(&size1);
        char*bs2 = static_cast<char*>(&size2);
        copyint(result, bs1);
        memcpy(result + 4, col1.str(), size1);
        copyint(result + 4 + size1, bs2);
        memcpy(result + 8 + size1, col2.str(), size2);

        return result;
    }

    // Decodes your encoded data to tree.
    TreeNode* deserialize(string data) {

    }
};


int main() {
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