#ifndef POINT_H
#define POINT_H

#include <vector>
#include <stdexcept>
#include "abstractBoundedClass.h"
#include "storable.h"

class Point: public AbstractBoundedClass, public Storable {
    // This class represents a point in a multi-dimensional space.
    // It is derived from AbstractBoundedClass to ensure interoperability.
protected:
    std::vector<double> coords;
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
    // For printing the point, NOT for serialization
    std::string toString() const;

    // Storage stuff:
    std::vector<char> serialize() const override;
    static Point deserialize(const std::vector<char>& data, int dimensions);
    static int getSerializedSize(int dimensions);
};

bool operator==(const Point& lhs, const Point& rhs);
bool operator!=(const Point& lhs, const Point& rhs);

#endif // POINT_H