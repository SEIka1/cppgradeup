#include <gtest/gtest.h>
#include "../include/Resource.h"

class ResourceManagerTest : public ::testing::Test {
protected:
    void SetUp() override {
        manager = new ResourceManager();
        res1 = &manager->createResource("res1", 100, ResourceType::FILE);
        res2 = &manager->createResource("res2", 200, ResourceType::MEMORY);
    }
    
    void TearDown() override {
        delete manager;
    }
    
    ResourceManager* manager;
    Resource* res1;
    Resource* res2;
};

TEST_F(ResourceManagerTest, ResourceCreationAndManagement) {
    EXPECT_EQ(manager->resourceCount(), 2);

    Resource& res3 = manager->createResource("res3", 300, ResourceType::NETWORK);
    EXPECT_EQ(manager->resourceCount(), 3);
    EXPECT_EQ(manager->findResource("res3")->size(), 300);

    EXPECT_THROW(manager->createResource("res1", 400, ResourceType::FILE), std::runtime_error);

    ResourceManager smallManager;
    for (int i = 0; i < 100; ++i) {
        EXPECT_NO_THROW(
            smallManager.createResource("res" + std::to_string(i), 1, ResourceType::FILE)
        );
    }
    EXPECT_THROW(
        smallManager.createResource("res101", 1, ResourceType::FILE),
        std::runtime_error
    );
}

TEST_F(ResourceManagerTest, SortingAndFiltering) {
    manager->createResource("res3", 50, ResourceType::FILE);
    manager->createResource("res4", 500, ResourceType::NETWORK);

    manager->sortBySize();
    EXPECT_EQ(manager->getResource(0).size(), 50);
    EXPECT_EQ(manager->getResource(3).size(), 500);

    auto files = manager->filterByType(ResourceType::FILE);
    EXPECT_EQ(files.size(), 2);
    for (auto* res : files) {
        EXPECT_EQ(res->type(), ResourceType::FILE);
    }

    auto networks = manager->filterByType(ResourceType::NETWORK);
    EXPECT_EQ(networks.size(), 1);
}

TEST_F(ResourceManagerTest, MemoryManagement) {
    ResourceManager localManager;
    Resource* dynamicRes = new Resource("dynamic", 100, ResourceType::FILE);

    EXPECT_NO_THROW(localManager.addResource(dynamicRes));
    EXPECT_EQ(localManager.resourceCount(), 1);
}

TEST_F(ResourceManagerTest, AccessMethods) {
    EXPECT_EQ(&manager->getResource(0), res1);
    EXPECT_THROW(manager->getResource(100), std::out_of_range);

    EXPECT_EQ(manager->findResource("res2"), res2);
    EXPECT_EQ(manager->findResource("non_existent"), nullptr);
}

TEST_F(ResourceManagerTest, CopyPrevention) {
    EXPECT_TRUE(std::is_copy_constructible<ResourceManager>::value == false);
    EXPECT_TRUE(std::is_copy_assignable<ResourceManager>::value == false);
}