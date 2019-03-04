#include <string>

#include "benchmark/benchmark.h"

#include "shelly/math/big_int.hpp"

using namespace std;
using namespace shelly;

void BM_DivisionBigDenominator(benchmark::State& state) {
  string tb, ta;
  for (int i = 0; i < 10000; i++) {
    tb.push_back('9');
    if (i % 2) {
      ta.push_back('1');
    }
  }
  BigInt b(tb), a(ta);

  for (auto _ : state) {
    BigInt res = b / a;
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_DivisionBigDenominator);

void BM_DivisionSmallDenominator(benchmark::State& state) {
  string tb;
  for (int i = 0; i < 10000; i++) {
    tb.push_back('9');
  }
  BigInt b(tb);

  for (auto _ : state) {
    BigInt res = b / 333;
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_DivisionSmallDenominator);

void BM_Multiplication(benchmark::State& state) {
  string tb, ta;
  for (int i = 0; i < 10000; i++) {
    tb.push_back('9');
    if (i % 2) {
      ta.push_back('1');
    }
  }
  BigInt b(tb), a(ta);

  for (auto _ : state) {
    BigInt res = b * a;
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_Multiplication);

void BM_MultiplicationByInt(benchmark::State& state) {
  string tb;
  for (int i = 0; i < 10000; i++) {
    tb.push_back('9');
  }
  BigInt b(tb);

  for (auto _ : state) {
    BigInt res = b * 1313;
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_MultiplicationByInt);
