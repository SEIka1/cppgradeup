#pragma once
#include <string>
#include <vector>
#include <stdexcept>
#include <new>
#include <cstddef>

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

    inline const std::string& id() const { return my_id; }
    inline size_t size() const { return my_size; }
    inline ResourceType type() const { return my_type; }
    inline ResourceStatus status() const { return my_status; }

    void setSize(size_t newSize);
    inline void setStatus(ResourceStatus status) { my_status = status; }
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
    void quickSort(Resource** arr, std::ptrdiff_t left, std::ptrdiff_t right, Less less) const;

    size_t partitionBySize(Resource** arr, size_t left, size_t right, size_t pivotIndex) const;
    void quickSelectK(Resource** arr, size_t left, size_t right, size_t k) const;

    class ResourceArray {
    public:
        explicit ResourceArray(Resource** ptr) : m_ptr(ptr) {}
        ~ResourceArray() { delete[] m_ptr; }

        ResourceArray(const ResourceArray&) = delete;
        ResourceArray& operator=(const ResourceArray&) = delete;

        ResourceArray(ResourceArray&& other) noexcept : m_ptr(other.m_ptr) {
            other.m_ptr = nullptr;
        }

        ResourceArray& operator=(ResourceArray&& other) noexcept {
            if (this != &other) {
                delete[] m_ptr;
                m_ptr = other.m_ptr;
                other.m_ptr = nullptr;
            }
            return *this;
        }

        Resource** get() const { return m_ptr; }
        Resource*& operator[](size_t idx) const { return m_ptr[idx]; }

    private:
        Resource** m_ptr;
    };

public:
    ResourceManager();
    ~ResourceManager();

    ResourceManager(const ResourceManager&) = delete;
    ResourceManager& operator=(const ResourceManager&) = delete;

    void clear();
    Resource& createResource(const std::string& id, size_t size, ResourceType type);
    void addResource(Resource* resource);
    inline size_t resourceCount() const { return m_count; }
    void sortBySize();
    Resource& getResource(size_t index);
    const Resource* getResource(size_t index) const;
    std::vector<Resource*> filterByType(ResourceType type) const;

    const Resource* findResource(const std::string& id) const;
    Resource** findKSmallest(size_t k) const;
};
