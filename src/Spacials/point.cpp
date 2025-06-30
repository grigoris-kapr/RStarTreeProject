#include "point.h"

#include <cstring>
#include <format>


std::string Point::toString(GlobalParameters* config) const {
    std::string result = "Point(";
    if(!coords.empty()) {
        for (size_t i = 0; i < config->dimensions-1; ++i) {
            result += std::format("{:.6f}, ", coords[i]);
        }
        result += std::format("{:.6f}", coords[config->dimensions-1]);
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
std::vector<char> Point::serialize(GlobalParameters* config) const {
    return Storable::serializeDoubles(coords);
}

// Deserialize the point from a string representation
Point Point::deserialize(GlobalParameters* config, const std::vector<char>& data) {
    if (data.size() < config->dimensions * sizeof(double)) {
        throw std::invalid_argument("Data size is too small for Point deserialization.");
    }
    std::vector<double> coords(config->dimensions);
    coords = Storable::deserializeDoubles(data, 0, config->dimensions);
    return Point(coords);
}

// Get the size of the serialized point
int Point::getSerializedSize(GlobalParameters* config) {
    return config->dimensions * sizeof(double);
}