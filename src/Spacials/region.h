#ifndef REGION_H
#define REGION_H

#include <vector>
#include <stdexcept>
#include "abstractBoundedClass.h"
#include <limits>
#include "storable.h"

class Region: public AbstractBoundedClass, public Storable {
    // This class represents a multi-dimensional rectangular region defined by its start and end coordinates.
    // It is derived from AbstractBoundedClass to ensure it can be used in a bounded context.
private:
    std::vector<double> start; // Starting coordinates of the region
    std::vector<double> end;   // Ending coordinates of the region
public:
    Region() = default;
    Region(const std::vector<double>& startCoords, const std::vector<double>& endCoords);
    // Get the start and end coordinates of the region
    const std::vector<double>& getStart() const { return start; }
    const std::vector<double>& getEnd() const { return end; }
    // Retun region's area and margin
    double area() const;
    double margin() const;

    // Get the area, margin and overlap as used in split for R*-tree
    double combinedArea(const Region& other) const;
    double combinedMargin(const Region& other) const;
    double overlap(const Region& other) const;
    // Can be used to check both if a path should be taken (overlaps Region) 
    // and if a point is inside the region (overlaps Point)
    bool overlaps(const AbstractBoundedClass& other) const;

    // Storage stuff:
    std::vector<char> serialize() const override;
    static int getSerializedSize(int dimensions);
    static Region deserialize(const std::vector<char>& data, int dimensions);
    
};

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


#endif // REGION_H