#pragma once
#include <type_traits>

template<typename T1, typename T2, typename T3>
class MyTuple {
private:
    T1 v1;
    T2 v2;
    T3 v3;

public:
    MyTuple() = default;
    MyTuple(const T1& a, const T2& b, const T3& c) : v1(a), v2(b), v3(c) {}

    template <std::size_t I, typename = std::enable_if_t<(I < 3)>>
    constexpr auto& get() {
        if constexpr (I == 0) return v1;
        else if constexpr (I == 1) return v2;
        else return v3;
    }

    template <std::size_t I, typename = std::enable_if_t<(I < 3)>>
    constexpr const auto& get() const {
        if constexpr (I == 0) return v1;
        else if constexpr (I == 1) return v2;
        else return v3;
    }
};
