/*


Leetcode 365, https://leetcode.cn/problems/water-and-jug-problem/

Notice how we overload the hash function for pair<int, int> to insert them into an unordered_set


*/


#include <unordered_set>
#include <utility>
using namespace std;

struct pair_hash {
    size_t operator()(const pair<int, int>& p) const {
        return hash<int>()(p.first) ^ hash<int>()(p.second);
    }
};

class Solution {
private:
    unordered_set<pair<int, int>, pair_hash> states;
    bool canMeasure = false;

    void searchWaterStates(pair<int, int> state, int x, int y, int target){
        // return conditions
        if(canMeasure) return;
        if(states.find(state) != states.end()) return;
        if(state.first + state.second == target){
            canMeasure = true;
            return;
        }

        // record current state
        states.insert(state);

        // move to other states
        if(state.first > 0){
            searchWaterStates(make_pair(0, state.second), x, y, target);
            int water_in_second = min(y, state.first + state.second);
            int water_in_first = state.first - (water_in_second - state.second);
            searchWaterStates(make_pair(water_in_first, water_in_second), x, y, target);
        }
        if(state.second > 0){
            searchWaterStates(make_pair(state.first, 0), x, y, target);
            int water_in_first = min(x, state.first + state.second);
            int water_in_second = state.second - (water_in_first - state.first);
            searchWaterStates(make_pair(water_in_first, water_in_second), x, y, target);
        }
        if(state.first < x){
            searchWaterStates(make_pair(x, state.second), x, y, target);
        }
        if(state.second < y){
            searchWaterStates(make_pair(state.first, y), x, y, target);
        }
    }
public:
    bool canMeasureWater(int x, int y, int target) { 
        searchWaterStates(make_pair(0, 0), x, y, target);
        return canMeasure;
    }
};