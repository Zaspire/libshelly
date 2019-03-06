#include <string>

#include "benchmark/benchmark.h"

#include "shelly/math/big_int.hpp"

using namespace std;
using namespace shelly;

BigInt GenerateBigInt(int size, int digit = 9) {
  string t;
  t.reserve(size);
  for (int i = 0; i < size; i++) {
    t.push_back(digit + '0');
  }
  return BigInt(t);
}

void BM_DivisionBigDenominator(benchmark::State& state) {
  BigInt b(GenerateBigInt(20000)), a(GenerateBigInt(10000, 1));

  int i = 0;
  for (auto _ : state) {
    BigInt res = b / a;
    benchmark::DoNotOptimize(res);

    i++;
    b += i;
    a += i;
  }
}
BENCHMARK(BM_DivisionBigDenominator);

void BM_DivisionSmallDenominator(benchmark::State& state) {
  BigInt b(GenerateBigInt(10000));

  int i = 0;
  for (auto _ : state) {
    i++;
    BigInt res = b / (333 + i);
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_DivisionSmallDenominator);

void BM_Multiplication(benchmark::State& state) {
  BigInt b(GenerateBigInt(10000)), a(GenerateBigInt(5000, 1));

  int i = 0;
  for (auto _ : state) {
    BigInt res = b * a;
    benchmark::DoNotOptimize(res);

    i++;
    b += i;
    a += i;
  }
}
BENCHMARK(BM_Multiplication);

void BM_MultiplicationByInt(benchmark::State& state) {
  BigInt b(GenerateBigInt(10000));

  int i = 0;
  for (auto _ : state) {
    i++;
    BigInt res = b * (1313 + i);
    benchmark::DoNotOptimize(res);
  }
}
BENCHMARK(BM_MultiplicationByInt);

void BM_MultiplicationByInt2(benchmark::State& state) {
  BigInt b(GenerateBigInt(10000)), a("11111111111");

  int i = 0;
  for (auto _ : state) {
    BigInt res = b * a;
    benchmark::DoNotOptimize(res);

    i++;
    b += i;
    a += i;
  }
}
BENCHMARK(BM_MultiplicationByInt2);

void BM_Sum(benchmark::State& state) {
  BigInt a = GenerateBigInt(10000), b = GenerateBigInt(11000);

  int i = 0;
  for (auto _ : state) {
    BigInt res = a + b;
    benchmark::DoNotOptimize(res);

    i++;
    b += i;
    a += i;
  }
}
BENCHMARK(BM_Sum);
