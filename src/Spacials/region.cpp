#include "region.h"
#include <cstring> // For memcpy

Region::Region(const std::vector<double>& startCoords, const std::vector<double>& endCoords) {
    if (startCoords.size() != endCoords.size()) {
        throw std::invalid_argument("Start and end coordinates must have the same dimension.");
    }
    for(size_t i = 0; i < startCoords.size(); ++i) {
        if (startCoords[i] >= endCoords[i]) {
            // Ensure that no point or invalid region is accidentally passed as a region
            throw std::invalid_argument("Start coordinates must be less than end coordinates in each dimension.");
        }
        start.push_back(startCoords[i]);
        end.push_back(endCoords[i]);
    }
}

// Ger the region's area and margin
double Region::area() const {
    double area = 1.0;
    for (size_t i = 0; i < start.size(); ++i) {
        area *= (end[i] - start[i]);
    }
    return area;
}

double Region::margin() const{
    double margin = 0.0;
    for (size_t i = 0; i < start.size(); ++i) {
        margin += (end[i] - start[i]);
    }
    return margin;
}

// Get the area, margin and overlap as used in split for R*-tree
double Region::combinedArea(const Region& other) const {
    return area() + other.area();
}
double Region::combinedMargin(const Region& other) const {
    return margin() + other.margin();
}
double Region::overlap(const Region& other) const {
    double overlapArea = 1.0;
    for(size_t i = 0; i < start.size(); ++i) {
        if (start[i] >= other.end[i] || other.start[i] >= end[i]) {
            return 0.0; // No overlap in this dimension
        }
        overlapArea *= std::min(end[i] - other.start[i], other.end[i] - start[i]); // one will be <= 0
    }
    return overlapArea;
}

// Can be used to check both if a path should be taken (overlaps Region) 
// and if a point is inside the region (overlaps Point)
bool Region::overlaps(const AbstractBoundedClass& other) const  {
    std::vector<double> otherStart = other.getStart();
    std::vector<double> otherEnd = other.getEnd();

    // Check for overlap in each dimension
    for(size_t i = 0; i < start.size(); ++i) {
        if (start[i] > otherEnd[i] || otherStart[i] > end[i]) {
            return false; // No overlap in this dimension
        }
    }

    return true; // Overlap exists in all dimensions
}

// Function to compute the bounding box of a collection of objects
// This function takes a vector of pointers to AbstractBoundedClass objects and 
// returns a Region that represents the bounding box of all the objects.
Region Region::boundingBox(const std::vector<AbstractBoundedClass*>& objects) {
    if (objects.empty()) {
        throw std::invalid_argument("Object list cannot be empty.");
    }
    
    size_t dimensions = objects[0]->getStart().size();
    std::vector<double> start(dimensions, std::numeric_limits<double>::max());
    std::vector<double> end(dimensions, std::numeric_limits<double>::lowest());

    for (const auto& obj : objects) {
        const auto& objStart = obj->getStart();
        const auto& objEnd = obj->getEnd();
        for (size_t i = 0; i < dimensions; ++i) {
            start[i] = std::min(start[i], objStart[i]);
            end[i] = std::max(end[i], objEnd[i]);
        }
    }

    return Region(start, end);
}

bool operator==(const Region& lhs, const Region& rhs) {
    return lhs.getStart() == rhs.getStart() && lhs.getEnd() == rhs.getEnd();
}

/* bool Region::operator==(const Region &rhs) {
    return this->start == rhs.getStart() && this->end == rhs.getEnd();
} */


/*
===============================================
================= STORAGE =====================
===============================================
*/


// Serialize the object to a string representation
std::vector<char> Region::serialize(GlobalParameters* config) const {
    if (start.size() != end.size()) {
        throw std::invalid_argument("Mismatched start/end dimensions during serialization");
    }

    std::vector<char> data = Storable::serializeDoubles(start);
    Storable::appendData(data, Storable::serializeDoubles(end));
    return data;
}

// Deserialize the object from a string representation
Region Region::deserialize(GlobalParameters* config, const std::vector<char>& data) {
    if (data.size() < 2 * config->dimensions * sizeof(double)) {
        throw std::invalid_argument("Invalid data size for Region deserialization. \
            Expected " + std::to_string(2 * config->dimensions * sizeof(double)) + " bytes, got " + std::to_string(data.size()) + ".");
    }

    std::vector<double> start = Storable::deserializeDoubles(data, 0, config->dimensions);
    std::vector<double> end = Storable::deserializeDoubles(data, config->dimensions * sizeof(double), config->dimensions);

    return Region(start, end);
}

// Get the size of the serialized object
int Region::getSerializedSize(GlobalParameters* config) {
    return config->dimensions * 2 * sizeof(double); // Each dimension has a start and end coordinate
}