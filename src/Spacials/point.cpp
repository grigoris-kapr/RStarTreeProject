#include "point.h"

#include <cstring>

// Serialize the point to a string representation
std::vector<char> Point::serialize() const {
    std::vector<char> bytes(coords.size() * sizeof(double));
    std::memcpy(bytes.data(), coords.data(), bytes.size());
    return bytes;
}

// Deserialize the point from a string representation
Point Point::deserialize(const std::vector<char>& data, int dimensions) {
    if (data.size() < dimensions * sizeof(double)) {
        throw std::invalid_argument("Data size is too small for Point deserialization.");
    }
    std::vector<double> coords(dimensions);
    std::memcpy(coords.data(), data.data(), data.size());
    return Point(coords);
}

// Get the size of the serialized point
int Point::getSerializedSize(int dimensions) {
    return dimensions * sizeof(double);
}