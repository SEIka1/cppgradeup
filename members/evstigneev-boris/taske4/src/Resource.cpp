#include "../include/Resource.h"
#include <utility>
#include <algorithm>

void Resource::validateId() const {
    if (my_id.empty() || my_id.size() > 32) {
        throw std::invalid_argument("id length must be 1 - 32");
    }
}

void Resource::validateSize() const {
    if (my_size == 0 || my_size > 1000000000) {
        throw std::invalid_argument("size must be 1 - 1bil bytes");
    }
}

Resource::Resource(const std::string& id, size_t size, ResourceType type)
    : my_id(id), my_size(size), my_type(type), my_status(ResourceStatus::FREE) 
{
    validateId();
    validateSize();
}

void Resource::setSize(size_t newSize) {
    if (newSize == 0 || newSize > 1000000000) {
        throw std::invalid_argument("size must be 1 - 1bil bytes");
    }
    my_size = newSize;
}

ResourceManager::ResourceManager() : m_resources(nullptr), m_count(0), m_capacity(0) 
{
    resize(8);
}

ResourceManager::~ResourceManager() {
    clear();
    delete[] m_resources;
    m_resources = nullptr;
    m_capacity = 0;
}

void ResourceManager::clear() {
    if (m_resources == nullptr) { m_count = 0; return; }
    for (size_t i = 0; i < m_count; ++i) {
        delete m_resources[i];
        m_resources[i] = nullptr;
    }
    m_count = 0;
}

void ResourceManager::resize(size_t newCapacity) {
    if (newCapacity > MAX_RESOURCES) newCapacity = MAX_RESOURCES;
    if (newCapacity <= m_capacity) return;

    Resource** newArray = new (std::nothrow) Resource*[newCapacity];
    if (newArray == nullptr) {
        throw std::runtime_error("allocation failed");
    }

    for (size_t i = 0; i < newCapacity; ++i) newArray[i] = nullptr;
    for (size_t i = 0; i < m_count; ++i) newArray[i] = m_resources[i];

    delete[] m_resources;
    m_resources = newArray;
    m_capacity = newCapacity;
}

size_t ResourceManager::findByIdLinear(const std::string& id) const {
    if (m_resources == nullptr) return m_count;
    for (size_t i = 0; i < m_count; ++i) {
        if (m_resources[i] && m_resources[i]->id() == id) return i;
    }
    return m_count;
}

Resource& ResourceManager::createResource(const std::string& id, size_t size, ResourceType type) {
    if (m_count >= MAX_RESOURCES) {
        throw std::runtime_error("max capacity reached");
    }
    if (findByIdLinear(id) != m_count) {
        throw std::runtime_error("resource with the same id already exists");
    }

    Resource* newResource = new (std::nothrow) Resource(id, size, type);
    if (newResource == nullptr) {
        throw std::runtime_error("Resource allocation failed");
    }

    try {
        if (m_count == m_capacity) {
            size_t newCap = (m_capacity == 0) ? 8 : m_capacity * 2;
            if (newCap > MAX_RESOURCES) newCap = MAX_RESOURCES;
            resize(newCap);
        }
        m_resources[m_count++] = newResource;
    } catch (...) {
        delete newResource;
        throw;
    }
    return *newResource;
}

void ResourceManager::addResource(Resource* resource) {
    if (resource == nullptr) throw std::invalid_argument("null pointer");
    if (m_count >= MAX_RESOURCES) throw std::runtime_error("max capacity reached");
    if (findByIdLinear(resource->id()) != m_count) {
        throw std::runtime_error("resource with the same id already exists");
    }
    if (m_count == m_capacity) {
        size_t newCap = (m_capacity == 0) ? 8 : m_capacity * 2;
        if (newCap > MAX_RESOURCES) newCap = MAX_RESOURCES;
        resize(newCap);
    }
    m_resources[m_count++] = resource;
}

void ResourceManager::sortBySize() {
    if (m_resources == nullptr || m_count < 2) return;
    auto lessBySize = [](const Resource* a, const Resource* b) {
        return a->size() < b->size();
    };
    quickSort(m_resources, 0, static_cast<std::ptrdiff_t>(m_count) - 1, lessBySize);
}

