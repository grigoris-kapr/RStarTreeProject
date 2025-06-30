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
    // TODO: Replace .size() with dimensions from GlobalParameters in methods that require it.
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

    // Function to compute the bounding box of a collection of objects
    static Region boundingBox(const std::vector<AbstractBoundedClass*>& objects);

    // Storage stuff:
    std::vector<char> serialize(GlobalParameters* config) const override;
    static int getSerializedSize(GlobalParameters* config);
    static Region deserialize(GlobalParameters* config, const std::vector<char>& data);
    
};

bool operator==(const Region& lhs, const Region& rhs);

#endif // REGION_H