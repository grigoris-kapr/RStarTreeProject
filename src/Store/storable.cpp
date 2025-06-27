#include "storable.h"
#include <stdexcept>

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
    for (size_t i = 0; i < sizeof(double); ++i) {
        data[i] = static_cast<char>((reinterpret_cast<const unsigned char*>(&value))[i]);
    }
    return data;
}

double Storable::deserializeDouble(const std::vector<char>& data, size_t offset) {
    if (data.size() < offset + sizeof(double)) {
        throw std::invalid_argument("Data size is too small for double deserialization.");
    }
    double value = 0.0;
    for (size_t i = 0; i < sizeof(double); ++i) {
        value += static_cast<unsigned char>(data[offset + i]) << (i * 8);
    }
    return value;
}

// ==================== APPEND ====================

void Storable::appendData(std::vector<char>& data, const std::vector<char>& additionalData) {
    printf("DBG: og data: %s\n", data.data());
    if (data.empty()) {
        throw std::invalid_argument("Data cannot be empty.");
    }
    if (additionalData.empty()) {
        throw std::invalid_argument("Additional data cannot be empty.");
    }
    data.insert(data.end(), additionalData.begin(), additionalData.end());
}