#ifndef LIBSHELLY_ALGORITHM_HPP_NBNMMN890
#define LIBSHELLY_ALGORITHM_HPP_NBNMMN890

#include <limits>
#include <vector>
#include <algorithm>

namespace shelly {

template<typename T>
int LongestIncreaseSequence(const std::vector<T> &a) {
  T inf = std::numeric_limits<T>::max();
  std::vector<T> A(a.size(), inf);
	for(size_t i = 0; i < a.size(); i++) {
		*std::lower_bound(A.begin(), A.end(), a[i]) = a[i];
  }
	return std::find(A.begin(), A.end(), inf) - A.begin();
}

}

#endif
