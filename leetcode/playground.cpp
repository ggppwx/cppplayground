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







TEST(lengthOfLongestSubstring, solution) {
    Solution s;
    cout << s.lengthOfLongestSubstring("abba") << endl;
}


