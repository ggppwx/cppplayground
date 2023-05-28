#include <iostream>
#include <vector>
#include <string>
#include <map>
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


TEST(lengthOfLongestSubstring, solution) {
    Solution s;
    cout << s.lengthOfLongestSubstring("abba") << endl;
}


TEST(longestPalindrome, solution1) {
    Solution1 s;
    cout << s.longestPalindrome("aacabdkacaa") << endl;
}

