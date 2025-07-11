#include <iostream>
#include <stdexcept>
#include <limits>
#include <vector>

bool is_prime(unsigned n);
unsigned next_prime(unsigned n);
bool is_twin_prime(unsigned n);
std::vector<unsigned> primes_in_range(unsigned start, unsigned end);

int main() {

    // Enter number and checking for correct input
    long inputNumber = 0;
    unsigned number = 0;
    do {
        std::cout << "Enter the number: ";
        if (!(std::cin >> inputNumber) || inputNumber < 0 || std::numeric_limits<unsigned>::max() < inputNumber) {
            std::cin.clear();
            std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
            continue;
        }
        number = inputNumber;
        break;
    } while (true);

    // Checking for prime number
    try {
        if (is_prime(number))
            std::cout << number << " is prime\n";
        else
            std::cout << number << " is not prime\n";
    }
    catch (const std::invalid_argument& i) {
        std::cout << i.what() << '\n';
    }

    // Calculate next prime number
    try {
        std::cout << "Next prime: " << next_prime(number) << '\n';
    }
    catch (const std::invalid_argument& i) {
        std::cout << i.what() << '\n';
    }

    // Checking for twin prime numbers
    try {
        if (is_twin_prime(number))
            std::cout << number << " and " << number + 2 << " are twin primes\n";
        else
            std::cout << number << " and " << number + 2 << " are not twin primes\n";
    }
    catch (const std::invalid_argument& i) {
        std::cout << i.what() << '\n';
    }

    // Calculate the prime numbers in range
    long inputStart = 0;
    long inputEnd = 0;
    unsigned start = 0;
    unsigned end = 0;
    do {
        std::cout << "Enter a range of prime numbers: ";
        if (!(std::cin >> inputStart >> inputEnd) || inputStart < 0 || inputEnd < 0
            || std::numeric_limits<unsigned>::max() < inputStart
            || std::numeric_limits<unsigned>::max() < inputEnd){
            std::cin.clear();
        std::cin.ignore(std::numeric_limits<std::streamsize>::max(), '\n');
        continue;
            }
            start = inputStart;
            end = inputEnd;
            break;
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
    if (n > std::numeric_limits<unsigned int>::max() - 100)
        throw std::invalid_argument{ "the number is too high" };

    if (n < 2) return 2;
    unsigned candidate = n % 2 == 0 ? n + 1 : n + 2;
    while (!is_prime(candidate)) candidate += 2;

    return candidate;
}

// Checking for twin prime numbers
bool is_twin_prime(unsigned n) {
    if (n < 2 || !is_prime(n)) throw std::invalid_argument{ "the number is not prime" };
    return is_prime(n + 2);
}

// Calculate the prime numbers in range
std::vector<unsigned> primes_in_range(unsigned start, unsigned end) {
    if (start > end) return primes_in_range(end, start);

    std::vector<bool> sieve = std::vector<bool>(end + 1);

    for (unsigned i = 2; i * i <= end; i++)
        if (!sieve[i])
            for (unsigned j = i * i; j <= end; j += i)
                sieve[j] = true;

    std::vector<unsigned> primes = {};
    for (unsigned i = start < 2 ? 2 : start; i <= end; i++)
        if (!sieve[i])
            primes.push_back(i);

    return primes;
}
