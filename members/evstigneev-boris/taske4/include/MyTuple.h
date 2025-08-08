#pragma once

template<typename T1, typename T2, typename T3>
class MyTuple {
private:
    T1 v1;
    T2 v2;
    T3 v3;

public:
    MyTuple() = default;
    MyTuple(const T1& a, const T2& b, const T3& c) : v1(a), v2(b), v3(c) {}

    template <size_t I>
    auto& get() {
        if constexpr (I == 0) return v1;
        else if constexpr (I == 1) return v2;
        else if constexpr (I == 2) return v3;
        else static_assert(I < 3, "MyTuple::get<I>: index out of range");
    }
};