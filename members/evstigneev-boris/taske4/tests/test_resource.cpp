#include <gtest/gtest.h>
#include "../include/Resource.h"

class ResourceTest : public ::testing::Test {
protected:
    void SetUp() override {
        validResource = new Resource("valid_id", 100, ResourceType::FILE);
    }

    void TearDown() override {
        delete validResource;
    }

    Resource* validResource;
};

TEST_F(ResourceTest, ConstructorValidation) {
    EXPECT_NO_THROW(Resource("short", 1, ResourceType::FILE));
    EXPECT_NO_THROW(Resource("max_length_12345678901234567890", 1'000'000'000, ResourceType::MEMORY));

    EXPECT_THROW(Resource("", 100, ResourceType::FILE), std::invalid_argument);
    EXPECT_THROW(Resource(std::string(33, 'a'), 100, ResourceType::FILE), std::invalid_argument);

    EXPECT_THROW(Resource("valid", 0, ResourceType::FILE), std::invalid_argument);
    EXPECT_THROW(Resource("valid", 1'000'000'001, ResourceType::FILE), std::invalid_argument);
}

TEST_F(ResourceTest, GettersWorkCorrectly) {
    EXPECT_EQ(validResource->id(), "valid_id");
    EXPECT_EQ(validResource->size(), 100);
    EXPECT_EQ(validResource->type(), ResourceType::FILE);
    EXPECT_EQ(validResource->status(), ResourceStatus::FREE);
}

TEST_F(ResourceTest, SettersValidation) {
    EXPECT_NO_THROW(validResource->setSize(1));
    EXPECT_NO_THROW(validResource->setSize(1'000'000'000));
    validResource->setStatus(ResourceStatus::BUSY);
    EXPECT_EQ(validResource->status(), ResourceStatus::BUSY);

    EXPECT_THROW(validResource->setSize(0), std::invalid_argument);
    EXPECT_THROW(validResource->setSize(1'000'000'001), std::invalid_argument);
}

TEST_F(ResourceTest, StatusTransitions) {
    validResource->setStatus(ResourceStatus::BUSY);
    EXPECT_EQ(validResource->status(), ResourceStatus::BUSY);

    validResource->setStatus(ResourceStatus::LOCKED);
    EXPECT_EQ(validResource->status(), ResourceStatus::LOCKED);

    validResource->setStatus(ResourceStatus::FREE);
    EXPECT_EQ(validResource->status(), ResourceStatus::FREE);
}

TEST_F(ResourceTest, TypePersistence) {
    Resource memResource("mem", 100, ResourceType::MEMORY);
    EXPECT_EQ(memResource.type(), ResourceType::MEMORY);

    Resource netResource("net", 100, ResourceType::NETWORK);
    EXPECT_EQ(netResource.type(), ResourceType::NETWORK);
}