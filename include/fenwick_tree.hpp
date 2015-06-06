#ifndef SHELLY_FENWICK_TREE_FASCXZ8989
#define SHELLY_FENWICK_TREE_FASCXZ8989

#include "common.hpp"

namespace shelly {
    template<typename T>
    class FenwickTree {
    public:
        FenwickTree(int n): v(n, 0) {
        }
        vector<T> v;
        void add(int i, T x) {
            for(; i < (int)v.size(); i |= i+1)
                v[i] += x;
        }
        T sum(int i) const {
            T r = 0;
            for(-- i; i >= 0; i = (i & (i+1)) - 1)
                r += v[i];
            return r;
        }
        T sum(int left, int right) const {
            return sum(right) - sum(left);
        }
    };
}

#endif
