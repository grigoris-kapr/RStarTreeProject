#include "point.h"

// Serialize the point to a string representation
std::vector<char> Point::serialize() const {
    std::vector<char> data;
    // Serialize the coordinates
    for (int i = 0; i < coords.size(); ++i) {
        Storable::appendData(data, Storable::serializeDouble(coords[i]));
    }
    return data;
}

// Deserialize the point from a string representation
static Point deserialize(const std::vector<char>& data, int dimensions) {
    if (data.size() < dimensions * sizeof(double)) {
        throw std::invalid_argument("Data size is too small for Point deserialization.");
    }
    std::vector<double> coords(dimensions);
    for (int i = 0; i < dimensions; ++i) {
        coords[i] = Storable::deserializeDouble(data, i * sizeof(double));
    }
    return Point(coords);
}