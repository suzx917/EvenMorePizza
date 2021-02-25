#include "EvenMorePizza.h"
#include <string>
#include <vector>
#include <algorithm>
#include <numeric>
#include <unordered_map>
#include <unordered_set>
#include <random>
#include <climits>
#include <chrono>

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

    void print_combo(vector<int> const& pzs) {
        unordered_set<int> ing;
        for (int pid : pzs) {
            cerr << "Pizza #" << pid << " : ";
            for (int t : ps[pid].tops) {
                ing.insert(t);
                cerr << t << ' ';
            }
            cerr << '\n';
        }
        cerr << "All Toppings { ";
        for (int t: ing) {
            cerr << t << ' ';
        }
        cerr << "}";        
    }

    long eval_combo(vector<int> const& pzs) {
        unordered_set<int> ing;
        for (int pid : pzs) {
            for (int t : ps[pid].tops) {
                ing.insert(t);
            }
        }
        // cerr << "eval toppings: ";
        // for (int t : ing) {
        //     cerr << t << ' ';
        // }
        // cerr << '\n';

        long res = (int) ing.size() * ing.size();
        return res;
    } 

    int pval(Order const& order, Pizza const& p, int lim) {
        int improve = 0;
        for (auto t : p.tops) {
            if (order.tops.find(t) == order.tops.end())
                ++improve;
        }
        // greedy predict whether worth it to include the pizza or leave it for other next order
        int room = lim - order.size();
        int predict = order.rootscr + (improve * room);
        long grow = predict * predict - order.rootscr * order.rootscr;
        if (grow < (p.value) * (p.value))
            return 0;

        return improve;
    }

    bool best_first(Order& order, int lim) {
        if (order.size() >= lim) return false;
        int max = -1;
        int maxid = -1;
        // int search = 0;
        // int range = 10000;
        for (auto pid : available) {
            // if (search == range)
            //     break;
            int p = pval(order, ps[pid], lim);
            if (p >= max) {
                max = p;
                maxid = pid;
                // shortcut
                // if (max == ps[pid].value)
                //     break;
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

            --T[order.size()];
            sol.add(order);
            
            // recalculate limit;
            while (T[limit] == 0) {
                --limit;
            } 
            if (limit < 2) break;
        }

        if (available.size() > 1) {
            Order order = Order();
            int limit = 4;
            while (T[limit] == 0) {
                --limit;
            } 
            if (limit > 1) {
                for (int a : available) {
                    order.add(ps[a]);
                    if (order.size() == limit)
                        break;
                }
                sol.add(order);
            }
        }
        cerr << "Greedy done.\n" << sol.brief() << '\n';
    }

    void random_swap(int cap) {
        // random_device rand_device;
        // mt19937 rand_engine(rand_device());
        // uniform_int_distribution<int> unidist(0, INT_MAX);
        srand(0);

        int sizes[5];
        sizes[2] = sol.data[2].size();
        sizes[3] = sol.data[3].size();
        sizes[4] = sol.data[4].size();
        
        const auto tbegin = chrono::high_resolution_clock::now();
        const double tlimit = 5 * 60 * 1e3; // ms
        int tries = 0;
        while (tries++ != cap) {
            // select team groups
            const int a = rand() % 3 + 2;
            const int b = rand() % 3 + 2;
            vector<vector<int>>& groupA = sol.data[a];
            vector<vector<int>>& groupB = sol.data[b];
            if (!sizes[a] || !sizes[b])
                continue;

            // select order pairs
            const int oa = rand() % sizes[a];
            const int ob = rand() % sizes[b];
            long before = eval_combo(groupA[oa]) + eval_combo(groupB[ob]);

            // select pizza pairs
            const int pa = rand() % a;
            const int pb = rand() % b;
            swap(groupA[oa][pa], groupB[ob][pb]);
            long after = eval_combo(groupA[oa]) + eval_combo(groupB[ob]);

            if (after <= before)
                swap(groupA[oa][pa], groupB[ob][pb]);
            else {
                sol.scr += after - before;
                cerr << "\n[score = " << sol.scr << "] "
                  << groupA[oa][pa] << " <-> " << groupB[ob][pb] << '\n';
                // cerr << "Group A:\n";
                // print_combo(groupA[oa]);
                // cerr << '\n';
                // cerr << "Group B:\n";
                // print_combo(groupB[ob]);
                // cerr << '\n';
                tries = 0;
            }
            const auto tend = chrono::high_resolution_clock::now();
            const double elapsed = chrono::duration<double, milli>(tend-tbegin).count();
            if (elapsed > tlimit)
                break;
        }
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

        // cerr << "Input data loaded.\nWe have " << S << " people in " << T[0] << " teams, "
        //     << P << " pizzas, " << Q << " types of toppings. T 2 3 4 = ( "
        //     << T[2] << ", " << T[3] << ", " << T[4] << " )\n";  

        for (int i = 0; i < P; ++i)
            available.insert(i);

        // sort(ps.begin(), ps.end(),
        //     [](Pizza const& p1, Pizza const& p2){ return p1.value > p2.value; });

        greedy();

        random_swap(100000);

        sol.output();
    }
};

int main() {
    EvenMorePizza game = EvenMorePizza();
    return 0;
}
