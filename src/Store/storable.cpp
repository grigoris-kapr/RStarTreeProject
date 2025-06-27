#include "storable.h"
#include <stdexcept>
#include <cstring>

// ==================== INT ====================

std::vector<char> Storable::serializeInt(int value) {
    std::vector<char> data(sizeof(int));
    for (size_t i = 0; i < sizeof(int); ++i) {
        data[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
    }
    return data;
}

int Storable::deserializeInt(const std::vector<char>& data, size_t offset) {
    if (data.size() < offset + sizeof(int)) {
        throw std::invalid_argument("Data size is too small for integer deserialization.");
    }
    int value = 0;
    for (size_t i = 0; i < sizeof(int); ++i) {
        value |= (static_cast<unsigned char>(data[offset + i]) << (i * 8));
    }
    return value;
}

// ==================== LONG LONG ====================

std::vector<char> Storable::serializeLongLong(long long value) {
    std::vector<char> data(sizeof(long long));
    for (size_t i = 0; i < sizeof(long long); ++i) {
        data[i] = static_cast<char>((value >> (i * 8)) & 0xFF);
    }
    return data;
}

long long Storable::deserializeLongLong(const std::vector<char>& data, size_t offset) {
    if (data.size() < offset + sizeof(long long)) {
        throw std::invalid_argument("Data size is too small for long long deserialization.");
    }
    long long value = 0;
    for (size_t i = 0; i < sizeof(long long); ++i) {
        value |= (static_cast<unsigned char>(data[offset + i]) << (i * 8));
    }
    return value;
}

// ==================== DOUBLE ====================

std::vector<char> Storable::serializeDouble(double value) {
    std::vector<char> data(sizeof(double));
    std::memcpy(data.data(), &value, sizeof(double));
    return data;
}

double Storable::deserializeDouble(const std::vector<char>& data, size_t offset) {
    if (data.size() < offset + sizeof(double)) {
        throw std::invalid_argument("Data size is too small for double deserialization.");
    }
    double value;
    std::memcpy(&value, data.data() + offset, sizeof(double));
    return value;
}

// ==================== MANY INTS ====================

std::vector<char> Storable::serializeInts(const std::vector<int>& values) {
    std::vector<char> data(values.size() * sizeof(int));
    std::memcpy(data.data(), values.data(), data.size());
    return data;
}

std::vector<int> Storable::deserializeInts(const std::vector<char>& data, size_t offset, size_t count) {
    if (data.size() < offset + count * sizeof(int)) {
        throw std::invalid_argument("Data size is too small for integer vector deserialization.");
    }
    std::vector<int> values(count);
    std::memcpy(values.data(), data.data() + offset, count * sizeof(int));
    return values;
}

// ==================== MANY LONG LONGS ====================

std::vector<char> Storable::serializeLongLongs(const std::vector<long long>& values) {
    std::vector<char> data(values.size() * sizeof(long long));
    std::memcpy(data.data(), values.data(), data.size());
    return data;
}

std::vector<long long> Storable::deserializeLongLongs(const std::vector<char>& data, size_t offset, size_t count) {
    if (data.size() < offset + count * sizeof(long long)) {
        throw std::invalid_argument("Data size is too small for long long vector deserialization.");
    }
    std::vector<long long> values(count);
    std::memcpy(values.data(), data.data() + offset, count * sizeof(long long));
    return values;
}

// ==================== MANY DOUBLES ====================

std::vector<char> Storable::serializeDoubles(const std::vector<double>& values) {
    std::vector<char> data(values.size() * sizeof(double));
    std::memcpy(data.data(), values.data(), data.size());
    return data;
}

std::vector<double> Storable::deserializeDoubles(const std::vector<char>& data, size_t offset, size_t count) {
    if (data.size() < offset + count * sizeof(double)) {
        throw std::invalid_argument("Data size is too small for double vector deserialization.");
    }
    std::vector<double> values(count);
    std::memcpy(values.data(), data.data() + offset, count * sizeof(double));
    return values;
}

// ==================== APPEND ====================

void Storable::appendData(std::vector<char>& data, const std::vector<char>& additionalData) {
    if (additionalData.empty()) {
        throw std::invalid_argument("Additional data cannot be empty.");
    }
    data.insert(data.end(), additionalData.begin(), additionalData.end());
}