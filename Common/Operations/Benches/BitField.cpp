#include <chrono>
#include <cstdint>
#include <iomanip>
#include <iostream>

#include "../BitField.hpp"


namespace sure::base {

    using Clock = std::chrono::steady_clock;
    using BenchmarkFunction = U32 (*)(U32, U32);

    constexpr U32 ITERATIONS = 1'000'000;
    constexpr U32 REPEATS = 1000;

    V_U32 benchmark_sink = 0;


    U32 write_field(U32 reg, U32 value) noexcept {
        return BitField<U32, 8, 8>::write(reg, value);
    }


    U32 extract_field(U32 reg, U32) noexcept {
        return BitField<U32, 8, 8>::extract(reg);
    }


    U32 raw_write_field(U32 reg, U32 value) noexcept {
        constexpr U32 mask = 0xFF00;
        return (reg & ~mask) | ((value & 0xFF) << 8);
    }


    U32 raw_extract_field(U32 reg, U32) noexcept {
        return (reg >> 8) & 0xFF;
    }


    U32 next_value(U32 value) noexcept {
        value ^= value << 13;
        value ^= value >> 17;
        return value ^ (value << 5);
    }


    double measure(BenchmarkFunction function) noexcept {
        U32 state = 0x12345678;

        for (U32 index = 0; index < 100'000; ++index) {
            state = next_value(state);
            benchmark_sink = function(state, state);
        }

        mut best_ns_per_operation = static_cast<double>(UINT64_MAX);

        for (U32 repeat = 0; repeat < REPEATS; ++repeat) {
            state = 0x12345678 + repeat;
            const auto start = Clock::now();

            for (U32 index = 0; index < ITERATIONS; ++index) {
                state = next_value(state);
                benchmark_sink ^= function(state, state);
            }

            const auto elapsed = std::chrono::duration<double, std::nano>(
                Clock::now() - start).count();
            const double ns_per_operation = elapsed / ITERATIONS;
            if (ns_per_operation < best_ns_per_operation) {
                best_ns_per_operation = ns_per_operation;
            }
        }

        return best_ns_per_operation;
    }


    void print_result(const char* name, BenchmarkFunction function) noexcept {
        std::cout << std::left << std::setw(20) << name
                  << std::right << std::fixed << std::setprecision(3)
                  << measure(function) << " ns/op\n";
    }
}


int main() {
    using namespace sure::base;

    std::cout << "BitField benchmark (best of " << REPEATS << ")\n";
    print_result("BitField::write", write_field);
    print_result("Raw write baseline", raw_write_field);
    print_result("BitField::extract", extract_field);
    print_result("Raw extract baseline", raw_extract_field);
    std::cout << "sink: " << benchmark_sink << '\n';
}