#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <new>

enum class ResourceType { FILE, MEMORY, NETWORK };
enum class ResourceStatus { FREE, BUSY, LOCKED };

class Resource {
private:
    std::string my_id;
    size_t my_size;
    ResourceType my_type;
    ResourceStatus my_status;

    void validateId() const;
    void validateSize() const;

public:
    Resource(const std::string& id, size_t size, ResourceType type);

    const std::string& id() const;
    size_t size() const;
    ResourceType type() const;
    ResourceStatus status() const;

    void setSize(size_t newSize);
    void setStatus(ResourceStatus status);
};

class ResourceManager {
private:
    Resource** m_resources;
    size_t m_count;
    size_t m_capacity;
    static constexpr size_t MAX_RESOURCES = 100;

    void resize(size_t newCapacity);
    size_t findByIdLinear(const std::string& id) const;

    template <typename Less>
    void quickSort(Resource** arr, long left, long right, Less less) const;

    size_t partitionBySize(Resource** arr, size_t left, size_t right, size_t pivotIndex) const;
    void quickSelectK(Resource** arr, size_t left, size_t right, size_t k) const;

public:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void clear();
    Resource& createResource(const std::string& id, size_t size, ResourceType type);
    void addResource(Resource* resource);
    size_t resourceCount() const;
    void sortBySize();
    Resource& getResource(size_t index);
    const Resource* getResource(size_t index) const;
    std::vector<Resource*> filterByType(ResourceType type) const;

    const Resource* findResource(const std::string& id) const;
    Resource** findKSmallest(size_t k) const;
};