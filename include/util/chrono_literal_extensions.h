#include <chrono>

constexpr std::chrono::month operator"" _mo(unsigned long long m) {
    return std::chrono::month{static_cast<unsigned>(m)};
}
