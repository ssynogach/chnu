#include "arrays_io.h"

#include <random>
#include <chrono>

// Один генератор на всю програму (ініціалізується один раз).
static std::mt19937& engine() {
    static std::mt19937 gen(
        static_cast<unsigned>(
            std::chrono::steady_clock::now().time_since_epoch().count()));
    return gen;
}

void fillRandomInt(int* a, int n, int lo, int hi) {
    std::uniform_int_distribution<int> dist(lo, hi);
    for (int i = 0; i < n; ++i)
        a[i] = dist(engine());
}

void fillRandomDouble(double* a, int n, double lo, double hi) {
    std::uniform_real_distribution<double> dist(lo, hi);
    for (int i = 0; i < n; ++i)
        a[i] = dist(engine());
}
