#include <iostream>
#include <stdexcept>
#include <climits>
#include <limits>
#include <vector>

bool is_prime(unsigned n);
unsigned next_prime(unsigned n);
bool is_twin_prime(unsigned n);
std::vector<unsigned> primes_in_range(unsigned start, unsigned end);

int main() {

    // Enter number and checking for correct input
    unsigned number = 0;
    do {
        std::cout << "Enter the number: ";
        if (std::cin >> number)
            break;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    } while (true);

    // Checking for prime number
    try {
        if (is_prime(number))
            std::cout << number << " is prime\n";
        else
            std::cout << number << " is not prime\n";
    }
    catch (std::invalid_argument i) {
        std::cout << i.what() << '\n';
    }

    // Calculate next prime number
    try {
        std::cout << "Next prime: " << next_prime(number) << '\n';
    }
    catch (std::invalid_argument i) {
        std::cout << i.what() << '\n';
    }

    // Chacking for twin prime numbers
    try {
        if (is_twin_prime(number))
            std::cout << number << " and " << number + 2 << " are twin primes\n";
        else
            std::cout << number << " and " << number + 2 << " are not twin primes\n";
    }
    catch (std::invalid_argument i) {
        std::cout << i.what() << '\n';
    }

    // Calculate the prime numbers in range
    unsigned start = 0;
    unsigned end = 0;
    do {
        std::cout << "Enter a range of prime numbers: ";
        if (std::cin >> start >> end)
            break;
        std::cin.clear();
        std::cin.ignore(INT_MAX, '\n');
    } while (true);

    std::vector<unsigned> primes = primes_in_range(start, end);
    std::cout << "Prime numbers from the range: [";
    if (primes.size() > 1)
        for (int i = 0; i < primes.size() - 1; i++)
            std::cout << primes[i] << ", ";
    if (primes.size() > 0)
        std::cout << primes[primes.size() - 1];
    std::cout << "]\n";

    return 0;
}

// Checking for the primality of a number
bool is_prime(unsigned n) {
    if (n < 2) throw std::invalid_argument{ "the number cannot be less than 2" };
    if (n == 2 || n == 3) return true;
    if (n % 2 == 0 || n % 3 == 0) return false;

    for (unsigned i = 3; i * i <= n; i += 6)
        if (n % i == 0 || n % (i + 2) == 0)
            return false;
    return true;
}

// Calculate the next prime number
unsigned next_prime(unsigned n) {
    if (n > std::numeric_limits<unsigned int>::max() - 100) throw std::invalid_argument{ "the number is too high" };

    unsigned candidate = n % 2 == 0 ? n + 1 : n + 2;
    while (!is_prime(candidate)) candidate += 2;

    return candidate;
}

// Chacking for twin prime numbers
bool is_twin_prime(unsigned n) {
    if (!is_prime(n)) throw std::invalid_argument{ "the number is not prime number" };
    return is_prime(n + 2);
}

// Calculate the prime numbers in range
std::vector<unsigned> primes_in_range(unsigned start, unsigned end) {
    if (start == end) return {};
    if (start > end) return primes_in_range(end, start);

    std::vector<unsigned> sieve = std::vector<unsigned>(end);

    for (unsigned i = 2; i * i < end; i++)
        if (sieve[i] == 0)
            for (unsigned j = i * i; j < end; j += i)
                sieve[j] = 1;

    std::vector<unsigned> primes = {};
    for (unsigned i = start; i < end; i++)
        if (sieve[i] == 0)
            primes.push_back(i);

    return primes;
}