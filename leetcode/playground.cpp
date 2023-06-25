#include <iostream>
#include <vector>
#include <string>
#include <map>
#include <queue>
#include <gtest/gtest.h>

using namespace std;

class Solution {
public:
    int lengthOfLongestSubstring(string s) {
        // abcabcbb 
        // abc a     [a:0 , b: 1, c: 2] 4  - (0 + 1) 
        // bca [a:4, b:1, c:2] 5 - (1+1)

        // pwwkew
        // p
        // pw 
        // pw w  [p:0, w: 2]

        // abba
        // abb

        map<char, int> letterIdx; 
        int result = 0;
        int left = 0;
        for (int i = 0 ; i < s.size(); i++) {
            char currLetter = s[i];
            if ( letterIdx.find(currLetter) != letterIdx.end()) {
                // find the letter 
                left = max(letterIdx[currLetter] + 1, left);
            }
            result = max(result, i - left + 1);
            letterIdx[currLetter] = i;
        }
        return result;
    }


};

class Solution1 {
    public:
    string longestPalindrome(string s) {
        // babad
        // isP(i, j) = s[i] == s[j] && isP(i+1, j-1)
        // find the longest j - i when isP(i,j) == true 

        string result("");

        vector<vector<bool>> isP (s.size(), vector<bool>(s.size(), false)); 
        for (int offset = 0 ; offset < s.size(); offset ++ ) {
            for (int i = 0; i < s.size(); i++ ) {
                // 0 0, 1 1
                int j = i + offset;

                if (j >= s.size()) { // don't forget exceeding 
                    break;
                }

                if (offset == 0) {
                    isP[i][j] = true;
                } else if ( offset == 1) {
                    isP[i][j] = (s[i] == s[j]);
                } else {
                    isP[i][j] = (s[i] == s[j]) && isP[i+1][j-1];
                }

                if (isP[i][j]) {
                    if ( j - i + 1 > result.size()) {
                        result = s.substr(i, j-i + 1);
                    }
                }

            }
        }
        
        return result;
    }
};


struct TreeNode {
    int val;
    TreeNode *left;
    TreeNode *right;
    TreeNode(): val(0), left(nullptr), right(nullptr) {}
    TreeNode(int x): val(x), left(nullptr), right(nullptr) {}
    TreeNode(int x, TreeNode* left, TreeNode *right) :val(x), left(left), right(right) {}
};

class Solution2 {
    public:
    vector<int> inorderTraversal(TreeNode* root) {
        // [1 3 2 ]
        // [1] node = null
        // [] node = 2
        // [2] node = 3
        // [2, 3] node = null 
        // [2] 3(right ? )
        vector<TreeNode*> stack;
        TreeNode* node = root; 
        vector<int> output; 
        while (node || !stack.empty()) {

            // all left nodes to the queue 
            while(node) {
                stack.push_back(node);
                node = node->left;
            }

            // no more nodes to push 
            // pop the stack 
            if (!stack.empty()) {
                TreeNode* last = stack.at(stack.size() - 1);
                output.push_back(last->val);
                stack.pop_back();

                // if last has a right node 
                if (last->right != nullptr) {
                    node = last->right;
                }
            } 

        }

        return output;
    }
};

class Solution3 {
public:
    int numIslands(vector<vector<char>>& grid) {
        // BFS queue
        int count {0};
        queue<pair<int,int>> q; 
        int m = grid.size();
        int n = grid[0].size();
        vector<pair<int, int>> directions {
            pair(0, 1), pair(0, -1), pair(1, 0), pair(-1, 0)
        };
        for (int i = 0; i < m; i++) {
            for (int j = 0; j < n; j++) {
                if (grid[i][j] == '1') {
                    count ++;

                    // bfs 
                    q.push(pair<int, int>(i, j));
                    grid[i][j] = '0';
                    while(!q.empty()) {
                        auto p = q.front();
                        q.pop();

                        // 4 neighbors 
                        for (int j = 0; j < directions.size(); j++) {
                            int newx = p.first + directions[j].first;
                            int newy = p.second + directions[j].second;
                            if (newx < 0 || newx >= m || newy < 0 || newy >= n) {
                                continue;
                            }
                            if (grid[newx][newy] == '1') {
                                grid[newx][newy] = '0';
                                q.push(pair(newx, newy));
                            }
                        }

                    }


                }

            }
        }
        return  count ;
    }
};


TEST(lengthOfLongestSubstring, solution) {
    Solution s;
    cout << s.lengthOfLongestSubstring("abba") << endl;
}


TEST(longestPalindrome, solution1) {
    Solution1 s;
    cout << s.longestPalindrome("aacabdkacaa") << endl;
}


TEST(inorderTraversal, solution2) {
    // build the tree 
    TreeNode* root = new TreeNode(1, nullptr, new TreeNode(2, new TreeNode(3), nullptr));
    Solution2 s; 
    vector<int> out = s.inorderTraversal(root);
    copy(out.begin(), out.end(), ostream_iterator<int>(cout, " "));
}

TEST(numIslands, solution) {
    vector<vector<char>> grid {
        {'1','1','1','1','0'},
        {'1','1','0','1','0'},
        {'1','1','0','0','0'},
        {'0','0','0','0','0'}
    };
    Solution3 s;
    s.numIslands(grid);


}