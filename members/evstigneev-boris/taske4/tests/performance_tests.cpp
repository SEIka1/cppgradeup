#include <gtest/gtest.h>
#include <chrono>
#include <thread>
#include <random>
#include "../include/Resource.h"

class PerformanceTest : public ::testing::Test {
protected:
    const int ITERATIONS = 100;
    ResourceManager manager;

    std::string generateRandomId(int length = 10) {
        static const char alphanum[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
        static std::random_device rd;
        static std::mt19937 gen(rd());
        static std::uniform_int_distribution<> dis(0, sizeof(alphanum) - 2);

        std::string s;
        s.reserve(length);
        for (int i = 0; i < length; ++i) {
            s += alphanum[dis(gen)];
        }
        return s;
    }
};

TEST_F(PerformanceTest, ResourceCreation) {
    manager.clear();

    auto start = std::chrono::high_resolution_clock::now();

    for (int i = 0; i < ITERATIONS; ++i) {
        manager.createResource(
            generateRandomId(),
            rand() % 1000 + 1,
            static_cast<ResourceType>(i % 3)
        );
    }

    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start
    );

    std::cout << "\n[PERF] Created " << ITERATIONS << " resources in "
              << duration.count() << " μs\n";
    EXPECT_EQ(manager.resourceCount(), ITERATIONS);
}

TEST_F(PerformanceTest, SortingPerformance) {
    manager.clear();

    for (int i = 0; i < ITERATIONS; ++i) {
        manager.createResource(
            generateRandomId(),
            rand() % 1000 + 1,
            ResourceType::FILE
        );
    }

    auto start = std::chrono::high_resolution_clock::now();
    manager.sortBySize();
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start
    );

    std::cout << "\n[PERF] Sorted " << ITERATIONS << " resources in "
              << duration.count() << " μs\n";

    for (size_t i = 1; i < manager.resourceCount(); ++i) {
        EXPECT_LE(manager.getResource(i-1).size(), manager.getResource(i).size());
    }
}

TEST_F(PerformanceTest, FilteringPerformance) {
    manager.clear();

    // (25% FILE, 25% MEMORY, 50% NETWORK)
    for (int i = 0; i < ITERATIONS; ++i) {
        ResourceType type = (i % 4 == 0) ? ResourceType::FILE :
                          ((i % 4 == 1) ? ResourceType::MEMORY : ResourceType::NETWORK);
        manager.createResource(generateRandomId(), i % 1000 + 1, type);
    }

    auto start = std::chrono::high_resolution_clock::now();
    auto filtered = manager.filterByType(ResourceType::NETWORK);
    auto duration = std::chrono::duration_cast<std::chrono::microseconds>(
        std::chrono::high_resolution_clock::now() - start
    );

    std::cout << "\n[PERF] Filtered " << ITERATIONS << " resources in "
              << duration.count() << " μs. Found: " << filtered.size() << " items\n";

    EXPECT_GT(filtered.size(), 0);
    for (const auto& res : filtered) {
        EXPECT_EQ(res->type(), ResourceType::NETWORK);
    }
}