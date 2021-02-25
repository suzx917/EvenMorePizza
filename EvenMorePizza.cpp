#include "EvenMorePizza.h"
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>

using namespace std;

class EvenMorePizza {
public:
    int T[5];
    int P, Q, S;
      
    unordered_map<string, int> tids; // topping name to id
    vector<Pizza> ps;

    unordered_set<int> available;
    vector<vector<int>> ans;
    Solution sol;

    template <typename T>
    void print2D(vector<vector<T>> const& m) {
        for (const auto& row: m) {
            for (const auto& val: row) {
                cerr << val << ' ';
            }
            cerr << '\n';
        }
    }

    // prioritize T4
    // long long baseline1(vector<Pizza> const& ps, int T2, int T3, int T4) {
    //   return 0;
    // }

    int pval(Order const& order, Pizza const& p) {
        int res = 0;
        for (auto t : p.tops) {
            if (order.tops.find(t) == order.tops.end())
                ++res;
        }
        return res;
    }

    bool best_first(Order& order, int lim) {
        if (order.size() >= lim) return false;
        int max = -1;
        int maxid = -1;
        // int search = 0;
        // int range = 40000;
        for (auto pid : available) {
            // if (search == range)
            //     break;
            int p = pval(order, ps[pid]);
            if (p >= max) {
                max = p;
                maxid = pid;
            }
            // ++search;
        }
        if (max <= 0 && order.size() >= 2) return false; // no improvement        
        order.add(ps[maxid]);
        available.erase(maxid);
        return true;
    }

    void greedy() {
        sol = Solution();
        int limit;
        for (int i = 2; i <= 4; ++i) {
            if (T[i] > 0)
              limit = i;
        }
        while((int) available.size() >= limit) {
            Order order = Order();
            while (best_first(order, limit)) {}

            int size = order.size();
            --T[size];
            sol.add(order);
            
            // recalculate limit;
            while (T[limit] == 0) {
                --limit;
            } 
            if (limit < 2) break;
        }
        cerr << sol.brief() << '\n';
        sol.output();
    }

    EvenMorePizza() {
        ios_base::sync_with_stdio(false);
        cin.tie(NULL);
        cin >> P >> T[2] >> T[3] >> T[4];
        int tid = 0;
        for (auto i = 0; i < P; ++i) {
            int t;
            cin >> t;
            Pizza p{i, t, unordered_set<int>()};
            for (auto j = 0; j < t; ++j) {
            string s;
            cin >> s;
            if (tids.find(s) == tids.end()) {
                tids[s] = tid++;
            }
            p.tops.insert(tids[s]);
            }
            ps.push_back(p);
        }

        Q = tids.size(); // num of toppings
        T[0] = T[2] + T[3] + T[4];
        S = 2*T[2] + 3*T[3] + 4*T[4];

        cerr << "Input data loaded.\nWe have " << S << " people in " << T[0] << " teams, "
            << P << " pizzas, " << Q << " types of toppings. T 2 3 4 = ( "
            << T[2] << ", " << T[3] << ", " << T[4] << " )\n";  

        for (int i = 0; i < P; ++i)
            available.insert(i);

        sort(ps.begin(), ps.end(),
            [](Pizza const& p1, Pizza const& p2){ return p1.value > p2.value; });

        greedy();

    }
};

int main() {
    EvenMorePizza game = EvenMorePizza();
    return 0;
}
