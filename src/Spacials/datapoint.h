#ifndef DATA_POINT_H
#define DATA_POINT_H

#include <vector>
#include "storable.h"
#include "point.h"

// TODO: Can this be a parameter? Do I want it to be one?
#define MAX_DATA_SIZE 24 // Maximum size of DataPoint data in bytes

class DataPoint: public Storable {
protected:
    Point point; // The point representing the coordinates in space
    std::vector<char> data; // Any data up to MAX_DATA_SIZE characters (enforced by constructor & setter)
    long long id; // Unique identifier for the data point

public:
    DataPoint() = default;

    // Constructor that initializes the data point with coordinates
    DataPoint(const std::vector<double>& coords, const std::vector<char>& data, long long id);
    // Constructor that initializes the data point with a Point object
    DataPoint(const Point& point, const std::vector<char>& data, long long id);

    // Getters & setters for point, data and ID
    const Point& getPoint() const { return point; }
    void setPoint(const Point& newPoint) { point = newPoint; }
    const std::vector<char>& getData() const { return data; }
    void setData(const std::vector<char>& newData); // enforce MAX_DATA_SIZE
    long long getID() const { return id; }

    // Serialize the DataPoint to a string representation
    std::vector<char> serialize(GlobalParameters* config) const override;

    // Deserialize the DataPoint from a string representation
    static DataPoint deserialize(GlobalParameters* config, const std::vector<char>& data);

    // Get the size of the serialized DataPoint
    static int getSerializedSize(GlobalParameters* config);
};

#endif // DATA_POINT_H