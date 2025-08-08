#include "../include/Resource.h"
#include <iostream>
#include <utility>

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

Resource::Resource(const std::string& id, size_t size, ResourceType type): my_id(id), my_size(size), my_type(type), my_status(ResourceStatus::FREE) 
{
    validateId();
    validateSize();
}

const std::string& Resource::id() const { return my_id; }
size_t Resource::size() const { return my_size; }
ResourceType Resource::type() const { return my_type; }
ResourceStatus Resource::status() const { return my_status; }

void Resource::setSize(size_t newSize) {
    if (newSize == 0 || newSize > 1000000000) {
        throw std::invalid_argument("size must be 1 - 1bil bytes");
    }
    my_size = newSize;
}

void Resource::setStatus(ResourceStatus status) {
    my_status = status;
}

ResourceManager::ResourceManager(): m_resources(nullptr), m_count(0), m_capacity(0) 
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
    for (size_t i = 0; i < m_count; ++i) {
        delete m_resources[i];
        m_resources[i] = nullptr;
    }
    m_count = 0;
}

void ResourceManager::resize(size_t newCapacity) {
    if (newCapacity > MAX_RESOURCES) { newCapacity = MAX_RESOURCES; }
    if (newCapacity <= m_capacity) { return; }

    Resource** newArray = new (std::nothrow) Resource*[newCapacity];

    for (size_t i = 0; i < newCapacity; ++i) {
        newArray[i] = nullptr;
    }
    for (size_t i = 0; i < m_count; ++i) {
        newArray[i] = m_resources[i];
    }

    delete[] m_resources;
    m_resources = newArray;
    m_capacity = newCapacity;
}

size_t ResourceManager::findByIdLinear(const std::string& id) const {
    for (size_t i = 0; i < m_count; ++i) {
        if (m_resources[i]->id() == id) {
            return i;
        }
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

    try {
        if (m_count == m_capacity) {
            size_t newCapacity = (m_capacity == 0) ? 8 : m_capacity * 2;
            if (newCapacity > MAX_RESOURCES) {
                newCapacity = MAX_RESOURCES;
            }
            resize(newCapacity);
        }
        m_resources[m_count++] = newResource;
    } catch (...) {
        delete newResource;
        throw;
    }

    return *newResource;
}

void ResourceManager::addResource(Resource* resource) {
    if (m_count >= MAX_RESOURCES) {
        throw std::runtime_error("max capacity reached");
    }
    if (findByIdLinear(resource->id()) != m_count) {
        throw std::runtime_error("resource with the same id already exists");
    }
    if (m_count == m_capacity) {
        size_t newCapacity = (m_capacity == 0) ? 8 : m_capacity * 2;
        if (newCapacity > MAX_RESOURCES) {
            newCapacity = MAX_RESOURCES;
        }
        resize(newCapacity);
    }
    m_resources[m_count++] = resource;
}

size_t ResourceManager::resourceCount() const {
    return m_count;
}

Resource& ResourceManager::getResource(size_t index) {
    return *m_resources[index];
}

const Resource* ResourceManager::getResource(size_t index) const {
    return m_resources[index];
}

template <typename Less>
void ResourceManager::quickSort(Resource** arr, long left, long right, Less less) const {
    if (left >= right) {
        return;
    }

    long i = left;
    long j = right;
    Resource* pivot = arr[left + (right - left) / 2];

    while (i <= j) {
        while (less(arr[i], pivot)) ++i;
        while (less(pivot, arr[j])) --j;

        if (i <= j) {
            std::swap(arr[i], arr[j]);
            ++i;
            --j;
        }
    }

    if (left < j) quickSort(arr, left, j, less);
    if (i < right) quickSort(arr, i, right, less);
}

void ResourceManager::sortBySize() {
    if (m_count < 2) {
        return;
    }
    auto lessBySize = [](const Resource* a, const Resource* b) {
        return a->size() < b->size();
    };
    quickSort(m_resources, 0, static_cast<long>(m_count) - 1, lessBySize);
}

std::vector<Resource*> ResourceManager::filterByType(ResourceType type) const {
    std::vector<Resource*> result;
    result.reserve(m_count);

    for (size_t i = 0; i < m_count; ++i) {
        if (m_resources[i]->type() == type) {
            result.push_back(m_resources[i]);
        }
    }
    return result;
}

const Resource* ResourceManager::findResource(const std::string& id) const {
    if (m_count == 0) {
        return nullptr;
    }

    Resource** tempArray = new (std::nothrow) Resource*[m_count];

    for (size_t i = 0; i < m_count; ++i) {
        tempArray[i] = m_resources[i];
    }

    auto lessById = [](const Resource* a, const Resource* b) {
        return a->id() < b->id();
    };

    quickSort(tempArray, 0, static_cast<long>(m_count) - 1, lessById);

    long low = 0;
    long high = static_cast<long>(m_count) - 1;
    const Resource* found = nullptr;

    while (low <= high) {
        long mid = low + (high - low) / 2;
        int cmp = tempArray[mid]->id().compare(id);

        if (cmp == 0) {
            found = tempArray[mid];
            break;
        } else if (cmp < 0) {
            low = mid + 1;
        } else {
            high = mid - 1;
        }
    }

    delete[] tempArray;
    return found;
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
    while (left < right) {
        size_t pivotIndex = left + (right - left) / 2;
        pivotIndex = partitionBySize(arr, left, right, pivotIndex);

        if (k == pivotIndex) {
            return;
        } else if (k < pivotIndex) {
            if (pivotIndex == 0) return;
            right = pivotIndex - 1;
        } else {
            left = pivotIndex + 1;
        }
    }
}

Resource** ResourceManager::findKSmallest(size_t k) const {
    if (k == 0) { return nullptr; }
    if (k > m_count) { k = m_count; }

    Resource** tempArray = new (std::nothrow) Resource*[m_count];

    for (size_t i = 0; i < m_count; ++i) { tempArray[i] = m_resources[i]; }

    if (k > 0) { quickSelectK(tempArray, 0, m_count - 1, k - 1); }

    Resource** result = new (std::nothrow) Resource*[k];
    for (size_t i = 0; i < k; ++i) { result[i] = tempArray[i]; }

    delete[] tempArray;
    return result;
}