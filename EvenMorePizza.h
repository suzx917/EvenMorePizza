#include <iostream>
#include <vector>
#include <unordered_set>
#include <sstream>

struct Pizza {
  int id;
  int value; // number of toppings
  std::unordered_set<int> tops;
};

struct Order {
  std::unordered_set<int> tops;
  std::vector<int> ps;
  int rootscr=0;

  Order() {}

  Order(std::vector<Pizza>& pzs) {
    for (Pizza& p : pzs) {
      add(p);
    }
  }

  int size() const {return ps.size();}
  
  void add(Pizza const& P) {
    ps.push_back(P.id);
    for (auto t : P.tops)
      tops.insert(t);
    rootscr = tops.size();
  }
  
  void merge(Order other) {
    for (auto t : other.tops)
      tops.insert(t);
    for (int p : other.ps)
      ps.push_back(p);
    rootscr += other.rootscr;
  }

  void clear() {
    tops.clear();
    ps.clear();
    rootscr = 0;
  }
};

struct Solution {
  long long scr;
  int teams;
  int pizzas;
  std::vector<std::vector<std::vector<int>>> data; // data[k] = k-orders

  Solution() {
    scr = 0;
    teams = 0;
    pizzas = 0;
    data = std::vector<std::vector<std::vector<int>>>(5, std::vector<std::vector<int>>());
  }

  void add(Order& order) {
    ++teams;
    int sz = order.size();
    pizzas += sz;
    scr += order.rootscr * order.rootscr;
    
    data[sz].push_back(std::vector<int>());
    std::vector<int>& row = data[sz].back();
    row.insert(row.end(),order.ps.begin(), order.ps.end());
  }


  std::string brief() {
    std::stringstream ss;
    ss << "[score = " << scr << "] used = " << pizzas << "( " << data[2].size() << ", "
      << data[3].size() << ", " << data[4].size() << " ) total = " << teams;
    return ss.str();
  }

  void output() {
    std::cout << teams << '\n';
    for (int i = 2; i <= 4; ++i) {
      for (auto const& row : data[i]) {
        std::cout << i << ' ';
        for (auto pid : row) {
          std::cout << pid << ' ';
        }
        std::cout << '\n';
      }
    }
  }
};

inline std::ostream& operator<<(std::ostream& os, Pizza const& p) {
    os << "Pizza( " << p.id << ", " << p.value << ", [ ";
    for (auto i : p.tops)
    os << i << ' ';
    os << "] )";
    return os;
}

inline bool compare(Order const& o1, Order const& o2) {
  return o1.rootscr < o2.rootscr;
}

inline bool compare(Solution const& s1, Solution const& s2) {
  return s1.scr < s2.scr;
}