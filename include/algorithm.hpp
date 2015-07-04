#ifndef LIBSHELLY_ALGORITHM_HPP_NBNMMN890
#define LIBSHELLY_ALGORITHM_HPP_NBNMMN890

#include <limits>
#include <vector>
#include <cassert>
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

int LongestCommonSubsequence(const std::string &s1, const std::string &s2) {
  if (s1.empty() || s2.empty())
    return 0;
  std::vector<std::vector<size_t>> dp(s1.size() + 1, std::vector<size_t>(s2.size() + 1, 0));

  for (size_t i = 1; i <= s1.size(); i++) {
    for (size_t k = 1; k <= s2.size(); k++) {
      if (s1[i - 1] == s2[k - 1]) {
        dp[i][k] = dp[i - 1][k - 1] + 1;
        continue;
      }
      dp[i][k] = std::max(dp[i][k - 1], dp[i - 1][k]);
    }
  }

  assert(dp[s1.size()][s2.size()] <= s1.size() && dp[s1.size()][s2.size()] <= s2.size());

  return dp[s1.size()][s2.size()];
}


}

#endif
