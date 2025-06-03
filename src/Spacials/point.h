#ifndef POINT_H
#define POINT_H

#include <vector>
#include <stdexcept>
#include "abstractBoundedClass.h"
#include "storable.h"

class Point: public AbstractBoundedClass, public Storable {
    // This class represents a point in a multi-dimensional space.
    // It is derived from AbstractBoundedClass to ensure interoperability.
private:
    std::vector<double> coords; // Starting coordinates of the region
public:
    Point() = default;
    Point(const std::vector<double>& coords) : coords(coords) {
        if (coords.empty()) {
            throw std::invalid_argument("Coordinates cannot be empty.");
        }
    }

    // Get the coordinates of the point
    const std::vector<double>& getCoordinates() const { return coords; }
    // Get the start and end coordinates of the point (both are the same)
    const std::vector<double>& getStart() const override { return coords; }
    const std::vector<double>& getEnd() const override { return coords; }

    // Serialize the point to a string representation
    std::vector<char> serialize() const override;
    // Deserialize the point from a string representation
    static Point deserialize(const std::vector<char>& data, int dimensions);
};

#endif // POINT_H