Resource& ResourceManager::getResource(size_t index) {
    if (m_resources == nullptr || index >= m_count) {
        throw std::out_of_range("index out of range");
    }
    if (m_resources[index] == nullptr) {
        throw std::runtime_error("null pointer");
    }
    return *m_resources[index];
}

const Resource* ResourceManager::getResource(size_t index) const {
    if (m_resources == nullptr || index >= m_count) {
        throw std::out_of_range("index out of range");
    }
    return m_resources[index];
}

template <typename Less>
void ResourceManager::quickSort(Resource** arr, std::ptrdiff_t left, std::ptrdiff_t right, Less less) const {
    if (arr == nullptr || left < 0 || right < 0 || left >= right) return;

    std::ptrdiff_t i = left;
    std::ptrdiff_t j = right;
    Resource* pivot = arr[left + (right - left) / 2];

    while (i <= j) {
        while (less(arr[i], pivot)) ++i;
        while (less(pivot, arr[j])) --j;
        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i; --j;
        }
    }
    if (left < j) quickSort(arr, left, j, less);
    if (i < right) quickSort(arr, i, right, less);
}

std::vector<Resource*> ResourceManager::filterByType(ResourceType type) const {
    std::vector<Resource*> result;
    result.reserve(m_count);
    if (m_resources == nullptr) return result;

    for (size_t i = 0; i < m_count; ++i) {
        if (m_resources[i] && m_resources[i]->type() == type) {
            result.push_back(m_resources[i]);
        }
    }
    return result;
}

const Resource* ResourceManager::findResource(const std::string& id) const {
    if (m_resources == nullptr || m_count == 0) return nullptr;
    for (size_t i = 0; i < m_count; ++i) {
        if (m_resources[i] && m_resources[i]->id() == id) {
            return m_resources[i];
        }
    }
    return nullptr;
}

size_t ResourceManager::partitionBySize(Resource** arr, size_t left, size_t right, size_t pivotIndex) const {
    size_t pivotValue = arr[pivotIndex]->size();
    std::swap(arr[pivotIndex], arr[right]);

    size_t storeIndex = left;
    for (size_t i = left; i < right; ++i) {
        if (arr[i]->size() < pivotValue) {
            std::swap(arr[storeIndex], arr[i]);
            ++storeIndex;
        }
    }
    std::swap(arr[storeIndex], arr[right]);
    return storeIndex;
}

void ResourceManager::quickSelectK(Resource** arr, size_t left, size_t right, size_t k) const {
    if (arr == nullptr || left > right) return;
    while (left < right) {
        size_t pivotIndex = left + (right - left) / 2;
        pivotIndex = partitionBySize(arr, left, right, pivotIndex);

        if (k == pivotIndex) return;
        else if (k < pivotIndex) {
            if (pivotIndex == 0) return;
            right = pivotIndex - 1;
        } else {
            left = pivotIndex + 1;
        }
    }
}

Resource** ResourceManager::findKSmallest(size_t k) const {
    if (k == 0 || m_count == 0) return nullptr;
    if (k > m_count) k = m_count;
    if (k > MAX_RESOURCES) {
        throw std::invalid_argument("k > MAX_RESOURCES");
    }

    if (k == 1) {
        Resource** minRes = new Resource*[1];
        minRes[0] = *std::min_element(m_resources, m_resources + m_count, [](Resource* a, Resource* b) { return a->size() < b->size(); });
        return minRes;
    }

    ResourceArray temp(new (std::nothrow) Resource*[m_count]);
    if (!temp.get()) throw std::runtime_error("allocation failed");

    for (size_t i = 0; i < m_count; ++i) {
        temp[i] = m_resources[i];
    }

    quickSelectK(temp.get(), 0, m_count - 1, k - 1);

    Resource** res = new (std::nothrow) Resource*[k];
    if (!res) throw std::runtime_error("allocation failed");

    for (size_t i = 0; i < k; ++i) {
        res[i] = temp[i];
    }

    return res;
}
