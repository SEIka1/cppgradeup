#include <gtest/gtest.h>
#include <fstream>
#include <thread>
#include <sys/stat.h>
#include <unistd.h>
#include "../include/ResourceLogger.h"

class ResourceLoggerTest : public ::testing::Test {
protected:
    void SetUp() override {
        testFile = "test_log.log";
        std::remove(testFile.c_str());

        if (getuid() != 0) {
            protectedDir = "protected_dir_test";
            mkdir(protectedDir.c_str(), 0000);
            protectedFile = protectedDir + "/test.log";
        }
    }

    void TearDown() override {
        std::remove(testFile.c_str());
        if (getuid() != 0) {
            std::remove(protectedFile.c_str());
            rmdir(protectedDir.c_str());
        }
    }

    std::string testFile;
    std::string protectedDir;
    std::string protectedFile;
};

// check basic logging operations
TEST_F(ResourceLoggerTest, BasicLoggingOperations) {
    ResourceLogger logger(testFile);

    EXPECT_NO_THROW(logger.log("Test message 1"));
    EXPECT_NO_THROW(logger.log("Test message 2"));

    std::ifstream in(testFile);
    std::string content((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());

    EXPECT_TRUE(content.find("Test message 1") != std::string::npos);
    EXPECT_TRUE(content.find("Test message 2") != std::string::npos);
}

// stress tests for basic logging operations
TEST_F(ResourceLoggerTest, BasicLoggingOperations2) {
    ResourceLogger logger(testFile);
    for (uint32_t i = 0; i < 999; ++i) {
        EXPECT_NO_THROW(logger.log("Test message " + std::to_string(i)));
    }

    std::ifstream in(testFile);
    std::string content((std::istreambuf_iterator<char>(in)),
                       std::istreambuf_iterator<char>());
    for (uint32_t i = 0; i < 999; ++i) {
        EXPECT_TRUE(content.find("Test message " + std::to_string(i)) != std::string::npos);
    }
}

// attempt to create a file with an invalid name
TEST_F(ResourceLoggerTest, InvalidFilenameCases) {
    EXPECT_THROW(ResourceLogger(""), std::invalid_argument);
    try {
        ResourceLogger logger(std::string(1000, 'a'));
        try {
            logger.log("Test");
            SUCCEED();
        } catch (...) {
            SUCCEED();
        }
    } catch (const std::invalid_argument&) {
        SUCCEED();
    } catch (...) {
        FAIL() << "Unexpected exception type";
    }
    if (getuid() != 0) {
        EXPECT_THROW({
            ResourceLogger logger(protectedFile);
            logger.log("Test");
        }, std::runtime_error);
    }
}

// attempt to write to an existing but missing file
TEST_F(ResourceLoggerTest, FileSystemEdgeCases) {

    if (getuid() != 0) {
        std::ofstream out(testFile);
        out.close();
        chmod(testFile.c_str(), 0000);

        EXPECT_THROW({
            ResourceLogger logger(testFile);
            logger.log("Test");
        }, std::runtime_error);

        chmod(testFile.c_str(), 0644);
    }
}

// concurrency stress test
TEST_F(ResourceLoggerTest, ConcurrencyStressTest) {
    ResourceLogger logger(testFile);
    const int THREAD_COUNT = 10;
    const int MESSAGES_PER_THREAD = 100;

    auto worker = [&logger](int thread_id) {
        for (int i = 0; i < MESSAGES_PER_THREAD; ++i) {
            logger.log("Thread " + std::to_string(thread_id) + " message " + std::to_string(i));
        }
    };

    std::thread threads[THREAD_COUNT];
    for (int i = 0; i < THREAD_COUNT; ++i) {
        threads[i] = std::thread(worker, i);
    }

    for (auto& t : threads) {
        t.join();
    }

    std::ifstream in(testFile);
    std::string content((std::istreambuf_iterator<char>(in)),
                     std::istreambuf_iterator<char>());

    for (int i = 0; i < THREAD_COUNT; ++i) {
        for (int j = 0; j < MESSAGES_PER_THREAD; ++j) {
            EXPECT_TRUE(content.find("Thread " + std::to_string(i) + " message " + std::to_string(j))
                      != std::string::npos);
        }
    }
}