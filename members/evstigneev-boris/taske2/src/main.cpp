#include <iostream>
#include <vector>
#include <cmath>
#include <stdexcept>
#include <climits>

static bool is_prime(int n) {
    if (n < 2)
        return false;
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
        throw std::overflow_error("overflow");

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
    if (start > end)
        throw std::invalid_argument("start > end");

    if (end < 0)
        throw std::invalid_argument("negative range not allowed");

    if (end < 2)
        return std::vector<int>();

	if (start < 2)
		start = 2;

    std::vector<bool> candidates(end + 1, false);
	candidates[2] = true;

    for (int i = 3; i <= end; i += 2) {
        candidates[i] = true;
    }
    for (int i = 3; i * i <= end; i += 2) {
        if (candidates[i]) {
            for (int j = i * i; j <= end; j += 2 * i) {
                candidates[j] = false;
            }
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
    std::cout << "Enter the number: ";
    std::cin >> num;

    if (std::cin.fail()) {
        std::cerr << "Input error";
        return 1;
    }

    try {
        if (is_prime(num)) {
            std::cout << num << " is prime\n";
            int next = next_prime(num);
            std::cout << "next prime: " << next << "\n";

            if (is_twin_prime(num))
            std::cout << num << " and " << (num + 2) << " are twin primes\n";
            else
                std::cout << num << " and " << (num + 2) << " are not twin primes\n";
        }
        else
			std::cout << num << " is not prime\n";

    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        return 1;
    }

    int start;
    int end;
    std::cout << "enter a range of prime numbers: ";
    std::cin >> start >> end;

    if (std::cin.fail()) {
        std::cerr << "Input error";
        return 1;
    }
    
    try {
        std::vector<int> primes = primes_in_range(start, end);
        std::cout << "prime numbers from the range: ";
        std::cout << "[";
        for (size_t i = 0; i < primes.size(); ++i) {
            std::cout << primes[i];
            if (i + 1 < primes.size())
                std::cout << ", ";
        }
        std::cout << "]";
    }
    catch (const std::exception& e) {
        std::cerr << e.what();
        return 1;
    }

    std::cout << "\n";
    return 0;
}