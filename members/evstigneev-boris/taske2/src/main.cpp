#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <climits>

static bool is_prime(int n) {
    if (n < 2)
        throw std::invalid_argument("cant be prime");
    if (n == 2)
        return true;
    if (n % 2 == 0)
        return false;

    for (int i = 3; i <= (int)(std::sqrt(n)); i += 2) {
        if (n % i == 0)
            return false;
    }
    return true;
}

int next_prime(int n) {
    if (n > INT_MAX - 100)
        throw std::overflow_error("overfolw");

    int cur = n + 1;
    while (true) {
        if (is_prime(cur))
            return (int)cur;
        cur++;
    }
}

bool is_twin_prime(int n) {
    if (!is_prime(n))
        throw std::invalid_argument("not prime");

    return is_prime(n + 2);
}

std::vector<int> primes_in_range(int start, int end) {
    std::vector<bool> candidates(end + 1, true);
    candidates[0] = candidates[1] = false;

    for (int i = 2; i * i <= end; i++) {
        if (candidates[i]) {
            for (int j = i * i; j <= end; j += i)
                candidates[j] = false;
        }
    }

    std::vector<int> result;
    for (int i = start; i <= end; i++) {
        if (candidates[i])
            result.push_back(i);
    }
    return result;
}

int main() {   
    int num;
    int start;
    int end;
    std::cout << "Enter the number: ";
    std::cin >> num;

    if (is_prime(num)) {
        std::cout << num << " is prime\n";
        int next = next_prime(num);
        std::cout << "next prime: " << next << "\n";

        if (is_twin_prime(num))
            std::cout << num << " and " << (num + 2) << " are twin primes\n";
        else
            std::cout << num << " and " << (num + 2) << " are not twin primes\n";
    }

    std::cout << "enter a range of prime numbers: ";
    std::cin >> start >> end;

    std::vector<int> primes = primes_in_range(start, end);
    std::cout << "prime numbers from the range: ";
    std::cout << "[";
    for (size_t i = 0; i < primes.size(); ++i) {
        std::cout << primes[i];
        if (i + 1 < primes.size())
            std::cout << ", ";
    }
    std::cout << "]";

    std::cout << "\n";
    return 0;
}