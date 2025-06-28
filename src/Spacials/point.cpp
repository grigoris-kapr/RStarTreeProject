#include "point.h"

#include <cstring>
#include <format>


std::string Point::toString() const {
    std::string result = "Point(";
    if(!coords.empty()) {
        for (size_t i = 0; i < coords.size()-1; ++i) {
            result += std::format("{:.6f}, ", coords[i]);
        }
        result += std::format("{:.6f}", coords[coords.size()-1]);
    }
    result += ")";

    return result;
}

bool operator==(const Point& lhs, const Point& rhs) {
    return lhs.getCoordinates() == rhs.getCoordinates();
}
bool operator!=(const Point& lhs, const Point& rhs) {
    return !(lhs == rhs);
}

// Serialize the point to a string representation
std::vector<char> Point::serialize() const {
    return Storable::serializeDoubles(coords);
}

// Deserialize the point from a string representation
Point Point::deserialize(const std::vector<char>& data, int dimensions) {
    if (data.size() < dimensions * sizeof(double)) {
        throw std::invalid_argument("Data size is too small for Point deserialization.");
    }
    std::vector<double> coords(dimensions);
    coords = Storable::deserializeDoubles(data, 0, dimensions);
    return Point(coords);
}

// Get the size of the serialized point
int Point::getSerializedSize(int dimensions) {
    return dimensions * sizeof(double);
}